/*
 * This file is part of BedrockProtocol for Endstone.
 * This file has no PHP counterpart: it bridges the ported protocol onto Endstone's packet events.
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/bridge/PacketInterceptor.h"

#include <string>
#include <unordered_set>

#include <endstone/logger.h>
#include <endstone/server.h>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketPool.h"

namespace bedrock_protocol::bridge {
namespace {

/**
 * Set while this interceptor is sending a packet of its own.
 *
 * Endstone routes Player::sendPacket through the same path that fires PacketSendEvent, so without
 * this guard a callback that sends a packet would re-enter its own handler.
 */
thread_local int g_injection_depth = 0;

/**
 * Balances the injection guard even if Player::sendPacket throws.
 *
 * Without this the counter stays above zero for the rest of the thread's life and every subsequent
 * packet is skipped by dispatch() - interception stops server-wide, silently, with no error. That
 * failure mode is invisible until someone notices a plugin has quietly stopped working.
 */
class InjectionGuard {
public:
    InjectionGuard() { ++g_injection_depth; }
    ~InjectionGuard() { --g_injection_depth; }
    InjectionGuard(const InjectionGuard &) = delete;
    InjectionGuard &operator=(const InjectionGuard &) = delete;
};

/** Packet IDs already reported as undecodable, so the log is not flooded once per packet. */
std::unordered_set<std::uint32_t> &reportedDecodeFailures()
{
    static std::unordered_set<std::uint32_t> ids;
    return ids;
}

}  // namespace

PacketInterceptor &PacketInterceptor::get()
{
    static PacketInterceptor instance;
    return instance;
}

void PacketInterceptor::enable(endstone::Plugin &plugin, endstone::EventPriority priority)
{
    if (plugin_ != nullptr) {
        return;  // already registered
    }
    plugin_ = &plugin;

    plugin.registerEvent<endstone::PacketReceiveEvent>(
        [this](endstone::PacketReceiveEvent &event) { onPacketReceive(event); }, priority);
    plugin.registerEvent<endstone::PacketSendEvent>(
        [this](endstone::PacketSendEvent &event) { onPacketSend(event); }, priority);
}

void PacketInterceptor::disable()
{
    // Endstone unregisters the two event handlers with the plugin, but the subscription tables are
    // ours and hold std::functions that captured objects the plugin is about to destroy. Clearing
    // them here is what makes a disable/enable cycle safe rather than a use-after-free waiting for
    // the next packet.
    receive_subscriptions_.clear();
    send_subscriptions_.clear();
    receive_handler_.reset();
    send_handler_.reset();
    plugin_ = nullptr;
    rewrite_count_ = 0;
    // Also process-static, so without this a protocol-drift warning that fired before a reload would
    // never be logged again for the rest of the server's life.
    reportedDecodeFailures().clear();
    // Deliberately NOT reset: next_subscription_id_, so an ID handed out before the reload can never
    // collide with one handed out after it.
}

bool PacketInterceptor::isInjecting()
{
    return g_injection_depth > 0;
}

SubscriptionId PacketInterceptor::subscribe(Direction direction, std::uint32_t packetId, Callback callback)
{
    const auto id = next_subscription_id_++;
    auto &table = direction == Direction::Receive ? receive_subscriptions_ : send_subscriptions_;
    table[packetId].push_back(Subscription{id, std::move(callback)});
    return id;
}

void PacketInterceptor::unsubscribe(SubscriptionId id)
{
    for (auto *table : {&receive_subscriptions_, &send_subscriptions_}) {
        for (auto &[packetId, subscriptions] : *table) {
            for (auto it = subscriptions.begin(); it != subscriptions.end(); ++it) {
                if (it->id == id) {
                    subscriptions.erase(it);
                    return;
                }
            }
        }
    }
}

void PacketInterceptor::setReceiveHandler(std::shared_ptr<PacketHandlerInterface> handler)
{
    receive_handler_ = std::move(handler);
}

void PacketInterceptor::setSendHandler(std::shared_ptr<PacketHandlerInterface> handler)
{
    send_handler_ = std::move(handler);
}

void PacketInterceptor::onPacketReceive(endstone::PacketReceiveEvent &event)
{
    if (g_injection_depth > 0) {
        return;
    }
    dispatch(Direction::Receive, static_cast<std::uint32_t>(event.getPacketId()), event.getPayload(), event.getPlayer(),
             event.getSubClientId(), event, [&event](std::string_view payload) { event.setPayload(payload); });
}

void PacketInterceptor::onPacketSend(endstone::PacketSendEvent &event)
{
    if (g_injection_depth > 0) {
        return;
    }
    dispatch(Direction::Send, static_cast<std::uint32_t>(event.getPacketId()), event.getPayload(), event.getPlayer(),
             event.getSubClientId(), event, [&event](std::string_view payload) { event.setPayload(payload); });
}

void PacketInterceptor::dispatch(Direction direction, std::uint32_t packetId, std::string_view payload,
                                 endstone::Player *player, int subClientId, endstone::ICancellable &cancellable,
                                 const std::function<void(std::string_view)> &applyPayload)
{
    auto &table = direction == Direction::Receive ? receive_subscriptions_ : send_subscriptions_;
    const auto &handler = direction == Direction::Receive ? receive_handler_ : send_handler_;

    const auto it = table.find(packetId);
    const auto *subscriptions = it != table.end() && !it->second.empty() ? &it->second : nullptr;
    if (subscriptions == nullptr && handler == nullptr) {
        return;  // nobody is interested; never touch the payload
    }

    auto packet = PacketPool::getInstance().getPacketById(packetId);
    if (packet == nullptr) {
        return;  // not a packet this library knows about
    }

    encoding::ByteBufferReader in(payload);
    try {
        packet->decodeBody(in);
    }
    catch (const std::exception &e) {
        reportDecodeFailure(packetId, packet->getName(), e.what());
        if (decode_failure_policy_ == DecodeFailurePolicy::Cancel) {
            cancellable.cancel();
        }
        return;
    }

    // Trailing bytes mean this library's idea of the packet layout no longer matches the server's.
    // Re-encoding would silently truncate them, so treat it as a decode failure.
    if (in.getUnreadLength() != 0) {
        reportDecodeFailure(packetId, packet->getName(),
                            std::to_string(in.getUnreadLength()) + " trailing bytes were left undecoded");
        if (decode_failure_policy_ == DecodeFailurePolicy::Cancel) {
            cancellable.cancel();
        }
        return;
    }

    if (direction == Direction::Receive) {
        packet->senderSubId = static_cast<std::uint8_t>(subClientId);
    }
    else {
        packet->recipientSubId = static_cast<std::uint8_t>(subClientId);
    }

    PacketEventBase event(player, cancellable, direction, subClientId, *packet);

    if (subscriptions != nullptr) {
        // Iterate over a copy: a callback is allowed to subscribe or unsubscribe.
        const auto snapshot = *subscriptions;
        for (const auto &subscription : snapshot) {
            subscription.callback(event);
            if (event.isCancelled()) {
                return;  // a cancelled packet is never re-encoded
            }
        }
    }

    if (handler != nullptr) {
        if (packet->handle(*handler)) {
            event.markModified();
        }
        if (event.isCancelled()) {
            return;
        }
    }

    if (!event.isModified()) {
        return;  // untouched: leave Endstone's fast path alone
    }

    // A subscriber was handed a mutable packet and can leave it in a state that will not encode - an
    // emptied required member, a BitSet resized to the wrong bit length. That is the plugin author's
    // mistake, but the throw would escape this event handler and take the server with it, so it is
    // contained here and the packet passes through unmodified instead.
    encoding::ByteBufferWriter out(payload.size() + 16);
    try {
        packet->encodeBody(out);
    }
    catch (const std::exception &e) {
        reportEncodeFailure(packetId, packet->getName(), e.what());
        return;
    }
    catch (...) {
        reportEncodeFailure(packetId, packet->getName(), "non-std::exception");
        return;
    }
    applyPayload(out.getData());
    ++rewrite_count_;
}

void PacketInterceptor::reportEncodeFailure(std::uint32_t packetId, std::string_view packetName,
                                            std::string_view reason)
{
    static std::unordered_set<std::uint32_t> reported;
    if (!reported.insert(packetId).second) {
        return;  // once per packet ID; a broken callback would otherwise flood the log every tick
    }
    if (plugin_ == nullptr) {
        return;
    }
    plugin_->getLogger().error(
        "A subscriber left {} (packet ID {}) in a state that cannot be encoded: {}. The packet was passed through "
        "unmodified. Check the callback that mutates this packet - a required member is probably empty.",
        std::string(packetName), packetId, std::string(reason));
}

void PacketInterceptor::reportDecodeFailure(std::uint32_t packetId, std::string_view packetName,
                                            std::string_view reason)
{
    if (!reportedDecodeFailures().insert(packetId).second) {
        return;  // already reported for this packet ID
    }
    if (plugin_ == nullptr) {
        return;
    }
    plugin_->getLogger().warning(
        "Could not decode {} (packet ID {}): {}. Packets of this type will be passed through unmodified - this usually "
        "means the server's protocol version differs from the one this library was generated against.",
        std::string(packetName), packetId, std::string(reason));
}

void PacketInterceptor::sendPacket(endstone::Player &player, const DataPacket &packet)
{
    encoding::ByteBufferWriter out;
    packet.encodeBody(out);

    const InjectionGuard guard;
    player.sendPacket(static_cast<int>(packet.pid()), out.getData());
}

}  // namespace bedrock_protocol::bridge

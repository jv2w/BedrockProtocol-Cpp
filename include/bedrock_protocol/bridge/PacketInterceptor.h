/*
 * This file is part of BedrockProtocol-Cpp.
 * This file has no PHP counterpart: it bridges the ported protocol onto Endstone's packet events.
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

// endstone/event/server/packet_{receive,send}_event.h name SocketAddress without including it;
// they only compile as part of endstone.hpp, which pulls this in first.
#include <endstone/util/socket_address.h>

#include <endstone/event/event_priority.h>
#include <endstone/event/server/packet_receive_event.h>
#include <endstone/event/server/packet_send_event.h>
#include <endstone/plugin/plugin.h>

#include "bedrock_protocol/bridge/PacketEvent.h"
#include "bedrock_protocol/encoding/ProtocolDialect.h"
#include "bedrock_protocol/protocol/DataPacket.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"

namespace bedrock_protocol::bridge {

/** Handle returned by a subscription, used to cancel it again. */
using SubscriptionId = std::uint64_t;

/** What to do with a packet whose body this library could not decode. */
enum class DecodeFailurePolicy {
    /**
     * Leave the packet untouched and let the server process it normally (default).
     *
     * This is the safe choice: a decode failure usually means the protocol has drifted from the
     * version this library was generated against, and re-encoding such a packet would corrupt it.
     */
    PassThrough,
    /** Drop the packet. Only sensible if you would rather lose a packet than let it through unfiltered. */
    Cancel,
};

/**
 * Decodes Endstone's raw packet events into typed packet objects, dispatches them to subscribers,
 * and writes back only what was actually changed.
 *
 * Usage from a plugin's onEnable():
 * @code
 *   auto &interceptor = bedrock_protocol::bridge::PacketInterceptor::get();
 *   interceptor.enable(*this);
 *   interceptor.onSend<bedrock_protocol::TextPacket>([](auto &event) {
 *       event.mutate().message = "[filtered] " + event.view().message;
 *   });
 * @endcode
 *
 * Threading: Endstone dispatches both packet events synchronously on the server thread, so no
 * locking is performed and none is needed. Subscribing from another thread is not supported.
 *
 * Scope: this library is linked statically into each plugin, so every plugin gets its own
 * interceptor with its own subscriptions. Plugins compose through Endstone's event priorities -
 * a lower-priority plugin sees whatever a higher-priority one wrote back, because the modified
 * payload is what the next listener decodes.
 *
 * Dialects: protocol 2168 covers two wire layouts (see encoding/ProtocolDialect.h). Both the decode
 * and the re-encode here use the layout the SERVER writes, resolved once in enable(). That is what
 * composition demands - a payload does not say which layout it is already in, so N interceptors each
 * reading and writing the server's layout is the only arrangement that stays correct when they chain.
 * Rewriting a packet into a RECIPIENT's layout is therefore not done here; it must happen exactly
 * once per server, in one place that owns the decision. sendPacket() is the one exception, and only
 * because an injected packet has a single recipient and nothing downstream re-reads it.
 */
class PacketInterceptor {
public:
    /** Returns this plugin's interceptor. */
    static PacketInterceptor &get();

    /**
     * Registers the underlying Endstone event handlers. Call once from onEnable().
     *
     * Endstone rejects event registration from a plugin that is not enabled yet, so this cannot be
     * called from onLoad().
     *
     * @param plugin the plugin whose event registrations these become
     * @param priority the priority both handlers are registered at
     */
    void enable(endstone::Plugin &plugin, endstone::EventPriority priority = endstone::EventPriority::Normal);

    /**
     * Drops every subscription and forgets the plugin. Call this from onDisable.
     *
     * This singleton outlives the plugin object. Without it, a disable/enable cycle leaves
     * std::functions bound to destroyed objects in the subscription tables, leaves plugin_ pointing
     * at freed memory, and makes the next enable() a silent no-op because it early-returns on a
     * non-null plugin_. The first matching packet after the reload then calls through freed memory.
     */
    void disable();

    /**
     * True while this interceptor is sending a packet of its own.
     *
     * Anything observing PacketSendEvent can use this to tell the server's own traffic apart from
     * traffic a plugin injected a microsecond earlier. Re-decoding our own bytes only demonstrates
     * self-consistency, so a measurement that counts them is grading its own homework.
     */
    [[nodiscard]] static bool isInjecting();

    /** Subscribes to packets of the given type arriving from a client. */
    template <typename TPacket>
    SubscriptionId onReceive(std::function<void(PacketEvent<TPacket> &)> callback)
    {
        return subscribe(Direction::Receive, TPacket::NETWORK_ID, wrap<TPacket>(std::move(callback)));
    }

    /** Subscribes to packets of the given type being sent to a client. */
    template <typename TPacket>
    SubscriptionId onSend(std::function<void(PacketEvent<TPacket> &)> callback)
    {
        return subscribe(Direction::Send, TPacket::NETWORK_ID, wrap<TPacket>(std::move(callback)));
    }

    /** Removes a subscription. Unknown IDs are ignored. */
    void unsubscribe(SubscriptionId id);

    /**
     * Installs a PMMP-style handler receiving every decoded packet in the given direction.
     *
     * Handlers run after the typed callbacks. A handler method that returns true is taken to mean
     * the packet was changed and must be re-encoded; return false when you only inspected it.
     *
     * Note that installing a handler makes this library decode EVERY packet in that direction,
     * which is considerably more expensive than subscribing to the few types you care about.
     */
    void setReceiveHandler(std::shared_ptr<PacketHandlerInterface> handler);
    void setSendHandler(std::shared_ptr<PacketHandlerInterface> handler);

    /**
     * Encodes and sends a packet to a player.
     *
     * The packet goes out through Endstone's normal send path, which fires PacketSendEvent again;
     * this interceptor suppresses its own handler for the duration so that a callback sending a
     * packet does not recurse into itself.
     */
    void sendPacket(endstone::Player &player, const DataPacket &packet);

    /** Sets what happens when a packet body cannot be decoded. Defaults to PassThrough. */
    void setDecodeFailurePolicy(DecodeFailurePolicy policy) { decode_failure_policy_ = policy; }

    [[nodiscard]] DecodeFailurePolicy getDecodeFailurePolicy() const { return decode_failure_policy_; }

    /** Number of packets this interceptor has re-encoded, for diagnostics. */
    [[nodiscard]] std::uint64_t getRewriteCount() const { return rewrite_count_; }

    /**
     * The wire layout this server writes, resolved from its version in enable().
     *
     * Exposed because a plugin that rewrites packets for clients on the other side of the 1.26.44
     * split needs the same answer, and resolving it twice invites the two copies to disagree.
     */
    [[nodiscard]] encoding::ProtocolDialect getServerDialect() const { return server_dialect_; }

    /**
     * False when the server's version could not be parsed and getServerDialect() is only the value
     * this library was built for. Anything that would rewrite traffic on that answer must not run.
     */
    [[nodiscard]] bool isServerDialectKnown() const { return server_dialect_known_; }

private:
    using Callback = std::function<void(PacketEventBase &)>;

    struct Subscription {
        SubscriptionId id;
        Callback callback;
    };

    PacketInterceptor() = default;

    /** Adapts a typed callback to the type-erased signature stored in the subscription table. */
    template <typename TPacket>
    static Callback wrap(std::function<void(PacketEvent<TPacket> &)> callback)
    {
        return [callback = std::move(callback)](PacketEventBase &event) {
            callback(static_cast<PacketEvent<TPacket> &>(event));
        };
    }

    SubscriptionId subscribe(Direction direction, std::uint32_t packetId, Callback callback);

    void onPacketReceive(endstone::PacketReceiveEvent &event);
    void onPacketSend(endstone::PacketSendEvent &event);

    /**
     * Shared dispatch path for both directions.
     *
     * @param payload the packet body, with the generic header already stripped by Endstone
     * @param applyPayload called with the re-encoded body, but only when a callback changed the packet
     */
    void dispatch(Direction direction, std::uint32_t packetId, std::string_view payload, endstone::Player *player,
                  int subClientId, endstone::ICancellable &cancellable,
                  const std::function<void(std::string_view)> &applyPayload);

    /** Logs a decode failure once per packet ID, so a protocol mismatch does not flood the log. */
    void reportDecodeFailure(std::uint32_t packetId, std::string_view packetName, std::string_view reason);

    /**
     * Reports a subscriber leaving a packet in a state that cannot be encoded.
     *
     * Separate from the decode counterpart because the cause is different and so is the fix: a decode
     * failure points at a protocol mismatch, this one points at the plugin's own callback.
     */
    void reportEncodeFailure(std::uint32_t packetId, std::string_view packetName, std::string_view reason);

    std::unordered_map<std::uint32_t, std::vector<Subscription>> receive_subscriptions_;
    std::unordered_map<std::uint32_t, std::vector<Subscription>> send_subscriptions_;
    std::shared_ptr<PacketHandlerInterface> receive_handler_;
    std::shared_ptr<PacketHandlerInterface> send_handler_;

    endstone::Plugin *plugin_ = nullptr;
    DecodeFailurePolicy decode_failure_policy_ = DecodeFailurePolicy::PassThrough;
    /** The layout this server writes; resolved once in enable() because it cannot change while it runs. */
    encoding::ProtocolDialect server_dialect_ = encoding::CURRENT_DIALECT;
    bool server_dialect_known_ = false;
    SubscriptionId next_subscription_id_ = 1;
    std::uint64_t rewrite_count_ = 0;
};

}  // namespace bedrock_protocol::bridge

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

#include <endstone/event/cancellable.h>
#include <endstone/player.h>

#include "bedrock_protocol/protocol/DataPacket.h"

namespace bedrock_protocol::bridge {

/** Which direction the intercepted packet was travelling in. */
enum class Direction {
    /** Client to server: delivered by endstone::PacketReceiveEvent. */
    Receive,
    /** Server to client: delivered by endstone::PacketSendEvent. */
    Send,
};

/**
 * The direction-agnostic part of an intercepted packet.
 *
 * Instances live only for the duration of one callback; nothing here may be stored and used later.
 */
class PacketEventBase {
public:
    PacketEventBase(endstone::Player *player, endstone::ICancellable &cancellable, Direction direction,
                    int subClientId, DataPacket &packet)
        : player_(player), cancellable_(&cancellable), direction_(direction), sub_client_id_(subClientId),
          packet_(&packet)
    {
    }

    /**
     * Returns the player this packet belongs to, or nullptr.
     *
     * Both events also fire before login completes, in which case there is no Player yet.
     */
    [[nodiscard]] endstone::Player *getPlayer() const { return player_; }

    /** Returns the split-screen sub-client this packet belongs to (0-3). */
    [[nodiscard]] int getSubClientId() const { return sub_client_id_; }

    [[nodiscard]] Direction getDirection() const { return direction_; }

    /** Returns the packet's network ID. */
    [[nodiscard]] std::uint32_t getPacketId() const { return packet_->pid(); }

    /**
     * Drops the packet.
     *
     * A cancelled incoming packet is never handled by the server; a cancelled outgoing packet is
     * never sent. The packet is not re-encoded, whether or not it was modified.
     */
    void cancel() { cancellable_->cancel(); }

    void setCancelled(bool cancel) { cancellable_->setCancelled(cancel); }

    [[nodiscard]] bool isCancelled() const { return cancellable_->isCancelled(); }

    /** Returns whether a callback has asked for the packet to be re-encoded. */
    [[nodiscard]] bool isModified() const { return modified_; }

    /**
     * Marks the packet as modified without going through mutate().
     *
     * Only needed when something outside the typed API changed the packet.
     */
    void markModified() { modified_ = true; }

protected:
    [[nodiscard]] DataPacket &getPacket() const { return *packet_; }

    bool modified_ = false;

private:
    endstone::Player *player_;
    endstone::ICancellable *cancellable_;
    Direction direction_;
    int sub_client_id_;
    DataPacket *packet_;
};

/**
 * An intercepted packet of a known type.
 *
 * Reading through view() leaves the packet alone, so the server keeps its fast path. Calling
 * mutate() flags the packet for re-encoding, which is comparatively expensive - so only call it
 * when you actually intend to change something.
 */
template <typename TPacket>
class PacketEvent final : public PacketEventBase {
public:
    using PacketEventBase::PacketEventBase;

    /** Returns the decoded packet for inspection, without marking it modified. */
    [[nodiscard]] const TPacket &view() const { return static_cast<const TPacket &>(getPacket()); }

    /** Returns the decoded packet for modification, and marks it to be re-encoded. */
    TPacket &mutate()
    {
        modified_ = true;
        return static_cast<TPacket &>(getPacket());
    }
};

}  // namespace bedrock_protocol::bridge

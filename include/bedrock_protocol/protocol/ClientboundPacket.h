/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ClientboundPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

namespace bedrock_protocol {

/** Marker base indicating that a packet may be sent from the server to the client. */
class ClientboundPacket {
protected:
    ~ClientboundPacket() = default;
};

}  // namespace bedrock_protocol

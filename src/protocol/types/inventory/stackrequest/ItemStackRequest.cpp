/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/stackrequest/ItemStackRequest.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequest.h"

#include <string>

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/BeaconPaymentStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/CraftRecipeAutoStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/CraftRecipeOptionalStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/CraftRecipeStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/CraftingConsumeInputStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/CraftingCreateSpecificResultStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/CreativeCreateStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/DeprecatedCraftingNonImplementedStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/DeprecatedCraftingResultsStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/DestroyStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/DropStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/GrindstoneStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestActionType.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/LabTableCombineStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/LoomStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/MineBlockStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/PlaceStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/SwapStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/TakeStackRequestAction.h"

namespace bedrock_protocol::types::inventory::stackrequest {

using encoding::Byte;
using encoding::LE;
using encoding::VarInt;
using serializer::CommonTypes;

std::unique_ptr<ItemStackRequestAction> ItemStackRequest::readAction(encoding::ByteBufferReader &in,
                                                                    std::int32_t typeId) {
    switch (typeId) {
    case TakeStackRequestAction::ID:
        return std::make_unique<TakeStackRequestAction>(TakeStackRequestAction::read(in));
    case PlaceStackRequestAction::ID:
        return std::make_unique<PlaceStackRequestAction>(PlaceStackRequestAction::read(in));
    case SwapStackRequestAction::ID:
        return std::make_unique<SwapStackRequestAction>(SwapStackRequestAction::read(in));
    case DropStackRequestAction::ID:
        return std::make_unique<DropStackRequestAction>(DropStackRequestAction::read(in));
    case DestroyStackRequestAction::ID:
        return std::make_unique<DestroyStackRequestAction>(DestroyStackRequestAction::read(in));
    case CraftingConsumeInputStackRequestAction::ID:
        return std::make_unique<CraftingConsumeInputStackRequestAction>(
            CraftingConsumeInputStackRequestAction::read(in));
    case CraftingCreateSpecificResultStackRequestAction::ID:
        return std::make_unique<CraftingCreateSpecificResultStackRequestAction>(
            CraftingCreateSpecificResultStackRequestAction::read(in));
    case LabTableCombineStackRequestAction::ID:
        return std::make_unique<LabTableCombineStackRequestAction>(LabTableCombineStackRequestAction::read(in));
    case BeaconPaymentStackRequestAction::ID:
        return std::make_unique<BeaconPaymentStackRequestAction>(BeaconPaymentStackRequestAction::read(in));
    case MineBlockStackRequestAction::ID:
        return std::make_unique<MineBlockStackRequestAction>(MineBlockStackRequestAction::read(in));
    case CraftRecipeStackRequestAction::ID:
        return std::make_unique<CraftRecipeStackRequestAction>(CraftRecipeStackRequestAction::read(in));
    case CraftRecipeAutoStackRequestAction::ID:
        return std::make_unique<CraftRecipeAutoStackRequestAction>(CraftRecipeAutoStackRequestAction::read(in));
    case CreativeCreateStackRequestAction::ID:
        return std::make_unique<CreativeCreateStackRequestAction>(CreativeCreateStackRequestAction::read(in));
    case CraftRecipeOptionalStackRequestAction::ID:
        return std::make_unique<CraftRecipeOptionalStackRequestAction>(CraftRecipeOptionalStackRequestAction::read(in));
    case GrindstoneStackRequestAction::ID:
        return std::make_unique<GrindstoneStackRequestAction>(GrindstoneStackRequestAction::read(in));
    case LoomStackRequestAction::ID:
        return std::make_unique<LoomStackRequestAction>(LoomStackRequestAction::read(in));
    case DeprecatedCraftingNonImplementedStackRequestAction::ID:
        return std::make_unique<DeprecatedCraftingNonImplementedStackRequestAction>(
            DeprecatedCraftingNonImplementedStackRequestAction::read(in));
    case DeprecatedCraftingResultsStackRequestAction::ID:
        return std::make_unique<DeprecatedCraftingResultsStackRequestAction>(
            DeprecatedCraftingResultsStackRequestAction::read(in));
    default:
        throw PacketDecodeException("Unhandled item stack request action type " + std::to_string(typeId));
    }
}

ItemStackRequest ItemStackRequest::read(encoding::ByteBufferReader &in) {
    const auto requestId = CommonTypes::readItemStackRequestId(in);
    std::vector<std::unique_ptr<ItemStackRequestAction>> actions;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        //gophertunnel minecraft/protocol/writer.go:394-403 and item_stack.go:47-52 - each action is now
        //prefixed by a variant, which is the action ID with the two never-sent container actions squeezed
        //out. It carries no information the following byte does not, so it is read and discarded.
        VarInt::readUnsignedInt(in);
        const auto typeId = Byte::readUnsigned(in);
        actions.push_back(readAction(in, typeId));
    }
    std::vector<std::string> filterStrings;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        filterStrings.push_back(std::string(CommonTypes::getString(in)));
    }
    const auto filterStringCause = LE::readSignedInt(in);
    return ItemStackRequest(requestId, std::move(actions), std::move(filterStrings), filterStringCause);
}

void ItemStackRequest::write(encoding::ByteBufferWriter &out) const {
    CommonTypes::writeItemStackRequestId(out, requestId);
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(actions.size()));
    for (const auto &action : actions) {
        const auto typeId = action->getTypeId();
        VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(
                                          typeId > ItemStackRequestActionType::TAKE_OUT_CONTAINER ? typeId - 2
                                                                                                  : typeId));
        Byte::writeUnsigned(out, static_cast<std::uint8_t>(typeId));
        action->write(out);
    }
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(filterStrings.size()));
    for (const auto &string : filterStrings) {
        CommonTypes::putString(out, string);
    }
    LE::writeSignedInt(out, filterStringCause);
}

ItemStackRequest ItemStackRequest::clone() const {
    std::vector<std::unique_ptr<ItemStackRequestAction>> clonedActions;
    clonedActions.reserve(actions.size());
    for (const auto &action : actions) {
        clonedActions.push_back(action->clone());
    }
    return ItemStackRequest(requestId, std::move(clonedActions), filterStrings, filterStringCause);
}

}  // namespace bedrock_protocol::types::inventory::stackrequest

/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/serializer/CommonTypes.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <utility>

#include "bedrock_protocol/nbt/NbtDataException.h"
#include "bedrock_protocol/protocol/serializer/NetworkNbtSerializer.h"
#include "bedrock_protocol/protocol/types/BoolGameRule.h"
#include "bedrock_protocol/protocol/types/FloatGameRule.h"
#include "bedrock_protocol/protocol/types/IntGameRule.h"
#include "bedrock_protocol/protocol/types/NullGameRule.h"
#include "bedrock_protocol/protocol/types/recipe/ItemDescriptorType.h"
#include "bedrock_protocol/protocol/types/skin/PersonaPieceTintColor.h"
#include "bedrock_protocol/protocol/types/skin/PersonaSkinPiece.h"
#include "bedrock_protocol/protocol/types/skin/SkinAnimation.h"
#include "bedrock_protocol/protocol/types/skin/SkinImage.h"

namespace bedrock_protocol::serializer {

using encoding::Byte;
using encoding::LE;
using encoding::VarInt;

namespace {

/** @throws DataDecodeException */
types::skin::SkinImage getSkinImage(encoding::ByteBufferReader &in)
{
    const auto width = LE::readUnsignedInt(in);
    const auto height = LE::readUnsignedInt(in);
    auto data = CommonTypes::getString(in);
    try {
        return types::skin::SkinImage(height, width, std::move(data));
    }
    catch (const std::invalid_argument &e) {
        throw PacketDecodeException(e.what());
    }
}

void putSkinImage(encoding::ByteBufferWriter &out, const types::skin::SkinImage &image)
{
    LE::writeUnsignedInt(out, image.getWidth());
    LE::writeUnsignedInt(out, image.getHeight());
    CommonTypes::putString(out, image.getData());
}

/**
 * @note The PHP original returns a 3-element list; this port returns the fields as a struct so that
 *       each one keeps the width its wire function implies.
 */
struct ItemStackHeader {
    std::int32_t id;
    std::uint16_t count;
    std::uint32_t meta;
};

/** @throws DataDecodeException */
ItemStackHeader getItemStackHeader(encoding::ByteBufferReader &in)
{
    //gophertunnel minecraft/protocol/reader.go:409-417 - every header field is present even for the air item.
    const auto id = VarInt::readSignedInt(in);
    const auto count = LE::readUnsignedShort(in);
    const auto meta = VarInt::readUnsignedInt(in);

    return {id, count, meta};
}

void putItemStackHeader(encoding::ByteBufferWriter &out, const types::inventory::ItemStack &itemStack)
{
    //gophertunnel minecraft/protocol/writer.go:332-339 - every header field is written even for the air item.
    VarInt::writeSignedInt(out, itemStack.getId());
    LE::writeUnsignedShort(out, itemStack.getCount());
    VarInt::writeUnsignedInt(out, itemStack.getMeta());
}

/** @throws DataDecodeException */
types::inventory::ItemStack getItemStackFooter(encoding::ByteBufferReader &in, std::int32_t id, std::uint32_t meta,
                                               std::uint16_t count)
{
    const auto blockRuntimeId = VarInt::readSignedInt(in);
    auto rawExtraData = CommonTypes::getString(in);

    return {id, meta, count, blockRuntimeId, std::move(rawExtraData)};
}

void putItemStackFooter(encoding::ByteBufferWriter &out, const types::inventory::ItemStack &itemStack)
{
    VarInt::writeSignedInt(out, itemStack.getBlockRuntimeId());
    //gophertunnel minecraft/protocol/writer.go:339 and :360-365 - the user-data blob is suppressed for the air
    //item, but its varuint32 length prefix is still written as zero.
    CommonTypes::putString(out, itemStack.getId() != 0 ? itemStack.getRawExtraData() : std::string());
}

/**
 * Converts the persona_* piece type names used in login data to the shorter names used by the v2168 skin
 * codec. Mirror of gophertunnel minecraft/protocol/skin.go:263-268.
 */
std::string personaPieceTintWireType(const std::string &pieceType)
{
    if (pieceType == "persona_hand") {
        return "hands";
    }
    static constexpr std::string_view PREFIX = "persona_";
    if (pieceType.rfind(PREFIX, 0) == 0) {
        return pieceType.substr(PREFIX.size());
    }
    return pieceType;
}

/**
 * Converts v2168 wire piece type names back to the persona_* names used in login data. Mirror of
 * gophertunnel minecraft/protocol/skin.go:270-279.
 */
std::string personaPieceTintLoginType(const std::string &pieceType)
{
    if (pieceType == "hands") {
        return "persona_hand";
    }
    if (pieceType == "unsupported") {
        return pieceType;
    }
    return "persona_" + pieceType;
}

/** @throws DataDecodeException */
std::unique_ptr<types::GameRule> readGameRule(encoding::ByteBufferReader &in, std::uint32_t type,
                                              bool isPlayerModifiable)
{
    switch (static_cast<std::int32_t>(type)) {
    case types::NullGameRule::ID:
        return std::make_unique<types::NullGameRule>(types::NullGameRule::decode(in, isPlayerModifiable));
    case types::BoolGameRule::ID:
        return std::make_unique<types::BoolGameRule>(types::BoolGameRule::decode(in, isPlayerModifiable));
    case types::IntGameRule::ID:
        return std::make_unique<types::IntGameRule>(types::IntGameRule::decode(in, isPlayerModifiable));
    case types::FloatGameRule::ID:
        return std::make_unique<types::FloatGameRule>(types::FloatGameRule::decode(in, isPlayerModifiable));
    default:
        throw PacketDecodeException("Unknown gamerule type " + std::to_string(type));
    }
}

/**
 * The wire name each descriptor type is written under inside an ItemDescriptorCount.
 * Mirror of gophertunnel minecraft/protocol/item_descriptor.go:64-76.
 */
std::string itemDescriptorTagName(std::uint8_t descriptorType)
{
    switch (descriptorType) {
    case types::recipe::ItemDescriptorType::STRING_ID_META:
        return "name";
    case types::recipe::ItemDescriptorType::MOLANG:
        return "molang";
    case types::recipe::ItemDescriptorType::TAG:
        return "item_tag";
    default:
        return "empty";
    }
}

/**
 * Inverse of itemDescriptorTagName. Mirror of gophertunnel minecraft/protocol/reader.go:362-375.
 * @throws PacketDecodeException
 */
std::uint8_t itemDescriptorTypeFromTagName(std::string_view name)
{
    if (name == "name") {
        return types::recipe::ItemDescriptorType::STRING_ID_META;
    }
    if (name == "molang") {
        return types::recipe::ItemDescriptorType::MOLANG;
    }
    if (name == "item_tag") {
        return types::recipe::ItemDescriptorType::TAG;
    }
    throw PacketDecodeException("Unknown item descriptor type " + std::string(name));
}

}  // namespace

std::string CommonTypes::getString(encoding::ByteBufferReader &in)
{
    return std::string(in.readByteArray(VarInt::readUnsignedInt(in)));
}

void CommonTypes::putString(encoding::ByteBufferWriter &out, std::string_view v)
{
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(v.size()));
    out.writeByteArray(v);
}

bool CommonTypes::getBool(encoding::ByteBufferReader &in)
{
    return Byte::readUnsigned(in) != 0;
}

void CommonTypes::putBool(encoding::ByteBufferWriter &out, bool v)
{
    Byte::writeUnsigned(out, v ? 1 : 0);
}

uuid::Uuid CommonTypes::getUUID(encoding::ByteBufferReader &in)
{
    // This is two little-endian longs: bytes 7-0 followed by bytes 15-8
    std::string bytes;
    bytes.reserve(16);
    for (int half = 0; half < 2; ++half) {
        const auto part = in.readByteArray(8);
        bytes.append(part.rbegin(), part.rend());
    }
    return uuid::Uuid::fromBytes(bytes);
}

void CommonTypes::putUUID(encoding::ByteBufferWriter &out, const uuid::Uuid &uuid)
{
    const auto bytes = uuid.getBytes();
    for (int half = 0; half < 2; ++half) {
        std::string part(bytes.substr(static_cast<std::size_t>(half) * 8, 8));
        std::reverse(part.begin(), part.end());
        out.writeByteArray(part);
    }
}

types::skin::SkinData CommonTypes::getSkin(encoding::ByteBufferReader &in)
{
    auto skinId = getString(in);
    auto skinPlayFabId = getString(in);
    auto skinResourcePatch = getString(in);
    auto skinData = getSkinImage(in);
    const auto animationCount = VarInt::readUnsignedInt(in);
    std::vector<types::skin::SkinAnimation> animations;
    for (std::uint32_t i = 0; i < animationCount; ++i) {
        auto skinImage = getSkinImage(in);
        const auto animationType = VarInt::readUnsignedInt(in);
        const auto animationFrames = LE::readFloat(in);
        const auto expressionType = VarInt::readUnsignedInt(in);
        animations.emplace_back(std::move(skinImage), animationType, animationFrames, expressionType);
    }
    auto capeData = getSkinImage(in);
    auto geometryData = getString(in);
    auto geometryDataVersion = getString(in);
    auto animationData = getString(in);
    auto capeId = getString(in);
    auto fullSkinId = getString(in);
    const auto armSize = Byte::readUnsigned(in);
    const auto skinColor = color::Color::fromARGB(LE::readUnsignedInt(in));
    const auto personaPieceCount = VarInt::readUnsignedInt(in);
    std::vector<types::skin::PersonaSkinPiece> personaPieces;
    for (std::uint32_t i = 0; i < personaPieceCount; ++i) {
        auto pieceId = getString(in);
        const auto pieceType = LE::readUnsignedInt(in);
        const auto packId = getUUID(in);
        const auto isDefaultPiece = getBool(in);
        auto productId = getString(in);
        personaPieces.emplace_back(std::move(pieceId), pieceType, packId, isDefaultPiece, std::move(productId));
    }
    const auto pieceTintColorCount = VarInt::readUnsignedInt(in);
    std::vector<types::skin::PersonaPieceTintColor> pieceTintColors;
    for (std::uint32_t i = 0; i < pieceTintColorCount; ++i) {
        auto pieceType = personaPieceTintLoginType(getString(in));
        types::skin::PersonaPieceTintColor::Colors colors{};
        for (auto &tintColor : colors) {
            tintColor = color::Color::fromARGB(LE::readUnsignedInt(in));
        }
        pieceTintColors.emplace_back(std::move(pieceType), colors);
    }

    const auto premium = getBool(in);
    const auto persona = getBool(in);
    const auto capeOnClassic = getBool(in);
    const auto isPrimaryUser = getBool(in);
    const auto override = getBool(in);
    //trusted is a "true"/"false" string, compared case-insensitively on read (skin.go:113-118)
    auto trusted = getString(in);
    std::transform(trusted.begin(), trusted.end(), trusted.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    auto profileHash = getString(in);

    return {
        std::move(skinId),
        std::move(skinPlayFabId),
        std::move(skinResourcePatch),
        std::move(skinData),
        std::move(animations),
        std::move(capeData),
        std::move(geometryData),
        std::move(geometryDataVersion),
        std::move(animationData),
        std::move(capeId),
        std::move(fullSkinId),
        armSize,
        skinColor,
        std::move(personaPieces),
        std::move(pieceTintColors),
        trusted == "true",
        premium,
        persona,
        capeOnClassic,
        isPrimaryUser,
        override,
        std::move(profileHash),
    };
}

void CommonTypes::putSkin(encoding::ByteBufferWriter &out, const types::skin::SkinData &skin)
{
    putString(out, skin.getSkinId());
    putString(out, skin.getPlayFabId());
    putString(out, skin.getResourcePatch());
    putSkinImage(out, skin.getSkinImage());
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(skin.getAnimations().size()));
    for (const auto &animation : skin.getAnimations()) {
        putSkinImage(out, animation.getImage());
        VarInt::writeUnsignedInt(out, animation.getType());
        LE::writeFloat(out, animation.getFrames());
        VarInt::writeUnsignedInt(out, animation.getExpressionType());
    }
    putSkinImage(out, skin.getCapeImage());
    putString(out, skin.getGeometryData());
    putString(out, skin.getGeometryDataEngineVersion());
    putString(out, skin.getAnimationData());
    putString(out, skin.getCapeId());
    putString(out, skin.getFullSkinId());
    Byte::writeUnsigned(out, skin.getArmSize());
    LE::writeUnsignedInt(out, skin.getSkinColor().toARGB());
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(skin.getPersonaPieces().size()));
    for (const auto &piece : skin.getPersonaPieces()) {
        putString(out, piece.getPieceId());
        LE::writeUnsignedInt(out, piece.getPieceType());
        putUUID(out, piece.getPackId());
        putBool(out, piece.isDefaultPiece());
        putString(out, piece.getProductId());
    }
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(skin.getPieceTintColors().size()));
    for (const auto &tint : skin.getPieceTintColors()) {
        putString(out, personaPieceTintWireType(tint.getPieceType()));
        for (const auto &tintColor : tint.getColors()) {
            LE::writeUnsignedInt(out, tintColor.toARGB());
        }
    }
    putBool(out, skin.isPremium());
    putBool(out, skin.isPersona());
    putBool(out, skin.isPersonaCapeOnClassic());
    putBool(out, skin.isPrimaryUser());
    putBool(out, skin.isOverride());
    putString(out, skin.isVerified() ? "true" : "false");
    putString(out, skin.getProfileHash());
}

types::inventory::ItemStack CommonTypes::getItemStackWithoutStackId(encoding::ByteBufferReader &in)
{
    const auto [id, count, meta] = getItemStackHeader(in);

    return getItemStackFooter(in, id, meta, count);
}

void CommonTypes::putItemStackWithoutStackId(encoding::ByteBufferWriter &out,
                                             const types::inventory::ItemStack &itemStack)
{
    putItemStackHeader(out, itemStack);
    putItemStackFooter(out, itemStack);
}

types::inventory::ItemStackWrapper CommonTypes::getNetworkItemStackDescriptor(encoding::ByteBufferReader &in)
{
    const auto id = LE::readSignedShort(in);
    const auto count = LE::readUnsignedShort(in);
    const auto meta = VarInt::readUnsignedInt(in);

    //gophertunnel minecraft/protocol/reader.go:396-403 - the extra stack-ID variant varuint32 is gone.
    const auto hasNetId = getBool(in);
    const auto stackId = hasNetId ? VarInt::readSignedInt(in) : 0;

    const auto blockRuntimeId = VarInt::readUnsignedInt(in);
    auto rawExtraData = getString(in);

    return {stackId, types::inventory::ItemStack(id, meta, count, static_cast<std::int32_t>(blockRuntimeId),
                                                 std::move(rawExtraData))};
}

void CommonTypes::putNetworkItemStackDescriptor(encoding::ByteBufferWriter &out,
                                                const types::inventory::ItemStackWrapper &itemStackWrapper)
{
    LE::writeSignedShort(out, static_cast<std::int16_t>(itemStackWrapper.getItemStack().getId()));
    LE::writeUnsignedShort(out, itemStackWrapper.getItemStack().getCount());
    VarInt::writeUnsignedInt(out, itemStackWrapper.getItemStack().getMeta());

    //gophertunnel minecraft/protocol/writer.go:318-325 - the extra stack-ID variant varuint32 is gone.
    const auto hasNetId = itemStackWrapper.getStackId() != 0;
    putBool(out, hasNetId);
    if (hasNetId) {
        VarInt::writeSignedInt(out, itemStackWrapper.getStackId());
    }

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(itemStackWrapper.getItemStack().getBlockRuntimeId()));
    putString(out, itemStackWrapper.getItemStack().getRawExtraData());
}

types::inventory::StackRequestItem CommonTypes::getStackRequestItem(encoding::ByteBufferReader &in)
{
    //gophertunnel minecraft/protocol/reader.go:423-444.
    const auto variant = VarInt::readUnsignedInt(in);
    Byte::readUnsigned(in);  //legacy copy of the variant
    const auto hasItem = variant == types::recipe::ItemDescriptorType::STRING_ID_META;
    if (variant != types::recipe::ItemDescriptorType::INVALID && !hasItem) {
        throw PacketDecodeException("Unknown stack request item descriptor variant " + std::to_string(variant));
    }

    std::string identifier;
    std::int32_t meta = 0;
    if (hasItem) {
        identifier = getString(in);
        meta = VarInt::readSignedInt(in);
    }

    const auto count = LE::readSignedShort(in);
    const auto blockRuntimeId = VarInt::readUnsignedInt(in);
    auto rawExtraData = getString(in);

    return {std::move(identifier), meta, static_cast<std::int32_t>(blockRuntimeId),
            static_cast<std::uint16_t>(count), std::move(rawExtraData)};
}

void CommonTypes::putStackRequestItem(encoding::ByteBufferWriter &out, const types::inventory::StackRequestItem &item)
{
    //gophertunnel minecraft/protocol/writer.go:342-358.
    const auto hasItem = !item.getIdentifier().empty();
    const auto variant = static_cast<std::uint32_t>(hasItem ? types::recipe::ItemDescriptorType::STRING_ID_META
                                                            : types::recipe::ItemDescriptorType::INVALID);
    VarInt::writeUnsignedInt(out, variant);
    Byte::writeUnsigned(out, static_cast<std::uint8_t>(variant));
    if (hasItem) {
        putString(out, item.getIdentifier());
        VarInt::writeSignedInt(out, item.getMeta());
    }

    LE::writeSignedShort(out, static_cast<std::int16_t>(item.getCount()));
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(item.getBlockRuntimeId()));
    //The user-data blob is suppressed when there is no item, but its length prefix is still written as zero.
    putString(out, hasItem ? item.getRawExtraData() : std::string());
}

types::recipe::RecipeIngredient CommonTypes::getRecipeIngredient(encoding::ByteBufferReader &in)
{
    //gophertunnel minecraft/protocol/reader.go:351-385 - the framing is a clamped variant, then either an
    //int32 filler (invalid) or the tag name plus the descriptor body, and finally the count.
    const auto variant = VarInt::readUnsignedInt(in);
    std::unique_ptr<types::recipe::ItemDescriptor> descriptor;
    if (variant == types::recipe::ItemDescriptorType::INVALID) {
        VarInt::readSignedInt(in);
    }
    else {
        if (variant != types::recipe::ItemDescriptorType::STRING_ID_META) {
            throw PacketDecodeException("Unknown item descriptor variant " + std::to_string(variant));
        }
        const auto descriptorType = itemDescriptorTypeFromTagName(getString(in));
        descriptor = types::recipe::ItemDescriptor::read(in, descriptorType);
        if (descriptorType == types::recipe::ItemDescriptorType::TAG) {
            VarInt::readSignedInt(in);
        }
    }
    const auto count = VarInt::readSignedInt(in);

    return {std::move(descriptor), count};
}

void CommonTypes::putRecipeIngredient(encoding::ByteBufferWriter &out, const types::recipe::RecipeIngredient &ingredient)
{
    //gophertunnel minecraft/protocol/writer.go:286-309.
    const auto *type = ingredient.getDescriptor();
    const auto descriptorType = type != nullptr ? type->getTypeId() : types::recipe::ItemDescriptorType::INVALID;

    VarInt::writeUnsignedInt(out, descriptorType > types::recipe::ItemDescriptorType::STRING_ID_META
                                      ? types::recipe::ItemDescriptorType::STRING_ID_META
                                      : descriptorType);
    if (descriptorType == types::recipe::ItemDescriptorType::INVALID) {
        VarInt::writeSignedInt(out, 32767);
    }
    else {
        putString(out, itemDescriptorTagName(descriptorType));
        type->write(out);
        if (descriptorType == types::recipe::ItemDescriptorType::TAG) {
            VarInt::writeSignedInt(out, 32767);
        }
    }

    VarInt::writeSignedInt(out, ingredient.getCount());
}

types::recipe::RecipeIngredient CommonTypes::getStackRequestRecipeIngredient(encoding::ByteBufferReader &in)
{
    //gophertunnel minecraft/protocol/item_descriptor.go:94-123 - no tag string, and the count is a fixed uint16.
    const auto variant = VarInt::readUnsignedInt(in);
    Byte::readUnsigned(in);  //legacy copy of the variant
    if (variant > types::recipe::ItemDescriptorType::TAG) {
        throw PacketDecodeException("Unknown stack request item descriptor type " + std::to_string(variant));
    }
    auto descriptor = types::recipe::ItemDescriptor::read(in, static_cast<std::uint8_t>(variant));
    const auto count = LE::readUnsignedShort(in);

    return {std::move(descriptor), static_cast<std::int32_t>(count)};
}

void CommonTypes::putStackRequestRecipeIngredient(encoding::ByteBufferWriter &out,
                                                  const types::recipe::RecipeIngredient &ingredient)
{
    //gophertunnel minecraft/protocol/item_descriptor.go:94-123.
    const auto *type = ingredient.getDescriptor();
    const auto descriptorType = type != nullptr ? type->getTypeId() : types::recipe::ItemDescriptorType::INVALID;

    VarInt::writeUnsignedInt(out, descriptorType);
    Byte::writeUnsigned(out, descriptorType);
    if (type != nullptr) {
        type->write(out);
    }
    LE::writeUnsignedShort(out, static_cast<std::uint16_t>(ingredient.getCount()));
}

CommonTypes::EntityMetadata CommonTypes::getEntityMetadata(encoding::ByteBufferReader &in)
{
    const auto count = VarInt::readUnsignedInt(in);
    EntityMetadata data;
    for (std::uint32_t i = 0; i < count; ++i) {
        const auto key = VarInt::readUnsignedInt(in);
        const auto type = VarInt::readUnsignedInt(in);

        assignKeyed(data, key, types::entity::MetadataProperty::read(in, static_cast<std::int32_t>(type)));
    }

    return data;
}

void CommonTypes::putEntityMetadata(encoding::ByteBufferWriter &out, const EntityMetadata &metadata)
{
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(metadata.size()));
    for (const auto &[key, d] : metadata) {
        VarInt::writeUnsignedInt(out, key);
        VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(d->getTypeId()));
        d->write(out);
    }
}

CommonTypes::GameRules CommonTypes::getGameRules(encoding::ByteBufferReader &in)
{
    const auto count = VarInt::readUnsignedInt(in);
    GameRules rules;
    for (std::uint32_t i = 0; i < count; ++i) {
        auto name = getString(in);
        const auto isPlayerModifiable = getBool(in);
        const auto type = VarInt::readUnsignedInt(in);
        assignKeyed(rules, name, readGameRule(in, type, isPlayerModifiable));
    }

    return rules;
}

void CommonTypes::putGameRules(encoding::ByteBufferWriter &out, const GameRules &rules)
{
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(rules.size()));
    for (const auto &[name, rule] : rules) {
        putString(out, name);
        putBool(out, rule->isPlayerModifiable());
        VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(rule->getTypeId()));
        rule->encode(out);
    }
}

types::entity::EntityLink CommonTypes::getEntityLink(encoding::ByteBufferReader &in)
{
    const auto fromActorUniqueId = getActorUniqueId(in);
    const auto toActorUniqueId = getActorUniqueId(in);
    const auto type = Byte::readUnsigned(in);
    const auto immediate = getBool(in);
    const auto causedByRider = getBool(in);
    const auto vehicleAngularVelocity = LE::readFloat(in);
    return {fromActorUniqueId, toActorUniqueId, type, immediate, causedByRider, vehicleAngularVelocity};
}

void CommonTypes::putEntityLink(encoding::ByteBufferWriter &out, const types::entity::EntityLink &link)
{
    putActorUniqueId(out, link.fromActorUniqueId);
    putActorUniqueId(out, link.toActorUniqueId);
    Byte::writeUnsigned(out, link.type);
    putBool(out, link.immediate);
    putBool(out, link.causedByRider);
    LE::writeFloat(out, link.vehicleAngularVelocity);
}

types::command::CommandOriginData CommonTypes::getCommandOriginData(encoding::ByteBufferReader &in)
{
    types::command::CommandOriginData result;

    result.type = CommonTypes::getString(in);
    result.uuid = getUUID(in);
    result.requestId = getString(in);
    result.playerActorUniqueId = LE::readSignedLong(in);

    return result;
}

void CommonTypes::putCommandOriginData(encoding::ByteBufferWriter &out, const types::command::CommandOriginData &data)
{
    putString(out, data.type);
    putUUID(out, data.uuid);
    putString(out, data.requestId);
    LE::writeSignedLong(out, data.playerActorUniqueId);
}

types::StructureSettings CommonTypes::getStructureSettings(encoding::ByteBufferReader &in)
{
    types::StructureSettings result;

    result.paletteName = getString(in);

    result.ignoreEntities = getBool(in);
    result.ignoreBlocks = getBool(in);
    result.allowNonTickingChunks = getBool(in);

    result.dimensions = getBlockPosition(in);
    result.offset = getBlockPosition(in);

    result.lastTouchedByPlayerID = getActorUniqueId(in);
    result.rotation = Byte::readUnsigned(in);
    result.mirror = Byte::readUnsigned(in);
    result.animationMode = Byte::readUnsigned(in);
    result.animationSeconds = LE::readFloat(in);
    result.integrityValue = LE::readFloat(in);
    result.integritySeed = LE::readUnsignedInt(in);
    result.pivot = getVector3(in);

    return result;
}

void CommonTypes::putStructureSettings(encoding::ByteBufferWriter &out,
                                       const types::StructureSettings &structureSettings)
{
    putString(out, structureSettings.paletteName);

    putBool(out, structureSettings.ignoreEntities);
    putBool(out, structureSettings.ignoreBlocks);
    putBool(out, structureSettings.allowNonTickingChunks);

    putBlockPosition(out, structureSettings.dimensions);
    putBlockPosition(out, structureSettings.offset);

    putActorUniqueId(out, structureSettings.lastTouchedByPlayerID);
    Byte::writeUnsigned(out, structureSettings.rotation);
    Byte::writeUnsigned(out, structureSettings.mirror);
    Byte::writeUnsigned(out, structureSettings.animationMode);
    LE::writeFloat(out, structureSettings.animationSeconds);
    LE::writeFloat(out, structureSettings.integrityValue);
    LE::writeUnsignedInt(out, structureSettings.integritySeed);
    putVector3(out, structureSettings.pivot);
}

types::StructureEditorData CommonTypes::getStructureEditorData(encoding::ByteBufferReader &in)
{
    types::StructureEditorData result;

    result.structureName = getString(in);
    result.filteredStructureName = getString(in);
    result.structureDataField = getString(in);

    result.includePlayers = getBool(in);
    result.showBoundingBox = getBool(in);

    result.structureBlockType = VarInt::readSignedInt(in);
    result.structureSettings = getStructureSettings(in);
    result.structureRedstoneSaveMode = Byte::readUnsigned(in);

    return result;
}

void CommonTypes::putStructureEditorData(encoding::ByteBufferWriter &out,
                                         const types::StructureEditorData &structureEditorData)
{
    putString(out, structureEditorData.structureName);
    putString(out, structureEditorData.filteredStructureName);
    putString(out, structureEditorData.structureDataField);

    putBool(out, structureEditorData.includePlayers);
    putBool(out, structureEditorData.showBoundingBox);

    VarInt::writeSignedInt(out, structureEditorData.structureBlockType);
    putStructureSettings(out, structureEditorData.structureSettings);
    Byte::writeUnsigned(out, structureEditorData.structureRedstoneSaveMode);
}

types::BlockPosition CommonTypes::getBlockPosition(encoding::ByteBufferReader &in)
{
    const auto x = VarInt::readSignedInt(in);
    const auto y = VarInt::readSignedInt(in);
    const auto z = VarInt::readSignedInt(in);
    return {x, y, z};
}

void CommonTypes::putBlockPosition(encoding::ByteBufferWriter &out, const types::BlockPosition &blockPosition)
{
    VarInt::writeSignedInt(out, blockPosition.getX());
    VarInt::writeSignedInt(out, blockPosition.getY());
    VarInt::writeSignedInt(out, blockPosition.getZ());
}

nbt::TreeRoot CommonTypes::getNbtRoot(encoding::ByteBufferReader &in)
{
    try {
        return NetworkNbtSerializer().read(in, 512);
    }
    catch (const nbt::NbtDataException &e) {
        throw PacketDecodeException::wrap(e, "Failed decoding NBT root");
    }
}

nbt::tag::CompoundTag CommonTypes::getNbtCompoundRoot(encoding::ByteBufferReader &in)
{
    try {
        return getNbtRoot(in).mustGetCompoundTag();
    }
    catch (const nbt::NbtDataException &e) {
        throw PacketDecodeException::wrap(e, "Expected TAG_Compound NBT root");
    }
}

}  // namespace bedrock_protocol::serializer

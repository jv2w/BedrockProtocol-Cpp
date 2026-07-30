/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ProtocolInfo.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string_view>

namespace bedrock_protocol {

/**
 * Version numbers and packet IDs for the current Minecraft PE protocol
 *
 * @note One deviation from PHP: the constant PHP calls MINECRAFT_VERSION is MINECRAFT_VERSION_DISPLAY
 *       here. Endstone's detail.h defines MINECRAFT_VERSION as an object-like macro, so the original
 *       name would be substituted away at every use site. Renaming is preferable to #undef, which
 *       would silently leak into every header included afterwards.
 *
 * This list was generated from the PHP original when the port was made. That generator is gone and upstream is archived, so a new packet is added here by hand.
 */
struct ProtocolInfo final {
    ProtocolInfo() = delete;

    /** Actual Minecraft: PE protocol version */
    static constexpr std::int32_t CURRENT_PROTOCOL = 1001;
    /** Display version shown in the server logs. This should match the version on the game's home screen. */
    static constexpr std::string_view MINECRAFT_VERSION_DISPLAY = "v26.30";
    /** Version sent on the network for client side compatibility checks. This may differ from the display version. */
    static constexpr std::string_view MINECRAFT_VERSION_NETWORK = "1.26.30";
    static constexpr std::uint32_t LOGIN_PACKET = 0x01;
    static constexpr std::uint32_t PLAY_STATUS_PACKET = 0x02;
    static constexpr std::uint32_t SERVER_TO_CLIENT_HANDSHAKE_PACKET = 0x03;
    static constexpr std::uint32_t CLIENT_TO_SERVER_HANDSHAKE_PACKET = 0x04;
    static constexpr std::uint32_t DISCONNECT_PACKET = 0x05;
    static constexpr std::uint32_t RESOURCE_PACKS_INFO_PACKET = 0x06;
    static constexpr std::uint32_t RESOURCE_PACK_STACK_PACKET = 0x07;
    static constexpr std::uint32_t RESOURCE_PACK_CLIENT_RESPONSE_PACKET = 0x08;
    static constexpr std::uint32_t TEXT_PACKET = 0x09;
    static constexpr std::uint32_t SET_TIME_PACKET = 0x0a;
    static constexpr std::uint32_t START_GAME_PACKET = 0x0b;
    static constexpr std::uint32_t ADD_PLAYER_PACKET = 0x0c;
    static constexpr std::uint32_t ADD_ACTOR_PACKET = 0x0d;
    static constexpr std::uint32_t REMOVE_ACTOR_PACKET = 0x0e;
    static constexpr std::uint32_t ADD_ITEM_ACTOR_PACKET = 0x0f;
    static constexpr std::uint32_t SERVER_PLAYER_POST_MOVE_POSITION_PACKET = 0x10;
    static constexpr std::uint32_t TAKE_ITEM_ACTOR_PACKET = 0x11;
    static constexpr std::uint32_t MOVE_ACTOR_ABSOLUTE_PACKET = 0x12;
    static constexpr std::uint32_t MOVE_PLAYER_PACKET = 0x13;
    static constexpr std::uint32_t UPDATE_BLOCK_PACKET = 0x15;
    static constexpr std::uint32_t ADD_PAINTING_PACKET = 0x16;
    static constexpr std::uint32_t LEVEL_EVENT_PACKET = 0x19;
    static constexpr std::uint32_t BLOCK_EVENT_PACKET = 0x1a;
    static constexpr std::uint32_t ACTOR_EVENT_PACKET = 0x1b;
    static constexpr std::uint32_t MOB_EFFECT_PACKET = 0x1c;
    static constexpr std::uint32_t UPDATE_ATTRIBUTES_PACKET = 0x1d;
    static constexpr std::uint32_t INVENTORY_TRANSACTION_PACKET = 0x1e;
    static constexpr std::uint32_t MOB_EQUIPMENT_PACKET = 0x1f;
    static constexpr std::uint32_t MOB_ARMOR_EQUIPMENT_PACKET = 0x20;
    static constexpr std::uint32_t INTERACT_PACKET = 0x21;
    static constexpr std::uint32_t BLOCK_PICK_REQUEST_PACKET = 0x22;
    static constexpr std::uint32_t ACTOR_PICK_REQUEST_PACKET = 0x23;
    static constexpr std::uint32_t PLAYER_ACTION_PACKET = 0x24;
    static constexpr std::uint32_t HURT_ARMOR_PACKET = 0x26;
    static constexpr std::uint32_t SET_ACTOR_DATA_PACKET = 0x27;
    static constexpr std::uint32_t SET_ACTOR_MOTION_PACKET = 0x28;
    static constexpr std::uint32_t SET_ACTOR_LINK_PACKET = 0x29;
    static constexpr std::uint32_t SET_HEALTH_PACKET = 0x2a;
    static constexpr std::uint32_t SET_SPAWN_POSITION_PACKET = 0x2b;
    static constexpr std::uint32_t ANIMATE_PACKET = 0x2c;
    static constexpr std::uint32_t RESPAWN_PACKET = 0x2d;
    static constexpr std::uint32_t CONTAINER_OPEN_PACKET = 0x2e;
    static constexpr std::uint32_t CONTAINER_CLOSE_PACKET = 0x2f;
    static constexpr std::uint32_t PLAYER_HOTBAR_PACKET = 0x30;
    static constexpr std::uint32_t INVENTORY_CONTENT_PACKET = 0x31;
    static constexpr std::uint32_t INVENTORY_SLOT_PACKET = 0x32;
    static constexpr std::uint32_t CONTAINER_SET_DATA_PACKET = 0x33;
    static constexpr std::uint32_t CRAFTING_DATA_PACKET = 0x34;
    static constexpr std::uint32_t GUI_DATA_PICK_ITEM_PACKET = 0x36;
    static constexpr std::uint32_t BLOCK_ACTOR_DATA_PACKET = 0x38;
    static constexpr std::uint32_t LEVEL_CHUNK_PACKET = 0x3a;
    static constexpr std::uint32_t SET_COMMANDS_ENABLED_PACKET = 0x3b;
    static constexpr std::uint32_t SET_DIFFICULTY_PACKET = 0x3c;
    static constexpr std::uint32_t CHANGE_DIMENSION_PACKET = 0x3d;
    static constexpr std::uint32_t SET_PLAYER_GAME_TYPE_PACKET = 0x3e;
    static constexpr std::uint32_t PLAYER_LIST_PACKET = 0x3f;
    static constexpr std::uint32_t SIMPLE_EVENT_PACKET = 0x40;
    static constexpr std::uint32_t LEGACY_TELEMETRY_EVENT_PACKET = 0x41;
    static constexpr std::uint32_t SPAWN_EXPERIENCE_ORB_PACKET = 0x42;
    static constexpr std::uint32_t CLIENTBOUND_MAP_ITEM_DATA_PACKET = 0x43;
    static constexpr std::uint32_t MAP_INFO_REQUEST_PACKET = 0x44;
    static constexpr std::uint32_t REQUEST_CHUNK_RADIUS_PACKET = 0x45;
    static constexpr std::uint32_t CHUNK_RADIUS_UPDATED_PACKET = 0x46;
    static constexpr std::uint32_t GAME_RULES_CHANGED_PACKET = 0x48;
    static constexpr std::uint32_t CAMERA_PACKET = 0x49;
    static constexpr std::uint32_t BOSS_EVENT_PACKET = 0x4a;
    static constexpr std::uint32_t SHOW_CREDITS_PACKET = 0x4b;
    static constexpr std::uint32_t AVAILABLE_COMMANDS_PACKET = 0x4c;
    static constexpr std::uint32_t COMMAND_REQUEST_PACKET = 0x4d;
    static constexpr std::uint32_t COMMAND_BLOCK_UPDATE_PACKET = 0x4e;
    static constexpr std::uint32_t COMMAND_OUTPUT_PACKET = 0x4f;
    static constexpr std::uint32_t UPDATE_TRADE_PACKET = 0x50;
    static constexpr std::uint32_t UPDATE_EQUIP_PACKET = 0x51;
    static constexpr std::uint32_t RESOURCE_PACK_DATA_INFO_PACKET = 0x52;
    static constexpr std::uint32_t RESOURCE_PACK_CHUNK_DATA_PACKET = 0x53;
    static constexpr std::uint32_t RESOURCE_PACK_CHUNK_REQUEST_PACKET = 0x54;
    static constexpr std::uint32_t TRANSFER_PACKET = 0x55;
    static constexpr std::uint32_t PLAY_SOUND_PACKET = 0x56;
    static constexpr std::uint32_t STOP_SOUND_PACKET = 0x57;
    static constexpr std::uint32_t SET_TITLE_PACKET = 0x58;
    static constexpr std::uint32_t ADD_BEHAVIOR_TREE_PACKET = 0x59;
    static constexpr std::uint32_t STRUCTURE_BLOCK_UPDATE_PACKET = 0x5a;
    static constexpr std::uint32_t SHOW_STORE_OFFER_PACKET = 0x5b;
    static constexpr std::uint32_t PURCHASE_RECEIPT_PACKET = 0x5c;
    static constexpr std::uint32_t PLAYER_SKIN_PACKET = 0x5d;
    static constexpr std::uint32_t SUB_CLIENT_LOGIN_PACKET = 0x5e;
    static constexpr std::uint32_t AUTOMATION_CLIENT_CONNECT_PACKET = 0x5f;
    static constexpr std::uint32_t SET_LAST_HURT_BY_PACKET = 0x60;
    static constexpr std::uint32_t BOOK_EDIT_PACKET = 0x61;
    static constexpr std::uint32_t NPC_REQUEST_PACKET = 0x62;
    static constexpr std::uint32_t PHOTO_TRANSFER_PACKET = 0x63;
    static constexpr std::uint32_t MODAL_FORM_REQUEST_PACKET = 0x64;
    static constexpr std::uint32_t MODAL_FORM_RESPONSE_PACKET = 0x65;
    static constexpr std::uint32_t SERVER_SETTINGS_REQUEST_PACKET = 0x66;
    static constexpr std::uint32_t SERVER_SETTINGS_RESPONSE_PACKET = 0x67;
    static constexpr std::uint32_t SHOW_PROFILE_PACKET = 0x68;
    static constexpr std::uint32_t SET_DEFAULT_GAME_TYPE_PACKET = 0x69;
    static constexpr std::uint32_t REMOVE_OBJECTIVE_PACKET = 0x6a;
    static constexpr std::uint32_t SET_DISPLAY_OBJECTIVE_PACKET = 0x6b;
    static constexpr std::uint32_t SET_SCORE_PACKET = 0x6c;
    static constexpr std::uint32_t LAB_TABLE_PACKET = 0x6d;
    static constexpr std::uint32_t UPDATE_BLOCK_SYNCED_PACKET = 0x6e;
    static constexpr std::uint32_t MOVE_ACTOR_DELTA_PACKET = 0x6f;
    static constexpr std::uint32_t SET_SCOREBOARD_IDENTITY_PACKET = 0x70;
    static constexpr std::uint32_t SET_LOCAL_PLAYER_AS_INITIALIZED_PACKET = 0x71;
    static constexpr std::uint32_t UPDATE_SOFT_ENUM_PACKET = 0x72;
    static constexpr std::uint32_t NETWORK_STACK_LATENCY_PACKET = 0x73;
    static constexpr std::uint32_t SPAWN_PARTICLE_EFFECT_PACKET = 0x76;
    static constexpr std::uint32_t AVAILABLE_ACTOR_IDENTIFIERS_PACKET = 0x77;
    static constexpr std::uint32_t NETWORK_CHUNK_PUBLISHER_UPDATE_PACKET = 0x79;
    static constexpr std::uint32_t BIOME_DEFINITION_LIST_PACKET = 0x7a;
    static constexpr std::uint32_t LEVEL_SOUND_EVENT_PACKET = 0x7b;
    static constexpr std::uint32_t LEVEL_EVENT_GENERIC_PACKET = 0x7c;
    static constexpr std::uint32_t LECTERN_UPDATE_PACKET = 0x7d;
    static constexpr std::uint32_t CLIENT_CACHE_STATUS_PACKET = 0x81;
    static constexpr std::uint32_t ON_SCREEN_TEXTURE_ANIMATION_PACKET = 0x82;
    static constexpr std::uint32_t MAP_CREATE_LOCKED_COPY_PACKET = 0x83;
    static constexpr std::uint32_t STRUCTURE_TEMPLATE_DATA_REQUEST_PACKET = 0x84;
    static constexpr std::uint32_t STRUCTURE_TEMPLATE_DATA_RESPONSE_PACKET = 0x85;
    static constexpr std::uint32_t CLIENT_CACHE_BLOB_STATUS_PACKET = 0x87;
    static constexpr std::uint32_t CLIENT_CACHE_MISS_RESPONSE_PACKET = 0x88;
    static constexpr std::uint32_t EDUCATION_SETTINGS_PACKET = 0x89;
    static constexpr std::uint32_t EMOTE_PACKET = 0x8a;
    static constexpr std::uint32_t MULTIPLAYER_SETTINGS_PACKET = 0x8b;
    static constexpr std::uint32_t SETTINGS_COMMAND_PACKET = 0x8c;
    static constexpr std::uint32_t ANVIL_DAMAGE_PACKET = 0x8d;
    static constexpr std::uint32_t COMPLETED_USING_ITEM_PACKET = 0x8e;
    static constexpr std::uint32_t NETWORK_SETTINGS_PACKET = 0x8f;
    static constexpr std::uint32_t PLAYER_AUTH_INPUT_PACKET = 0x90;
    static constexpr std::uint32_t CREATIVE_CONTENT_PACKET = 0x91;
    static constexpr std::uint32_t PLAYER_ENCHANT_OPTIONS_PACKET = 0x92;
    static constexpr std::uint32_t ITEM_STACK_REQUEST_PACKET = 0x93;
    static constexpr std::uint32_t ITEM_STACK_RESPONSE_PACKET = 0x94;
    static constexpr std::uint32_t PLAYER_ARMOR_DAMAGE_PACKET = 0x95;
    static constexpr std::uint32_t CODE_BUILDER_PACKET = 0x96;
    static constexpr std::uint32_t UPDATE_PLAYER_GAME_TYPE_PACKET = 0x97;
    static constexpr std::uint32_t EMOTE_LIST_PACKET = 0x98;
    static constexpr std::uint32_t POSITION_TRACKING_DB_SERVER_BROADCAST_PACKET = 0x99;
    static constexpr std::uint32_t POSITION_TRACKING_DB_CLIENT_REQUEST_PACKET = 0x9a;
    static constexpr std::uint32_t DEBUG_INFO_PACKET = 0x9b;
    static constexpr std::uint32_t PACKET_VIOLATION_WARNING_PACKET = 0x9c;
    static constexpr std::uint32_t MOTION_PREDICTION_HINTS_PACKET = 0x9d;
    static constexpr std::uint32_t ANIMATE_ENTITY_PACKET = 0x9e;
    static constexpr std::uint32_t CAMERA_SHAKE_PACKET = 0x9f;
    static constexpr std::uint32_t PLAYER_FOG_PACKET = 0xa0;
    static constexpr std::uint32_t CORRECT_PLAYER_MOVE_PREDICTION_PACKET = 0xa1;
    static constexpr std::uint32_t ITEM_REGISTRY_PACKET = 0xa2;
    static constexpr std::uint32_t CLIENTBOUND_DEBUG_RENDERER_PACKET = 0xa4;
    static constexpr std::uint32_t SYNC_ACTOR_PROPERTY_PACKET = 0xa5;
    static constexpr std::uint32_t ADD_VOLUME_ENTITY_PACKET = 0xa6;
    static constexpr std::uint32_t REMOVE_VOLUME_ENTITY_PACKET = 0xa7;
    static constexpr std::uint32_t SIMULATION_TYPE_PACKET = 0xa8;
    static constexpr std::uint32_t NPC_DIALOGUE_PACKET = 0xa9;
    static constexpr std::uint32_t EDU_URI_RESOURCE_PACKET = 0xaa;
    static constexpr std::uint32_t CREATE_PHOTO_PACKET = 0xab;
    static constexpr std::uint32_t UPDATE_SUB_CHUNK_BLOCKS_PACKET = 0xac;
    static constexpr std::uint32_t SUB_CHUNK_PACKET = 0xae;
    static constexpr std::uint32_t SUB_CHUNK_REQUEST_PACKET = 0xaf;
    static constexpr std::uint32_t PLAYER_START_ITEM_COOLDOWN_PACKET = 0xb0;
    static constexpr std::uint32_t SCRIPT_MESSAGE_PACKET = 0xb1;
    static constexpr std::uint32_t CODE_BUILDER_SOURCE_PACKET = 0xb2;
    static constexpr std::uint32_t TICKING_AREAS_LOAD_STATUS_PACKET = 0xb3;
    static constexpr std::uint32_t DIMENSION_DATA_PACKET = 0xb4;
    static constexpr std::uint32_t AGENT_ACTION_EVENT_PACKET = 0xb5;
    static constexpr std::uint32_t CHANGE_MOB_PROPERTY_PACKET = 0xb6;
    static constexpr std::uint32_t LESSON_PROGRESS_PACKET = 0xb7;
    static constexpr std::uint32_t REQUEST_ABILITY_PACKET = 0xb8;
    static constexpr std::uint32_t REQUEST_PERMISSIONS_PACKET = 0xb9;
    static constexpr std::uint32_t TOAST_REQUEST_PACKET = 0xba;
    static constexpr std::uint32_t UPDATE_ABILITIES_PACKET = 0xbb;
    static constexpr std::uint32_t UPDATE_ADVENTURE_SETTINGS_PACKET = 0xbc;
    static constexpr std::uint32_t DEATH_INFO_PACKET = 0xbd;
    static constexpr std::uint32_t EDITOR_NETWORK_PACKET = 0xbe;
    static constexpr std::uint32_t FEATURE_REGISTRY_PACKET = 0xbf;
    static constexpr std::uint32_t SERVER_STATS_PACKET = 0xc0;
    static constexpr std::uint32_t REQUEST_NETWORK_SETTINGS_PACKET = 0xc1;
    static constexpr std::uint32_t GAME_TEST_REQUEST_PACKET = 0xc2;
    static constexpr std::uint32_t GAME_TEST_RESULTS_PACKET = 0xc3;
    static constexpr std::uint32_t UPDATE_CLIENT_INPUT_LOCKS_PACKET = 0xc4;
    static constexpr std::uint32_t CAMERA_PRESETS_PACKET = 0xc6;
    static constexpr std::uint32_t UNLOCKED_RECIPES_PACKET = 0xc7;
    static constexpr std::uint32_t CAMERA_INSTRUCTION_PACKET = 0x12c;
    static constexpr std::uint32_t TRIM_DATA_PACKET = 0x12e;
    static constexpr std::uint32_t OPEN_SIGN_PACKET = 0x12f;
    static constexpr std::uint32_t AGENT_ANIMATION_PACKET = 0x130;
    static constexpr std::uint32_t REFRESH_ENTITLEMENTS_PACKET = 0x131;
    static constexpr std::uint32_t PLAYER_TOGGLE_CRAFTER_SLOT_REQUEST_PACKET = 0x132;
    static constexpr std::uint32_t SET_PLAYER_INVENTORY_OPTIONS_PACKET = 0x133;
    static constexpr std::uint32_t SET_HUD_PACKET = 0x134;
    static constexpr std::uint32_t AWARD_ACHIEVEMENT_PACKET = 0x135;
    static constexpr std::uint32_t CLIENTBOUND_CLOSE_FORM_PACKET = 0x136;
    static constexpr std::uint32_t SERVERBOUND_LOADING_SCREEN_PACKET = 0x138;
    static constexpr std::uint32_t JIGSAW_STRUCTURE_DATA_PACKET = 0x139;
    static constexpr std::uint32_t CURRENT_STRUCTURE_FEATURE_PACKET = 0x13a;
    static constexpr std::uint32_t SERVERBOUND_DIAGNOSTICS_PACKET = 0x13b;
    static constexpr std::uint32_t CAMERA_AIM_ASSIST_PACKET = 0x13c;
    static constexpr std::uint32_t CONTAINER_REGISTRY_CLEANUP_PACKET = 0x13d;
    static constexpr std::uint32_t MOVEMENT_EFFECT_PACKET = 0x13e;
    static constexpr std::uint32_t CAMERA_AIM_ASSIST_PRESETS_PACKET = 0x140;
    static constexpr std::uint32_t CLIENT_CAMERA_AIM_ASSIST_PACKET = 0x141;
    static constexpr std::uint32_t CLIENT_MOVEMENT_PREDICTION_SYNC_PACKET = 0x142;
    static constexpr std::uint32_t UPDATE_CLIENT_OPTIONS_PACKET = 0x143;
    static constexpr std::uint32_t PLAYER_VIDEO_CAPTURE_PACKET = 0x144;
    static constexpr std::uint32_t PLAYER_UPDATE_ENTITY_OVERRIDES_PACKET = 0x145;
    static constexpr std::uint32_t PLAYER_LOCATION_PACKET = 0x146;
    static constexpr std::uint32_t CLIENTBOUND_CONTROL_SCHEME_SET_PACKET = 0x147;
    static constexpr std::uint32_t PRIMITIVE_SHAPES_PACKET = 0x148;
    static constexpr std::uint32_t SERVERBOUND_PACK_SETTING_CHANGE_PACKET = 0x149;
    static constexpr std::uint32_t CLIENTBOUND_DATA_STORE_PACKET = 0x14a;
    static constexpr std::uint32_t GRAPHICS_OVERRIDE_PARAMETER_PACKET = 0x14b;
    static constexpr std::uint32_t SERVERBOUND_DATA_STORE_PACKET = 0x14c;
    static constexpr std::uint32_t CLIENTBOUND_DATA_DRIVEN_UI_SHOW_SCREEN_PACKET = 0x14d;
    static constexpr std::uint32_t CLIENTBOUND_DATA_DRIVEN_UI_CLOSE_SCREEN_PACKET = 0x14e;
    static constexpr std::uint32_t CLIENTBOUND_DATA_DRIVEN_UI_RELOAD_PACKET = 0x14f;
    static constexpr std::uint32_t CLIENTBOUND_TEXTURE_SHIFT_PACKET = 0x150;
    static constexpr std::uint32_t VOXEL_SHAPES_PACKET = 0x151;
    static constexpr std::uint32_t CAMERA_SPLINE_PACKET = 0x152;
    static constexpr std::uint32_t CAMERA_AIM_ASSIST_ACTOR_PRIORITY_PACKET = 0x153;
    static constexpr std::uint32_t RESOURCE_PACKS_READY_FOR_VALIDATION_PACKET = 0x154;
    static constexpr std::uint32_t LOCATOR_BAR_PACKET = 0x155;
    static constexpr std::uint32_t PARTY_CHANGED_PACKET = 0x156;
    static constexpr std::uint32_t SERVERBOUND_DATA_DRIVEN_SCREEN_CLOSED_PACKET = 0x157;
    static constexpr std::uint32_t SYNC_WORLD_CLOCKS_PACKET = 0x158;
    static constexpr std::uint32_t CLIENTBOUND_ATTRIBUTE_LAYER_SYNC_PACKET = 0x159;
    static constexpr std::uint32_t SERVER_STORE_INFO_PACKET = 0x15a;
    static constexpr std::uint32_t SERVER_PRESENCE_INFO_PACKET = 0x15b;
    static constexpr std::uint32_t CLIENTBOUND_UPDATE_SOUND_DATA_PACKET = 0x15c;
    static constexpr std::uint32_t SEND_PARTY_DESTINATION_COOKIE_PACKET = 0x15d;
    static constexpr std::uint32_t PARTY_DESTINATION_COOKIE_RESPONSE_PACKET = 0x15e;
};

}  // namespace bedrock_protocol

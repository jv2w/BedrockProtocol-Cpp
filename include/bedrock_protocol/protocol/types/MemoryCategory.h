/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/MemoryCategory.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types {

/**
 * @see MemoryCategoryCounter
 */
class MemoryCategory final {
public:
    MemoryCategory() = delete;

    static constexpr std::uint8_t UNKNOWN = 0;
    static constexpr std::uint8_t INVALID_SIZE_UNKNOWN = 1;
    static constexpr std::uint8_t ACTOR = 2;
    static constexpr std::uint8_t ACTOR_ANIMATION = 3;
    static constexpr std::uint8_t ACTOR_RENDERING = 4;
    static constexpr std::uint8_t BALANCER = 5;
    static constexpr std::uint8_t BLOCK_TICKING_QUEUES = 6;
    static constexpr std::uint8_t BIOME_STORAGE = 7;
    static constexpr std::uint8_t CEREAL = 8;
    static constexpr std::uint8_t CIRCUIT_SYSTEM = 9;
    static constexpr std::uint8_t CLIENT = 10;
    static constexpr std::uint8_t COMMANDS = 11;
    static constexpr std::uint8_t DB_STORAGE = 12;
    static constexpr std::uint8_t DEBUG = 13;
    static constexpr std::uint8_t DOCUMENTATION = 14;
    static constexpr std::uint8_t ECS_SYSTEMS = 15;
    static constexpr std::uint8_t FMOD = 16;
    static constexpr std::uint8_t FONTS = 17;
    static constexpr std::uint8_t IM_GUI = 18;
    static constexpr std::uint8_t INPUT = 19;
    static constexpr std::uint8_t JSON_UI = 20;
    static constexpr std::uint8_t JSON_UI_CONTROL_FACTORY_JSON = 21;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE = 22;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_CONTROL_ELEMENT = 23;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_POPULATE_DATA_BINDING = 24;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_POPULATE_FOCUS = 25;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_POPULATE_LAYOUT = 26;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_POPULATE_OTHER = 27;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_POPULATE_SPRITE = 28;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_POPULATE_TEXT = 29;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_POPULATE_TTS = 30;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_VISIBILITY = 31;
    static constexpr std::uint8_t JSON_UI_CREATE_UI = 32;
    static constexpr std::uint8_t JSON_UI_DEFS = 33;
    static constexpr std::uint8_t JSON_UI_LAYOUT_MANAGER = 34;
    static constexpr std::uint8_t JSON_UI_LAYOUT_MANAGER_REMOVE_DEPENDENCIES = 35;
    static constexpr std::uint8_t JSON_UI_LAYOUT_MANAGER_INIT_VARIABLE = 36;
    static constexpr std::uint8_t LANGUAGES = 37;
    static constexpr std::uint8_t LEVEL = 38;
    static constexpr std::uint8_t LEVEL_STRUCTURES = 39;
    static constexpr std::uint8_t LEVEL_CHUNK = 40;
    static constexpr std::uint8_t LEVEL_CHUNK_GEN = 41;
    static constexpr std::uint8_t LEVEL_CHUNK_GEN_THREAD_LOCAL = 42;
    static constexpr std::uint8_t NETWORK = 43;
    static constexpr std::uint8_t MARKETPLACE = 44;
    static constexpr std::uint8_t MATERIAL_DRAGON_COMPILED_DEFINITION = 45;
    static constexpr std::uint8_t MATERIAL_DRAGON_MATERIAL = 46;
    static constexpr std::uint8_t MATERIAL_DRAGON_RESOURCE = 47;
    static constexpr std::uint8_t MATERIAL_DRAGON_UNIFORM_MAP = 48;
    static constexpr std::uint8_t MATERIAL_RENDER_MATERIAL = 49;
    static constexpr std::uint8_t MATERIAL_RENDER_MATERIAL_GROUP = 50;
    static constexpr std::uint8_t MATERIAL_VARIATION_MANAGER = 51;
    static constexpr std::uint8_t MOLANG = 52;
    static constexpr std::uint8_t ORE_UI = 53;
    static constexpr std::uint8_t PERSONA = 54;
    static constexpr std::uint8_t PLAYER = 55;
    static constexpr std::uint8_t RENDER_CHUNK = 56;
    static constexpr std::uint8_t RENDER_CHUNK_INDEX_BUFFER = 57;
    static constexpr std::uint8_t RENDER_CHUNK_VERTEX_BUFFER = 58;
    static constexpr std::uint8_t RENDERING = 59;
    static constexpr std::uint8_t RENDERING_LIBRARY = 60;
    static constexpr std::uint8_t REQUEST_LOG = 61;
    static constexpr std::uint8_t RESOURCE_PACKS = 62;
    static constexpr std::uint8_t SOUND = 63;
    static constexpr std::uint8_t SUB_CHUNK_BIOME_DATA = 64;
    static constexpr std::uint8_t SUB_CHUNK_BLOCK_DATA = 65;
    static constexpr std::uint8_t SUB_CHUNK_LIGHT_DATA = 66;
    static constexpr std::uint8_t TEXTURES = 67;
    static constexpr std::uint8_t VR = 68;
    static constexpr std::uint8_t WEATHER_RENDERER = 69;
    static constexpr std::uint8_t WORLD_GENERATOR = 70;
    static constexpr std::uint8_t TASKS = 71;
    static constexpr std::uint8_t TEST = 72;
    static constexpr std::uint8_t SCRIPTING = 73;
    static constexpr std::uint8_t SCRIPTING_RUNTIME = 74;
    static constexpr std::uint8_t SCRIPTING_CONTEXT = 75;
    static constexpr std::uint8_t SCRIPTING_CONTEXT_BINDINGS_MC = 76;
    static constexpr std::uint8_t SCRIPTING_CONTEXT_BINDINGS_GT = 77;
    static constexpr std::uint8_t SCRIPTING_CONTEXT_RUN = 78;
    static constexpr std::uint8_t DATA_DRIVEN_UI = 79;
    static constexpr std::uint8_t DATA_DRIVEN_UI_DEFS = 80;
};

}  // namespace bedrock_protocol::types

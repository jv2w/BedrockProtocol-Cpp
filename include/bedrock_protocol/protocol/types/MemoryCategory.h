/*
 * This file is part of BedrockProtocol-Cpp.
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
    static constexpr std::uint8_t BLOBS = 8;
    static constexpr std::uint8_t CEREAL = 9;
    static constexpr std::uint8_t CIRCUIT_SYSTEM = 10;
    static constexpr std::uint8_t CLIENT = 11;
    static constexpr std::uint8_t COMMANDS = 12;
    static constexpr std::uint8_t DB_STORAGE = 13;
    static constexpr std::uint8_t DEBUG = 14;
    static constexpr std::uint8_t DOCUMENTATION = 15;
    static constexpr std::uint8_t ECS_SYSTEMS = 16;
    static constexpr std::uint8_t FMOD = 17;
    static constexpr std::uint8_t FONTS = 18;
    static constexpr std::uint8_t IM_GUI = 19;
    static constexpr std::uint8_t INPUT = 20;
    static constexpr std::uint8_t JSON_UI = 21;
    static constexpr std::uint8_t JSON_UI_CONTROL_FACTORY_JSON = 22;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE = 23;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_CONTROL_ELEMENT = 24;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_POPULATE_DATA_BINDING = 25;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_POPULATE_FOCUS = 26;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_POPULATE_LAYOUT = 27;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_POPULATE_OTHER = 28;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_POPULATE_SPRITE = 29;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_POPULATE_TEXT = 30;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_POPULATE_TTS = 31;
    static constexpr std::uint8_t JSON_UI_CONTROL_TREE_VISIBILITY = 32;
    static constexpr std::uint8_t JSON_UI_CREATE_UI = 33;
    static constexpr std::uint8_t JSON_UI_DEFS = 34;
    static constexpr std::uint8_t JSON_UI_LAYOUT_MANAGER = 35;
    static constexpr std::uint8_t JSON_UI_LAYOUT_MANAGER_REMOVE_DEPENDENCIES = 36;
    static constexpr std::uint8_t JSON_UI_LAYOUT_MANAGER_INIT_VARIABLE = 37;
    static constexpr std::uint8_t LANGUAGES = 38;
    static constexpr std::uint8_t LEVEL = 39;
    static constexpr std::uint8_t LEVEL_STRUCTURES = 40;
    static constexpr std::uint8_t LEVEL_CHUNK = 41;
    static constexpr std::uint8_t LEVEL_CHUNK_GEN = 42;
    static constexpr std::uint8_t LEVEL_CHUNK_GEN_THREAD_LOCAL = 43;
    static constexpr std::uint8_t NETWORK = 44;
    static constexpr std::uint8_t MARKETPLACE = 45;
    static constexpr std::uint8_t MATERIAL_DRAGON_COMPILED_DEFINITION = 46;
    static constexpr std::uint8_t MATERIAL_DRAGON_MATERIAL = 47;
    static constexpr std::uint8_t MATERIAL_DRAGON_RESOURCE = 48;
    static constexpr std::uint8_t MATERIAL_DRAGON_UNIFORM_MAP = 49;
    static constexpr std::uint8_t MATERIAL_RENDER_MATERIAL = 50;
    static constexpr std::uint8_t MATERIAL_RENDER_MATERIAL_GROUP = 51;
    static constexpr std::uint8_t MATERIAL_VARIATION_MANAGER = 52;
    static constexpr std::uint8_t MOLANG = 53;
    static constexpr std::uint8_t ORE_UI = 54;
    static constexpr std::uint8_t ORE_UI_CLIENT = 55;
    static constexpr std::uint8_t PERSONA_PIECES = 56;
    static constexpr std::uint8_t PERSONA_ANIMATIONS = 57;
    static constexpr std::uint8_t PERSONA_TEXTURES = 58;
    static constexpr std::uint8_t PERSONA_CHARACTERS = 59;
    static constexpr std::uint8_t PERSONA_SKIN_PACKS = 60;
    static constexpr std::uint8_t PERSONA_REPO = 61;
    static constexpr std::uint8_t PLAYER = 62;
    static constexpr std::uint8_t RENDER_CHUNK = 63;
    static constexpr std::uint8_t RENDER_CHUNK_INDEX_BUFFER = 64;
    static constexpr std::uint8_t RENDER_CHUNK_VERTEX_BUFFER = 65;
    static constexpr std::uint8_t RENDERING = 66;
    static constexpr std::uint8_t RENDERING_BGFX_INIT = 67;
    static constexpr std::uint8_t RENDERING_BGFX_START_FRAME = 68;
    static constexpr std::uint8_t RENDERING_BLOCK_TESSELLATOR = 69;
    static constexpr std::uint8_t RENDERING_END_FRAME = 70;
    static constexpr std::uint8_t RENDERING_GRAPHICS_TASKS_INIT = 71;
    static constexpr std::uint8_t RENDERING_LIBRARY = 72;
    static constexpr std::uint8_t RENDERING_POLYGON_OPERATOR_POOL = 73;
    static constexpr std::uint8_t RENDERING_PBR_TEXTURE_DATA = 74;
    static constexpr std::uint8_t RENDERING_RENDER_REGISTRY = 75;
    static constexpr std::uint8_t RENDERING_SETUP = 76;
    static constexpr std::uint8_t RENDERING_VERTICES = 77;
    static constexpr std::uint8_t REQUEST_LOG = 78;
    static constexpr std::uint8_t RESOURCE_PACKS = 79;
    static constexpr std::uint8_t SOUND = 80;
    static constexpr std::uint8_t SUB_CHUNK_BIOME_DATA = 81;
    static constexpr std::uint8_t SUB_CHUNK_BLOCK_DATA = 82;
    static constexpr std::uint8_t SUB_CHUNK_LIGHT_DATA = 83;
    static constexpr std::uint8_t TEXTURES = 84;
    static constexpr std::uint8_t WEATHER_RENDERER = 85;
    static constexpr std::uint8_t WORLD_GENERATOR = 86;
    static constexpr std::uint8_t TASKS = 87;
    static constexpr std::uint8_t TEST = 88;
    static constexpr std::uint8_t TEST_LOAD_TEST_TAGS = 89;
    static constexpr std::uint8_t SCRIPTING = 90;
    static constexpr std::uint8_t SCRIPTING_RUNTIME = 91;
    static constexpr std::uint8_t SCRIPTING_CONTEXT = 92;
    static constexpr std::uint8_t SCRIPTING_CONTEXT_BINDINGS_MC = 93;
    static constexpr std::uint8_t SCRIPTING_CONTEXT_BINDINGS_GT = 94;
    static constexpr std::uint8_t SCRIPTING_CONTEXT_RUN = 95;
    static constexpr std::uint8_t DATA_DRIVEN_UI = 96;
    static constexpr std::uint8_t DATA_DRIVEN_UI_DEFS = 97;
    static constexpr std::uint8_t GAMEFACE = 98;
    static constexpr std::uint8_t GAMEFACE_SYSTEM = 99;
    static constexpr std::uint8_t GAMEFACE_DOM = 100;
    static constexpr std::uint8_t GAMEFACE_CSS = 101;
    static constexpr std::uint8_t GAMEFACE_DISPLAY = 102;
    static constexpr std::uint8_t GAMEFACE_TEMP_ALLOCATOR = 103;
    static constexpr std::uint8_t GAMEFACE_POOL_ALLOCATOR = 104;
    static constexpr std::uint8_t GAMEFACE_DUMP = 105;
    static constexpr std::uint8_t GAMEFACE_MEDIA = 106;
    static constexpr std::uint8_t GAMEFACE_JSON = 107;
    static constexpr std::uint8_t GAMEFACE_SCRIPT_ENGINE = 108;
    static constexpr std::uint8_t GAMEFACE_SCRIPT = 109;
    static constexpr std::uint8_t GAMEFACE_LAYOUT = 110;
    static constexpr std::uint8_t VR = 111;
};

}  // namespace bedrock_protocol::types

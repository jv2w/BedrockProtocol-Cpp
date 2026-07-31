cmake_minimum_required(VERSION 3.20)

project(bedrock_protocol LANGUAGES CXX)

option(BEDROCK_PROTOCOL_BUILD_TESTS "Build the BedrockProtocol round-trip tests" OFF)

# The verification suite is for developing this library, not for using it. A plugin that pulls this
# in with FetchContent would otherwise compile all of verify/ on every build and link none of it, so
# it defaults on only when this repository is the project being built.
option(BEDROCK_PROTOCOL_BUILD_VERIFY "Build the BedrockProtocol verification suite"
        ${PROJECT_IS_TOP_LEVEL})

# ---------------------------------------------------------------------------
# Core library: packets, types and serialization. No Endstone dependency, so it
# can be unit-tested standalone.
# ---------------------------------------------------------------------------
file(GLOB_RECURSE BEDROCK_PROTOCOL_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(FILTER BEDROCK_PROTOCOL_SOURCES EXCLUDE REGEX "/src/bridge/")

add_library(bedrock_protocol STATIC ${BEDROCK_PROTOCOL_SOURCES})
add_library(bedrock_protocol::bedrock_protocol ALIAS bedrock_protocol)

target_compile_features(bedrock_protocol PUBLIC cxx_std_20)
target_include_directories(bedrock_protocol PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
set_target_properties(bedrock_protocol PROPERTIES POSITION_INDEPENDENT_CODE ON)

if (MSVC)
    # /bigobj: the generated packet translation units exceed the default section limit.
    target_compile_options(bedrock_protocol PRIVATE /bigobj /utf-8 /EHsc /W3)
else ()
    target_compile_options(bedrock_protocol PRIVATE -Wall -Wextra)
endif ()

target_precompile_headers(bedrock_protocol PRIVATE
        <cstdint>
        <memory>
        <optional>
        <string>
        <string_view>
        <vector>
        "${CMAKE_CURRENT_SOURCE_DIR}/include/bedrock_protocol/encoding/ByteBufferReader.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/bedrock_protocol/encoding/ByteBufferWriter.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/bedrock_protocol/encoding/VarInt.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/bedrock_protocol/encoding/LE.h"
)

# ---------------------------------------------------------------------------
# Verification library: the deep round-trip suite and the shared packet selection.
#
# Free of any Endstone dependency, so the offline test executable and an in-game selftest can share
# one implementation rather than drifting into two. Skipped when this library is being consumed by
# another project, which never links it.
# ---------------------------------------------------------------------------
if (BEDROCK_PROTOCOL_BUILD_VERIFY)
    file(GLOB_RECURSE BEDROCK_PROTOCOL_VERIFY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/verify/src/*.cpp")

    add_library(bedrock_protocol_verify STATIC ${BEDROCK_PROTOCOL_VERIFY_SOURCES})
    add_library(bedrock_protocol::verify ALIAS bedrock_protocol_verify)

    target_compile_features(bedrock_protocol_verify PUBLIC cxx_std_20)
    target_link_libraries(bedrock_protocol_verify PUBLIC bedrock_protocol)
    target_include_directories(bedrock_protocol_verify PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/verify/include")
    set_target_properties(bedrock_protocol_verify PROPERTIES POSITION_INDEPENDENT_CODE ON)

    if (MSVC)
        # /EHsc, matching every sibling target. The fuzz stages want /EHa for _set_se_translator, but
        # this build is clang-cl (Endstone forces it) and clang-cl accepts /EHa without implementing the
        # translator - so /EHa would buy nothing here while making this the only target with a different
        # EH model, which is what produced an undefined std::variant destructor at plugin link time.
        # The authoritative gate compiles these same sources with MSVC cl and /EHa; see
        # tools/run_deep_roundtrip.ps1.
        target_compile_options(bedrock_protocol_verify PRIVATE /bigobj /utf-8 /EHsc /W3)
    else ()
        target_compile_options(bedrock_protocol_verify PRIVATE -Wall -Wextra)
    endif ()
endif ()

# ---------------------------------------------------------------------------
# Bridge library: glues the core onto Endstone's PacketReceiveEvent/PacketSendEvent.
# Only built when Endstone is available (i.e. when consumed from a plugin project).
# ---------------------------------------------------------------------------
file(GLOB_RECURSE BEDROCK_PROTOCOL_BRIDGE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/bridge/*.cpp")

if (BEDROCK_PROTOCOL_BRIDGE_SOURCES AND TARGET endstone::endstone)
    add_library(bedrock_protocol_bridge STATIC ${BEDROCK_PROTOCOL_BRIDGE_SOURCES})
    add_library(bedrock_protocol::bridge ALIAS bedrock_protocol_bridge)

    target_compile_features(bedrock_protocol_bridge PUBLIC cxx_std_20)
    target_link_libraries(bedrock_protocol_bridge PUBLIC bedrock_protocol endstone::endstone)
    set_target_properties(bedrock_protocol_bridge PROPERTIES POSITION_INDEPENDENT_CODE ON)

    if (MSVC)
        target_compile_options(bedrock_protocol_bridge PRIVATE /bigobj /utf-8 /EHsc /W3)
    endif ()
endif ()

if (BEDROCK_PROTOCOL_BUILD_TESTS)
    add_subdirectory(tests)
endif ()

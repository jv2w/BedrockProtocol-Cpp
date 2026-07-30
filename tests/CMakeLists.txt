enable_testing()

# Tests link the core library only; the bridge needs Endstone and is exercised in-game instead.
foreach (test_name EncodingTest PacketRoundTripTest)
    add_executable(${test_name} ${test_name}.cpp)
    target_link_libraries(${test_name} PRIVATE bedrock_protocol)
    target_include_directories(${test_name} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    if (MSVC)
        target_compile_options(${test_name} PRIVATE /utf-8 /bigobj)
    endif ()
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach ()

if (MSVC)
    # PacketRoundTripTest installs an SE translator so a null required member is reported as a
    # failure instead of killing the process; that needs asynchronous exception handling.
    target_compile_options(PacketRoundTripTest PRIVATE /EHa)
    target_compile_options(EncodingTest PRIVATE /EHsc)
endif ()

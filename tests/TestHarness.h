/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * A dependency-free assertion harness. The library is deliberately free of third-party
 * dependencies, and these tests are simple enough that pulling in a framework would cost more in
 * build time and network access than it saves.
 */

#pragma once

#include <cstdio>
#include <string>
#include <string_view>
#include <type_traits>

namespace bedrock_protocol::test {

inline int g_failures = 0;
inline int g_checks = 0;

/** Renders a byte string as hex, so mismatches are readable in the failure output. */
inline std::string toHex(std::string_view bytes)
{
    static constexpr char DIGITS[] = "0123456789abcdef";
    std::string result;
    result.reserve(bytes.size() * 2);
    for (const char c : bytes) {
        const auto b = static_cast<unsigned char>(c);
        result.push_back(DIGITS[b >> 4]);
        result.push_back(DIGITS[b & 0x0f]);
    }
    return result;
}

inline void reportFailure(const char *file, int line, const std::string &message)
{
    ++g_failures;
    std::fprintf(stderr, "FAIL %s:%d: %s\n", file, line, message.c_str());
}

/** Renders a value for a failure message; anything not printable falls back to a placeholder. */
template <typename T>
std::string describe(const T &value)
{
    if constexpr (std::is_arithmetic_v<T>) {
        return std::to_string(value);
    }
    else if constexpr (std::is_convertible_v<T, std::string_view>) {
        return std::string(std::string_view(value));
    }
    else {
        return "<value>";
    }
}

template <typename A, typename B>
void checkEqual(const char *file, int line, const char *expr, const A &actual, const B &expected)
{
    ++g_checks;
    if (!(actual == expected)) {
        reportFailure(file, line,
                      std::string(expr) + ": got " + describe(actual) + ", expected " + describe(expected));
    }
}

inline void checkEqualBytes(const char *file, int line, const char *expr, std::string_view actual,
                            std::string_view expected)
{
    ++g_checks;
    if (actual != expected) {
        reportFailure(file, line, std::string(expr) + ": got " + toHex(actual) + ", expected " + toHex(expected));
    }
}

inline void checkTrue(const char *file, int line, const char *expr, bool condition)
{
    ++g_checks;
    if (!condition) {
        reportFailure(file, line, std::string(expr) + " was false");
    }
}

/** Returns the process exit code and prints a summary. */
inline int summarise(const char *suite)
{
    if (g_failures == 0) {
        std::printf("[OK] %s: %d checks passed\n", suite, g_checks);
        return 0;
    }
    std::printf("[FAIL] %s: %d of %d checks failed\n", suite, g_failures, g_checks);
    return 1;
}

}  // namespace bedrock_protocol::test

#define CHECK_EQ(actual, expected) ::bedrock_protocol::test::checkEqual(__FILE__, __LINE__, #actual, (actual), (expected))
#define CHECK_BYTES(actual, expected) \
    ::bedrock_protocol::test::checkEqualBytes(__FILE__, __LINE__, #actual, (actual), (expected))
#define CHECK_TRUE(condition) ::bedrock_protocol::test::checkTrue(__FILE__, __LINE__, #condition, (condition))

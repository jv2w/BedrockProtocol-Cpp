/*
 * This file is part of BedrockProtocol for Endstone.
 *
 * The filler registry: the single, shared definition of "the selected packets".
 *
 * PacketSelection.inc is included by every consumer - the offline suite, the live traffic audit and
 * the in-game showcase - so the three systems cannot disagree about which packets are under test.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <span>
#include <string_view>

#include "bedrock_protocol/protocol/DataPacket.h"
#include "bedrock_protocol/verify/ValueWell.h"

namespace bedrock_protocol::verify {

/**
 * How likely the packet is to appear in ordinary gameplay.
 *
 * This is what makes ABSENCE meaningful in the live audit: a Core packet that was never observed
 * means the audit did not really run, whereas an unobserved Rare packet is expected.
 */
enum class Tier { Core, Common, Rare };

std::string_view toString(Tier tier);

struct FillContext {
    ValueWell &well;
};

/**
 * Builds one fully-populated instance of a packet.
 *
 * Returning unique_ptr<DataPacket> rather than the concrete type is what erases the move-only /
 * copyable split: 17 of the 229 packets hold a unique_ptr and cannot be copied, but all 229 are
 * movable, so make_unique<T>(T::create(...)) compiles identically for both groups and no caller
 * ever has to special-case them.
 */
using Filler = std::unique_ptr<DataPacket> (*)(FillContext &);

struct SelectedPacket {
    std::uint32_t pid;
    std::string_view name;
    Tier tier;
    std::string_view reasons;  ///< any of R (real traffic), F (field mass), T (hard type)
    int declaredFields;
    std::uint32_t sampleEvery;  ///< live audit: decode 1 in N. 1 means every packet.
    std::string_view note;
    Filler fill;
};

/** The selection, in PacketSelection.inc order. */
std::span<const SelectedPacket> getSelection();

/** Returns nullptr if the packet ID is not part of the suite. */
const SelectedPacket *findSelected(std::uint32_t pid);

/** Case-insensitive lookup by class name, for -Only and /proto selftest <name>. */
const SelectedPacket *findSelectedByName(std::string_view name);

namespace detail {

/** Deduces how many parameters a generated create() factory takes. */
template <typename R, typename... Args>
constexpr std::size_t arityOf(R (*)(Args...))
{
    return sizeof...(Args);
}

}  // namespace detail

/**
 * Defines a filler and proves, at compile time, that it populates every field.
 *
 * 218 of the 229 packets expose `static T create(<every field, in declaration order>)`. Writing the
 * filler as a single create() call makes omitting a field IMPOSSIBLE rather than merely discouraged:
 * a missing argument is a wrong-arity error, swapping two arguments of different types is a type
 * error, and a field added upstream breaks the build instead of silently going untested.
 *
 * This is why field coverage is reported as "compile-checked" and not as a percentage someone has to
 * trust. The static_assert below pins the declared field count to create()'s real arity, so the
 * number printed in the report cannot drift away from the header.
 */
#define BP_FILLER(Type, FieldCount)                                                                        \
    static_assert(::bedrock_protocol::verify::detail::arityOf(&::bedrock_protocol::Type::create) ==         \
                      static_cast<std::size_t>(FieldCount),                                                \
                  #Type ": declared field count does not match create()'s arity - the packet gained or "   \
                        "lost a field upstream, so this filler no longer populates all of them");          \
    std::unique_ptr<::bedrock_protocol::DataPacket> fill##Type(::bedrock_protocol::verify::FillContext &ctx)

/**
 * Defines a filler for one of the 11 packets that have no generated create() factory.
 *
 * These must assign fields individually, so there is no compile-time completeness guarantee and they
 * are covered by the per-field mutation check instead. Every use of this macro is a packet named in
 * the comment block at the top of its filler file, with the reason it lacks create().
 */
#define BP_FILLER_NOCREATE(Type, FieldCount)                                                               \
    std::unique_ptr<::bedrock_protocol::DataPacket> fill##Type(::bedrock_protocol::verify::FillContext &ctx)

}  // namespace bedrock_protocol::verify

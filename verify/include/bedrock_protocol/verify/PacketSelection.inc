/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * THE selection list. Included by the offline deep round-trip suite, the live traffic audit and the
 * in-game showcase, so all three systems test the same set and cannot drift apart.
 *
 * Deliberately not a header: it has no include guard and is included repeatedly with a different
 * BP_SELECTED_PACKET definition each time (forward declarations, then the dispatch table, then the
 * audit table). Define the macro, include, undefine.
 *
 *   BP_SELECTED_PACKET(Type, Tier, Reasons, Fields, SampleEvery, Note)
 *
 *     Type         packet class name, unqualified, in namespace bedrock_protocol
 *     Tier         Core | Common | Rare  - how likely it is to appear in ordinary gameplay.
 *                  This is what makes ABSENCE meaningful in the live audit: an unobserved Core
 *                  packet means the audit did not really run; an unobserved Rare one is expected.
 *     Reasons      why it is in the suite; any of:
 *                    R  real traffic between BDS 26.33 and a vanilla client
 *                    F  field mass - four or more declared public fields
 *                    T  hard type - polymorphic member, BitSet, CacheableNbt, vector<struct>, optional
 *     Fields       declared public field count. BP_FILLER static_asserts this against create()'s
 *                  arity, so a wrong number here is a compile error rather than a silent miscount.
 *     SampleEvery  live audit only: decode one packet in N. 1 = every packet. Raised only for the
 *                  high-frequency IDs, where decoding all of them would be real server load.
 *     Note         what makes this packet worth testing
 *
 * Zero-field packets are deliberately excluded: the anti-tautology check (a filled packet must not
 * encode to the same bytes as a default-constructed one) is meaningless when there is nothing to
 * fill, so including them would only inflate the count.
 *
 * The list is split into fragments by subject area. That is not cosmetic - each fragment is owned by
 * exactly one filler translation unit, so batches of work never collide in the same file.
 */

#include "bedrock_protocol/verify/selection/Core.inc"
#include "bedrock_protocol/verify/selection/World.inc"
#include "bedrock_protocol/verify/selection/Entity.inc"
#include "bedrock_protocol/verify/selection/Inventory.inc"
#include "bedrock_protocol/verify/selection/Ui.inc"
#include "bedrock_protocol/verify/selection/Misc.inc"
#include "bedrock_protocol/verify/selection/Extra1.inc"
#include "bedrock_protocol/verify/selection/Extra2.inc"
#include "bedrock_protocol/verify/selection/Extra3.inc"
#include "bedrock_protocol/verify/selection/Extra4.inc"

#pragma once
#include <cstdint>
#include "Actor.h"

namespace acnl::fish {

#pragma pack(push, 1)

/**
 * @brief Official Nintendo Fish AI State Machine IDs (from 0x008C77DC table).
 */
enum class FishAiState : uint32_t {
    SpeedUp   = 0,  // Acceleration
    MoveS     = 1,  // Straight swimming motion
    MoveR     = 2,  // Turning / rotational swimming motion
    SpeedDown = 3,  // Deceleration / idle drift
    DriftS    = 4,  // Drifting along river current straight
    DriftR    = 5,  // Drifting along river current rotating
    Escape    = 6,  // Fleeing from running footsteps or missed reel-in
    Pick      = 7,  // Approaching & nibbling on fishing rod bobber
    Hit       = 8,  // Bobber bitten underwater (splash SFX, reel-in window)
    Pass      = 9,  // Swimming past without noticing
    Leave     = 10, // Losing interest in bobber and wandering away
    Delete    = 11  // Despawn from world
};

/**
 * @brief Fish shadow size categories.
 */
enum class FishShadowSize : uint8_t {
    Tiny     = 1, // Crucian Carp, Goldfish, Pale Chub
    Small    = 2, // Yellow Perch, Crawfish, Frog
    Medium   = 3, // Bass, Sweetfish, Red Snapper
    Large    = 4, // Barbel Steed, Sea Bass, Salmon
    Huge     = 5, // Stringfish, Coelacanth, Tuna, Oarfish
    Finned   = 6  // Ocean Sharks (Great White, Hammerhead, Saw Shark, Sunfish)
};

/**
 * @brief Active Fish Instance State (6 bytes packed per slot in BsFishFieldMgr).
 * 8 active fish slots in town ('T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[').
 */
struct PackedFishSlot {
    uint8_t species_id;       // Bits 0..6: Species ID (0..106), Bit 7: Active flag
    uint8_t packed_coord_x0;  // Coordinate X low bits
    uint8_t packed_coord_xz;  // Coordinate X high / Z low bits
    uint8_t packed_coord_z1;  // Coordinate Z high bits
    uint8_t fish_flags;       // Status flags
    uint8_t slot_id;          // Manager index (0..7)
};
static_assert(sizeof(PackedFishSlot) == 6, "PackedFishSlot must be 6 bytes");

/**
 * @brief Fish actor instance data (AcFsFdShadow).
 * Inherits from AcObjectBase.
 */
struct AcFsFdShadow : public AcObjectBase {
    FishAiState current_state;          // +0x190: Current AI state (0..11)
    uint32_t    state_timer_frames;     // +0x194: Frame counter within state
    uint32_t    reel_in_elapsed_frames; // +0x198: Elapsed frames since bite
    uint32_t    reel_in_window_frames;  // +0x19C: Max reaction time window (fVar1 * 30.0)
    int16_t     heading_angle;          // +0x200: Heading direction
    uint8_t     nibble_state_timer;     // +0x202: Timer during bobber nudge
    uint8_t     flee_trigger_flag;      // +0x204: Triggered by player running
    uint8_t     is_facing_bobber;       // +0x206: Boolean flag
    uint8_t     nibble_count;           // +0x1CE: Current nibble count (1..5)
};

/**
 * @brief Probability formula for bobber bite:
 *   P(Bite) = nibble_count * 20%
 *   Nibble 1: 20%
 *   Nibble 2: 40%
 *   Nibble 3: 60%
 *   Nibble 4: 80%
 *   Nibble 5: 100% (Guaranteed Bite)
 */
inline bool EvaluateFishBite(uint8_t currentNibble, uint8_t randomPercent100) {
    uint32_t threshold = currentNibble * 20;
    return randomPercent100 < threshold;
}

#pragma pack(pop)

} // namespace acnl::fish

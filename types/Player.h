#pragma once
#include <cstdint>
#include <array>
#include "Actor.h"

/**
 * @file Player.h
 * @brief Player Controller, Movement Physics, Tools & Luck Subsystem for ACNL: Welcome amiibo
 * 
 * Reverses the exact runtime player mechanisms from exefs.elf (AcPlayer @ 0x008F74B0):
 * - Locomotion state machine (Idle, Walk, Run, Sneak, Trip, Swim, Dive)
 * - 3D velocity displacement and facing angle rotation
 * - Tool action targeting functor (Shovel, Fishing Rod, Net, Axe, Slingshot, Watering Can)
 * - Katrina Daily Luck & King Tut Mask running trip RNG
 */

namespace acnl::player {

#pragma pack(push, 1)

// ============================================================================
// 1. Locomotion States (exefs.elf +0x1A9)
// ============================================================================

enum class LocomotionState : uint8_t {
    Idle        = 0x00,  ///< Standing still
    Walk        = 0x18,  ///< Walking (analog stick tilt < 0.8)
    Run         = 0x47,  ///< Running (Hold B / L / R, scuffs grass, scares fish/bugs)
    SneakNet    = 0x22,  ///< Creeping with net held ready (Hold A with Net equipped)
    Tripping    = 0x55,  ///< Tumbling/falling flat on ground (Bad Luck / King Tut Mask)
    PitfallTrap = 0x6E,  ///< Trapped in buried pitfall seed hole
    CliffJump   = 0x9D,  ///< Vaulting off town cliff into ocean
    Swimming    = 0x9E,  ///< Floating/paddling on ocean surface with wetsuit
    Diving      = 0x9F   ///< Submerged underwater searching for sea creatures
};

inline constexpr float kSpeedWalk  = 1.50f;  ///< Standard walking velocity
inline constexpr float kSpeedRun   = 3.00f;  ///< Running velocity (2x walk)
inline constexpr float kSpeedSneak = 0.60f;  ///< Net sneak velocity (0.4x walk)
inline constexpr float kSpeedSwim  = 1.20f;  ///< Ocean swim speed

// ============================================================================
// 2. Katrina Daily Fortune & Luck
// ============================================================================

enum class LuckType : uint8_t {
    Money       = 0,     ///< Increased bells from shaking trees & hitting money rock
    Item        = 1,     ///< Rare bugs/fish spawn more often; villagers give rare gifts
    Friendship  = 2,     ///< Villagers run up to talk more frequently; faster friendship
    Physical    = 3,     ///< Health luck: Good = faster run; Bad = frequent tripping while running!
    Love        = 4      ///< Opposite-gender villagers act infatuated
};

enum class LuckLevel : int8_t {
    BadLuck       = -1,  ///< Causes stumbling while running (if Physical)
    Neutral       =  0,
    GoodLuck      =  1,
    SuperGoodLuck =  2
};

inline constexpr uint16_t kItemKingTutMask = 0x21A5; ///< Headwear item ID forcing tripping

// ============================================================================
// 3. Tool Action Interaction Flags (AcPlayer::ToolFunctor)
// ============================================================================

enum class ToolTargetFlag : uint32_t {
    None       = 0x0000,
    HoleDig    = 0x0001, ///< Soil tile can be dug with Shovel
    WaterFish  = 0x0002, ///< River/Ocean water body for Fishing Rod bobber
    BugNet     = 0x0400  ///< Catchable insect within net swing cone
};

struct ToolFunctor {
    uint32_t vtable;     ///< 0x00909CA0
    uint8_t  active_tool;///< Equipped tool ID
    uint8_t  pad[3];
    uint32_t target_action; ///< Resolved action code (1=Dig, 2=Fish, 0x400=Catch)
    uint32_t target_actor_id;
};

// ============================================================================
// 4. AcPlayer Instance Layout (exefs.elf 0x008F74B0)
// ============================================================================

struct AcPlayer {
    uint32_t vtable;             ///< 0x008F74B0
    uint8_t  reserved0[0x10];
    float    pos_x;              ///< +0x14: 3D world X
    float    pos_y;              ///< +0x18: 3D world Y (Ground elevation)
    float    pos_z;              ///< +0x1C: 3D world Z
    float    prev_pos_x;         ///< +0x20: Previous frame X
    float    prev_pos_y;         ///< +0x24: Previous frame Y
    float    prev_pos_z;         ///< +0x28: Previous frame Z
    float    velocity[3];        ///< +0x2C: Current velocity vector
    uint16_t facing_angle;       ///< +0x2E: Facing rotation around Y axis (0..65535)
    uint8_t  reserved1[0x17A];
    LocomotionState state;       ///< +0x1A9: Active locomotion state
    uint8_t  reserved2[0x4A6];
    ToolFunctor tool_functor;    ///< Tool interaction detector
};

#pragma pack(pop)

// ============================================================================
// 5. Tripping Probability Check (exefs.elf 0x006540B8)
// ============================================================================

/**
 * @brief Evaluates whether player stumbles and falls while running
 * @param state Active locomotion state
 * @param is_wearing_king_tut 1 if King Tut mask equipped
 * @param physical_luck Active daily physical luck level
 * @param rng_roll Uniform random byte (0..255)
 * @return true if player trips
 */
inline bool ShouldPlayerTrip(LocomotionState state, bool is_wearing_king_tut, LuckLevel physical_luck, uint8_t rng_roll) {
    if (state != LocomotionState::Run) return false;
    if (is_wearing_king_tut) {
        // King Tut mask forces ~1/8 tripping probability every step
        return (rng_roll < 32);
    }
    if (physical_luck == LuckLevel::BadLuck) {
        // Bad Physical luck causes ~1/16 tripping probability
        return (rng_roll < 16);
    }
    return false;
}

} // namespace acnl::player

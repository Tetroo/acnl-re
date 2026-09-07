#pragma once
#include <cstdint>
#include <array>

namespace acnl {

#pragma pack(push, 1)

struct Vector3 {
    float x;
    float y;
    float z;
};

struct Rot3 {
    uint16_t x;
    uint16_t y;
    uint16_t z;
    uint16_t pad;
};

/**
 * @brief Lifecycle states for all AcObjectBase entities.
 */
enum class ActorLifecycleState : uint8_t {
    Uninitialized = 0,
    Initializing  = 1,
    Active        = 2,
    PausedOrDead  = 3
};

/**
 * @brief Base class for visual model representation (AcObjectShow).
 * RTTI: 12AcObjectShow (0x008C9270), Vtable at 0x008EB11C.
 */
struct AcObjectShow {
    void* vptr;                     // +0x00: Vtable at 0x008EB11C
    uint8_t show_flags;             // +0x04
    uint8_t pad[3];
    void*   model_instance;         // +0x08: Pointer to nw::gfx / sead Model
    Vector3 model_scale;            // +0x0C
};

/**
 * @brief Base entity class for all interactive world objects (AcObjectBase).
 * RTTI: 12AcObjectBase (0x008C9250), Vtable at 0x0083CF98.
 */
struct AcObjectBase {
    void*               vptr;               // +0x00: Vtable at 0x0083CF98
    uint8_t             pad0[16];           // +0x04
    Vector3             current_pos;        // +0x14: Current world position
    Vector3             prev_pos;           // +0x20: Previous world position
    int16_t             acre_x;             // +0x2C: Acre coordinate X (0..6)
    int16_t             acre_y;             // +0x2E: Acre coordinate Y (0..5)
    uint16_t            acre_id;            // +0x30: ID of current acre
    uint8_t             pad1[42];           // +0x32
    Vector3             velocity;           // +0x60: Linear velocity (integrated in FUN_001f4b94)
    Vector3             world_pos;          // +0x6C: World reference position
    Vector3             visual_offset;      // +0x78: Visual displacement offset
    Rot3                rotation;           // +0x84: Euler angles
    float               shadow_radius;      // +0x8C: Shadow circle radius
    float               shadow_aspect;      // +0x90: Shadow squish aspect
    float               shadow_alpha;       // +0x94: Shadow darkness alpha
    uint8_t             pad2[4];            // +0x98
    float               ground_height;      // +0xA0: Sampled terrain elevation
    uint8_t             pad3[4];            // +0xA4
    uint8_t             is_visible;         // +0xA8: Visibility flag
    uint8_t             pad4[19];           // +0xA9
    int32_t             acre_tile_x;        // +0xBC: Tile X within town (0..111)
    int32_t             acre_tile_z;        // +0xC0: Tile Z within town (0..95)
    uint8_t             pad5[2];            // +0xC4
    ActorLifecycleState lifecycle_state;    // +0xC6: 0=Uninit, 1=Init, 2=Active, 3=Dead
    uint8_t             flags_c7;           // +0xC7
    AcObjectShow*       show_component;     // +0xC8: Model renderer component
};

/**
 * @brief Normal villager actor (AcNpcNml).
 * RTTI: 8AcNpcNml (0x008CB2AC), Vtable at 0x008F7304.
 */
struct AcNpcNml : public AcObjectBase {
    uint8_t  npc_personality;       // Personality type (0..7)
    uint8_t  current_emotion;       // Emoticon ID
    uint16_t villager_id;           // Villager database index
    Vector3  target_wander_pos;     // Current wander waypoint
    uint32_t ai_state;              // Current AI action state
    uint32_t speech_recept_flags;   // Dialogue reception bitmask
};

/**
 * @brief Player character actor (AcPlayer).
 * RTTI: 8AcPlayer (0x008CB2B8), Vtable at 0x008F74B0.
 */
struct AcPlayer : public AcObjectBase {
    uint8_t  player_index;          // 0..3 (P1, P2, P3, P4)
    uint8_t  equipped_tool_category;// 0=None, 1=Shovel, 2=Rod, 3=Net, 4=Axe, 5=Can, 6=Slingshot
    uint16_t equipped_tool_item_id; // Item ID of tool in hand
    uint32_t locomotion_mode;       // 0=Idle, 1=Walk, 2=Jog, 3=Run, 4=Sneak
    float    run_dust_timer;        // Dust particle timer
    uint32_t pocket_selection;      // Active pocket inventory index
};

#pragma pack(pop)

} // namespace acnl

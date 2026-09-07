#pragma once
#include <cstdint>
#include <array>

namespace acnl::pwp {

#pragma pack(push, 1)

// ============================================================================
// Public Works Projects Limits & Footprints
// ============================================================================
namespace PWPConfig {
    constexpr uint32_t MAX_BUILT_PROJECTS        = 30;   // Maximum completed PWPs in town
    constexpr uint32_t MAX_UNLOCKED_PROJECTS     = 40;   // 0x28 unlocked project list capacity
    constexpr uint32_t ACTIVE_CONSTRUCTION_SLOTS = 6;    // Slots for active/pending projects
    constexpr uint32_t COLLISION_GRID_SIZE       = 10;   // 10x10 tile clearance buffer
    constexpr uint32_t PLACEMENT_GRID_SIZE       = 8;    // 8x8 tile foundation buffer
}

// Single Public Works Project Slot in Save File (770 bytes / 0x302)
// Initialized by FUN_002D4F24
struct PWPEntryRecord {
    uint8_t   unknown_header[0x1E];       // +0x000
    uint8_t   is_active;                  // +0x01E: 1 = under construction / placed
    uint8_t   completion_state;           // +0x01F: 0 = fundraising, 1 = completed
    uint8_t   pad_20[2];                  // +0x020
    uint8_t   metadata_block[0x40];       // +0x022..0x061
    uint32_t  collision_grid[10][10];     // +0x062..0x1F1: 10x10 collision test tile buffer (400 bytes)
    uint32_t  placement_grid[8][8];       // +0x1F2..0x2F1: 8x8 foundation footprint buffer (256 bytes)
    uint16_t  structure_item_id;          // +0x2F2: Structure item identifier (e.g. 0x23A0)
    uint16_t  secondary_item_id;          // +0x2F6: Ancillary item identifier (e.g. 0x243D)
    uint8_t   pad_2fa[8];                 // +0x2FA..0x301
};
static_assert(sizeof(PWPEntryRecord) == 0x302, "PWPEntryRecord size mismatch");

// Public Works Projects Master Save Sub-Block (4,648 bytes / 0x1228)
// Located at offset 0x5D880 in garden_plus.dat (4 instances for 18,508 bytes)
// Initialized by FUN_0024DBC4
struct PWPSaveBlock {
    uint8_t         unknown_header[0x18]; // +0x0000
    PWPEntryRecord  projects[6];          // +0x0018..0x1223: 6 Construction / active slots (0x302 each)
    uint16_t        built_projects_count; // +0x1224: Current number of built PWPs in town
    uint8_t         max_allowed_projects; // +0x1226: Hardcoded limit (0x1E = 30)
    uint8_t         pad_1227;             // +0x1227
};
static_assert(sizeof(PWPSaveBlock) == 0x1228, "PWPSaveBlock size mismatch");

// Unlocked Projects List (40 bytes / 0x28)
// Modified by PWP_UnlockProject (0x0024DD34)
struct PWPUnlockedList {
    uint8_t  project_ids[40];             // 0x00..0x27: Array of unlocked project IDs
};
static_assert(sizeof(PWPUnlockedList) == 0x28, "PWPUnlockedList size mismatch");

#pragma pack(pop)

} // namespace acnl::pwp

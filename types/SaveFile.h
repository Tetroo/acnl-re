#pragma once
#include <cstdint>
#include <array>
#include "TerrainTile.h"
#include "Item.h"

namespace acnl::save {

// Total garden_plus.dat size
inline constexpr size_t SAVE_GARDEN_PLUS_SIZE = 563968; // 0x89AB7 + 1 = 0x89AB8? (approx 550KB)

// ============================================================================
// Major Save Sub-Object Offsets in garden_plus.dat
// ============================================================================
inline constexpr size_t OFFSET_TILES_ITEMS      = 0x00020;
inline constexpr size_t OFFSET_VILLAGERS        = 0x29220; // 10 Villagers slots (0x22BCC bytes total)
inline constexpr size_t VILLAGER_SLOT_SIZE      = 0x02518; // 9,496 bytes per villager
inline constexpr size_t VILLAGER_COUNT          = 10;

inline constexpr size_t OFFSET_FIELD_OBJECTS    = 0x4BE00;
inline constexpr size_t OFFSET_EXHIBITION       = 0x502BC;
inline constexpr size_t OFFSET_FIELD_TOWN       = 0x52BB0;

inline constexpr size_t OFFSET_TOWN_REGION_CRC  = 0x53424; // 4-byte CRC32
inline constexpr size_t OFFSET_TOWN_MASTER_GRID = 0x53400; // Master Grid Object
inline constexpr size_t OFFSET_TOWN_ACRES_7X6   = 0x53484; // 42 Acre IDs (84 bytes)
inline constexpr size_t OFFSET_PLAYABLE_ACRES   = 0x534D8; // 20 Playable Acre structures

inline constexpr size_t OFFSET_TOWN_BUILDINGS   = 0x5D880; // Public Works Projects (PWP)
inline constexpr size_t OFFSET_TOWN_GATES_FLAGS = 0x621CC;
inline constexpr size_t OFFSET_MAIN_STREET      = 0x6AE38; // Shops & Museum
inline constexpr size_t OFFSET_TOWN_HALL_PLAZA  = 0x6E1F4; // Town Hall, Town Tree
inline constexpr size_t OFFSET_CAMPSITE_RV_PARK = 0x70E60; // Harvey's RV Campground (Welcome amiibo)
inline constexpr size_t OFFSET_AMIIBO_WISP      = 0x717EC; // Wisp magic lamp state

inline constexpr size_t OFFSET_PLAYERS          = 0x73958; // 4 Player slots
inline constexpr size_t PLAYER_SLOT_SIZE        = 0x05858; // Approx 22,616 bytes per player
inline constexpr size_t PLAYER_COUNT            = 4;

// ============================================================================
// Villager Data Structure (0x2518 = 9,496 bytes)
// ============================================================================
#pragma pack(push, 1)

struct VillagerRecord {
    uint16_t villagerId;       // Species and personality ID
    uint8_t  personality;      // Jock, Lazy, Smug, Cranky, Normal, Peppy, Snooty, Sisterly
    uint8_t  statusFlags;      // In town, moving in, boxed up to move out
    uint16_t catchphrase[16];  // UTF-16 catchphrase string
    uint16_t housePosX;        // Town tile X coordinate
    uint16_t housePosZ;        // Town tile Z coordinate
    uint8_t  padding[0x2518 - 42]; // Internal dialogue memory, furniture layout, letters
};
static_assert(sizeof(VillagerRecord) == VILLAGER_SLOT_SIZE, "VillagerRecord must be exactly 9,496 bytes");

#pragma pack(pop)

} // namespace acnl::save

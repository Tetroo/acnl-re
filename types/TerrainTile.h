#pragma once
#include <cstdint>
#include <array>

namespace acnl::world {

// ============================================================================
// Grid Dimension Constants
// ============================================================================
inline constexpr size_t ACRE_TILE_DIM = 16;
inline constexpr size_t ACRE_TILE_COUNT = ACRE_TILE_DIM * ACRE_TILE_DIM; // 256 tiles

inline constexpr size_t TOWN_ACRE_COLS = 7;
inline constexpr size_t TOWN_ACRE_ROWS = 6;
inline constexpr size_t TOWN_TOTAL_ACRES = TOWN_ACRE_COLS * TOWN_ACRE_ROWS; // 42 acres

inline constexpr size_t PLAYABLE_ACRE_COLS = 5;
inline constexpr size_t PLAYABLE_ACRE_ROWS = 4;
inline constexpr size_t PLAYABLE_TOTAL_ACRES = PLAYABLE_ACRE_COLS * PLAYABLE_ACRE_ROWS; // 20 acres

inline constexpr size_t TOWN_TOTAL_TILES_X = TOWN_ACRE_COLS * ACRE_TILE_DIM; // 112 tiles
inline constexpr size_t TOWN_TOTAL_TILES_Y = TOWN_ACRE_ROWS * ACRE_TILE_DIM; // 96 tiles

inline constexpr size_t PLAYABLE_TILES_X = PLAYABLE_ACRE_COLS * ACRE_TILE_DIM; // 80 tiles
inline constexpr size_t PLAYABLE_TILES_Y = PLAYABLE_ACRE_ROWS * ACRE_TILE_DIM; // 64 tiles

// ============================================================================
// Enums
// ============================================================================
enum class GrassPattern : uint8_t {
    Circle   = 0,
    Triangle = 1,
    Square   = 2
};

enum class NativeFruit : uint8_t {
    Apple  = 0,
    Orange = 1,
    Pear   = 2,
    Peach  = 3,
    Cherry = 4
};

enum class RiverMouthSide : uint8_t {
    West = 0, // Sea side left (river mouth flows west)
    East = 1  // Sea side right (river mouth flows east)
};

// ============================================================================
// Binary Data Structures (garden_plus.dat and memory layout)
// ============================================================================
#pragma pack(push, 1)

using AcreId = uint16_t;

// 42-Acre Master Town Grid (Offset +0x53484 in garden_plus.dat)
struct TownAcreMap {
    std::array<std::array<AcreId, TOWN_ACRE_COLS>, TOWN_ACRE_ROWS> acres; // 7x6 = 42 acres (84 bytes)
};
static_assert(sizeof(TownAcreMap) == 84, "TownAcreMap must be exactly 84 bytes");

// 20-Acre Template Layout (TemplateData/village/sea_side_*.bin)
struct VillageTemplateLayout {
    std::array<std::array<AcreId, PLAYABLE_ACRE_COLS>, PLAYABLE_ACRE_ROWS> acres; // 5x4 = 20 acres (40 bytes)
};
static_assert(sizeof(VillageTemplateLayout) == 40, "VillageTemplateLayout must be exactly 40 bytes");

// Individual 32-bit Terrain Tile representation
struct TerrainTile {
    uint16_t itemId;     // Associated Item ID / Object placed on this tile
    uint8_t  elevation;  // Terrace elevation level (0..3) & cliff edge flags
    uint8_t  flags;      // Collision, water flag, dig spot, pattern flag
};
static_assert(sizeof(TerrainTile) == 4, "TerrainTile must be exactly 4 bytes");

// 16x16 Tile Data for an entire Acre (1024 bytes per acre)
struct AcreTileGrid {
    std::array<std::array<TerrainTile, ACRE_TILE_DIM>, ACRE_TILE_DIM> tiles;
};
static_assert(sizeof(AcreTileGrid) == 1024, "AcreTileGrid must be exactly 1024 bytes");

#pragma pack(pop)

} // namespace acnl::world

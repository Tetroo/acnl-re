#pragma once

#include <cstdint>
#include <array>

namespace acnl::world {

#pragma pack(push, 1)

/**
 * @brief Rock appearance / visual categories (0x71..0x7A in exefs.elf).
 * Each rock model has a base ID and an activated/hit ID (+5).
 */
enum class RockType : uint8_t {
    NormalA      = 0x71,
    NormalA_Hit  = 0x76,
    MoneyRock    = 0x72, // Designated Money Rock of the day
    MoneyRock_Hit= 0x77,
    NormalB      = 0x73,
    NormalB_Hit  = 0x78,
    NormalC      = 0x74,
    NormalC_Hit  = 0x79,
    NormalD      = 0x75,
    NormalD_Hit  = 0x7A
};

/**
 * @brief Item IDs of mineral gems extracted from rock hits (0x0059EA7C / STR_Item_name.umsbt).
 */
enum class OreItem : uint16_t {
    GoldNugget   = 0x20A2,
    SilverNugget = 0x20A3,
    Ruby         = 0x20A4,
    Sapphire     = 0x20A5,
    Emerald      = 0x20A6,
    Amethyst     = 0x20A7
};

/**
 * @brief Item IDs for Bell currency rewards from Money Rock.
 */
enum class BellDropItem : uint16_t {
    Coin100      = 0x209E, // Single 100 Bells coin
    BagNormal    = 0x20AC, // Progressive Bell bag
    BagUpgraded  = 0x20AD, // High-value Bell bag (Good Money Luck 0x00)
    BagMax       = 0x2119  // Maximum 32,000 Bells sack
};

/**
 * @brief Shovel tool IDs for stone interactions (0x00766A5C).
 */
enum class ShovelTool : uint16_t {
    RentalShovel = 0x3359,
    NormalShovel = 0x335A,
    SilverShovel = 0x335B, // Enables mineral/gem drop chance on Money Rock
    GoldenShovel = 0x335C
};

/**
 * @brief Adjacent tile drop coordinate offset tables extracted from DAT_0085e0c8 & DAT_0085e0e8.
 * Defines 8 surrounding tiles around the stone in clockwise/spiral scan order.
 */
struct TileOffset {
    int8_t dx;
    int8_t dy;
};

inline constexpr std::array<TileOffset, 8> kRockDropOffsets = {{
    { 1,  1}, // North-East
    { 0,  1}, // North
    { 1,  0}, // East
    { 1, -1}, // South-East
    {-1,  1}, // North-West
    {-1, -1}, // South-West
    { 0, -1}, // South
    {-1,  0}  // West
}};

/**
 * @brief Standard 8-strike Bell payout progression for Money Rock (total 16,100 Bells).
 */
inline constexpr std::array<uint32_t, 8> kStandardMoneyRockPayouts = {{
    100,
    200,
    300,
    500,
    1000,
    2000,
    4000,
    8000
}};

/**
 * @brief Upgraded 8-strike Bell payout progression under Good Money Luck (total 32,000 Bells).
 */
inline constexpr std::array<uint32_t, 8> kUpgradedMoneyRockPayouts = {{
    100,
    200,
    500,
    1000,
    2000,
    4000,
    8000,
    16000
}};

#pragma pack(pop)

} // namespace acnl::world

#pragma once
#include <cstdint>

namespace acnl::flora {

#pragma pack(push, 1)

/**
 * @brief Tree growth stages (from nursery sapling to mature fruit-bearing tree).
 */
enum class TreeGrowthStage : uint8_t {
    NurserySapling = 0, // Day 1: Planted nursery sapling (0x005E)
    SmallTree      = 1, // Day 2: Small young tree (0x005F)
    MediumTree     = 2, // Day 3: Medium tree (0x0060)
    FullMatureTree = 3  // Day 4: Full adult tree (0x0061 / 0x0062..0x0066)
};

/**
 * @brief Fruit regrowth days after shaking.
 */
struct FruitRegrowth {
    static constexpr uint8_t DaysToRegrowNativeFruit = 3;
    static constexpr uint8_t DaysToRegrowIslandFruit = 3;
    static constexpr uint8_t PerfectTreeMaxHarvests = 5; // Drops rotten fruit and dies after 4-7 harvests
};

/**
 * @brief Flower condition state flags.
 */
enum class FlowerState : uint8_t {
    Healthy         = 0, // Normal flower
    Watered         = 1, // Watered by can or rain/snow (sparkles, survives next day)
    Wilted          = 2, // Dry/brown flower (will die tomorrow if not watered)
    Dead            = 3  // Disappears on 6:00 AM reset
};

/**
 * @brief Flower Hybrid Breeding cross-pollination pairs.
 */
enum class FlowerSpecies : uint8_t {
    Tulip      = 0,
    Pansy      = 1,
    Cosmos     = 2,
    Rose       = 3,
    Carnation  = 4,
    Lily       = 5,
    Violet     = 6
};

#pragma pack(pop)

} // namespace acnl::flora

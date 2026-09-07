#pragma once
#include <cstdint>
#include <array>

namespace acnl::inventory {

#pragma pack(push, 1)

/**
 * @brief Individual item slot in player pockets (4 bytes).
 */
struct PocketSlot {
    uint16_t item_id;     // 16-bit Item ID
    uint8_t  stack_count; // Fruit basket stack count (1..9), or 0 for unstacked
    uint8_t  flags;       // Bit 7: Wrapped in present paper, Bit 6: Letter attached
};
static_assert(sizeof(PocketSlot) == 4, "PocketSlot must be 4 bytes");

/**
 * @brief Player pockets container (16 slots = 64 bytes).
 * Stored inside Player save struct in garden_plus.dat.
 */
struct PlayerPockets {
    std::array<PocketSlot, 16> slots;

    static constexpr size_t SlotCount = 16;
    static constexpr uint8_t MaxFruitStack = 9;

    [[nodiscard]] bool isSlotEmpty(size_t index) const {
        if (index >= SlotCount) return true;
        return slots[index].item_id == 0x7FFE; // 0x7FFE = Empty slot
    }

    [[nodiscard]] bool canStack(size_t targetIndex, uint16_t fruitId) const {
        if (targetIndex >= SlotCount) return false;
        const auto& s = slots[targetIndex];
        return (s.item_id == fruitId && s.stack_count < MaxFruitStack);
    }
};
static_assert(sizeof(PlayerPockets) == 64, "PlayerPockets must be 64 bytes");

#pragma pack(pop)

} // namespace acnl::inventory

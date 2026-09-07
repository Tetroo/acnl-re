#pragma once
#include <cstdint>
#include <array>

namespace acnl::item {

using ItemId = uint16_t;

// ============================================================================
// Special Item Sentinel Values
// ============================================================================
inline constexpr ItemId ITEM_EMPTY         = 0x7FFE; // In memory empty slot (or 0x0000 depending on context)
inline constexpr ItemId ITEM_NONE          = 0x0000;
inline constexpr ItemId ITEM_BURIED_FOSSIL = 0x008E;
inline constexpr ItemId ITEM_BURIED_PITFALL= 0x008F;
inline constexpr ItemId ITEM_BURIED_GYROID = 0x0090;

// ============================================================================
// Item Categories & Ranges
// ============================================================================
enum class ItemCategory : uint8_t {
    Empty        = 0,
    Tool         = 1,
    Furniture    = 2,
    Clothing     = 3,
    Flora        = 4,  // Flowers, Trees, Bushes, Weeds
    Fauna        = 5,  // Insects, Fish, Deep-sea creatures
    FossilGyroid = 6,
    MailPresent  = 7,
    Miscellaneous= 8
};

// ============================================================================
// Player Pocket Inventory (16 Slots)
// ============================================================================
#pragma pack(push, 1)

struct InventorySlot {
    ItemId itemId;
    uint16_t flags; // customization / state / flags
};
static_assert(sizeof(InventorySlot) == 4, "InventorySlot must be 4 bytes");

struct PlayerPockets {
    std::array<InventorySlot, 16> slots; // 16 inventory pockets (64 bytes)
};
static_assert(sizeof(PlayerPockets) == 64, "PlayerPockets must be exactly 64 bytes");

// Equipped item slot
struct EquippedItem {
    ItemId itemId;
    uint16_t state;
};

#pragma pack(pop)

} // namespace acnl::item

#pragma once
#include <cstdint>
#include <array>

/**
 * @file Economy.h
 * @brief Shop, Commerce, Turnip Market & Re-Tail Subsystem for ACNL: Welcome amiibo
 * 
 * Reverses the exact game economy rules:
 * - Nookling Shop (Junction -> T&T Mart -> Super T&T -> T.I.Y. -> T&T Emporium)
 * - Turnip price trends and weekly schedule (morning 9:00 AM, afternoon 12:00 PM)
 * - Re-Tail flea market, Reese appraisal (100% vs Nookling 80%), and daily premium items
 * - Catalog terminal 640-byte player order history records
 */

namespace acnl::economy {

#pragma pack(push, 1)

// ============================================================================
// 1. Nookling Store Tiers & Upgrade Progression
// ============================================================================

enum class ShopTier : uint8_t {
    Junction     = 0,  ///< Nookling Junction (Initial tier)
    TT_Mart      = 1,  ///< T&T Mart (Convenience store)
    Super_TT     = 2,  ///< Super T&T (Supermarket)
    TIY          = 3,  ///< T.I.Y. (Home center / hardware store)
    TT_Emporium  = 4   ///< T&T Emporium (Department store)
};

struct ShopUpgradeThresholds {
    uint32_t bells_required;  ///< Spending required at current tier to unlock next
    uint32_t min_days_open;   ///< Minimum days current tier must be operating
};

/// Exact spending requirements from exefs.elf DAT_0088e5e4
inline constexpr std::array<uint32_t, 4> kShopSpendingThresholds = {
    12000,   ///< Junction -> T&T Mart: 12,000 Bells
    25000,   ///< T&T Mart -> Super T&T: 25,000 Bells
    50000,   ///< Super T&T -> T.I.Y.: 50,000 Bells
    100000   ///< T.I.Y. -> T&T Emporium: 100,000 Bells
};

/// Exact minimum operating days from exefs.elf DAT_0088e5f8
inline constexpr std::array<uint32_t, 4> kShopDaysThresholds = {
    7,       ///< Junction -> T&T Mart: 7 days town age
    10,      ///< T&T Mart -> Super T&T: 10 days since Mart opened
    21,      ///< Super T&T -> T.I.Y.: 21 days since Super opened
    30       ///< T.I.Y. -> T&T Emporium: 30 days since T.I.Y. opened
};

// ============================================================================
// 2. Turnip Market & Stalk Exchange
// ============================================================================

enum class TurnipPattern : uint32_t {
    LargeSpike   = 0,  ///< 3 decreasing half-days -> 3-period sharp spike (200% - 600%)
    SmallSpike   = 1,  ///< Moderate spike (140% - 200%)
    Decreasing   = 2,  ///< Consistent drop each half-day (Bad luck / rotting risk)
    Fluctuating  = 3   ///< Random variations (90% - 140%)
};

struct TurnipWeeklyPrices {
    uint32_t buy_price_joan;         ///< Sunday AM purchase price from Joan (90..110 Bells)
    TurnipPattern pattern;           ///< Active weekly market pattern (0..3)
    uint32_t prices[12];             ///< 12 half-day prices: [Mon_AM, Mon_PM, ..., Sat_PM]
};

// ============================================================================
// 3. Re-Tail Commerce & Item Appraisal
// ============================================================================

/// Pricing ratios relative to Re-Tail baseline sell price
inline constexpr float kReeseAppraisalRate      = 1.00f;  ///< Reese pays 100% of base item sell price
inline constexpr float kNooklingAppraisalRate   = 0.80f;  ///< Timmy & Tommy pay 80% (0.8x) of Reese's price
inline constexpr float kPremiumItemMultiplier   = 2.00f;  ///< Double price (200%) for daily blackboard item
inline constexpr float kCatalogReorderMultiplier= 4.00f;  ///< Catalog purchase price is 4x base sell value

struct FleaMarketSlot {
    uint16_t item_id;                ///< Displayed item ID
    uint16_t item_flags;             ///< Color/customization variant
    uint32_t price;                  ///< Asking price set by villager or player
    uint8_t  seller_player_idx;      ///< 0..3 for human players, 0xFF for NPC villager
    uint8_t  is_sold_out;            ///< 1 if item purchased
    uint8_t  padding[2];
};
static_assert(sizeof(FleaMarketSlot) == 12, "FleaMarketSlot must be 12 bytes");

struct ReTailState {
    uint16_t premium_item_id;        ///< Item of the day on blackboard (STR_Recycle_High)
    uint8_t  active_flea_slots;      ///< Number of occupied flea market spaces (max 8)
    uint8_t  padding;
    FleaMarketSlot flea_market[8];   ///< 8 flea market selling tables
};

// ============================================================================
// 4. Shop Floor Display & Sold Out Items
// ============================================================================

inline constexpr uint16_t kItemSoldOutNormal   = 0x2083; ///< Red Sold Out card for furniture/tools
inline constexpr uint16_t kItemSoldOutWallpaper= 0x2086; ///< Sold Out card for carpets/wallpaper

struct ShopDisplayItem {
    uint16_t item_id;                ///< Current item on sale
    uint16_t flags;
    uint8_t  is_purchased;           ///< Set to 1 upon purchase
    uint8_t  category;
    uint16_t reserved;
};

// ============================================================================
// 5. Catalog Terminal Record (0x280 = 640 bytes per player)
// ============================================================================

struct CatalogRecord {
    uint8_t  player_name[49];        ///< 0x00: Player name string
    uint8_t  pad0[3];
    uint8_t  town_name[49];          ///< 0x34: Town name string
    uint8_t  pad1[3];
    uint8_t  meta_data[66];          ///< 0x68: Metadata
    uint8_t  unlocked_bitfield[386]; ///< 0xAA: Bitmask of registered catalog items
    uint8_t  recent_orders[66];      ///< 0x22C: Recent order queue
    uint8_t  flags[5];               ///< 0x26E..0x272: Status flags
    uint8_t  pad2;
    uint32_t crc32;                  ///< 0x274: Record CRC32 checksum
    uint32_t last_order_time;        ///< 0x278: Timestamp of last order
};
static_assert(sizeof(CatalogRecord) == 0x280, "CatalogRecord must be exactly 640 bytes");

#pragma pack(pop)

} // namespace acnl::economy

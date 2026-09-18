#pragma once

#include <cstdint>
#include <array>

namespace acnl::luck {

#pragma pack(push, 1)

/**
 * @brief Daily luck categories in Animal Crossing: New Leaf.
 * Evaluated by Player_CalculateDailyLuckType (0x0023D750).
 * Stored globally in DAT_00952f68.
 */
enum class LuckType : uint8_t {
    MoneyGood        = 0, // Increased Bells from rocks & trees
    MoneyBad         = 1, // Decreased Bells, empty rocks
    FriendshipGood   = 2, // Enhanced friendship progression & villager gifts
    FriendshipBad    = 3, // Villagers colder, slower friendship
    GoodsGood        = 4, // Special items & merchant rare offerings
    GoodsBad         = 5, // Common items, fewer special dialogues
    OreGood          = 6, // High gem drop rate from silver shovel rock hits
    OreBad           = 7, // Standard ore rock drops only
    PhysicalGood     = 8, // Faster swimming, protective status
    PhysicalBad      = 9, // Tumble / trip stumble while running (TUMB)
    Neutral          = 10 // Normal day, no luck modifiers applied
};

/**
 * @brief 12 Astronomical Zodiac signs based on birth month and day.
 * Evaluated by Time_GetZodiacSignFromDate (0x0056AEE8) from table DAT_0088f142.
 */
enum class ZodiacSign : uint8_t {
    Capricorn   = 0,  // Dec 22 – Jan 19
    Aquarius    = 1,  // Jan 20 – Feb 18
    Pisces      = 2,  // Feb 19 – Mar 20
    Aries       = 3,  // Mar 21 – Apr 19
    Taurus      = 4,  // Apr 20 – May 20
    Gemini      = 5,  // May 21 – Jun 21
    Cancer      = 6,  // Jun 22 – Jul 22
    Leo         = 7,  // Jul 23 – Aug 22
    Virgo       = 8,  // Aug 23 – Sep 22
    Libra       = 9,  // Sep 23 – Oct 23
    Scorpio     = 10, // Oct 24 – Nov 22
    Sagittarius = 11  // Nov 23 – Dec 21
};

/**
 * @brief Zodiac cutoff date record (month and max day) from DAT_0088f142.
 */
struct ZodiacCutoff {
    uint8_t month;
    uint8_t maxDay;
};

// Exact 12 cutoff date pairs extracted from DAT_0088f142 in exefs.elf
inline constexpr std::array<ZodiacCutoff, 12> kZodiacCutoffDates = {{
    { 1, 19}, // Capricorn:   <= Jan 19
    { 2, 18}, // Aquarius:    <= Feb 18
    { 3, 20}, // Pisces:      <= Mar 20
    { 4, 19}, // Aries:       <= Apr 19
    { 5, 20}, // Taurus:      <= May 20
    { 6, 21}, // Gemini:      <= Jun 21
    { 7, 22}, // Cancer:      <= Jul 22
    { 8, 22}, // Leo:         <= Aug 22
    { 9, 22}, // Virgo:       <= Sep 22
    {10, 23}, // Libra:       <= Oct 23
    {11, 22}, // Scorpio:     <= Nov 22
    {12, 21}  // Sagittarius: <= Dec 21
}};

// Tumble / Tripping constants (0x00653EB0)
inline constexpr uint16_t ITEM_KING_TUT_MASK   = 0x28B8;
inline constexpr int16_t  TUMBLE_BASE_FRAMES   = 450;  // 15.0 seconds at 30 fps
inline constexpr int16_t  TUMBLE_RNG_WINDOW    = 300;  // 10.0 seconds at 30 fps (0..299)
inline constexpr float    TUMBLE_COLLISION_RAY = 24.0f; // Forward probe distance (0x41C00000)
inline constexpr uint8_t  ACTION_TUMBLE_SLIDE  = 0x9F;  // Action ID for sliding face-down

#pragma pack(pop)

/**
 * @brief Resolves Zodiac sign from month and day (Time_GetZodiacSignFromDate @ 0x0056AEE8).
 */
inline ZodiacSign getZodiacSign(uint8_t month, uint8_t day) {
    for (size_t i = 0; i < kZodiacCutoffDates.size(); ++i) {
        if (month < kZodiacCutoffDates[i].month) {
            return static_cast<ZodiacSign>(i);
        }
        if (month == kZodiacCutoffDates[i].month && day <= kZodiacCutoffDates[i].maxDay) {
            return static_cast<ZodiacSign>(i);
        }
    }
    return ZodiacSign::Capricorn;
}

} // namespace acnl::luck

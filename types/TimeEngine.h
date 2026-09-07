#pragma once
#include <cstdint>

namespace acnl {

#pragma pack(push, 1)

/**
 * @brief 9-byte raw date/time structure used in save files and RTC conversion.
 * Stored at offset 0x55A8 in garden_plus.dat.
 */
struct GameDateTime {
    uint16_t year;        // 2012..2050 (Base epoch: 2012, Max limit: 2050)
    uint8_t  month;       // 1..12
    uint8_t  day;         // 1..31
    uint8_t  day_of_week; // 0 = Sunday, 1 = Monday, ..., 6 = Saturday
    uint8_t  hour;        // 0..23 (Daily rollover boundary is 06:00:00 AM)
    uint8_t  minute;      // 0..59
    uint8_t  second;      // 0..59
    uint8_t  subsecond;   // Fractional tick / frame counter
};
static_assert(sizeof(GameDateTime) == 9, "GameDateTime must be 9 bytes");

/**
 * @brief Save file clock header record at offset 0x55A6 in garden_plus.dat.
 */
struct SaveClockRecord {
    uint16_t flags;               // 0x55A6: Status flags
    GameDateTime last_saved_time; // 0x55A8: Last saved timestamp (9 bytes)
    uint8_t  dst_flag;            // 0x55BA: Daylight Saving Time / Summer Time toggle (bit 0)
};

/**
 * @brief Turnip price forecast patterns generated every Sunday at 6:00 AM.
 */
enum class TurnipPattern : uint32_t {
    Fluctuating = 0, // Random ups and downs (50..140 bells)
    LargeSpike  = 1, // Rapid surge up to 600 bells
    Decreasing  = 2, // Monotonic loss every half-day (0.9x..0.8x)
    SmallSpike  = 3  // Modest surge up to 200 bells
};

/**
 * @brief 12-price turnip tracking array (Mon AM to Sat PM).
 */
struct TurnipForecast {
    TurnipPattern pattern;
    uint16_t buy_price;      // Joan's Sunday price (90..110 bells)
    uint16_t sell_prices[12];// [0]=Mon AM, [1]=Mon PM, ..., [11]=Sat PM
};

/**
 * @brief Villager wake/sleep schedule based on personality.
 */
struct VillagerSchedule {
    uint8_t wake_hour;
    uint8_t wake_minute;
    uint8_t sleep_hour;
    uint8_t sleep_minute;
};

/**
 * @brief Daily spawn limits rolled over at 6:00 AM.
 */
struct DailyWorldSpawnLimits {
    static constexpr uint8_t MaxFossilsPerDay = 4;
    static constexpr uint8_t MaxPitfallsPerDay = 1;
    static constexpr uint8_t MaxGyroidsAfterRain = 3;
    static constexpr uint8_t MaxMoneyRocksPerDay = 1;
    static constexpr uint8_t MaxOreRocksPerDay = 1;
    static constexpr uint8_t MaxNewWeedsPerDay = 3;
};

/**
 * @brief Internal clock subsystem state (matching 0x0095B4D0 - 0x0095B548).
 */
struct ClockSubsystemState {
    uint8_t  is_initialized;      // 0x0095B4D0
    uint8_t  clock_status_flags;  // 0x0095B4D1
    uint8_t  time_limit_reached;  // 0x0095B4D2: Bit 0 = Clamped to 2050, Bit 1 = RTC error
    uint8_t  pad;
    int64_t  current_tick_count;  // 0x0095B4D8: 64-bit microsecond counter
    int64_t  hardware_rtc_base;   // 0x0095B4E0: Raw 3DS RTC tick count
    int64_t  user_time_offset;    // 0x0095B4E8: Time-travel offset applied by player
    int64_t  system_drift_offset; // 0x0095B4F8: Correction offset
    uint64_t epoch_2000_ticks;    // 0x0095B528: Base year 2000 ticks
    uint64_t epoch_2012_ticks;    // 0x0095B530: Epoch year 2012 ticks
    uint64_t limit_2051_ticks;    // 0x0095B538: Upper boundary 2051 ticks
};

#pragma pack(pop)

} // namespace acnl

#pragma once
#include <cstdint>
#include <array>

namespace acnl::weather {

#pragma pack(push, 1)

// Hourly weather types resolved by Thunk_ResolveWeatherType (0x0062E084)
enum class WeatherType : uint32_t {
    Fine          = 0, // Sunny / Clear Sky
    Cloudy        = 1, // Overcast
    RainClouds    = 2, // Pre-rain overcast
    LightRain     = 3, // Light Rain (32 particles)
    HeavyRain     = 4, // Heavy Rain / Thunderstorm (128 particles + Lightning)
    LightSnow     = 5, // Light Snow (100 particles)
    Blizzard      = 6  // Heavy Snow / Blizzard (400 particles)
};

// Particle limits and interpolation constants
namespace PrecipitationConfig {
    constexpr uint32_t LIGHT_RAIN_PARTICLES  = 32;   // 0x20
    constexpr uint32_t HEAVY_RAIN_PARTICLES  = 128;  // 0x80
    constexpr uint32_t LIGHT_SNOW_PARTICLES  = 100;
    constexpr uint32_t BLIZZARD_PARTICLES    = 400;
    constexpr float    TRANSITION_DURATION_S = 30.0f; // Crossfade at top-of-hour (seconds 0..29)
    constexpr float    LERP_STEP             = 1.0f / 30.0f; // 0.033333335f
}

// BsThunderMgr actor (actor ID 0x011A, class 0x0026, size 0x64 = 100 bytes)
// Vtable: 0x008EC120
struct BsThunderMgr {
    void*     vtable;                     // +0x00: 0x008EC120
    uint8_t   pad_04[0x0F];               // +0x04..0x12
    uint8_t   minute_strike_map[60];      // +0x13: 1 byte per second of the minute (1 = strike, 0 = idle)
    uint8_t   cached_second;              // +0x4F: Current second tracker
    uint32_t  cached_time_lo;             // +0x50: Time snapshot lo
    uint32_t  cached_time_mid;            // +0x54: Time snapshot mid
    uint8_t   cached_hour;                // +0x57: Hour when schedule was generated
    uint8_t   cached_minute;              // +0x58: Minute when schedule was generated
    uint8_t   pad_59[7];                  // +0x59..0x5F
    uint8_t   force_strike_trigger;       // +0x60: Forced strike flag
    uint8_t   pad_61[3];                  // +0x61..0x63
};
static_assert(sizeof(BsThunderMgr) == 0x64, "BsThunderMgr size mismatch");

// Shooting star slot within BsShootingStarMgr (4 slots total)
struct ShootingStarSlot {
    float     pos_x;                      // +0x00
    float     pos_y;                      // +0x04
    float     pos_z;                      // +0x08
    int32_t   lifetime_frames;            // +0x0C
    uint8_t   has_wished;                 // +0x10
    uint8_t   pad_11[3];                  // +0x11..0x13
};

// BsShootingStarMgr actor (actor ID 0x0119, class 0x0025, size 0x3A4 = 932 bytes)
// Vtable: 0x008F1410
struct BsShootingStarMgr {
    void*     vtable;                     // +0x00: 0x008F1410
    uint8_t   pad_04[0x194];              // +0x04..0x197
    uint8_t   minute_star_map[60];        // +0x198..0x1D3: 60-second star spawn map
    uint32_t  cached_time_lo;             // +0x1D4
    uint32_t  cached_time_mid;            // +0x1D8
    uint8_t   cached_hour;                // +0x1DC
    uint8_t   cached_minute;              // +0x1DD
    uint8_t   pad_1DE[2];                 // +0x1DE..0x1DF
    float     star_offset_x[4];           // +0x1E0..0x1EF
    float     star_offset_y[4];           // +0x1F0..0x1FF
    float     star_offset_z[4];           // +0x200..0x20F
    int32_t   slot_timers[4];             // +0x210..0x21F: Remaining lifetime frames
    uint8_t   wish_sound_triggers[4];     // +0x220..0x223: Flag to trigger 0x01000570 sound
    uint8_t   audio_emitters[4 * 0x60];   // +0x224..0x3A3: 4 positional audio emitters
};
static_assert(sizeof(BsShootingStarMgr) == 0x3A4, "BsShootingStarMgr size mismatch");

// Audio sound IDs for weather and sky events
namespace WeatherSoundId {
    constexpr uint32_t THUNDER_OUTDOORS   = 0x01000780; // Sharp close lightning strike
    constexpr uint32_t THUNDER_INDOORS    = 0x0100077F; // Muffled interior thunder rumble
    constexpr uint32_t SHOOTING_STAR_PASS = 0x01000570; // High pitch chime streak
    constexpr uint32_t STAR_WISH_SPARKLE  = 0x01000571; // Sparkle chime on successful player wish
}

#pragma pack(pop)

} // namespace acnl::weather

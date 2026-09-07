#pragma once
#include <cstdint>
#include <array>

/**
 * @file Audio.h
 * @brief Sound, BGM & Speech Synthesis Engine for ACNL: Welcome amiibo
 * 
 * Nintendo Opus & Sound Object runtime specifications:
 * - Hourly Town BGM (24 hourly compositions x 3 weather variations: Sunny, Rainy, Snowy)
 * - Minute 59 crossfade timer calculation
 * - Positional outdoor sound emitters (SoundObjRiver, SoundObjFall waterfall)
 * - Animalese / Bebebese NPC voice synthesizer with chromatic semitone pitch shifting
 */

namespace acnl::audio {

#pragma pack(push, 1)

// ============================================================================
// 1. Stage BGM Opus Categories & Routing
// ============================================================================

enum class StageOpusType : uint32_t {
    Null            = 0,
    Title           = 1,
    VillageDowntown = 3,   ///< 24 hourly tracks + Main Street
    Tour            = 0x1E,///< Tortimer Island minigames
    TropicalIsland  = 0x21,///< Kapp'n boat & Island resort
    TownHall        = 0x28,///< Isabelle town hall
    ClubLOL         = 0x2F,///< DJ KK / Live Saturday concerts
    SpecialEvent    = 0x33,///< Fireworks, Halloween, New Year's countdown
    CampgroundRV    = 0x38 ///< Welcome amiibo campground
};

// ============================================================================
// 2. 24 Hourly Background Music (BGM)
// ============================================================================

enum class WeatherBgmVariant : uint8_t {
    Sunny = 0,             ///< Standard acoustic arrangement
    Rain  = 1,             ///< Muted acoustics with gentle piano & raindrops
    Snow  = 2              ///< Crystalline glockenspiel, bells & soft pads
};

struct HourlyBgmTrack {
    uint8_t hour;          ///< 0..23 (00:00 to 23:00)
    uint16_t sunny_track_id;
    uint16_t rain_track_id;
    uint16_t snow_track_id;
};

// ============================================================================
// 3. Animalese Speech Synthesizer Pitch Modulation
// ============================================================================

inline constexpr uint32_t kSfxAnimaleseVoice = 0x01000565;

/**
 * Chromatic semitone transposition offsets from exefs.elf 0x001D9E6C:
 * Modulates raw phoneme samples to match NPC personality pitch.
 */
inline constexpr std::array<int16_t, 13> kAnimalesePitchScale = {
    -5, -3, -1, 0, 2, 4, 5, 7, 9, 11, 12, 14, 16
};

inline constexpr int16_t kAnimaleseBaseTransposition = -9;

// ============================================================================
// 4. Positional 3D Sound Emitters
// ============================================================================

inline constexpr float    kRiverAudioCullingRadius = 300.0f; ///< Inaudible beyond 300 units
inline constexpr uint32_t kSfxWaterfallRoar        = 0x01000786;

struct SoundObjRiverState {
    uint32_t vtable;        ///< 0x008ED618
    uint8_t  is_active;     ///< +0x12: 1 if player within 300 units
    uint8_t  padding[3];
    float    nearest_pos[3];///< +0x13..+0x15: Spline-interpolated 3D emitter coordinates
    uint32_t sound_handle;  ///< Active audio channel handle
};

struct SoundObjFallState {
    uint32_t vtable;        ///< 0x008EC6B4
    uint8_t  is_active;     ///< 1 if playing
    uint8_t  padding[3];
    float    world_pos[3];  ///< 3D world coordinates of waterfall cascade
    uint32_t sfx_id;        ///< Always 0x01000786
};

#pragma pack(pop)

// ============================================================================
// 5. Minute 59 Crossfade Formula (exefs.elf 0x00588BEC)
// ============================================================================

/**
 * @brief Computes audio fade frames remaining until the top of the hour
 * @param minute Current minute (0..59)
 * @param second Current second (0..59)
 * @param millisecond Current millisecond (0..999)
 * @return Number of 30 FPS video/audio frames over which to crossfade
 */
inline int32_t CalculateHourlyCrossfadeFrames(int32_t minute, int32_t second, int32_t millisecond) {
    if (minute == 59) {
        int32_t ms_remaining = 60000 - ((second + 5) * 1000 + millisecond);
        if (ms_remaining * 30 < -999) {
            return 0;
        }
        return (ms_remaining * 30) / 1000;
    }
    return 20; // Default 20-frame crossfade
}

} // namespace acnl::audio

# Weather & Environmental System Specification

## 1. Architecture Overview

The weather subsystem in *Animal Crossing: New Leaf — Welcome amiibo* is a fully deterministic, data-driven simulation engine tightly coupled to the wall-clock calendar and game day lifecycle. It controls:
- **Global sky & ambient lighting** (sunlight direction, ambient colors, fog, cloud tints, horizon gradients).
- **Precipitation & particle systems** (rain droplets, snowflakes, falling cherry blossom petals, confetti/paper scraps).
- **Audio ambiance** (wind sounds, rain on roofs/umbrellas, thunder claps).

The system operates across three distinct abstraction layers:
1. **Macro Level (Weekly Table):** A hardcoded $52 \times 24$ byte matrix indexing the base weather type for every hour of every week of the year.
2. **Micro Level (Hourly LERP Interpolator):** A per-frame interpolator (`Weather_InterpolateWeatherEnvironment` / `0x001E63DC`) that smoothly blends between the current hour's record and the upcoming hour's record from `EnvironmentParameter.bin`.
3. **GPU Uniform Streamer:** A dedicated shader uniform block (`DAT_009b1978`) pushed to the PICA200 vertex and fragment shaders every frame.

---

## 2. Weather Types & Promotion Logic

The engine defines 7 primary weather types:

| Type ID | Internal Name | Group | Particle Script | Model Asset | Description |
|---|---|---|---|---|---|
| `0` | Clear / Sunny | 0, 3, 5, 6 | None | None | Crisp clear sky with sun/stars |
| `1` | Cloudy / Overcast | 0, 3, 5, 6 | None | None | High cloud cover, diffused sun |
| `2` | Light Rain | 1, 4 | `BsWeatherRain` | `Weather/ame_model.bcres` | Gentle rain showers |
| `3` | Heavy Rain | 1 | `BsWeatherRain` | `Weather/ame_model.bcres` | Intense rain, dark overcast |
| `4` | Snow | 2 | `BsWeatherSnow` | `Weather/snow_model.bcres` | Snowfall (winter season) |
| `5` | Thunderstorm | 1 | `BsWeatherRain` | `Weather/ame_model.bcres` | Promoted type 3 (lightning & heavy thunder) |
| `6` | Blizzard | 2 | `BsWeatherSnow` | `Weather/snow_model.bcres` | Promoted type 4 (severe wind & whiteout fog) |

### Seasonal Event Particle Systems
In addition to rain and snow, the engine features two seasonal particle behaviour scripts:
- **`BsWeatherSakura` (`0x002960D0`):** Spring cherry blossom petals drifting across the screen (model: `cherryblossoms_model.bcres`, shader: `CherryShader.shbin`).
- **`BsWeatherPaper` (`0x002663F0`):** Confetti/paper scraps for Countdown/New Year festivities (model: `paper_model.bcres`, shader: `PaperShader.shbin`).

---

## 3. Weather Resolution Algorithm (`Weather_ResolveWeatherType_52x24` / `0x0062E094`)

Each frame, the game invokes `Weather_ResolveWeatherType_52x24` to compute the target weather type:

```c
char Weather_ResolveWeatherType_52x24(DateObject* date) {
    int state = Core_GetStateIndex();
    
    // 1. Interior and special state exclusion
    // States 0x5D, 0x99, 0x9B are indoors / special UI screens: always clear/dry
    if (state == 0x5D || state == 0x99 || state == 0x9B) {
        return 0;
    }
    
    // 2. Manual debug / event override check
    // If DAT_0095cf98 != 7, a fixed override is forced (values 0..6)
    char override_type = DAT_0095cf98;
    if (override_type != 7) {
        return override_type;
    }
    
    // 3. Resolve week row index
    uint32_t week_row;
    if (!Core_CheckStateFlag(0x20000, state)) {
        // Main town: 52-week calendar period
        week_row = Weather_ResolveWeatherPeriodIndex(date);
    } else {
        // Tortimer Island: 3-day tropical cycle (JulianDay % 3 -> [4, 21, 33])
        week_row = Weather_ResolveIslandWeatherWeekIndex(date);
    }
    
    // 4. Animal Crossing Game-Day Hour Normalization (06:00 AM Boundary)
    // In ACNL, a new day begins at 06:00 AM, not 00:00.
    int raw_hour = date->hour;
    int game_hour = raw_hour - 6;
    if (game_hour < 0) {
        game_hour += 24; // 00:00 -> game hour 18, 05:00 -> game hour 23
    }
    
    // 5. Matrix lookup
    char weather_type = 0;
    if (week_row < 52 && game_hour < 24) {
        weather_type = WeatherTable[week_row][game_hour];
    }
    
    // 6. Thunderstorm & Blizzard Promotion
    if (!Core_CheckStateFlag(0x20000, state)) {
        // Check season parameter 8 (is_heavy_weather_enabled)
        if (Weather_GetSeasonParameter(8) == 1) {
            if (weather_type == 3) {
                weather_type = 5; // Promote Heavy Rain -> Thunderstorm
            } else if (weather_type == 4) {
                weather_type = 6; // Promote Snow -> Blizzard
            }
        }
    }
    
    return weather_type;
}
```

---

## 4. `EnvironmentParameter.bin` Data Format

The binary asset `romfs_out/Environment/EnvironmentParameter.bin` (30,256 bytes) defines the visual parameters of the sky, lighting, and atmosphere.

### File Layout
- **Header (16 bytes):**
  - `0x00`: `0x00000BB8` (3000 dec)
  - `0x04`: `0x000003E8` (1000 dec)
  - `0x08..0x0F`: Reserved zeroes
- **Records (216 entries = 9 Groups $\times$ 24 Hours):**
  - Stride: **`0x8C` (140 bytes)** per record.
  - Total data size: $216 \times 140 = 30,240$ bytes ($+ 16 = 30,256$ bytes).

### Record Structure (`0x8C` bytes)
```cpp
#pragma pack(push, 1)
struct EnvironmentRecord {
    // 0x00 - 0x0F: Primary 4-color palette (RGBA, Alpha = 0xFF)
    uint32_t sky_primary_color;       // +0x00: Sky dome zenith / primary fog
    uint32_t sun_light_color;         // +0x04: Direct sunlight / moon illumination
    uint32_t ambient_light_color;     // +0x08: Ground/terrain ambient bounce
    uint32_t shadow_tint_color;       // +0x0C: Shadow color tone
    
    // 0x10 - 0x67: 22 Environmental RGB Color Vectors (88 bytes)
    // Lerped and normalized into [0.0f, 1.0f] float4 vectors
    struct {
        uint8_t r, g, b, unused;
    } color_channels[22];
    
    // 0x68 - 0x8B: Atmosphere & Lighting Float Parameters
    float fog_start_distance;         // +0x68: Linear fog start
    float fog_near_distance;          // +0x6C: Near plane fog cutoff
    float fog_density_multiplier;     // +0x70: Density scale (multiplied by weather scalar)
    uint32_t sky_blend_mode;          // +0x74: Sky texture compositing mode
    float cloud_layer_alpha;          // +0x78: Cloud opacity & horizon coverage
    float sun_bloom_intensity;        // +0x7C: Sun shafts / god-ray intensity (outdoor flag)
    float shadow_attenuation;         // +0x80: Directional shadow softness
    float ambient_multiplier;         // +0x84: Global ambient intensity multiplier
    float directional_multiplier;     // +0x88: Directional sun light multiplier
};
#pragma pack(pop)
```

---

## 5. Smooth Inter-Hour Interpolation (`Weather_InterpolateWeatherEnvironment` / `0x001E63DC`)

Rather than abruptly switching parameters on the hour, the game performs continuous linear interpolation ($LERP$) every frame:

$$t = \frac{\text{minutes} \times 60 + \text{seconds}}{3600.0}$$

1. **Calculate Indices:**
   - Current record: $\text{rec}_{\text{curr}} = \text{hour}_{\text{curr}} + \text{group}_{\text{curr}} \times 24$
   - Next record: $\text{rec}_{\text{next}} = \text{hour}_{\text{next}} + \text{group}_{\text{next}} \times 24$
2. **Color Vectors Lerp:**
   $$\text{Color}_i = (1.0 - t) \times \text{Color}_{\text{curr}, i} + t \times \text{Color}_{\text{next}, i}$$
3. **Atmospheric Floats Lerp:**
   $$\text{Param} = (1.0 - t) \times \text{Param}_{\text{curr}} + t \times \text{Param}_{\text{next}}$$
4. **Push to GPU:** The resulting struct is copied into uniform memory at `0x009B1978` to drive the PICA200 vertex lighting and pixel pipeline.

---

## 6. Tortimer Island Weather Specification

On Tortimer Island (flag `0x20000`), the 52-week annual calendar is bypassed:
- The game queries the current Julian Day:
  $$J = \text{CalculateJulianDay}(\text{date})$$
  $$\text{if } (\text{date}\to\text{hour} \ge 6) \quad J = J + 1$$
- The week row index is selected from a static 3-day cyclic array:
  $$\text{WeekRow} = \text{Table}[J \bmod 3] \quad \text{where } \text{Table} = \{4, 21, 33\}$$
- These rows correspond to warm, tropical summer presets (dry breezy days, occasional tropical afternoon showers).

---

## 7. Precipitation Particle Systems & Crossfading

The engine dynamically adjusts particle emission rates based on weather intensity, performing smooth 30-second linear interpolation ($t \in [0, 1]$) across hour boundaries ($mm:00..mm:29$):

### Particle Count Constants
- **Light Rain (`BsWeatherRain` / `0x00229D10`):** 32 particles (`0x20`)
- **Heavy Rain / Storm (`BsWeatherRain` / `0x00229D10`):** 128 particles (`0x80`)
- **Light Snow (`BsWeatherSnow` / `0x0022AFA0`):** 100 particles
- **Blizzard (`BsWeatherSnow` / `0x0022AFA0`):** 400 particles

### Top-of-Hour Crossfade Formula
When entering a new hour, if the precipitation state changes, the particle count is interpolated over the first 30 seconds of the hour ($s \in [0, 29]$):
$$t = s \times 0.033333335 \quad (t = s / 30.0f)$$
$$\text{Particles}_{\text{active}} = (1.0f - t) \times \text{Count}_{\text{prev}} + t \times \text{Count}_{\text{curr}}$$

After second 29, the system returns the steady-state integer particle target.

---

## 8. Thunder & Lightning Engine (`BsThunderMgr` / `0x001FE740`, `0x001FE8DC`)

> **⚠️ Audit 2026-09-07:** the previous version of this section contained a fabricated LCG seed formula (`seed = ((year<<9)+(month<<5)+day)<<8 + ...`) and a 60-second loop generating 3–9 sec intervals, neither of which exists in the real code. Below is what's confirmed by the decompile. See [`functions/001FE740_Weather_BsThunderMgr_GenerateMinuteStrikes.md`](../functions/001FE740_Weather_BsThunderMgr_GenerateMinuteStrikes.md) for details and remaining open questions.

### 1. `Weather_BsThunderMgr_GenerateMinuteStrikes` (`0x001FE740`)
Clears a 60-byte buffer at `param_1+0x13` (`FUN_003014d4`, presumably memset — unconfirmed). If the current weather type (`Thunk_ResolveWeatherType() == 4`) is a thunderstorm, it shifts a copy of the current time by -6 months and calls `Save_GetSaveBufferPointer()` (the result's destination is untraced). Otherwise it simply caches the current time into fields `+0x50/+0x54/+0x58`.

**HYPOTHESIS (not confirmed by bytes):** the actual strike-pattern generation algorithm (which minutes get struck) is not visible in this function — either it's inside the unexamined `FUN_003014d4`, or it's generated elsewhere.

### 2. `Weather_BsThunderMgr_TickLightning` (`0x001FE8DC`) — confirmed
- Buffer `+0x13` is **60 bytes, indexed by minute-of-hour (0..59)**, not by second.
- If `strike_bitmap[current_minute] != 0`: clears the flag and:
  - **Screen flash:** `FUN_001E6304(1, 0, 10, 100)` — confirmed byte-for-byte.
  - **Sound:** outdoors `0x01000780`, indoors `0x0100077F` — confirmed byte-for-byte (addresses match the previous version of the text — this was the one part that wasn't fabricated).
- There's a field `+0x60` — looks like a manual/forced strike trigger (not examined in detail).

---

## 9. Meteor Showers & Shooting Star Wishing (`BsShootingStarMgr` / `0x002C9964`, `0x002C9AAC`)

> **⚠️ Not covered by the 2026-09-07 audit.** These addresses (`0x0062F4F4`, `0x002C9964`, `0x002C9AAC`, `0x002C965C`, `0x0067AF08`, `0x00680E00`, `0x002C9850`) have no dedicated articles in `functions/` and were not re-checked line-by-line against `decompile_function` in this session — the section is untouched, but also unconfirmed. Requires separate verification before treating it as fact.

On clear nights with meteor showers, shooting stars streak across the sky and can be wished upon by the player:

### 1. Active Window Condition (`0x0062F4F4`)
`Weather_CheckMeteorShowerActiveHours` enforces strict astronomical constraints:
- Base weather must be Clear Sky (`WeatherType == 0`).
- Time window: **19:00 (7:00 PM) to 04:00 (4:00 AM)**. Outside this window, shooting star spawning is inhibited.

### 2. Actor Slots & Particle Streaks
`BsShootingStarMgr` manages 4 concurrent shooting star slots (`0x002C9AAC`). Each star streak:
- Spawns at a procedurally offset 3D vector relative to the camera focal center (`0x0094DDD8 + 0x1A4`).
- Plays a high-pitched passing chime (`0x01000570`).
- Emits particle trails (`0x196`, `0x197`) oriented along its motion vector via directional matrix calculation (`0x002C965C`).

### 3. Player Wishing Mechanics (`0x0067AF08`, `0x00680E00`, `0x002C9850`)
When a shooting star is visible:
1. The player must be empty-handed and tilt the camera upward toward the sky.
2. Pressing **A** triggers action animation `0xC6` (closing eyes and pressing hands together in prayer).
3. `Player_PerformStarWish` queries `Weather_BsShootingStarMgr_TryWish` (`0x002C9850`):
   - Validates that an active shooting star is currently within its visible flight arc.
   - Triggers sparkle burst particle emitters `0x194` and `0x195`.
   - Plays chime sound effect `0x01000571`.
   - Increments the player's pending wish counter in the save data, prompting Wishy the Star to mail a gift letter the following morning.

---

## 10. C++ Reimplementation Blueprint

For our PC reimplementation, the entire weather pipeline can be cleanly decoupled:

```cpp
namespace acnl::weather {

class WeatherSystem {
public:
    void Init(const std::filesystem::path& romfs_root);
    void Update(const DateObject& current_time, uint32_t state_flags);
    
    [[nodiscard]] uint8_t GetCurrentWeatherType() const { return m_current_type; }
    [[nodiscard]] const EnvironmentRecord& GetActiveEnvironment() const { return m_blended_env; }
    
private:
    std::vector<EnvironmentRecord> m_env_records; // 216 records loaded from EnvironmentParameter.bin
    uint8_t m_weather_table[52][24];              // Extracted from exefs.elf
    
    uint8_t m_current_type{0};
    uint8_t m_next_type{0};
    EnvironmentRecord m_blended_env{};
};

} // namespace acnl::weather
```

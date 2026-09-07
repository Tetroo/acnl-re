---
tags: [binary, strings, rodata]
aliases: [strings, extracted strings]
---

# Strings

Extracted strings from `.rodata` and `.text`, grouped by category.

---

## Weather

| Address | String | Notes |
|---------|--------|-------|
| `0x001003F0` | `WeatherHeap` | Heap name, used in init |
| `0x00229700` | `BsWeatherRain` | BehaviourScript component name |
| `0x0022AA44` | `BsWeatherSnow` | BehaviourScript component name |
| `0x0026646C` | `BsWeatherPaper` | BehaviourScript component name |
| `0x00296188` | `BsWeatherSakura` | BehaviourScript component name |
| `0x008D35A9` | `13BsWeatherRain` | RTTI / mangled class name |
| `0x008D35B9` | `13BsWeatherSnow` | RTTI / mangled class name |
| `0x008D38C7` | `14BsWeatherPaper` | RTTI / mangled class name |
| `0x008D3CD9` | `15BsWeatherSakura` | RTTI / mangled class name |
| `0x008D555B` | `20BsWeatherRainHioNode` | HioNode class name |
| `0x008D5572` | `20BsWeatherSnowHioNode` | HioNode class name |
| `0x008D57BB` | `21BsWeatherPaperHioNode` | HioNode class name |
| `0x008D5972` | `22BsWeatherSakuraHioNode` | HioNode class name |
| `0x009121D6` | `Weather/paper_model.bcres` | Model asset path |
| `0x009121F0` | `Weather/ame_model.bcres` | Model asset path (rain; "ame" = rain in JP) |
| `0x00912208` | `Weather/cherryblossoms_model.bcres` | Model asset path (sakura) |
| `0x0091222B` | `Weather/snow_model.bcres` | Model asset path |

### Shader paths
| Address | String |
|---------|--------|
| `0x00228FB8` | `Shader/RainShader.shbin` |
| TBD | `Shader/SnowShader.shbin` |
| TBD | `Shader/PaperShader.shbin` |
| TBD | `Shader/CherryShader.shbin` |

> [!TODO] Verify shader string addresses
> Shader string addresses for Snow/Paper/Cherry not yet verified after segment correction.

---

## Date / Time

Strings found during date system investigation.

| Address | String | Notes |
|---------|--------|-------|
| `0x00852408` | `SetDateTimeBg` | UI screen class name — date/time settings |
| `0x008D36B9` | `13SetDateTimeBg` | RTTI mangled name for SetDateTimeBg |
| `0x008E61F1` | `N5state4BaseI13SetDateTimeBgEE` | RTTI — `state::Base<SetDateTimeBg>` |
| `0x008E6FB9` | `N5state4ModeI13SetDateTimeBgEE` | RTTI — `state::Mode<SetDateTimeBg>` |
| `0x008DDB1D` | `N2nn3pia6common8DateTimeE` | RTTI — `nn::pia::common::DateTime` (Nintendo network lib) |
| `0x008E54B4` | `N5event9EventDateE` | RTTI — `event::EventDate` class |
| `0x008E5416` | `N5event14EventEveryDateE` | RTTI — `event::EventEveryDate` class |
| `0x008D33C1` | `12RollTextDate` | UI element — date display (rolling/scrolling text) |
| `0x0090D9DB` | `T_date_00` | UI texture/layout name |
| `0x0090E1D6` | `T_date_01` | UI texture/layout name |
| `0x0090FFF1` | `DateIn` | Animation/transition name |
| `0x009100F7` | `DateOut` | Animation/transition name |
| `0x0090FFD4` | `DateNeutral` | Animation/transition name |

### Notes on class names

- `nn::pia::common::DateTime` — Nintendo's network library DateTime. Used for online timestamps
  but likely also for local RTC time. Key class for understanding how date is stored/accessed.
- `event::EventDate` — fires on a specific date (e.g. holiday events)
- `event::EventEveryDate` — fires every day (daily reset logic?)
- `state::Mode<SetDateTimeBg>` — confirms SetDateTimeBg uses the game's state machine framework

---

## System Calls

**Imports table is empty.** The game makes 3DS system calls via ARM SVC instructions
directly, not through named imports.

Confirmed syscall (from decompilation of [[FUN_0012994C]]):

| SVC    | Name            | Description                                          |
|--------|-----------------|------------------------------------------------------|
| `0x28` | `GetSystemTick` | Returns total CPU ticks since power-on (monotonic). Used to derive current time. |

The game does NOT use a direct RTC syscall on each time query — it derives wall-clock
time from ticks + a base timestamp set at startup. The initial RTC read (startup only)
has not yet been located; likely goes through IPC (`ptm:u` service).

> [!WARNING] Discarded
> Previous note about `SVC 0x1A = GetSystemTime` was unverified. Per 3dbrew,
> `0x1A` = `CreateTimer`. Discarded.

---

## RTTI note

Strings like `13BsWeatherRain` are Itanium ABI mangled type names. The number is the length of the class name. Confirms C++ with RTTI enabled.

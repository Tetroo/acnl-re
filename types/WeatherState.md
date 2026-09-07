---
tags: [type, weather, state-machine, engine]
aliases: [WeatherState, weather tables]
---

# WeatherState & Tables — Weather System Structures

Runtime structures and tables driving the dynamic weather simulation in ACNL (Welcome amiibo EUR).

## Architecture Overview

The weather simulation is controlled by a 2D matrix (period index vs. hour-of-day), combined with shader interpolation and environmental parameter blending.

### 1. Weather Type Resolver Table (52 × 24)
- **Row pointer array:** `DAT_0062E1AC` (52 pointers to row arrays)
- **Axis 0 (Rows, 0..51):** `period_index` (yearly period / week index, derived via [[FUN_0062EF1C]] / [[FUN_0062EEE8]]).
- **Axis 1 (Columns, 0..23):** `(hour - 6) % 24` (shifted relative to 06:00 AM).
- **Lookup function:** [[FUN_0062E094]] (`WeatherTypeResolver_52x24`).

### 2. EnvironmentParameter.bin
- Loaded during weather initialization in [[FUN_001E5BC8]].
- Records have a fixed size: `0x8C` (140) bytes.
- Parameter groups: `group_index * 0x18 + hour_of_day = record_index` (used in [[FUN_001E63DC]]).
- Mapping: [[FUN_001E82E0]] maps `(weather_type, period_index) -> group_index`.

### 3. Weather Precipitation Classes (C++ Vtables)
Precipitation effects are modeled as polymorphic C++ objects with dedicated vtables:
- **Rain (`BsWeatherRain`):** `0x008ECFE4` (Model: `Weather/ame_model.bcres`, Shader: `Shader/RainShader.shbin`)
- **Snow (`BsWeatherSnow`):** `0x008ED044` (Model: `Weather/snow_model.bcres`, Shader: `Shader/SnowShader.shbin`)
- **Paper / Confetti (`BsWeatherPaper`):** `0x008EE040` (Model: `Weather/paper_model.bcres`, Shader: `Shader/PaperShader.shbin`)
- **Sakura / Cherry Blossom (`BsWeatherSakura`):** `0x008EF688` (Model: `Weather/cherryblossoms_model.bcres`, Shader: `Shader/CherryShader.shbin`)

## Related Documentation

- [[systems/weather]] — Comprehensive weather lifecycle and state machine description.
- [[systems/sky-rendering]] — Sky dome / color grading compositor pipeline.

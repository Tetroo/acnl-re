---
address: 0x001E5BC8
name: InitializeWeatherSystem
tags: [function, weather, init, critical]
status: fully understood — weather system full initializer
---

# [[FUN_001E5BC8|InitializeWeatherSystem]] — Weather System Full Init

## Signature
```c
void FUN_001E5BC8(void);
```

## Purpose

Initializes the entire weather system: loads all four shaders, then populates
a set of weather data tables from an external data source (`FUN_0061494C`).
This is the top-level weather init, called once at startup.

## Decompiled (annotated)
```c
void FUN_001E5BC8(void) {
    // Load all four weather shaders
    FUN_00228F9C();  // RainShader.shbin
    FUN_0022A278();  // SnowShader.shbin
    FUN_00265D58();  // PaperShader.shbin
    FUN_00295990();  // CherryShader.shbin

    FUN_001BA7E8();  // unknown — possibly audio/sound init for weather

    // Populate weather data tables
    int source = FUN_0061494C();  // get data source object
    if (source != 0) {
        for (int month = 0; month < 0x18; month++) {      // 0x18 = 24
            for (int slot = 0; slot < 9; slot++) {         // 9 slots
                int record = month + slot * 0x18;          // record index
                for (int param = 0; param < 0x16; param++) { // 0x16 = 22 params
                    // Copy 22 bytes per record into table DAT_001E5DBC
                    table[slot][month][param] = source[record * 0x8C + param + 0x10];
                }
                // Copy additional float params at fixed offsets
                DAT_001E5DC0[slot][month] = source[record * 0x8C + 0x68];
                DAT_001E5DC4[slot][month] = source[record * 0x8C + 0x8C];
                // ... (offsets 0x90, 0x94, 0x98, 0x6C, 0x70, 0x78, 0x7C, 0x80, 0x84, 0x88)
            }
        }
    }
}
```

## Loop Structure — Confirmed

The double loop `0x18 × 9` copies data from [[binary/EnvironmentParameter]] into weather tables.

| Value | Meaning |
|-------|---------|
| `0x18` = 24 | Hours of day |
| `9` | Weather groups (Clear, Rain variants, Sakura, Fallback) |
| `0x8C` = 140 | Size of one source record in bytes |
| `0x16` = 22 | Bytes per param block within a record |

> [!IMPORTANT] 2050 limit is NOT here
> The outer dimension is **hours (24)**, not years or months.
> EnvironmentParameter.bin is a pure `group × hour` table with no year encoding.
> See [[FUN_0056BBD4]] for the actual 2050 limit source.

## Key Function

[[FUN_0061494C]] — returns pointer to EnvironmentParameter.bin data loaded into memory.

## Data Tables Written

| Address | Contents |
|---------|----------|
| `DAT_001E5DBC` | Main weather param table (22 bytes × 9 slots × 24 periods) |
| `DAT_001E5DC0` | Float param at source+0x68 |
| `DAT_001E5DC4` | Float param at source+0x8C |
| `DAT_001E5DC8` | Float param at source+0x90 |
| `DAT_001E5DCC` | Float param at source+0x94 |
| `DAT_001E5DD0` | Float param at source+0x98 |
| `DAT_001E5DD4` | Float param at source+0x6C |
| `DAT_001E5DD8` | Float param at source+0x70 |
| `DAT_001E5DDC` | Float param at source+0x78 |
| `DAT_001E5DE0` | Float param at source+0x7C |
| `DAT_001E5DE4` | Float param at source+0x80 |
| `DAT_001E5DE8` | Float param at source+0x84 |
| `DAT_001E5DEC` | Float param at source+0x88 |

## See Also

[[systems/weather]], [[binary/EnvironmentParameter]], [[FUN_0061494C]], [[FUN_001E63DC]]


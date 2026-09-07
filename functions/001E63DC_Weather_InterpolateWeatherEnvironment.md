---
address: 0x001E63DC
name: InterpolateWeatherEnvironment
tags: [function, weather, update, critical]
status: understood — this is the core weather interpolation function
---

# [[FUN_001E63DC|InterpolateWeatherEnvironment]] — Weather Data Interpolation Update

## Signature
```c
void FUN_001E63DC(void);
```

## Purpose

Per-frame weather update. Reads current and next weather state indices from a
time/date object, interpolates all weather parameters between two source records,
and writes the result to the active weather data object `DAT_001E67B4`.

**This is the core of the weather rendering pipeline.**

## Key Logic (annotated)

```c
void FUN_001E63DC(void) {
    int source = FUN_0061494C();    // weather data source
    int out    = DAT_001E67B4;      // output weather state object
    if (source == 0) return;
    if (*(out + 7) != 0) return;    // skip if disabled

    // Get current time object
    void* time_obj = FUN_002FB39C();  // ← TIME/DATE GETTER — critical

    // Decode two time values from time_obj
    // local_7C = hours+minutes composite from time_obj
    // uStack_74, uStack_80 = fields used as record indices

    // Compute interpolation factor (0.0 → 1.0)
    // fVar15 = (hours + minutes/60) * DAT_001E67B8
    fVar15 = (float)(time_obj.minutes + time_obj.hours * 60) * DAT_001E67B8;

    // Get current weather type index via state system
    FUN_002F755C();
    int weather_type = FUN_005B2FE0();  // converts state → weather type index

    // Special cases: states 3, 0x18(=24), 0xB(=11) → force index 0x16(=22)
    if (weather_type == 3 || weather_type == 0x18 || weather_type == 0xB) {
        iVar7 = 0x16;
        iVar2 = 0x16;
    } else {
        // Compute two record indices: current period and next period
        iVar7 = time_field_hi + iVar7 * 0x18;  // current record
        iVar2 = time_field_lo + iVar2 * 0x18;  // next record (for interpolation)
    }

    // Interpolate 22 color/param bytes between record iVar2 and record iVar7
    for (int i = 0; i < 0x16; i++) {
        out_table[i] = lerp(source[iVar2*0x8C + i + 0x10],
                            source[iVar7*0x8C + i + 0x10],
                            fVar15) * scale;
    }

    // Interpolate fog, sky color, sun params... (many more fields)
    // All follow: lerp(source[iVar2*0x8C + offset], source[iVar7*0x8C + offset], fVar15)

    // Additional flag checks via state system:
    FUN_002F755C(); FUN_002F7574(0x400, state);  // bit 10 — affects fog params
    FUN_002F755C(); FUN_002F7574(4, state);       // bit 2 — affects cloud params
}
```

## Critical Findings

### [[FUN_002FB39C]] — Time/Date Getter
Called to get the time object. Fields extracted:
- `local_7C._0_1_` — hours (used in `hours * 60 + minutes`)
- `local_7C._1_1_` — minutes
- `uStack_74._3_1_` — upper byte, used as record index base
- `uStack_80._3_1_` — upper byte, used as second record index base

**This function returns the current date/time. Must be reversed — it's the link to RTC.**

### [[FUN_0062E094]] / [[FUN_0062EEE8]] — Period Index Extractors

Both called on date objects to produce the two record indices for interpolation.
- [[FUN_0062E094]] — weather type resolver (52×24 table)
- [[FUN_0062EEE8]] — period index resolver (3 paths: flag/state/normal)

Both are fully understood. See their individual docs.

### [[FUN_005B2FE0]] — State→Type Converter

Called after `FUN_002F755C()`. Returns an integer used as weather type index.
Not yet decompiled.

| Return | Meaning |
|--------|---------|
| `3` | Special case → use record `0x16` (22) |
| `0xB` (11) | Special case → use record `0x16` |
| `0x18` (24) | Special case → use record `0x16` |
| other | Normal → index = `uStack_74._3_1_ + value * 0x18` |

### Record Indexing Formula
```
record_index = time_field + weather_type * 0x18
```
Where `0x18 = 24`. This means: **24 weather entries per type**.
If there are multiple types and 24 sub-entries, the total table size =
`N_types × 24 × 140 bytes`.

### Interpolation Factor
```
fVar15 = (minutes + hours * 60) * DAT_001E67B8
```
`DAT_001E67B8` is a scale factor. `fVar15` ∈ [0.0, 1.0] over the course of a day
(or some sub-period). This interpolates smoothly between two time-of-day entries.

### New Bit Flags Found
| Mask | Meaning |
|------|---------|
| `0x400` (bit 10) | Affects fog/atmosphere params |
| `4` (bit 2) | Affects cloud params |

(Previously known: `0x40` bit 6, `0x400000` bit 22, `0x10000000` bit 28)

## Open Questions

- [[FUN_005B2FE0]] — state→weather type converter, not yet decompiled
- Accessors on period-index value (`FUN_0062e7f8`, `FUN_0062e810`, `FUN_0062e828`, etc.) — not yet reversed
- `DAT_001E67B8` — interpolation scale factor, exact value unknown

## See Also

[[FUN_0061494C]], [[FUN_002F755C]], [[FUN_0062E094]], [[FUN_0062EEE8]], [[FUN_001E82E0]],
[[binary/EnvironmentParameter]], [[systems/weather]]


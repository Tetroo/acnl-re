---
tags: [binary, weather, romfs, data]
aliases: [EnvironmentParameter, EnvironmentParameter.bin, weather parameters]
---

# EnvironmentParameter.bin

**Path:** romfs_out/Environment/EnvironmentParameter.bin
**Size:** 30,256 bytes (16 header + 216 x 0x8C)
**Loaded by:** [[FUN_001E5BC8]] — copies via loop 0x18 x 9
**Used by:** [[FUN_001E63DC]] — indexes records at stride 0x8C

## Header
- 0x00: 0x00000BB8 (3000 dec)
- 0x04: 0x000003E8 (1000 dec)
- 0x08..0x0F: zeros

## Record Structure

Count: 216 = 0x18 x 9
Stride: 0x8C bytes
First 16 bytes: 4x RGBA (alpha always 0xFF)
  Color 0: sky/fog primary
  Color 1: sun/light
  Color 2: ambient
  Color 3: always 00 00 00 FF
Float params at +0x68


## Grid: 9 Groups x 24 Hours

Axis 0 (9 groups): weather type / sky condition
Axis 1 (24 records): hour of day

| Group | Recs    | Start Color (first 4 bytes) | Notes |
|-------|---------|-----------------------------|-------|
| 0     | 0-23    | 67 80 53 FF                 | Clear - full day/night cycle |
| 1     | 24-47   | 5F 74 53 FF                 | Overcast/rain, dark blue-grey |
| 2     | 48-71   | 5F 74 53 FF                 | Near-identical to G1 (heavy rain?) |
| 3     | 72-95   | 67 80 53 FF                 | Clear variant |
| 4     | 96-119  | 5F 74 53 FF                 | Overcast |
| 5     | 120-143 | 67 80 53 FF                 | Clear, purple dusk (rec126: 8D 6B 94 FF) |
| 6     | 144-167 | 5F 74 53 FF                 | Overcast/clear mix |
| 7     | 168-191 | 80 80 67 FF                 | ALL 24 IDENTICAL - FF FF CC sun - Sakura? |
| 8     | 192-215 | 67 80 53 FF                 | ALL 24 IDENTICAL - same value repeated - fallback |

## Day-Night Cycle (Group 0 example)

R0-4:   pre-dawn, dark blue (67 80 53 FF)
R5-7:   sunrise, warm orange (CC CC 80 FF)
R8-16:  midday, bright (FF FF CC FF sun)
R17-18: sunset (FF E6 B3 FF, FF CC 99 FF)
R19-20: dusk transition
R21-23: evening, returning to blue

## Key Observations

> [!IMPORTANT] No year encoding
> No year encoding in this file. Pure weather_type × hour grid.
> The 2050 limit is NOT here — see [[research/2050-limit]].

1. Group 7 all-identical: Sakura/Paper likely doesn't interpolate by hour.
2. Group 8 all-identical: default/fallback state.
3. Groups 1+2 nearly identical: Rain vs Heavy Rain (types 3 and 5, [[FUN_0062E094]] promotes under heavy rain flag).
4. [[FUN_001E63DC]] lerps between two records (current/tomorrow) using fVar15 (time fraction).

## Code Connection

[[FUN_001E63DC]]:

```c
local_68 = FUN_0062e094(current_date)    // weather type index now
uVar4    = FUN_0062e094(tomorrow)        // weather type index tomorrow
iVar2    = FUN_001e82e0(local_68, uVar5) // flat record index
iVar7    = FUN_001e82e0(uVar4, uVar5)    // flat record index next period
// lerp all fields between record[iVar2] and record[iVar7] by fVar15
```
[[FUN_001E82E0]]: (weather_type, hour) → flat record index into this table.

## Open Questions

- Confirm group -> weather type mapping (Rain=1/2? Snow=? Clear=0/3?)
- Fields +0x10..+0x67: between RGBA block and floats - unknown
- Float fields +0x68, +0x6C, +0x70.. +0x8B - fog, wind, etc.
- Header 3000 / 1000 - version? scale?

---
tags: [binary, state, weather, mapping, data]
aliases: [state table, state index, weather scenario]
---

# State Index → Weather Scenario Table (DAT_005B3068)

**Source:** `DAT_005B3068` = `0x008884CD` (rodata pointer)  
**Size:** 165 bytes (sentinel at index 0xA5)  
**Used by:** [[FUN_005B2FE0]] — maps `state_index → scenario_index`  
**Then used by:** [[FUN_001E63DC]] — `record = time_field + scenario * 0x18`

> [!NOTE] Not simple weather types
> Values are **environment scenario indices** (0–24), not the 0-4 weather types
> from the 52×24 table. Higher values correspond to indoor/special environments
> with their own EnvironmentParameter.bin records.

---

## Scenario Groups (inferred)

| Value | Count | Inferred meaning | Example states |
|-------|-------|-----------------|----------------|
| `0` | 0 | (none in table) | — |
| `1` | 35 | Outdoor village — normal | 0x00–0x01, 0x5E–0x80 |
| `2` | 20 | Outdoor — rain possible | 0x04, 0x06–0x08, 0x0A |
| `3` | 5 | Outdoor — heavy weather | 0x05, 0x0B, 0x11, 0x17 |
| `4` | 20 | Special outdoor / cliff area | 0x1B–0x24, 0x81–0x8A |
| `5` | 1 | Museum entrance? | 0x49 |
| `6–8` | 3 each | Museum wings (Fish/Ins/Fossil) | 0x4A–0x52 |
| `9–10` | 2–1 | Museum sub-areas | 0x53–0x55 |
| `11` | 4 | Museum special | 0x56–0x59 |
| `12` | 16 | **Indoor / no weather** | 0x25, 0x2B, 0x5C–0x5D, 0x98–0x9E |
| `13` | 2 | Specific interior | 0x3C–0x3D |
| `14` | 7 | Shop interiors | 0x60–0x62, 0x64, 0x8B–0x8D |
| `15` | 17 | Building interiors | 0x26–0x2A, 0x30–0x31, 0x39–0x3B |
| `16` | 4 | Station / tour | 0x32–0x33, 0x38, 0x67 |
| `17` | 4 | Downtown | 0x2C–0x2F |
| `18` | 3 | Club 444 | 0x45–0x47 |
| `19` | 5 | Dream / special | 0x34, 0x36–0x37, 0x5B, 0xA4 |
| `21` | 5 | **Transition states** (FUN_005B2FE0 special case) | 0x03, 0x09, 0x0F, 0x15, 0x92 |
| `22` | 2 | AutoCamp | 0x9F–0xA0 |
| `23` | 2 | AutoCamp variant | 0xA1–0xA2 |
| `24` | 1 | (unknown) | 0xA3 |

> [!IMPORTANT] Type 12 = indoor/no weather
> States 0x5D, 0x99, 0x9B have type 12. These are the same states that
> cause [[FUN_0062E094]] to return 0 immediately — confirming type 12 = no visible weather.

> [!IMPORTANT] Type 21 = FUN_005B2FE0 special case
> States 0x03, 0x09, 0x0F, 0x15, 0x92 return 21.
> In [[FUN_005B2FE0]], type 21 triggers additional checks via `FUN_0071f3ec(obj, 3/4/5)`:
> - all > 1 → promote to type 3 (heavy)
> - otherwise → type 2 (normal)
> Likely: outdoor transition zones or event states.

---

## Full Table (hex index : scenario value)

```
[00]  1  1 15 21  2  3  2  2  2 21  2  3  2  2  2 21
[10]  2  3  2  2  2 21  2  3  2  2  2  4  4  4  4  4
[20]  4  4  4  4  4 12 15 15 15 15 15 12 17 17 17 17
[30] 15 15 16 16 19 12 19 19 16 15 15 15 13 13 12 15
[40] 12 15 12 15 15 18 18 18 15  5  6  6  6  7  7  7
[50]  8  8  8  9  9 10 11 11 11 11 15 19 12 12  1  1
[60] 14 14 14 12 14  1  1 16  1  1  1  1  1  1  1  1
[70]  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1
[80]  1  4  4  4  4  4  4  4  4  4  4 14 14 14  1  1
[90]  1  1 21  2  3  2  2  2 12 12 12 12 12 12 12 22
[A0] 22 23 23 24 19
```

---

## Periodic Pattern (states 0x00–0x1A)

States 0x00–0x1A show a repeating pattern every 6 states:
```
+0: 1 or 2   (outdoor)
+1: 1 or 2
+2: 15 or 2  
+3: 21 or 2  (transition)
+4: 2
+5: 3        (heavy)
```
Suggests these are related groups of game states (e.g. different town areas or time phases).

---

## See Also

[[FUN_005B2FE0]], [[FUN_001E63DC]], [[FUN_0062E094]], [[binary/weather-table]]

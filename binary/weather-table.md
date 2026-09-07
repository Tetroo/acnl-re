---
tags: [binary, weather, table, data]
aliases: [weather table, 52x24 table, weekly weather]
---

# Weather Type Table (52×24)

**Source:** `DAT_0062E1AC` → `0x0095CF9C` (pointer in .bss) → 52 row pointers → 24 bytes each  
**Used by:** [[FUN_0062E094]] — indexes `table[week][hour]` to get weather type byte  
**Hours:** 0–23, where hour 0 = midnight, hour 6 = start of game day  

> [!IMPORTANT] Global table — same for all players
> This table is identical for every town. There is no per-town weather seed in ACNL.
> Weather is fully deterministic by week-of-year and hour-of-day.

---

## Weather Type Meanings

| Type | Name | FUN_001E82E0 group | Notes |
|------|------|--------------------|-------|
| `0` | Clear | 0, 3, 5, or 6 (by period) | Sunny |
| `1` | Cloudy | 0, 3, 5, or 6 (by period) | Overcast but dry |
| `2` | Rain | 1 or 4 (by period) | Standard rain |
| `3` | Heavy Rain | 1 | Promoted to type 5 by heavy rain flag |
| `4` | Snow/Heavy | 2 | Promoted to type 6 by heavy flag |
| `5` | Heavy Rain+ | 1 | Dynamic — type 3 promoted by [[FUN_005B2FE0]] |
| `6` | Snow/Blizzard | 2 | Dynamic — type 4 promoted by [[FUN_005B2FE0]] |

Types 5 and 6 do not appear in the table — they are applied dynamically at runtime.

---

## Full 52×24 Table

Hours 0–23 (H06 = start of game day, H05 = end of previous day)

```
Wk |  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
Wk00|  0  0  0  0  1  1  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
Wk01|  0  0  0  0  0  0  0  0  0  0  1  1  1  1  1  1  1  0  0  0  0  0  0  0
Wk02|  0  0  0  0  0  0  0  0  0  1  1  1  1  1  1  0  0  0  0  0  1  1  1  1
Wk03|  1  0  0  0  0  1  1  1  1  1  1  1  0  0  0  0  0  0  0  0  0  0  0  1
Wk04|  1  1  1  1  1  1  1  1  0  0  1  1  1  0  0  0  0  0  0  0  0  1  1  1
Wk05|  1  1  1  1  1  1  1  2  2  2  2  1  1  1  1  1  1  1  1  1  1  1  1  1
Wk06|  2  2  2  1  1  1  1  1  1  1  1  1  2  2  1  1  1  1  1  1  1  1  1  1
Wk07|  2  2  2  2  1  1  2  2  2  1  1  2  2  2  2  2  2  1  1  1  1  2  2  2
Wk08|  2  2  2  2  2  2  2  1  2  2  2  2  2  2  2  1  1  1  1  2  2  2  2  2
Wk09|  2  2  3  3  3  2  2  2  3  3  2  2  2  2  2  2  2  2  2  2  2  2  2  2
Wk10|  3  2  2  1  2  3  3  3  3  3  3  2  2  2  3  3  3  2  2  2  2  3  3  3
Wk11|  3  3  3  2  2  3  3  2  2  2  3  3  3  3  3  2  2  2  2  3  3  3  3  3
Wk12|  3  3  3  3  3  2  3  3  3  2  3  3  3  3  3  3  3  3  2  2  3  3  3  3
Wk13|  3  3  3  4  4  3  3  4  4  4  4  3  3  3  3  3  2  2  3  3  3  3  3  3
Wk14|  3  4  4  4  4  4  4  4  4  3  3  4  4  4  4  4  4  3  3  3  4  4  4  3
Wk15|  4  4  4  4  4  4  4  4  4  4  4  4  4  4  3  3  3  2  2  3  3  3  4  4
Wk16|  2  2  2  2  2  1  1  1  1  1  1  1  1  1  2  2  2  2  2  2  2  2  2  3
Wk17|  1  1  1  1  1  1  1  1  1  2  2  3  3  2  2  2  2  2  2  2  2  2  2  2
Wk18|  0  0  1  1  1  2  2  2  2  2  2  2  2  2  2  2  2  3  3  3  2  2  2  2
Wk19|  3  3  3  2  2  2  2  2  2  1  1  1  1  1  1  1  0  0  0  1  1  1  1  2
Wk20|  3  2  2  2  2  2  3  2  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1
Wk21|  2  2  3  2  2  2  1  1  1  1  0  0  0  0  0  0  0  0  1  1  1  1  1  0
Wk22|  1  0  0  0  0  0  1  3  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1
Wk23|  0  0  0  0  1  1  3  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1  1
Wk24|  0  0  0  0  0  0  0  1  1  4  2  1  0  0  0  0  0  0  0  0  0  1  1  0
Wk25|  1  1  0  0  0  1  1  1  4  2  1  1  1  1  1  1  1  1  1  1  1  1  1  1
Wk26|  2  2  1  2  2  2  3  3  3  4  4  3  3  3  3  3  3  3  3  3  3  3  3  3
Wk27|  2  2  2  2  3  3  3  2  2  2  3  3  3  3  3  2  3  3  3  3  3  3  2  2
Wk28|  2  2  2  2  2  2  2  2  3  3  3  3  3  3  3  3  3  3  3  3  3  3  4  3
Wk29|  3  3  2  2  2  3  3  2  2  2  1  1  2  2  2  2  2  2  2  2  2  2  2  2
Wk30|  3  3  3  3  3  3  3  3  3  2  2  2  2  2  2  2  1  1  2  2  2  2  2  2
Wk31|  4  4  3  3  3  2  2  3  2  2  2  2  2  2  2  2  2  2  1  1  2  2  2  2
Wk32|  0  0  0  0  0  0  0  0  0  0  1  1  1  1  1  1  1  0  0  0  0  0  0  0
Wk33|  0  0  0  0  0  0  0  0  0  1  1  1  1  1  1  0  0  0  0  0  1  1  1  1
Wk34|  1  0  0  0  0  1  1  1  1  1  1  1  1  1  0  0  0  0  0  0  0  0  0  1
Wk35|  2  2  2  2  1  1  2  2  2  1  1  2  2  2  2  2  2  1  1  1  1  2  2  2
Wk36|  2  2  2  2  2  2  2  1  2  2  2  2  2  2  2  1  1  1  1  2  2  2  2  2
Wk37|  2  2  3  3  3  2  2  2  3  3  2  2  2  2  2  2  2  2  2  2  2  2  2  2
Wk38|  1  1  1  1  1  1  1  1  0  0  1  1  1  0  0  0  0  0  0  0  0  1  1  1
Wk39|  1  1  1  1  1  1  1  2  2  2  2  1  1  1  1  1  1  1  1  1  1  1  1  1
Wk40|  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
Wk41|  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
Wk42|  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
Wk43|  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
Wk44|  1  1  1  1  1  1  1  1  0  0  1  1  1  0  0  0  0  0  0  0  0  1  1  1
Wk45|  1  1  1  1  1  1  1  2  2  2  2  1  1  1  1  1  1  1  1  1  1  1  1  1
Wk46|  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2
Wk47|  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2
Wk48|  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2
Wk49|  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2  2
Wk50|  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
Wk51|  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
```

---

## Season Patterns

| Weeks | Approx dates | Pattern |
|-------|-------------|---------|
| 00–04 | Jan 1 – Feb 1 | Clear with occasional cloud |
| 05–15 | Feb – Apr | Gradual shift to rain, peak type 4 at wk13-15 |
| 16–23 | Apr – Jun | Clearing, mixed rain |
| 24–31 | Jun – Aug | Summer rain pattern, heavy wk26-31 |
| 32–39 | Aug – Oct | Mirrors early year pattern |
| 40–43 | Oct | All clear — unusual dead zone |
| 44–49 | Nov – Dec | Light cloud → sustained rain (wk46-49 all type 2) |
| 50–51 | Dec | All clear — year-end |

> [!NOTE] Week numbering vs calendar
> Week 0 = first week of January (game year starts Jan 1).
> Exact date mapping depends on `FUN_0056AC80` period thresholds.

---

## Statistical Summary

| Type | Count | % of year |
|------|-------|-----------|
| 0 Clear | 329 | 26.4% |
| 1 Cloudy | 329 | 26.4% |
| 2 Rain | 381 | 30.6% |
| 3 Heavy Rain | 163 | 13.1% |
| 4 Snow/Heaviest | 46 | 3.7% |

Rainy conditions (type 2+) = **47.4%** of the year.

---

## Godot Port Notes

Direct implementation:
```gdscript
const WEATHER_TABLE: Array = [
    [0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],  # week 0
    # ... 51 more rows
]

func get_weather_type(week: int, hour: int) -> int:
    var raw = WEATHER_TABLE[week][hour]
    # Apply heavy rain promotion if needed (separate flag system)
    return raw
```

No RNG needed — pure table lookup.

---

## See Also

[[FUN_0062E094]], [[FUN_001E82E0]], [[FUN_005B2FE0]], [[systems/weather]]

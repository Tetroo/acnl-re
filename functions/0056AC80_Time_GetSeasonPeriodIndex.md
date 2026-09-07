---
address: 0x0056AC80
name: GetSeasonPeriodIndex
tags: [function, date, season, period, weather]
status: fully understood — year period index getter (0-22)
---

# FUN_0056AC80 — Season Period Index Getter

## Signature
```c
uint FUN_0056ac80(date_obj* param_1);
```

## Purpose

Returns a period index (0–22) representing where the current date falls
within the annual season calendar. Used by [[FUN_006a3578]] to select
the Summer or Winter .cro module.

## Logic

```c
uint FUN_0056ac80(date_obj* param_1) {
    int state = FUN_002f755c();
    if (FUN_002f7574(0x20000, state) != 0)
        return 12;  // forced mid-year (special flag)

    // Walk threshold table until date <= threshold[i]
    uint i = 0;
    while (table[i] < current_date_as_mmdd(param_1)) {
        i++;
        if (i > 22) return 0;
    }
    return i;
}
```

## Threshold Table (DAT_0056ace8 — 23 uint16 values, format 0xMMDD)

| Index | Date | Notes |
|-------|------|-------|
| 0 | Jan 15 | |
| 1 | Feb 18 | |
| 2 | Feb 24 | |
| 3 | Mar 31 | |
| 4 | Apr 5 | |
| 5 | Apr 10 | |
| 6 | Apr 14 | |
| 7 | Jun 7 | |
| 8 | Jun 15 | |
| 9 | Jun 27 | |
| 10 | Jul 5 | |
| 11 | Jul 23 | |
| 12 | Sep 7 | |
| 13 | Sep 15 | |
| 14 | Oct 16 | |
| 15 | Oct 24 | |
| 16 | Nov 1 | |
| 17 | Nov 9 | |
| 18 | Nov 17 | |
| 19 | Nov 25 | |
| 20 | Nov 30 | |
| 21 | Dec 10 | |
| 22 | Dec 31 | |

## Usage in Module Dispatch

In [[FUN_006a3578]], result used as:
```c
int period = FUN_0056ac80(date);
if ((period - 3 < 0xC) && hemisphere != 0x02)
    load_module("Summer");
else
    load_module("Winter");
```
Periods 3–14 (Apr–Sep) = Summer in northern hemisphere.
Hemisphere byte `0x02` = southern hemisphere → flips seasons.

## See Also

[[FUN_006a3578]], [[FUN_002F755C]], [[FUN_002F7574]], [[research/town-generation]]

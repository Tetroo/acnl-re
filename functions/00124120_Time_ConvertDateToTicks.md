---
address: 0x00124120
name: ConvertDateToTicks
tags: [function, time, date, ticks]
status: fully understood — pure date→ticks converter, no clamp
---

# [[FUN_00124120|ConvertDateToTicks]] — Date → 64-bit Ticks Converter

## Signature
```c
void FUN_00124120(uint64_t* out, date_obj* param_1);
```

## Purpose

Converts a date object to a 64-bit tick representation.
Called as Phase 1 of [[FUN_00308F5C]] before tick-space arithmetic.

## Findings (Session 10)

> [!IMPORTANT] No year clamp here
> Pure arithmetic conversion — no clamping, no bounds check on year.
> The 2050 limit is NOT in this function. See [[FUN_0056BBD4]].

## Call Context

```c
// Inside FUN_00308F5C:
FUN_00124120(&local, param_1);   // date → 64-bit ticks (no clamp)
local += phase1_offset;          // add DAT_003090f0-derived offset
FUN_00126514(&local, &result);   // ticks → date
```

## See Also

[[FUN_00308F5C]], [[FUN_00126514]], [[FUN_0056BBD4]], [[research/2050-limit]]

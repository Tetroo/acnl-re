---
address: 0x00126514
name: ConvertTicksToDate
tags: [function, time, date, ticks]
status: fully understood — pure ticks→date converter, no clamp
---

# [[FUN_00126514|ConvertTicksToDate]] — 64-bit Ticks → Date Converter

## Signature
```c
void FUN_00126514(date_obj* out, uint64_t* ticks);
```

## Purpose

Converts a 64-bit tick value back to a date struct. Inverse of [[FUN_00124120]].
Core utility for the entire date arithmetic pipeline.

## Call Contexts

Called in two separate functions:
1. [[FUN_00308F5C]] — date normalizer (Phases 1–5)
2. [[FUN_0030A594]] — date offset calculator (final step after accumulation)

## Findings (Session 10)

> [!IMPORTANT] No year clamp here
> Pure arithmetic via `FUN_0012952c` (Julian Day → date fields). No clamping on output year.
> The 2050 limit is NOT in this function. See [[FUN_0056BBD4]].

## See Also

[[FUN_00124120]], [[FUN_00308F5C]], [[FUN_00312A5C_0030A594]], [[FUN_0056BBD4]], [[research/2050-limit]]

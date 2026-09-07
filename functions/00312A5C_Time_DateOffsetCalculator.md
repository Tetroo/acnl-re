---
address: 0x00312A5C / 0x0030A594
name: DateOffsetCalculator
tags: [function, date, arithmetic, utility]
status: understood
---

# [[FUN_00312A5C]] / [[FUN_0030A594]] — Date Offset Calculator

## Addresses

- `FUN_00312A5C` — thunk → `FUN_0030A594`
- `FUN_0030A594` — real implementation

## Signature
```c
void FUN_0030a594(date_obj* param_1, uint years, uint months, uint days, uint hours);
```

## Purpose

Adds signed offsets (year/month/day/hour) to a date struct in-place.
Uses 64-bit arithmetic internally for overflow handling.

## Key Detail — Called with -6

In `FUN_0031872c` it is called as:
```c
FUN_00312a5c(&date_copy, 0, 0xfffffffa, 0, 0);
//                             ^^^^^^^^ = -6 as uint
```
So it shifts the date back by **6 months**. This "normalize to season start"
operation is the normal period calculation path.

## Internal Structure

Each parameter is multiplied by a DAT constant (ticks-per-year, ticks-per-month,
etc.) using 64-bit mul, then accumulated into the date value via `FUN_00133080`.
Final result written back to param_1 via `FUN_00126514` + `FUN_0056bbd4`.

## See Also

[[FUN_0031872C]], [[FUN_0062EEE8]]

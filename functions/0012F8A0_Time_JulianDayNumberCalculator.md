---
address: 0x0012F8A0
name: JulianDayNumberCalculator
tags: [function, time, calendar, julian-day]
status: understood — Julian Day Number calculator, epoch 2000
---

# [[FUN_0012F8A0|JulianDayNumberCalculator]] — Julian Day Number Calculator

## Signature
```c
int FUN_0012f8a0(int year, int month, int day);
```

## Purpose

Converts a calendar date to a Julian Day Number (or equivalent day-count integer),
using an epoch anchored at year 2000. Handles leap years correctly via standard
Gregorian calendar algorithm.

## Key Logic (annotated)

```c
int FUN_0012f8a0(int year, int month, int day) {
    uint year_offset = year - 2000;  // ← EPOCH IS 2000

    // Month adjustment: Jan/Feb belong to previous "year" in the algorithm
    if (month < 3) {
        month += 9;
        year_offset = year - 2001;
    } else {
        month -= 3;
    }

    // Leap year detection (Gregorian: /400 yes, /100 no, /4 yes)
    int is_leap = 1;  // default
    // ... standard Gregorian leap year checks on year_offset ...

    // Century and 4-year cycle counts
    int centuries = year_offset / 100;
    int quad_years = (year_offset - centuries * 100) /* remaining */ ;

    // Month-to-days lookup via fast multiply
    int month_days = (param_2 * 0x99 + 2) >> (some shift);

    // Combine everything
    return (centuries * DAT + (quad_years * DAT >> 2))
         + (month_days >> 1)
         + day
         + is_leap
         + 0x3a;   // constant offset (58 decimal — likely aligns epoch)
}
```

## Key Detail — No Year Cap

`year_offset = year - 2000`. No upper bound check. **The 2050 limit does not originate here.**
Confirmed: the limit is applied by [[FUN_0056BBD4]] (39-year modular wrap, called from [[FUN_00308F5C]]).

## Notes

- The `+ 0x3A` (58) offset likely aligns the result to Julian Day for 2000-01-01.
- `DAT_0012F988` — reciprocal magic constant for fast division by 100.
- `DAT_0012F990` — reciprocal for month lookup.

## See Also

[[FUN_001296BC]], [[FUN_0056BBD4]], [[research/2050-limit]]
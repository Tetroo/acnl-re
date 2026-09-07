---
tags: [type, date, time, engine]
aliases: [DateObject, date struct]
---

# DateObject — Runtime Date & Time Structure

Runtime date and time structure used throughout ACNL (Welcome amiibo EUR).

## Memory Layout

| Offset | Type | Field Name | Description |
|--------|------|------------|-------------|
| `+0x00` | `int32_t` | `year` | Absolute year (valid range `[2012, 2050]`) |
| `+0x04` | `int32_t` | `month` | Month (`1..12`) |
| `+0x08` | `int32_t` | `day` | Day of month (`1..31`) |
| `+0x0C` | `int32_t` | `hour` | Hour (`0..23`) |
| `+0x10` | `int32_t` | `minute` | Minute (`0..59`) |
| `+0x14` | `int32_t` | `second` | Second (`0..59`) |

**Total Size:** 24 bytes (`0x18`)

## C Definition

```c
typedef struct DateObject {
    int32_t year;    // [2012, 2050]
    int32_t month;   // 1 - 12
    int32_t day;     // 1 - 31
    int32_t hour;    // 0 - 23
    int32_t minute;  // 0 - 59
    int32_t second;  // 0 - 59
} DateObject;
```

## Related Functions

- [[FUN_0056BBD4]] (`NormalizeYearRange_2050Limit`): Validates `year` and applies a 39-year modular wrap when exceeding 2050.
- [[FUN_00124120]] (`DateToTicks`): Converts `DateObject` to 64-bit monotonic system ticks.
- [[FUN_00126514]] (`TicksToDate`): Unpacks system ticks into a `DateObject`.
- [[FUN_00308F5C]] (`NormalizeDate`): Normalizes all date fields in 5 phases (Phase 1 calls [[FUN_0056BBD4]]).
- [[FUN_0031872C]] (`ShiftDateMinus6Months`): Shifts date 6 months backward for seasonal weather calculations.

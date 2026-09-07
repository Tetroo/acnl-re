---
address: 0x001296BC
name: CalculateJulianDay
tags: [function, time, timestamp, epoch]
status: understood — converts date fields to 64-bit timestamp (epoch 2000)
---

# [[FUN_001296BC|CalculateJulianDay]] — Date → 64-bit Timestamp

## Signature
```c
void FUN_001296bc(longlong *out, int year, int month, int day,
                  int hour, int minute, int second, int ms);
```

## Purpose

Converts a broken-down date/time into a single 64-bit integer timestamp.
Used as the foundation of all date arithmetic in the time subsystem.

## Key Logic

```c
void FUN_001296bc(longlong *out, int year, int month, int day,
                  int hour, int minute, int second, int ms) {
    int jdn = FUN_0012f8a0(year, month, day);  // Julian Day Number from epoch 2000
    *out = (longlong)DAT_00129728 * jdn        // days → ticks
         + (longlong)DAT_00129724 * hour       // hours → ticks
         + (longlong)second * 1000             // seconds → ms
         + (longlong)minute * DAT_00129720     // minutes → ticks
         + ms;
}
```

## Scale Constants (DAT addresses, values unknown)
| Address | Role (inferred) |
|---------|-----------------|
| `DAT_00129728` | Ticks per day |
| `DAT_00129724` | Ticks per hour |
| `DAT_00129720` | Ticks per minute |

## Notes

- The tick unit is not confirmed — may be milliseconds, or a game-internal unit.
- `ms` (param_8) is added directly without scaling → it IS the base unit.
- Year is passed directly to `FUN_0012F8A0` which subtracts 2000 — so year is an
  absolute calendar year (e.g. 2025), not an offset.

## See Also

[[FUN_0012F8A0]], [[FUN_002FD4A0]], [[FUN_0012994C]]

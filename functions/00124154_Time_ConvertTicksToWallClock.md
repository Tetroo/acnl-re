---
address: 0x00124154
name: ConvertTicksToWallClock
tags: [function, time, rtc, ticks, critical]
status: fully understood — derives wall-clock time from system ticks + base timestamp
---

# FUN_00124154 — Current Time Derivation

## Signature
```c
void FUN_00124154(uint64_t* out);
```

## Purpose

Computes current wall-clock time by combining:
1. Elapsed ticks since startup (from [[FUN_0012994C]])
2. Base wall-clock timestamp set at game startup (from RTC via IPC)

## Logic

```c
void FUN_00124154(uint64_t* out) {
    uint64_t ticks = FUN_0012994c();          // calibrated tick count
    uint64_t elapsed = ticks * DAT_001241ac;  // scale to microseconds

    uint64_t base = *DAT_001241b0;            // base wall-clock (from RTC at startup)
    *out = FUN_0012972c(base, elapsed);       // base + elapsed = current time
}
```

## Constants

| Address | Value | Meaning |
|---------|-------|---------|
| `DAT_001241ac` | `0x000F4240` = 1,000,000 | Tick→microsecond scale factor |
| `DAT_001241b0` | → `0x00973ED8` (.data) | Base wall-clock timestamp pointer |

## DAT_001241B0 — Base Timestamp

Points to `0x00973ED8` in `.data` — the wall-clock time captured at startup
from the 3DS RTC via IPC (`ptm:u` service).
This is the only place where real-world time enters the game.
Set once at startup in [[FUN_0011E3D8]] init chain.

## See Also

[[FUN_0012994C]], [[FUN_0056A74C]], [[FUN_002FB39C]], [[research/2050-limit]]

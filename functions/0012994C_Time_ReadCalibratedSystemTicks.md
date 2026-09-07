---
address: 0x0012994C
name: ReadCalibratedSystemTicks
tags: [function, time, rtc, syscall]
status: understood — system tick reader; time is derived from ticks, not direct RTC
---

# [[FUN_0012994C|ReadCalibratedSystemTicks]] — System Time via GetSystemTick (SVC 0x28)

## Signature
```c
longlong FUN_0012994c(void);
```

## Purpose

Returns the current time as a 64-bit tick count derived from the 3DS system tick
counter. Critically: **this is not a direct RTC read**. The function calls
`SVC 0x28` (`GetSystemTick`) and converts the result to a game timestamp using
a stored epoch offset.

## SVC Used

```
software_interrupt(0x28)  →  svcGetSystemTick()
```

Per 3dbrew: `svcGetSystemTick` returns total CPU ticks since power-on as `s64`.
This is a monotonic counter, not wall-clock time.

## Key Logic

```c
longlong FUN_0012994c(void) {
    // Build timestamp for 2000-01-01 00:00:00 as reference
    puVar5 = FUN_0012f998(buf, 2000, 1, 1, 0, 0, 0, 0);

    // delta from epoch to some stored base time (DAT_00129BE8)
    lVar2 = (uint64)DAT_00129bec * (*puVar5 - *DAT_00129be8);

    // local_48 = base timestamp at startup (from DAT_001241B0)
    local_48 = FUN_00133080(&local_40, ...);

    // Compute second reference date (DAT_00129BF8)
    puVar5 = FUN_0012f998(buf2, DAT_00129bf8, 1, 1, 0, 0, 0, 0);

    // SVC 0x28 — read CPU tick counter
    software_interrupt(0x28);

    // Post-SVC: convert raw ticks to timestamp using stored calibration data
    // (DAT_00129C00, DAT_00129BFC, DAT_00129C04...)
    // Result = calibrated tick offset + local_48 (base date)
    return local_48 + calibrated_tick_offset;
}
```

## 2050 Limit — Not Here

The tick-based time derivation in this function has no year cap.
The 2050 limit is applied by [[FUN_0056BBD4]] inside [[FUN_00308F5C]].

## Open Questions

- DAT_00129BE8 — stored base epoch (game-start date), exact layout unknown
- DAT_00129BF8 — second reference year, meaning unknown
- DAT_00129C00 / DAT_00129C04 — tick calibration constants, values not read
- Who sets the initial RTC base at startup? (not yet located in init chain)

## Calibration Constants (read from ELF)

| Address | Value | Meaning |
|---------|-------|---------|
| `DAT_00129be8` | `0x00973ED8` | Pointer to base tick count |
| `DAT_00129bec` | `1,000,000` | Scale factor (microseconds) |
| `DAT_00129bf0` | `0xD7B634DB` | Fixed-point magic (same as [[FUN_00308F5C]]) |
| `DAT_00129bf4` | `0x431BDE82` | Fixed-point magic (same as [[FUN_00308F5C]]) |
| `DAT_00129bf8` | `2100` | Calibration anchor year |
| `DAT_00129bfc` | `0x1FF81000` | Calibration table selector (bit 0) |
| `DAT_00129c04` | `3,600,000` | Ticks per time unit denominator |

Calibration anchor year = **2100** (not 2000 as previously assumed).

## See Also

[[FUN_00124154]], [[FUN_0056A74C]], [[FUN_001296BC]], [[FUN_0012F8A0]], [[FUN_0012F998]], [[research/2050-limit]]
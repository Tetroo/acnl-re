---
address: 0x001e5ef4
name: PeriodicWeatherUpdate
status: partially decompiled
tags: [weather, rendering, update, timer]
decompiled_at: Session 23
---

# [[FUN_001e5ef4|PeriodicWeatherUpdate]] — Periodic Weather Update (300-frame interval)

## Summary

Per-frame tick that throttles weather parameter updates to every 300 frames (~5 seconds at 60fps). When the counter expires, calls [[FUN_001e63dc]] (interpolation) then [[FUN_001e6fe0]] (sky compositor).

```c
void FUN_001e5ef4(void) {
    if (*DAT_001e6010 != 0) return;          // disabled flag
    if (FUN_0012cb48() != 0) return;         // some system check
    if (FUN_0056bf8c() != 0) return;         // another gate
    if (*(char*)(DAT_001e6014 + 7) != 0) return;  // weather object disabled

    int obj = DAT_001e6014;
    int counter = *(obj + 0x50) - 1;
    *(obj + 0x50) = counter;

    if (counter < 1 || *(char*)(obj + 10) != 0) {
        *(obj + 0x50) = 300;        // reset counter
        FUN_001e63dc();             // interpolate weather params
        if (FUN_002f7574(0x200000, state))
            FUN_001e7618();         // weather update variant (bit 0x200000)
        FUN_001e7e98();
    } else {
        FUN_001e7e98();
    }
    FUN_001e6fe0();                 // push sky colors to GPU
}
```

## Notes

- Counter field at `obj + 0x50`, resets to **300** (frames) after each update
- `FUN_001e7618()` called when state bit `0x200000` is set — weather transition effect?
- `FUN_001e7e98()` called every frame regardless of counter
- `FUN_001e6fe0()` called every frame (sky GPU push always happens)

## See Also

- [[FUN_001e63dc]] — weather interpolation (called every 300 frames)
- [[FUN_001e6fe0]] — sky color compositor (called every frame)
- [[FUN_001e8518]] — immediate updater (resets and recomputes on state change)

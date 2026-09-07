---
address: 0x0056A74C
name: PeriodicTimeUpdater
tags: [function, time, update, singleton, critical]
status: fully understood — periodic time updater; applies 2050 wrap
---

# FUN_0056A74C — Time Singleton Updater

## Signature
```c
void FUN_0056A74C(void);
```

## Purpose

Periodically updates the global time singleton object at `DAT_0056a928` → `0x00AD46B0`.
This is the function that keeps the in-game clock running.
Critically: applies [[FUN_0056BBD4]] (2050 year normalizer) on every update.

## Logic

```c
void FUN_0056A74C(void) {
    uint64_t now;
    FUN_00124154(&now);               // get current wall-clock time

    // Compute delta since last update
    uint64_t delta = (now - *DAT_0056a918) * DAT_0056a91c;
    write_to_time_obj(DAT_0056a920, delta);

    FUN_0056b2b0(DAT_0056a924);       // tick subsystem

    time_obj* t = DAT_0056a928;       // = 0x00AD46B0 (primary time singleton)

    if (t->year < DAT_0056a92c) {
        // Normal path
        FUN_0056bbd4(t);              // ⭐ apply 2050 year wrap
        uint64_t scalar = FUN_0056ae78(t);
        write_scalar(DAT_0056a920, scalar);
    }

    FUN_0056c1f4(t);                  // finalize

    // Notify listeners if state changed
    if (*DAT_0056a938 == 0) {
        if (FUN_002fb35c(DAT_0056a940) == 0) {
            FUN_005cce0c(DAT_0056a940, t);  // notify subscriber A
            FUN_00314a68(DAT_0056a940);
        }
        FUN_002fb3d0(&now, DAT_0056a940);   // notify subscriber B
    }
}
```

## Key Globals

| Address | Points to | Role |
|---------|-----------|------|
| `DAT_0056a928` | `0x00AD46B0` | Primary time singleton (same as `DAT_002FB3A4` target) |
| `DAT_0056a920` | output object | Secondary time state |
| `DAT_0056a918` | last tick count | Delta computation base |
| `DAT_0056a92c` | year threshold | Triggers 2050 normalizer |
| `DAT_0056a938` | state flag | Gates listener notification |

## 2050 Normalizer Applied Here

[[FUN_0056BBD4]] is called on every time update.
Any year that reaches 2051+ is immediately wrapped back to 2012.
This is the runtime enforcement point — not just at date normalization.

## See Also

[[FUN_00124154]], [[FUN_0056BBD4]], [[FUN_002FB39C]], [[FUN_0056AE78]],
[[research/2050-limit]]

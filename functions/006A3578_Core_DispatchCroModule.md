---
address: 0x006A3578
name: DispatchCroModule
tags: [function, modules, dispatch, state, critical]
status: fully understood — .cro module dispatcher by game state
---

# FUN_006A3578 — CRO Module Dispatcher

## Signature
```c
void FUN_006a3578(int param_1);
```

## Purpose

Dispatches `.cro` dynamic modules based on current game state flags.
Called during scene transitions. Reads state via [[FUN_002F755C]] /
[[FUN_002F7574]], then calls [[FUN_006A332C]] for each required module.

## Module Dispatch Table

| State bit | Module(s) loaded |
|-----------|-----------------|
| `0x8` | `Indoor`, `Ftr` |
| `0x4` | `Outdoor` |
| `0x20` | `Village` + `RealVillage` (or `Dream` + `PlayerGhost` if state==3) + `Summer`/`Winter` |
| `0x8000` | `Museum` + `NpcDt` + one of: `MusFish`/`MusIns`/`MusFossil` |
| `0x1000` | `Shop` + `NpcDt` (many sub-states) |
| `0x200` | `DowntownEx` + `Downtown` + `NpcDt` |
| `0x400` | `DowntownEx` + `Exhibition` |
| `0x400000` | `Station` |
| `0x10000` | `PoliceBox` |
| `0x1000000` | `Cafe` + `NpcDt` |
| `0x800000` | `Club` + `NpcDt` |
| `0x20000` | `Kotobuki` + `Tour` |
| `0x80` + state==3 | dream-related module |
| state `0x9F`/`0xA0` | `AutoCamp` |
| state `0x67` | `Tour` |

## Village Path (bit 0x20) — Detail

```c
if (FUN_0030601C() == 3) {
    load("Dream"); load("PlayerGhost");
} else {
    load("Village"); load("RealVillage");
    date = FUN_002fb39c();
    FUN_00308f5c(date);  // normalize date (applies 2050 wrap)
    int period = FUN_0056ac80(date);
    if ((period - 3 < 0xC) && hemisphere != 0x02)
        load("Summer");
    else
        load("Winter");
}
```

> [!IMPORTANT] RealVillage contains town generation code
> `ModuleRealVillage.cro` is where the actual village logic lives.
> The town seed / acre layout system is inside this .cro, not in the main ELF.

## See Also

[[FUN_006A332C]], [[FUN_0056AC80]], [[FUN_002F755C]], [[FUN_002F7574]],
[[FUN_0030601C]], [[FUN_002FB39C]], [[FUN_00308F5C]], [[research/town-generation]]

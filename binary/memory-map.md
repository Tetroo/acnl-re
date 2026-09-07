---
tags: [binary, memory, segments, addresses]
aliases: [memory map, segments, address map]
---

# Memory Map

## Segments

| Segment   | Start        | End          | Description                                  |
| --------- | ------------ | ------------ | -------------------------------------------- |
| `.text`   | `0x00100000` | `0x00837FFF` | Executable code                              |
| `.rodata` | `0x00838000` | `0x00943FFF` | Read-only data (strings, vtables, constants) |
| `.data`   | `0x00944000` | `0x00981FFF` | Initialized data                             |
| `.bss`    | `0x00982000` | `0x00AF7FFF` | Uninitialized data (runtime pointers, heaps) |

> [!WARNING] Correction
> Previous documentation had wrong `.bss` base (`0x00950000`). Corrected from Ghidra segments.

---

## Known Addresses

| Address | Section | Type | Description |
|---------|---------|------|-------------|
| `0x001003F0` | `.text` | string ref | `"WeatherHeap"` — used in heap init |
| `0x00229700` | `.rodata` | string | `"BsWeatherRain"` |
| `0x0022AA44` | `.rodata` | string | `"BsWeatherSnow"` |
| `0x0026646C` | `.rodata` | string | `"BsWeatherPaper"` |
| `0x00296188` | `.rodata` | string | `"BsWeatherSakura"` |
| `0x009121D6` | `.rodata` | string | `"Weather/paper_model.bcres"` |
| `0x009121F0` | `.rodata` | string | `"Weather/ame_model.bcres"` (rain) |
| `0x00912208` | `.rodata` | string | `"Weather/cherryblossoms_model.bcres"` |
| `0x0091222B` | `.rodata` | string | `"Weather/snow_model.bcres"` |

### `.data` — Global Pointers (initialized)
| Address      | Description                                                    | Known from            |
| ------------ | -------------------------------------------------------------- | --------------------- |
| `0x002F7568` | Pointer to global state object (byte +2 = current state index) | [[FUN_002F755C]] READ |
| `0x002F7598` | Pointer to state flag table (array of uint32, max 165 entries) | [[FUN_002F7574]] READ |
| `0x002F7428` | "Pending" state object — written during transition setup       | [[FUN_002F72BC]] WRITE |
| `0x002F67E8` | "Active" state object — written when transition completes      | [[FUN_002F72BC]] WRITE |
| `0x002FB358` | Pointer to period-index singleton object (getter: [[FUN_002FB34C]]) | [[FUN_0062EEE8]] READ |
| `0x002FB3A4` | Primary time singleton (getter: [[FUN_002FB39C]])                  | [[FUN_002FB39C]] READ |
| `0x003090F0` | `0x30B8A000` — tick-space offset constant in [[FUN_00308F5C]]      | [[FUN_00308F5C]] READ |
| `0x003090F4` | `0xD7B634DB` — fixed-point magic constant (tick arithmetic)        | [[FUN_00308F5C]] READ |
| `0x003090F8` | `0x431BDE82` — fixed-point magic constant (tick arithmetic)        | [[FUN_00308F5C]] READ |

### `.bss` — Runtime Pointers
| Address      | Description | Known from |
| ------------ | ----------- | ---------- |
| `0x00951C34` | **HeapManager** — parent object for ALL named heaps | data_items (all heap DATs share this address) |

> [!NOTE]
> `DAT_001003FC` and `DAT_001009F8` both equal `0x00951C34` — confirmed same object.  
> WeatherHeap pointer lives at `HeapManager + 0x10`, not at a separate global.  
> Old address `0x00951C44` was incorrect — discarded.

### Root Game Object
| Address      | Description |
| ------------ | ----------- |
| `0x00120CA0` | Root game object pointer — set before main loop, used in [[FUN_0011EDFC]] |

---

## System Calls

The imports table is **empty**. 3DS syscalls are made via ARM `SVC` instructions directly.
No symbol names are available from imports — call sites must be found by scanning for SVC opcodes
or known syscall numbers.

Known syscalls (confirmed from decompilation):

| SVC    | Name            | Description                                                        |
| ------ | --------------- | ------------------------------------------------------------------ |
| `0x28` | `GetSystemTick` | Returns total CPU ticks since power-on as `s64` (monotonic clock) |

**Important:** `SVC 0x28` is confirmed in [[FUN_0012994C]]. This is a monotonic tick counter,
NOT a wall-clock read. The game derives current time by: stored base timestamp (set at startup)
+ elapsed ticks since then. The initial RTC read has not yet been located — may use IPC
(e.g. `ptm:u` service) rather than a direct SVC.

> [!WARNING] Discarded
> Previous note listed `SVC 0x1A` as `GetSystemTime` — this was unverified and incorrect.
> Per 3dbrew: `0x1A` = `CreateTimer`. Discarded.

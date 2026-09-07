---
tags: [type, save, save-buffer, file-format]
aliases: [SaveFile, save buffer, garden_plus.dat, SaveBuffer]
---

# SaveFile — Save Format & Memory Buffer Layout

Architecture and in-memory buffer layout of the primary save file (`garden_plus.dat`) in Animal Crossing: New Leaf — Welcome amiibo (EUR).

---

## File Overview

ACNL manages two persistent save archives mounted under the `data:` virtual drive:
- **`/garden_plus.dat`** (string at `0x00930480`) — Primary town, villager, player, and item save state.
- **`/exhibition.dat`** (string at `0x00930474`) — Happy Home Showcase / streetpass exhibition data.

---

## Loading & Decryption Pipeline

```
nn::fs Mount ("data:") ──▶ FUN_001b927c
                                │
File Pre-flight Check   ──▶ FUN_001b9428
                                │
Raw Read into Buffer    ──▶ FUN_001b8b9c / FUN_001b96c8
                                │
XOR Decrypt / Checksum  ──▶ FUN_0047dac0 (256-byte substitution table)
                                │
C++ Object Construction ──▶ FUN_005ca4f4 (chained placement ctors)
```

1. **Archive Mount:** [[FUN_001b927c]] mounts the save archive via `nn::fs` into `"data:"`.
2. **Access Verification:** [[FUN_001b9428]] performs pre-flight checks.
3. **Raw I/O:** [[FUN_001b8b9c]] (wrapped by [[FUN_001b96c8]]) reads the raw encrypted file into memory.
4. **Decryption & Checksum:** [[FUN_0047dac0]] decrypts the buffer in-place using a 256-entry lookup table.
5. **C++ Object Graph Init:** [[FUN_005ca4f4]] runs placement constructors over the structured sub-regions.

---

## Save Buffer Layout (563,968 bytes / `0x89B00`)

The save file in memory spans exactly `563,968` bytes (`0x89B00`). The global runtime pointer to this buffer is stored at `DAT_0095AF74` and accessed via [[FUN_005c9d64]], [[FUN_00300fdc]], or [[FUN_002fb34c]].

| Buffer Offset | Size | Region Description | Handler / Constructor |
|---------------|------|--------------------|-----------------------|
| `+0x00000` | `0x292A4` (168,612 B) | Raw POD terrain, grid items, patterns (no C++ ctors) | Raw byte access |
| `+0x292A4` | `0x18EF0` (94,960 B) | Villager data array (10 slots × `0x2518` stride) | [[FUN_005c9a24]], [[FUN_00301ebc]] |
| `+0x42194` | `0x2F76C` (~194 KB) | C++ structured objects constructed by [[FUN_005ca4f4]] chain | [[FUN_005ca4f4]] |
| ↳ `+0x53430` | — | Field / Town container object | [[FUN_002d457c]] |
| ↳ `+0x53484` | `0x54` (84 B) | **TownAcres[7×6]** (42 × `uint16_t` AcreID) | `[FUN_002d457c] + 0x54` |
| `+0x71900` | ~36 B | Small map state object | [[FUN_001e0340]], [[FUN_002fb860]] |
| `+0x73958` | `0x1CE56` (117,334 B) | Player data (4 players × `0x1B88` / 7,048 B stride) | [[FUN_006f4234]] |
| `+0x7A778` | ~5,760 B | Player secondary data (4 players × `0x5A0` / 1,440 B) | Sub-structs |
| `+0x89AB8` | `0x54` (84 B) | Tail fields & footer integrity markers | State dispatchers |
| `+0x89B00` | — | **End of Buffer** (`563,968` bytes total) | — |

---

## C Structure Definition (High-Level Header)

```c
#include <stdint.h>

#define SAVE_BUFFER_SIZE  0x89B00  // 563,968 bytes
#define VILLAGER_SLOTS    10
#define VILLAGER_STRIDE   0x2518   // 9,496 bytes
#define PLAYER_SLOTS      4
#define PLAYER_STRIDE     0x1B88   // 7,048 bytes
#define TOWN_ACRES_X      7
#define TOWN_ACRES_Y      6
#define TOWN_ACRES_COUNT  42       // 84 bytes

typedef struct TownAcres {
    uint16_t acres[TOWN_ACRES_Y][TOWN_ACRES_X]; // [6][7] = 42 acres
} TownAcres;

typedef struct VillagerData {
    uint8_t raw[VILLAGER_STRIDE];
} VillagerData;

typedef struct PlayerData {
    uint8_t raw[PLAYER_STRIDE];
} PlayerData;
```

---

## Key References

- [[FUN_005ca4f4]] — Master C++ save object constructor.
- [[FUN_002d457c]] — Constructor for Field / Town object containing `TownAcres`.
- [[FUN_005c9a24]] — Villager slot reader and validation iterator.
- [[FUN_0047dac0]] — XOR decryption engine.
- [[research/town-generation]] — Research on town layout and acre allocation.

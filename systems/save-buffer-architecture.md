---
tags: [system, save, memory-layout, architecture, confirmed]
aliases: [save buffer architecture, garden_plus layout, save memory map]
---

# Save Buffer Architecture

> [!WARNING] Correction (audit 2026-09-07, first + second pass)
> This document was labeled "Confirmed & Byte-Exact". A live re-decompile of the two functions this map is derived from ([[FUN_005ca4f4]]/`Save_SaveObjectMasterConstructor` and `Save_SaveObjectMasterConstructor`'s gap table) found the constructor-chain gap table had an **off-by-one error**: the gap attributed to `FUN_002d457c` (`Town_ConstructFieldTownObject`) was wrong by a full sub-object's width (see `functions/005CA4F4_Save_SaveObjectMasterConstructor.md` for the corrected table). This document's per-line offsets below have **not** been individually re-verified against that correction — treat every absolute offset past `0x42194` as needing re-derivation, not "confirmed".
>
> **Second-pass update:** `Town_ConstructFieldTownObject` (`0x002D457C`) has now been decompiled. It **refutes** the `TownAcres @ +0x54` claim outright — the real constructor zeroes `[+0x4,+0x20)` and `[+0x6c,+0x7f4)`, never touching `+0x54`. See the table row below for detail. TownAcres' real location is unresolved, not merely unverified.

**Status:** Partial — several offsets require re-verification (see warning above)  
**Target File:** `/garden_plus.dat` (Save Archive `data:`)  
**Total Buffer Size:** `563,968` bytes (`0x89B00`)  
**Global Pointer:** `DAT_0095AF74` (.data)

---

## High-Level Overview

Animal Crossing: New Leaf — Welcome amiibo (EUR) loads its primary save file into a contiguous 563,968-byte memory buffer. Unlike older Nintendo titles where the save file was purely a static C struct, ACNL splits the buffer into:
1. **Raw POD Region (`0x00000`–`0x292A3`):** Terrain tiles, placed acre items, patterns, and town flags.
2. **Villager Region (`0x292A4`–`0x42193`):** Array of 10 villager slots (`0x2518` stride).
3. **Structured C++ Sub-Object Region (`0x42194`–`0x718FF`):** Managed by placement constructors chained in [[FUN_005ca4f4]]. Contains the Field/Town object with **TownAcres[7×6]** (`0x53484`).
4. **Player Region (`0x73958`–`0x89AB7`):** 4 player character profiles (`0x1B88` stride) + secondary data (`0x5A0` stride).
5. **Integrity & Tail Markers (`0x89AB8`–`0x89AFF`):** Save state machine status flags and checksum sentinels.

---

## Full Save Buffer Memory Map

| Offset Range | Size | Component | Handler / Validator | Description |
|---|---|---|---|---|
| `0x00000` – `0x292A3` | 168,612 B (`0x292A4`) | Raw POD Terrain & Grid Items | Raw byte loaders | Contains acre tile indices, weeds, flowers, patterns. No C++ ctors called. |
| `0x292A4` – `0x42193` | 94,960 B (`0x18EF0`) | Villager Array (10 slots) | [[FUN_005c9a24]] | 10 villager records × `0x2518` (9,496 B). Validated per-slot via [[FUN_00301ebc]]. |
| `0x42194` – `0x718FF` | ~194 KB | C++ Sub-Object Chain | [[FUN_005ca4f4]] | Chained placement constructors following the "ctor returns `this`" ABI pattern. Gap table between sub-ctors was corrected 2026-09-07 — re-check before relying on any single offset. |
| ↳ `+0x53430` | — | Field / Town Object | [[FUN_002d457c]] | Top-level container for town map grid and acre attributes. Gap before this call is `+0x28f4` from the prior sub-ctor's return, corrected 2026-09-07 (was previously miscalculated). |
| ↳ `+0x53484` | 84 B (`0x54`) | **TownAcres[7×6] — REFUTED at this offset** | ~~`[FUN_002d457c] + 0x54`~~ | See warning above the table: `[TOOL]` decompile of `Town_ConstructFieldTownObject` (2026-09-07, second pass) shows it zeroes `[+0x4,+0x20)` and `[+0x6c,+0x7f4)` only — **it never touches offset `+0x54` at all**. The claimed `+0x54` TownAcres location does not survive contact with the real constructor. `HYPOTHESIS`: TownAcres may instead be populated later by `Town_InitTownGridAndAcres` (`0x00612820`, separately confirmed real in the Town/Player/Villager audit pass) rather than zero-initialized here — but the absolute buffer offset that function writes to was not cross-checked against `+0x53430`/`+0x53484` in this pass either. Do not use `+0x53484` for anything until `Town_InitTownGridAndAcres` is traced back to an absolute save-buffer offset. |
| `0x71900` – `0x71923` | ~36 B | Small Map State Object | [[FUN_001e0340]] | Minimal state struct; accessed via getter [[FUN_002fb860]]. |
| `0x71924` – `0x73957` | ~8 KB | Intermediate Subsystems | Lifecycle dispatchers | System flags, event states, NPC visit queue. |
| `0x73958` – `0x7A777` | 28,192 B (`0x6E20`) | Player Primary Data (4 slots) | [[FUN_006f4234]] | 4 player slots × `0x1B88` (7,048 B). Contains inventory, appearance, badges, wallet. |
| `0x7A778` – `0x7BDF7` | 5,760 B (`0x1680`) | Player Secondary Data (4 slots) | Sub-struct loaders | 4 slots × `0x5A0` (1,440 B). Extended player configurations. |
| `0x7BDF8` – `0x89AB7` | ~56 KB | Town Hall & Main Street State | Sub-struct ctors | Buildings, museum donations, catalog, Happy Home Showcase link. |
| `0x89AB8` – `0x89AFF` | 72 B | Tail & State Descriptors | [[FUN_001d3748]] | Header/footer sync tokens, state machine dispatch bits. |
| `0x89B00` | — | **Buffer End (EOF)** | — | Exact size: 563,968 bytes. |

---

## Runtime Access Architecture

```
Global Pointer: DAT_0095AF74 (.data)
        │
        ├──▶ FUN_005c9d64()  ── Returns raw buffer pointer (trivial getter)
        ├──▶ FUN_00300fdc()  ── Buffer getter with pre-check validation (FUN_006064bc)
        └──▶ FUN_002fb34c()  ── Central accessor (*DAT_002FB358, 200+ callers)
```

- **`DAT_0095AF74`**: Global pointer variable holding the heap-allocated address of the decrypted buffer.
- **Allocation & Lifetime:** Allocated in [[FUN_005c9dd4]] (`operator new(0x89b00)`), destroyed in [[FUN_0012cc9c]].
- **Decryption:** Read by [[FUN_001b96c8]] and decrypted in-place by [[FUN_0047dac0]] using a 256-entry XOR lookup table before any C++ constructor or parser runs.

---

## C++ Constructor Chain Discovery (Session 26 Finding)

When the save file is loaded, [[FUN_005ca4f4]] initializes sub-objects at fixed offsets:
```
FUN_005ca4f4(buffer + 0x42194)
  ├── FUN_00301d9c(cursor + 0x00000)
  ├── FUN_00301d9c(cursor + 0x1129c)
  ├── ...
  └── FUN_002d457c(buffer + 0x53430)  ── Field / Town Object
        └── [this + 0x54] = TownAcres[7][6] @ 0x53484   ⚠️ REFUTED — see warning above, real ctor never touches +0x54
```

> [!IMPORTANT] Compiler ABI Invariant
> Every sub-constructor in this chain follows the ARM C++ ABI convention: **`return this` (returns `param_1` unmodified)**.
> The call-site offsets (e.g. `buffer + 0x53430`) are additive and reliable. **What is NOT reliable is guessing a sub-object's *internal* field layout without decompiling it** — the `+0x54` TownAcres guess above is exactly that mistake: a plausible-looking offset inferred without ever reading `Town_ConstructFieldTownObject`'s actual body. Internal field offsets require decompiling the constructor, full stop.

---

## Downstream Implementation for Native PC Port

For the native PC rewrite:
1. Load `garden_plus.dat` directly into a `uint8_t[563968]` buffer.
2. Apply the inverse XOR transformation via `FUN_0047dac0` table.
3. Map typed structs directly over offsets:
   - `TownAcres* acres = (TownAcres*)(buffer + 0x53484);` — ⚠️ **do not use this offset**, refuted 2026-09-07 (see warning above). Needs re-derivation from `Town_InitTownGridAndAcres` before the port can rely on it.
   - `VillagerSlot* villagers = (VillagerSlot*)(buffer + 0x292A4);`
   - `PlayerSlot* players = (PlayerSlot*)(buffer + 0x73958);`

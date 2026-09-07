# Items & Inventory Subsystem

> [!WARNING] `⚠️ External game knowledge, NOT reverse engineering` (audit 2026-09-07, second pass): this entire file cites **zero function addresses** and no `decompile_function`/`read_memory` output. The item ID range table below is plausible general knowledge about how ACNL categorizes items, not something confirmed against the binary in any session. Treat every number here as `[RECALL]`/`HYPOTHESIS (not confirmed by bytes)` until someone finds and dumps the actual item definition table in Ghidra and cross-checks these ranges against it.

## 1. Overview

Every physical object in Animal Crossing: New Leaf — whether placed in the town grid, stored in player pockets, hung in a house, or worn as clothing — is identified by a **16-bit Item ID** (`uint16_t`).

---

## 2. Item ID Allocation Ranges

| Range (Hex) | Category | Description |
|---|---|---|
| `0x0000` | Empty / Null | No item / unassigned slot |
| `0x0001` .. `0x007F` | Environment Objects | Rocks, signboards, bulletin boards, event triggers |
| `0x0080` .. `0x00BF` | Buried Objects | Fossils, pitfalls, gyroids, buried time capsules |
| `0x00C0` .. `0x1FFF` | Town Flora | Trees (oak, cedar, palm, fruit trees), bushes, flowers, weeds |
| `0x2000` .. `0x20FF` | Equipment & Tools | Shovels, nets, fishing rods, axes, watering cans, slingshots, wet suits |
| `0x2100` .. `0x2FFF` | Clothing & Accessories | Tops, bottoms, dresses, hats, accessories, socks, shoes |
| `0x3000` .. `0x4FFF` | Furniture & Decorations | Indoor and outdoor furniture pieces, wallpaper, flooring |
| `0x5000` .. `0x5FFF` | Fauna & Museum Specimens | Insects, river fish, ocean fish, deep-sea creatures |
| `0x7FFD` .. `0x7FFE` | Sentinels | Wrapped presents, letter attachments, inventory sentinel |

---

## 3. Player Inventory Architecture

A player's active inventory (`PlayerPockets`) contains **16 slots**:
- Each slot occupies **4 bytes** in `garden_plus.dat`:
  - `ItemId` (2 bytes): 16-bit item identifier.
  - `Flags` (2 bytes): Refurbishing variation, wrapped status, fruit quantity (for baskets of up to 9 fruits).
- Total player pocket footprint: $16 \times 4 = 64$ bytes per player.

---

## 4. C++ Port Integration

All item structures and categories are defined in [`types/Item.h`](file:///c:/Users/user/Documents/acnl_re/types/Item.h) for direct consumption in the C++20 engine.

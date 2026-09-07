---
address: 0x006EED70
name: Town_InitPlayableAcreTileGrid
subsystem: Town
tags: [town, acre, tiles, grid, terrain, confirmed]
status: done
---

# `006EED70_Town_InitPlayableAcreTileGrid` — Playable Acre Tile Initializer

## Overview

Called from [[00612820_Town_InitTownGridAndAcres]] for each of the $20$ playable acres ($5 \times 4$ grid, stride `0x400` bytes).

This function initializes the $16 \times 16$ tile array inside each $1024$-byte playable acre data block.

---

## Memory & Tile Geometry

- **Acre Dimensions:** $16 \times 16$ tiles.
- **Tile Size:** $4$ bytes per tile (`uint32_t`).
- **Row Stride:** $16 \times 4 = 64$ bytes (`0x40`).
- **Total Acre Tile Data:** $16 \times 64 = 1024$ bytes (`0x400`).

Address formula for tile $(col, row)$:
$$\text{tile\_ptr} = \text{param\_1} + (\text{row} \times 0x40) + (\text{col} \times 4)$$

---

## Decompiled Code (Ghidra 12.1.3)

```c
void Town_InitPlayableAcreTileGrid(int param_1)
{
  int iVar1;
  uint uVar2;
  uint uVar3;
  
  uVar3 = 0;
  do {
    uVar2 = 0;
    do {
      iVar1 = FUN_0057ac84(uVar2, uVar3);
      if (iVar1 != 0) {
        FUN_002fc978(param_1 + uVar3 * 0x40 + uVar2 * 4, u_>CHMR_0095afc8 + 5);
      }
      uVar2 = uVar2 + 1;
    } while (uVar2 < 0x10);
    uVar3 = uVar3 + 1;
  } while (uVar3 < 0x10);
  return;
}
```

---

## Key Findings

1. **Tile Loop Bounds:**
   - Outer loop `uVar3` iterates rows: `0 .. 0xF` ($16$ rows).
   - Inner loop `uVar2` iterates columns: `0 .. 0xF` ($16$ columns).
2. **Tile Initialization Logic:**
   - Evaluates tile position via `FUN_0057ac84(col, row)`.
   - If non-zero, applies default terrain tile state through `FUN_002fc978` writing into `param_1 + row * 0x40 + col * 4`.

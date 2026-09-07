---
address: 0x00612820
name: InitTownGridAndAcres
tags: [town, acres, grid, generation, save-system, confirmed]
status: fully understood — SOLVED TOWN ACRES RUNTIME HANDLER
---

# [[FUN_00612820|InitTownGridAndAcres]] — Town Acres & Playable Grid Initializer

## ⭐ KEY FINDING — TOWN ACRES RUNTIME HANDLER RESOLVED

**This is the missing runtime handler for `TownAcres[7×6]` (`0x53484`)!**

It initializes three nested grids inside the Field/Town region:
1. **$7 \times 6$ TownAcres Array (`param_1 + 0x04` = `buffer + 0x53484`):**
   Iterates $7$ columns (`uVar1 < 7`) $\times$ $6$ rows (`uVar2 < 6`), stride `0xE` (14 bytes/row) $\times$ $2$ bytes/acre. Calls [[FUN_00611d34|SetDefaultAcreId_109]] to set each acre to `0x109`.
2. **$5 \times 4$ Playable Acre Tile Grid (`param_1 + 0x58`):**
   Iterates $5$ columns $\times$ $4$ rows ($20$ playable inner acres). Stride: `0x400` ($1024$ bytes per acre = $16 \times 16$ tiles $\times$ $4$ bytes) and `0x1400` ($5120$ bytes per row). Calls [[FUN_006eed70]] to initialize terrain tile data.
3. **$5 \times 4$ Playable Acre Flag Grid (`param_1 + 0x5058`):**
   Iterates $5$ columns $\times$ $4$ rows ($20$ inner acres). Stride: `0x100` ($256$ bytes per acre = $16 \times 16$ tile attribute flags) and `0x500` ($1280$ bytes per row). Calls [[FUN_003014cc]].

---

## Decompiled Code

```c
void InitTownGridAndAcres(undefined1 *param_1) // param_1 = buffer + 0x53480
{
  uint uVar1;
  uint uVar2;
  
  *param_1 = 2;
  param_1[1] = 3;
  param_1[2] = 3;
  
  // 1. 7x6 TownAcres grid (42 acres x 2 bytes):
  uVar2 = 0;
  do {
    uVar1 = 0;
    do {
      // param_1 + 4 = buffer + 0x53484: TownAcres[row][col]!
      SetDefaultAcreId_109(param_1 + uVar1 * 2 + uVar2 * 0xe + 4);
      uVar1 = uVar1 + 1;
    } while (uVar1 < 7);
    uVar2 = uVar2 + 1;
  } while (uVar2 < 6);
  
  // 2. 5x4 Playable Acres terrain grid (20 acres x 1024 bytes):
  uVar2 = 0;
  do {
    uVar1 = 0;
    do {
      FUN_006eed70(param_1 + uVar1 * 0x400 + uVar2 * 0x1400 + 0x58);
      uVar1 = uVar1 + 1;
    } while (uVar1 < 5);
    uVar2 = uVar2 + 1;
  } while (uVar2 < 4);
  
  // 3. 5x4 Playable Acres flag grid (20 acres x 256 bytes):
  uVar2 = 0;
  do {
    uVar1 = 0;
    do {
      FUN_003014cc(param_1 + uVar1 * 0x100 + uVar2 * 0x500 + 0x5058);
      uVar1 = uVar1 + 1;
    } while (uVar1 < 5);
    uVar2 = uVar2 + 1;
  } while (uVar2 < 4);
  
  FUN_003014d4(param_1 + 0x6480, 0x4000);
  return;
}
```

---

## Callers

1. [[FUN_005ca2c4|ResetSaveBuffer_AllSubObjects]] — during save data reset / initialization.
2. [[FUN_0027c8fc|GenerateTownLayout_4Options]] — during new game town layout generation on the train.

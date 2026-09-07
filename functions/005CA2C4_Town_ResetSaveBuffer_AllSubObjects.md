---
address: 0x005CA2C4
name: ResetSaveBuffer_AllSubObjects
tags: [save-system, reset, town, villagers, players, confirmed]
status: fully understood
---

# [[FUN_005ca2c4|ResetSaveBuffer_AllSubObjects]] — Save Buffer Default State Reset

## Summary

Resets all sub-objects in the `garden_plus.dat` in-memory buffer to their initial/default state when creating a new save file.

Calls [[FUN_00612820|InitTownGridAndAcres]] at `param_1 + 0x53400` (`buffer + 0x53480`), clearing the $7 \times 6$ TownAcres grid and $5 \times 4$ playable acre tile grids. Also resets villager array, player slots, and town descriptors.

## Key Subroutine Calls

> [!WARNING] Correction (audit 2026-09-07): non-exhaustive list, one wrong function name
> Live re-decompile shows this function makes **~30 calls total**, not just the 4 below — this table is a hand-picked subset, not the full picture. Also, the villager-slots call at `+0x29220` was previously attributed to `FUN_0011253c`; the real decompile shows it is `Save_InitVillagersBlock` (already renamed/analyzed elsewhere in the project). Corrected:

- `InitTownGridAndAcres(param_1 + 0x53400)` — clears TownAcres and acre grids. ✓ verified
- `FUN_005ccc50(param_1 + 0x4be00)` — sub-system reset. ✓ verified
- `Save_InitVillagersBlock(param_1 + 0x29220)` — villager slots reset (was wrongly labeled `FUN_0011253c`). ✓ verified after correction
- `FUN_002d4524(param_1 + 0x52bb0)` — field object cleanup. ✓ verified
- Also present (previously undocumented): `Save_InitPublicWorksBlock(+0x5d880)`, `Museum_InitCustomExhibitions(+0x502bc)`, `Player_ConstructPlayerTailObject` (×2, `+0x6e212` and `+0x70e62`), plus ~20 smaller field resets/zeroing calls not itemized here.

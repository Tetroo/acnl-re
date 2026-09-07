---
address: 0x00112584
name: GridSubObjectConstructor
status: partially decompiled
tags: [save-system, constructor, grid, closed-candidate]
decompiled_at: Session 24
---

# [[FUN_00112584|GridSubObjectConstructor]] — 2×2 Grid Sub-Object Constructor (called right after villager array in [[FUN_005ca4f4]] chain)

## Summary

First sub-ctor called in [[FUN_005ca4f4]]'s chain, at `param_1 = iVar1 + 0x29200` (where `iVar1` is the cursor after the initial `DAT_005cabf4 × 4` array). Builds **two separate 2×2 grids** of sub-objects via [[FUN_00308d08]], not a 7×6 grid:

```c
int FUN_00112584(int param_1) {
    iVar1 = FUN_00111d48(param_1 + 4);
    iVar1 = FUN_00301d9c(iVar1 + -4 + DAT_00112600, DAT_00112608, DAT_00112604, 4, param_1);
    // Grid A: 2x2
    for (iVar5 = 0; iVar5 < 2; iVar5++)
        for (iVar3 = 0; iVar3 < 2; iVar3++)
            FUN_00308d08(DAT_0011260c*iVar5 + iVar1 + iVar3*DAT_00112604);
    iVar1 = iVar1 + DAT_00112610;
    FUN_002f8280(iVar1, DAT_00112614);
    FUN_00310950(iVar1 + 4);
    iVar1 = iVar1 + DAT_00112600;
    // Grid B: 2x2 (same pattern)
    for (iVar2 = 0; iVar2 < 2; iVar2++)
        for (iVar4 = 0; iVar4 < 2; iVar4++)
            FUN_00308d08(DAT_0011260c*iVar2 + iVar1 + iVar4*DAT_00112604);
    return iVar5;  // = pointer to start of Grid B
}
```

> [!WARNING] Discarded hypothesis
> The offset arithmetic in [[FUN_005ca4f4]] (`param_1(=buffer+0x80) + 0x20 + 4*DAT_005cabf4(elem size, unread) + 0x29200`) lands suspiciously close to the confirmed villager array start (`buffer+0x292A4`), which raised hope this constructs TownAcres. **Rejected as TownAcres candidate**: loop bounds are `2×2` (4 elements per grid, 2 grids), not `7×6` (42). Not the acre grid.

## Open Questions

- What are these two 2×2 grids? (4 cardinal directions × 2 objects? Bridge/dock endpoints? Camera anchors?)
- What does [[FUN_00308d08]] construct per cell?
- Exact absolute buffer offset of this call — depends on unread `DAT_005cabf4` value

## See Also

- [[FUN_005ca4f4]] — caller (main save object constructor)
- [[FUN_005c9a24]] — villager slot reader (raw block ends where this chain begins)

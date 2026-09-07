---
address: 0x005ccdc8
name: SaveBufferSubRegionConstructor
status: partially decompiled
tags: [save-system, constructor, struct-init]
decompiled_at: Session 23
---

# [[FUN_005ccdc8|SaveBufferSubRegionConstructor]] — Save Buffer Sub-Region Constructor (post-167K raw block)

## Summary

Initialises the C++ object region immediately after the 167,936-byte raw data block in the save buffer. Called from [[FUN_005ca4f4]] after jumping +0x29200 bytes past the first constructed array.

```c
int FUN_005ccdc8(int param_1) {
    int iVar1 = FUN_00301d9c(param_1 + 8, DAT_005cce04, 4, 0x38);  // 56 × 4B
    iVar1 = FUN_00301d9c(iVar1 + 0xe0, DAT_005cce04, 4, 2);         // 2 × 4B
    iVar1 = FUN_0027a320(iVar1 + 8);
    return iVar1 + -0xf0;
}
```

## Notes

- Total region managed: ~(56×4) + 0xe0 + 8 + sub = small (~600B)
- The **167,936-byte gap** before this function is raw data (no C++ ctor) — likely terrain items and TownAcres

> [!IMPORTANT] The 0x29200 (167,936B) raw block before this constructor is the primary candidate for TownAcres and terrain item data.

## See Also

- [[FUN_005ca4f4]] — caller; jumps +0x29200 to reach this
- [[FUN_00301d9c]] — forward iterator (array constructor)

---
address: 0x002ca040
name: VillageIslandTemplateResourceLoader
status: partially decompiled
tags: [town-generation, resource-loader, template, tortimer-island]
decompiled_at: Session 24
---

# [[FUN_002ca040|VillageIslandTemplateResourceLoader]] — Village + Island Template Resource Loader (18-island loop)

## Summary

Larger sibling of [[FUN_0027c56c]]: same grass-baldness/village-template setup, then loops **18 times** (`uVar15 <= 0x11`, i.e. indices 0–17) loading per-island resources via [[FUN_00200a94]], before finally kicking off async loads of all four template files:

```c
do {
    // ... build resource name from local_f8 (island name table), async-load via FUN_00200a94 ...
    uVar15 = uVar15 + 1;
    if (0x11 < uVar15) {   // loop runs exactly 18 times
        uVar15 = FUN_002ee9cc(param_1+5000,   "TemplateData/village/sea_side_left.bin",  local_a0);
        uVar9  = FUN_002ee9cc(param_1+0x1478, "TemplateData/village/sea_side_right.bin", local_a0);
        uVar10 = FUN_002ee9cc(param_1+0x1568, "TemplateData/island/peninsula_type.bin",  local_a0);
        uVar11 = FUN_002ee9cc(param_1+0x1658, "TemplateData/island/island_type.bin",     local_a0);
        // ... final check, returns 2 (success) or 3 (failure)
    }
} while (true);
```

## Key Findings

> [!IMPORTANT] The **18-iteration loop matches exactly** the 18 known Tortimer Island fixed layouts documented in [[research/town-generation]] (`Default, A0, C0, F0–F5, G0–G2, H0–H3, J0, L0`). This confirms [[FUN_002ca040]] is the Tortimer Island resource loader, consistent with the existing note "[[FUN_002ca040]] — loads all 18 via [[FUN_00200A94]]".

Like [[FUN_0027c56c]], this function only loads raw template bytes into resource-manager buffers — no visible write into the save buffer (`0x0095AF74`).

## Open Questions

- `local_f8` — island name/index table driving the 18-iteration loop; not read from ELF
- Where do the four final template loads (`sea_side_left/right.bin`, `island_type.bin`, `peninsula_type.bin`) get consumed after loading?

## See Also

- [[FUN_0027c56c]] — sibling/subset loader (village templates only, no island loop)
- [[FUN_00200a94]] — per-island resource loader called inside the loop
- [[research/town-generation]] — Tortimer Island layout table, broader context

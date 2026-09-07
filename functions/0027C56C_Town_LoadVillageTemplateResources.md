---
address: 0x0027c56c
name: LoadVillageTemplateResources
status: partially decompiled
tags: [town-generation, resource-loader, template]
decompiled_at: Session 24
---

# [[FUN_0027c56c|LoadVillageTemplateResources]] — Village Template Resource Loader (sea_side_left/right)

## Summary

Async resource loader: loads `Bg/Outdoor/Textures/GrassBaldness.bcres` and then kicks off async loads of `TemplateData/village/sea_side_left.bin` and `TemplateData/village/sea_side_right.bin` into scratch/object buffers. Referenced from [[research/town-generation]] as one of the loaders used during new-game town-layout setup.

```c
bool FUN_0027c56c(void) {
    // ... grass baldness texture setup, string/path building ...
    uVar9 = FUN_00754adc(DAT_0027c8a0);
    uVar10 = FUN_002ee9cc(DAT_0027c8cc, "TemplateData/village/sea_side_left.bin", uVar4);
    uVar11 = FUN_002ee9cc(DAT_0027c8f8, "TemplateData/village/sea_side_right.bin", uVar4);
    return (uVar11 & uVar9 & uVar10) != 0;
}
```

## Key Findings

> [!IMPORTANT] Loads the raw template `.bin` files into resource-manager-tracked buffers (`FUN_002ee9cc` = async file-load-into-slot pattern used throughout the resource system) but does **not** visibly write into the save buffer (`0x0095AF74`) anywhere in this function. Template data is handed off elsewhere (likely to the actual layout-assignment code, not found — see [[research/town-generation]]).

## Open Questions

- Where does the loaded template data go after this function returns? (no save-buffer write visible here)
- What is `DAT_0027c860`/`+0x128`/`+0x140` — some kind of linked-list/resource-tree object being walked (`FUN_00754aec` lookups)?

## See Also

- [[FUN_002ca040]] — sibling/superset loader (also loads island templates, has the "18 islands" loop)
- [[research/town-generation]] — broader context, TownAcres search

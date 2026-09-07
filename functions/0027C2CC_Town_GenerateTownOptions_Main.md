---
address: 0x0027C2CC
name: Town_GenerateTownOptions_Main
subsystem: Town
tags: [town, generation, rover, train, prologue, partial]
status: partial
---

# `0027C2CC_Town_GenerateTownOptions_Main` — Master Town Generation Driver

## Overview

Called during the new game prologue on the train with Rover. This function manages the entire pipeline for generating the 4 candidate town layouts presented to the player on Rover's map.

---

## Call Tree & Flow

```text
Town_GenerateTownOptions_Main (0x0027C2CC)
├── 1. Load RomFS Templates:
│   ├── "Bg/Outdoor/Textures/GrassBaldness.bcres"
│   ├── "TemplateData/village/sea_side_left.bin"  -> DAT_00ad4ce8
│   └── "TemplateData/village/sea_side_right.bin" -> DAT_00ad4dd8
├── 2. Layout Generation Loop:
│   do {
│       success = Town_GenerateTownLayout_4Options(param_1, option_index);
│   } while (success == 0);
└── 3. Cleanup & Resource Release:
    ├── Flush state transitions
    └── Unload template buffers
```

---

## Decompiled Code (Ghidra 12.1.3)

```c
void Town_GenerateTownOptions_Main(undefined4 param_1, undefined4 param_2)
{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  undefined1 auStack_18 [8];
  
  uVar2 = FUN_00304638(DAT_00974338);
  if (DAT_00ad4ba4 == 0) {
    FUN_00814518(&DAT_00ad4ba0, uVar2, 0x32000);
  }
  FUN_00814354(auStack_18, &DAT_00ad4ba0, "Bg/Outdoor/Textures/GrassBaldness.bcres", 0);
  FUN_005b0a84(&DAT_0095cf58, auStack_18);
  FUN_00754adc(&DAT_0095cf58);
  FUN_002ee92c(&DAT_00ad4ce8, "TemplateData/village/sea_side_left.bin", uVar2);
  FUN_002ee92c(&DAT_00ad4dd8, "TemplateData/village/sea_side_right.bin", uVar2);
  
  do {
    iVar3 = Town_GenerateTownLayout_4Options(param_1, param_2);
    iVar1 = DAT_00ad4cc8;
  } while (iVar3 == 0);
  
  // Cleanup resources...
  return;
}
```

---

## Key Findings

1. **Template-Based Loading:** loads `sea_side_left.bin`/`sea_side_right.bin` via RomFS before calling [[0027C8FC_Town_GenerateTownLayout_4Options]] in a retry loop — this part matches the real decompile directly.
2. **HYPOTHESIS (not confirmed by bytes):** the specific sizes (2240 bytes), the number of template variants, and the "5 houses / 1 town hall" interpretation of `Town_GenerateTownLayout_4Options`'s internal counters are not confirmed by direct byte/string reads — this is speculation. See the 2026-09-07 audit note in [[0027C8FC_Town_GenerateTownLayout_4Options]].

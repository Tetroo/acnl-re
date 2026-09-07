---
address: 0x0027C8FC
name: Town_GenerateTownLayout_4Options
subsystem: Town
tags: [town, generation, rover, layout, acres, rng, templates, partial]
status: partial
---

# `0027C8FC_Town_GenerateTownLayout_4Options` — 4 Town Layout Options Generator (Rover Sequence)

> [!WARNING] Audit 2026-09-07
> The previous version of this article contained a "Decompiled Code (Ghidra 12.1.3 - Post No-Return Fix)" block with functions `Town_CopyAcreCell`, `Town_UnpackAcreTileData`, `sead_GlobalRandom`, and variables `placed_houses`/`placed_bridges` — none of these exist in the real Ghidra output or in `symbols.csv`. It was fabricated and presented as a literal decompile. The section below has been replaced with the real `decompile_function` output; description text is kept only where the logic is directly visible in the code.

## Overview

Called during the new-game prologue (the train scene with Rover). Drives three states via `DAT_0095cf50` (0/1/2) and calls `InitTownGridAndAcres` + `Town_GenerateVillageFieldFromTemplate` (this function is confirmed — actually renamed in Ghidra, but missing from symbols.csv, needs to be added).

## Actually confirmed by the code

- State `DAT_0095cf50 == 0`: seeds the RNG from `DAT_00ad4b90[param_2]`, calls `InitTownGridAndAcres(param_1)`, writes 2 bytes at `param_1+1`/`param_1+2` (some pair of values, range 0..2 — **HYPOTHESIS**: looks like a grass-shape/fruit selection, but no text labels or enum were found in the code itself — this is pure speculation). Calls `Town_GenerateVillageFieldFromTemplate`, copies 7×6 and 5×4 grids via nested loops (bounds `<7`/`<6` and `<5`/`<4` are confirmed), and across three sets of nested loops computes some quantities (`iVar5`, `iVar14`, `iVar15`), with a final check `if (iVar14 == 1 && iVar5 == 1 && iVar15 == 1)`.
  - **HYPOTHESIS (not confirmed by bytes):** what exactly these three counters count (Town Hall / Plaza / Re-Tail, etc.) is not confirmed — no strings/constants with such names were found during decompilation. Requires tracing `FUN_006cb9c4`/`FUN_006cb9a0`/`FUN_006cb97c`.
- State `DAT_0095cf50 == 1`: a different pair of loops and a different final check `if (iVar15 == 1 && iVar5 == 5)`. **HYPOTHESIS:** the "5 villager houses + 1 bridge" interpretation is not confirmed by the code — the real semantics of the counters have not been traced.
- State `DAT_0095cf50 == 2`: handles `FUN_005b0a60`/list lookup, ending in `Core_ErrorAbortHandler`.
- On validation failure: `DAT_0095cf50 = 0`, a new seed via `FUN_002ff788(0x7fffffff)` (does look like `sead::Random`, but the name `sead_GlobalRandom` is unconfirmed — the function hasn't been renamed).

## Decompiled Code (real `decompile_function` output, 2026-09-07)

```c
undefined4 Town_GenerateTownLayout_4Options(int param_1,uint param_2)
{
  char cVar1;
  undefined1 uVar2;
  undefined1 uVar3;
  ushort uVar4;
  int iVar5;
  undefined4 *puVar6;
  uint uVar7;
  char *pcVar8;
  undefined2 *puVar9;
  undefined2 *puVar10;
  uint uVar11;
  int iVar12;
  undefined4 uVar13;
  int iVar14;
  int iVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  uint uVar18;
  uint uVar19;
  uint local_2c0;
  undefined4 local_2bc;
  ushort uStack_2b8;
  byte abStack_2b6 [6];
  undefined1 auStack_2b0 [8];
  char local_2a8;
  undefined1 auStack_2a0 [8];
  undefined1 auStack_298 [16];
  undefined1 uStack_288;
  undefined4 uStack_20c;
  undefined4 uStack_208;
  int iStack_204;
  int iStack_58;
  undefined4 uStack_4c;
  undefined4 *local_38;
  undefined1 auStack_34 [8];
  int local_2c;
  uint uStack_28;

  local_2c = param_1;
  uStack_28 = param_2;
  iVar5 = FUN_002fe85c(1);
  local_38 = &DAT_00ad4b90 + param_2;
  if (DAT_0095cf50 == 0) {
    puVar6 = &DAT_00ad4b90;
    if (param_2 < 4) {
      puVar6 = local_38;
    }
    uVar13 = *puVar6;
    FUN_0055bb14(&DAT_00ad4ec8,uVar13);
    FUN_00240594();
    FUN_00240014(1);
    FUN_002e5a50();
    InitTownGridAndAcres(local_2c);
    iVar15 = Core_ErrorAbortHandler();
    FUN_005ccc50(iVar15 + 0x4be00);
    uVar7 = FUN_0055bbb0(&DAT_00ad4ec8);
    *(char *)(local_2c + 1) = (char)((ulonglong)uVar7 * 3 >> 0x20);
    uVar7 = FUN_0055bbb0(&DAT_00ad4ec8);
    *(char *)(local_2c + 2) = (char)((ulonglong)uVar7 * 3 >> 0x20);
    pcVar8 = &DAT_0095cf54;
    if (param_2 < 4) {
      pcVar8 = &DAT_0095cf54 + param_2;
    }
    cVar1 = *pcVar8;
    if (cVar1 == '\0') {
      Town_GenerateVillageFieldFromTemplate(local_2c,&DAT_00ad4ec8);
      uVar17 = 0x9b;
      uVar16 = 0x9a;
    }
    else {
      Town_GenerateVillageFieldFromTemplate(local_2c,&DAT_00ad4ec8);
      uVar17 = 0xa5;
      uVar16 = 0xa4;
    }
    local_2c0 = (uint)(cVar1 != '\0');
    Core_ConstructObject_0027D15C(local_2c,uVar13,uVar16,uVar17);
    uVar7 = 0;
    do {
      uVar18 = 0;
      do {
        puVar9 = (undefined2 *)FUN_00317184(iVar5,uVar18,uVar7);
        if (puVar9 != (undefined2 *)0x0) {
          puVar10 = (undefined2 *)FUN_0075cbb0(local_2c,uVar18,uVar7);
          *puVar9 = *puVar10;
        }
        uVar18 = uVar18 + 1;
      } while (uVar18 < 7);
      uVar7 = uVar7 + 1;
    } while (uVar7 < 6);
    uVar7 = 0;
    do {
      uVar18 = 0;
      do {
        iVar15 = FUN_0061274c(uVar18,uVar7);
        if (iVar15 != 0) {
          uVar13 = func_0x002faee0(local_2c,uVar18,uVar7);
          iVar15 = FUN_00317184(iVar5,uVar18 + 1,uVar7 + 1);
          if (iVar15 != 0) {
            FUN_0057ac78(iVar15,uVar13,0);
          }
        }
        uVar18 = uVar18 + 1;
      } while (uVar18 < 5);
      uVar7 = uVar7 + 1;
    } while (uVar7 < 4);
    uVar13 = func_0x006127a0(local_2c);
    *(undefined4 *)(iVar5 + 0x28) = uVar13;
    FUN_006f8ec4(1);
    func_0x006a5f98();
    // ... (3 counting loops with FUN_006cb9c4/FUN_006cb9a0/FUN_006cb97c — semantics unconfirmed)
    if (iVar14 == 1 && (iVar5 == 1 && iVar15 == 1)) {
      DAT_0095cf50 = 1;
      return 0;
    }
    DAT_0095cf50 = 0;
    uVar13 = FUN_002ff788(0x7fffffff);
  }
  else {
    if (DAT_0095cf50 != 1) {
      if (DAT_0095cf50 != 2) {
        DAT_0095cf50 = 0;
        return 1;
      }
      // state 2: list lookup + Core_ErrorAbortHandler
    }
    // state 1: a different pair of counting loops, checks iVar15==1 && iVar5==5
    if (iVar15 == 1 && iVar5 == 5) {
      DAT_0095cf50 = 2;
      return 0;
    }
    DAT_0095cf50 = 0;
    uVar13 = FUN_002ff788(0x7fffffff);
  }
  puVar6 = &DAT_00ad4b90;
  if (param_2 < 4) {
    puVar6 = local_38;
  }
  *puVar6 = uVar13;
  return 0;
}
```

(Full unabbreviated version — via `mcp__ghidra__decompile_function` on `0x0027C8FC`.)

## Open Questions

- What exactly do the three groups of nested loops count before the `iVar14==1/iVar5==1/iVar15==1` and `iVar15==1/iVar5==5` checks? Needs tracing of `FUN_006cb9c4`, `FUN_006cb9a0`, `FUN_006cb97c`, `FUN_001de428`, `FUN_006e3b60`.
- `Town_GenerateVillageFieldFromTemplate` has actually been renamed in Ghidra, but is missing from `symbols.csv` — add it.
- Bytes `param_1+1`/`param_1+2` (0..2) — the grass-shape/fruit hypothesis is unconfirmed.

## See Also

- [[0027C2CC_Town_GenerateTownOptions_Main]] — the calling function (on review, the file itself broadly matches the real code, apart from the same kind of speculative claims in the "Key Findings" section — see the note there)

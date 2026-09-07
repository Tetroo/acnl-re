# Function: Insect_BsInsectFieldMgr_UpdateBugLoop (`0x002ACB7C`)

> **Address:** `0x002ACB7C`
> **Subsystem:** Item / Insects (`BsInsectFieldMgr`)
> **Status:** Partial — confirmed real function, sub-calls not fully traced
> **Note:** This is the correct function for "insect manager per-frame update." A previous audit found `functions/002ADCC8_Item_BsInsectFieldMgr_Update.md` pointed to an invalid mid-function address inside an unrelated function (`Core_BsLightPointFix_Vtbl09`); that file now redirects here. `symbols.csv` already listed this address as `done` with an accurate one-line description before this file existed — this article expands it with the real decompile `[TOOL]`.

---

## 1. Decompiled Implementation (`[TOOL]`, verbatim from `decompile_function`)

```c
/* [Item_BsInsectFieldMgr_Vtbl09]
   Virtual method slot 9 of class BsInsectFieldMgr (RTTI: 16BsInsectFieldMgr) */

undefined4 Insect_BsInsectFieldMgr_UpdateBugLoop(int param_1)
{
  int iVar1;
  undefined4 uVar2;
  byte *pbVar3;
  int iVar4;
  uint uVar5;

  iVar1 = FUN_00300994();
  if (iVar1 == 0) {
    Insect_BsInsectFieldMgr_CheckWeatherDespawn(param_1);
    FUN_002aacec(param_1);
    FUN_002ab3c0(param_1);
    FUN_002ac5e0(param_1);
  }
  else {
    iVar1 = FUN_006197e8();
    if (iVar1 == 0) {
      FUN_002ac170(param_1);
    }
    else {
      Insect_BsInsectFieldMgr_CheckWeatherDespawn();
      if (*(char *)(param_1 + 0x1206) != '\0') {
        uVar5 = 0;
        do {
          uVar2 = FUN_00625940("HIJKLMNOPQRS"[uVar5]);
          FUN_00778604(param_1 + uVar5 * 6 + 0x1370,uVar2);
          uVar5 = uVar5 + 1;
        } while (uVar5 < 0xc);
      }
      *(undefined1 *)(param_1 + 0x1206) = 0;
      FUN_002ab3c0(param_1);
      for (iVar1 = *(int *)(param_1 + 0x20); iVar1 != 0; iVar1 = *(int *)(iVar1 + 4)) {
        pbVar3 = (byte *)(param_1 + *(int *)(iVar1 + 0x14) * 6 + 0x1370);
        *pbVar3 = *pbVar3 & 0x80 | 0x53;
        iVar4 = *(int *)(iVar1 + 0x14);
        if (iVar4 < 0xc) {
          FUN_00624174("HIJKLMNOPQRS"[iVar4],param_1 + iVar4 * 6 + 0x1370,1);
        }
      }
      FUN_002ac5e0(param_1);
    }
  }
  return 2;
}
```

## 2. What's confirmed (`[TOOL]` / `[DERIVED]`)

- Two top-level branches gated by `FUN_00300994()` (likely a game-state/pause gate — not analyzed) and, in the else-branch, `FUN_006197e8()` (unanalyzed, likely "should insects be active" check).
- **`[DERIVED]`** The `"HIJKLMNOPQRS"[uVar5]`, `uVar5 < 0xc` loop confirms **12 iterations over slot letters H through S** — this matches the "12 concurrent insect slots" figure already recorded in `symbols.csv` and `systems/insects.md`.
- Slot record stride is **6 bytes** (`param_1 + uVar5 * 6 + 0x1370`) per active-slot entry in this array, base offset `+0x1370` inside the manager object.
- The reset loop clears the high bit-preserving byte to `0x53` (`& 0x80 | 0x53`) per active slot found via a linked list at `+0x20` (next-pointer at `+4` of each node).

## 3. Not yet confirmed (`HYPOTHESIS` / unanalyzed)

- `FUN_002aacec`, `FUN_002ab3c0`, `FUN_002ac5e0`, `FUN_002ac170`, `FUN_00625940`, `FUN_00778604`, `FUN_00624174` — none decompiled in this pass. Do not assume specific behavior (spawn/despawn/AI-tick roles) beyond what's structurally visible above.
- Whether this function is called once per frame or on a slower tick — not traced to a caller in this pass.
- Relationship to `0x00867724` (insect species table cited elsewhere in `systems/insects.md`) — not touched by this function directly; likely used deeper in `FUN_002aacec`/`FUN_002ac170`.

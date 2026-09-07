---
address: 0x0070d894
name: UiStringBuilderForSaveFileLabels
status: partial — corrected audit-2026-09-07, string-source claims need re-verification
tags: [ui, string, localization, save-error-dialog]
decompiled_at: Session 22c, corrected Session 25c, re-corrected audit-2026-09-07
---

# [[FUN_0070d894|UiStringBuilderForSaveFileLabels]] — UI String Builder for Save File Labels (error/confirmation dialog)

## Summary

> [!WARNING] Discarded
> "Player/Town Name String Reader" (Session 22c) — the 0x105-byte buffers are real, but they're UTF-8→UTF-16 conversion scratch space for **UI label text**, not save-data fields. This function does not read `garden_plus.dat` field data.

> [!WARNING] Correction (audit 2026-09-07): previous "Full disassembly" block did not match live Ghidra output
> The call chain described below (`FUN_0012eeb4` → `FUN_00100cfc`/`FUN_00100d0e` → `FUN_002f7898`, UTF-8→UTF-16 conversion) is confirmed present in the real decompile. But the specific claim "string source #2/#3 resolve to the literal `/garden.dat` via `get_xrefs_to(0x00930474)`" does **not** appear anywhere in the live decompile and could not be re-derived as shown — no vtable-`+8` string lookup exists in the real code. Re-decompiled and corrected below.

## Decompiled Implementation (verbatim from Ghidra, 2026-09-07, abridged to the relevant branch)

```c
if (param_3 == 0x800) {
  pcVar9 = "data:/updated.dat";
  /* ... UTF-8 -> UTF-16 conversion loop using pcVar9 ... */

  piVar2 = (int *)FUN_00810a50(auStack_bc, "data:%s", DAT_0095af6c);
  (**(code **)(*piVar2 + 8))(piVar2);
  iVar10 = piVar2[1];
  /* ... same UTF-8 -> UTF-16 loop using iVar10 as the source string ... */

  piVar2 = (int *)FUN_00810a50(auStack_bc, "data:%s", DAT_0095b344);
  (**(code **)(*piVar2 + 8))(piVar2);
  iVar10 = piVar2[1];
  /* ... same loop a third time ... */
}
```

**Corrected sources of the 3 strings:**
1. Literal `"data:/updated.dat"` — a hardcoded string, not a lookup.
2. `FUN_00810a50(scratch, "data:%s", DAT_0095af6c)` — this is an `sprintf`-style formatter (`FUN_00810a50` builds a formatted string object), not a vtable string-table lookup as previously claimed. `DAT_0095af6c` is the same address independently documented in [[FUN_005c9d64]] as the **`/garden.dat`** (base-game save) path pointer — so this likely produces `"data:/garden.dat"`, but that inference has NOT been re-confirmed by reading the actual bytes at `DAT_0095af6c` in this audit pass.
3. `FUN_00810a50(scratch, "data:%s", DAT_0095b344)` — same pattern with a different global. `DAT_0095b344` also appears in `Save_DispatchSaveLoadStateMachine` (state `0x10`) as a path used for a 4-byte read; its exact file identity is unconfirmed.

## Open Questions

- What UI screen uses this function? (likely save-corruption / format-confirmation dialog)
- Confirm the actual byte contents at `DAT_0095af6c` and `DAT_0095b344` (read them directly from the ELF rather than inferring from variable names/prior docs).
- The previous claim of "`get_xrefs_to` returns exactly one reference to `/garden.dat`" should be treated as unverified until redone against the corrected understanding above.

## See Also

- [[FUN_00300fdc]] — save buffer getter (with pre-check); called earlier in this function for an unrelated early-exit branch, not part of the string-building logic
- [[FUN_005c9dd4]] — save buffer constructor
- `research/town-generation.md` — TownAcres search status, `/garden.dat` lead now marked low-priority

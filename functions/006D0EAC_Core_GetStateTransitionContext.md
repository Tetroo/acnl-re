---
name: GetStateTransitionContext
tags: [function, state-system, getter]
status: partial — corrected audit-2026-09-07, caller/layout claims removed as unverified
address: "0x006D0EAC"
---

# FUN_006D0EAC — State Transition Context Getter

## Summary

Simple one-liner getter.

> [!WARNING] Correction (audit 2026-09-07): whole "Returned Object Layout" / caller-logic section was unverifiable and did not match live decompile
> The previous version of this doc claimed the function does `*(undefined4*)(DAT_006d0eb8 + 0x30)` (a pointer-field load) and asserted a detailed layout for the returned object (+0x40 transition flag, +0x1F4 sub-object pointer) plus a specific "Caller: FUN_002F72BC — Transition Logic" narrative with global names `DAT_002F7428`/`DAT_002F67E8`. Live re-decompile shows this function is actually just:
> ```c
> undefined4 Core_GetStateTransitionContext(void)
> {
>   return DAT_0094ed3c;
> }
> ```
> — a direct global variable read, not a pointer-plus-offset load. There is **no evidence in this function** for the claimed object layout or for the `FUN_002F72BC` caller narrative; that function is not in this project's `functions/` set and was not independently checked. Everything below the code block down to "Open Questions" in the old version is unconfirmed speculation and has been removed. If this context/transition mechanism matters for the PC port, it needs to be re-derived from scratch: decompile `FUN_002F72BC` directly and trace `DAT_0094ed3c`'s writers via xrefs.

## Open Questions

- What writes to `DAT_0094ed3c`?
- Who calls `Core_GetStateTransitionContext` and what do they do with the returned value? (xrefs not checked in this audit)

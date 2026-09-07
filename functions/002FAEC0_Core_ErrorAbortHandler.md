---
address: 0x002faec0
name: ErrorAbortHandler
status: fully understood
tags: [error-handling, abort, closed-candidate]
decompiled_at: Session 24
---

# [[FUN_002faec0|ErrorAbortHandler]] — Error/Abort Handler (NOT a buffer accessor)

## Summary

```c
int FUN_002faec0(void) {
    FUN_00606c24();
    FUN_006064bc();
    return *DAT_002faedc + 0x80;
}
```

> [!WARNING] Discarded hypothesis
> Initially looked like a "save buffer + 0x80" accessor (candidate for TownAcres reader). **Rejected**: every call site invokes it in a `/* WARNING: Subroutine does not return */` position — i.e. Ghidra treats it as **noreturn**, called only on invalid/unreachable state branches (e.g. [[FUN_006f1938]] default case, [[FUN_006f8c38]] out-of-range state index, [[FUN_0028de1c]] case 1 unexpected path). This is an **assert/abort handler**, not a data reader. `DAT_002faedc + 0x80` is likely building an error/crash context pointer, not a save field.

## Key Findings

> [!IMPORTANT] Closes the "buffer accessors @ 0x0095AF74" TODO candidate — not related to save data at all.

## See Also

- [[FUN_0071ee44]] — another error abort wrapper (same pattern, closed earlier)

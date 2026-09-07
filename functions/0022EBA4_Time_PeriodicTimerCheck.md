---
address: 0x0022eba4
name: PeriodicTimerCheck
status: flagged — address does not resolve
tags: [save-system, timer, tick]
decompiled_at: Session 23
---

> [!WARNING] FLAGGED (audit 2026-09-07, second pass)
> `get_function_by_address(0x0022eba4)` → "No function found". The nearest known function is `FUN_0022eaec` (body `0x0022eaec`–`0x0022eb93`); after it, up to `0x0022ebc0`, there is no defined function in Ghidra at all (unanalyzed/unmarked region). Either the address shifted during re-analysis, the function was merged/re-marked, or this was never a real function entry point to begin with. Requires manual disassembly of the range `0x0022eb94`–`0x0022ebff` in Ghidra (`disassemble_bytes`/`create_function`) before trusting the content of the article below.

# [[FUN_0022eba4|PeriodicTimerCheck]] — Periodic Timer Check (save buffer incidental)

## Summary

Timer/tick-based check function. Uses SVC 0x28 (GetSystemTick) and divides by 0x1a/0x5a constants. Calls [[FUN_00300fdc]] only in the else-branch (result unused).

```c
void FUN_0022eba4(int param_1) {
    if (FUN_00625bf0() == 0) {
        // SVC 0x28 — GetSystemTick
        // timer arithmetic: delta vs param_1+0x50/+0x54
        // divide by 0x1a (26) with threshold 0x5a (90)
        if (condition_met) {
            // accesses *DAT_0022ec58 + 0x1326a (save buffer field?)
            if (*(char*)(*DAT_0022ec58 + 0x1326a) == 0)
                *(char*)(*DAT_0022ec58 + 0x1326a) = 2;
        }
    } else {
        FUN_00300fdc();   // result discarded
        FUN_005c9d54();
        FUN_00625cec();
        *(param_1+0x44)++;
    }
}
```

## Notes

- `0x5a = 90`, `0x1a = 26` — likely seconds or frames threshold
- `*DAT_0022ec58 + 0x1326a` — field at offset 0x1326A (= 78,442) in some large object (possibly save buffer, within 563,968-byte range)
- `FUN_00625bf0` — unknown gate/check function

## Open Questions

- What is `DAT_0022ec58`? Is it the save buffer ptr or a different object?
- What does field at offset 0x1326a represent?

> [!WARNING] Discarded: direct TownAcres reader — [[FUN_00300fdc]] return value is discarded in else branch.

## See Also

- [[FUN_00300fdc]] — save buffer getter
- [[FUN_005c9d54]] — another save accessor (not yet documented)

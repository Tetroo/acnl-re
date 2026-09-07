---
address: 0x002cd508
name: ReadExhibitionFile
status: fully understood
tags: [save-system, exhibition, loader]
decompiled_at: Session 22
---

# [[FUN_002cd508|ReadExhibitionFile]] — exhibition.dat Reader

## Summary

Thin wrapper. Reads `/exhibition.dat` into the buffer pointed to by `*param_1`.

```c
void FUN_002cd508(undefined4 *param_1)
{
    FUN_001b8b9c(DAT_002cd520,   // path → "/exhibition.dat" @ 0x0085DBBE
                 0,
                 *param_1,        // destination buffer
                 DAT_002cd51c);  // size = 0x17BE10 = 1,555,984 bytes
}
```

## Confirmed Constants

| Symbol | Value | Meaning |
|--------|-------|---------|
| `DAT_002cd520` | `0x0085DBBE` | String `"/exhibition.dat"` in .rodata |
| `DAT_002cd51c` | `0x0017BE10` | File size = **1,555,984 bytes** (~1.5 MB) |

## Callers

- [[FUN_001d3d3c]] state `0x04` — called as `FUN_002cd508(DAT_001d43d0)` where `DAT_001d43d0 = 0x009B2AC0` (buffer ptr in .bss)

> [!IMPORTANT]
> Ghidra marks return type as `void`, but caller in [[FUN_001d3d3c]] uses the return value (reads from R0 after call). The actual return is the result of [[FUN_001b8b9c]] propagated through R0.

## See Also

- [[FUN_001b8b9c]] — raw file reader
- [[FUN_001d3d3c]] — state machine dispatcher (state 0x04)

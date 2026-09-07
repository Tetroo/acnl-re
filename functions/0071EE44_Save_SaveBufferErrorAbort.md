---
address: 0x0071ee44
name: SaveBufferErrorAbort
status: fully understood
tags: [save-system, error, abort]
decompiled_at: Session 23
---

# [[FUN_0071ee44|SaveBufferErrorAbort]] — Save Buffer Error Abort

## Summary

Calls [[FUN_00300fdc]] (result unused) then immediately calls the non-returning [[FUN_002faec0]]. This is an error/abort path, not a data reader.

```c
void FUN_0071ee44(void) {
    FUN_00300fdc();    // result discarded
    FUN_002faec0();    // non-returning
}
```

> [!WARNING] Discarded: TownAcres/Villager reader hypothesis — [[FUN_00300fdc]] return value is discarded; [[FUN_002faec0]] is non-returning.

## See Also

- [[FUN_00300fdc]] — save buffer getter (called but result unused)
- [[FUN_002faec0]] — non-returning (abort/error handler)

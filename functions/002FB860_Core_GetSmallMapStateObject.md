---
address: 0x002fb860
name: GetSmallMapStateObject
status: fully understood
tags: [save-system, accessor, map-state]
decompiled_at: Session 24
---

# [[FUN_002fb860|GetSmallMapStateObject]] — Small Map State Object Getter

## Summary

```c
int FUN_002fb860(void) {
    FUN_00606c24();
    FUN_006064bc();          // pre-check, same as FUN_00300fdc pattern
    return *DAT_002fb880 + 0x71900;
}
```

Returns pointer to `buffer + 0x71900` — the already-documented small map state object ([[FUN_001e0340]], ~36 bytes). This is simply its dedicated getter. Not TownAcres (offset is well past the raw data block, inside the C++-object region).

## See Also

- [[FUN_001e0340]] — small map state object ctor at this offset
- [[FUN_00300fdc]] — sibling save buffer getter (same pre-check pattern via [[FUN_006064bc]])

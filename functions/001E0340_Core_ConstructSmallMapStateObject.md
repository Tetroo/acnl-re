---
address: 0x001e0340
name: ConstructSmallMapStateObject
status: fully understood
tags: [save-system, struct-init, map]
decompiled_at: Session 23
---

# [[FUN_001e0340|ConstructSmallMapStateObject]] — Small Map State Object Constructor

## Summary

Minimal constructor — zeroes 3 uint32 fields and 1 uint16 field at offset +0x22, then calls [[FUN_0027a490]].

```c
undefined4* FUN_001e0340(undefined4* param_1) {
    param_1[0] = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    *(uint16*)(param_1 + 0x22) = 0;
    param_1[1] = 0;   // redundant zero
    param_1[2] = 0;
    FUN_0027a490();
    *(uint16*)(param_1 + 0x22) = 0;
    return param_1;
}
```

Struct size: at least 0x24 bytes (36 bytes).

## Notes

- Called from [[FUN_005c9dd4]] for sub-object at buffer+`0x71900`
- This object is **not** TownAcres — too small (36 bytes vs 84 bytes for 7×6 uint16)
- Likely a small state/flag object for the map subsystem
- `FUN_0027a490` — purpose unknown, no params passed

## See Also

- [[FUN_005c9dd4]] — save buffer constructor; places this at buffer+0x71900

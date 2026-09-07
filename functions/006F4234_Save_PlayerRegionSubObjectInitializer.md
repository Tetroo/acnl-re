---
address: 0x006f4234
name: PlayerRegionSubObjectInitializer
status: fully understood
tags: [save-system, player, initializer]
decompiled_at: Session 22
---

# [[FUN_006f4234|PlayerRegionSubObjectInitializer]] — Player Region Sub-Object Initializer

## Summary

Initialises the array of 10+1 sub-structs within a player's save region. Called once per player in [[FUN_005c9dd4]]'s 4-player loop.

```c
void FUN_006f4234(int param_1) {  // param_1 = player base ptr
    for (uint i = 0; i < 10; i++)
        FUN_002ff304(param_1 + i * 0x280);   // init 10 × 640-byte structs
    FUN_002ff304(param_1 + 0x1900);           // 11th struct at +0x1900
    FUN_00303d80(param_1 + 0x1b80);           // different type at +0x1b80
}
```

## Layout Within Player Region

| Byte offset (rel. to player base) | Size | Description |
|-----------------------------------|------|-------------|
| `+0x0000` | 0x280 | Sub-struct 0 (`FUN_002ff304`) |
| `+0x0280` | 0x280 | Sub-struct 1 |
| `+0x0500` | 0x280 | Sub-struct 2 |
| `...` | | |
| `+0x1680` | 0x280 | Sub-struct 9 |
| `+0x1900` | 0x280 | Sub-struct 10 (extra, same type) |
| `+0x1b80` | ? | Different type (`FUN_00303d80`) |

Total for first 11 structs: `11 × 0x280 = 0x1B80 = 7,040 bytes`

## Open Questions

- What are these 10+1 structs? Candidates: villager relationship data, house items, storage slots
- `FUN_00303d80` handles a different type at +0x1b80 — needs decompile

## See Also

- [[FUN_005c9dd4]] — calls this in 4-player loop at `piVar5 + 0x1ce56 + i * 0x6E2`
- `FUN_002ff304` — initialises one 640-byte sub-struct
- `FUN_00303d80` — init for different type at +0x1b80

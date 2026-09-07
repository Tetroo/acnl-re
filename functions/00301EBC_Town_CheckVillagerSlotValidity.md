---
address: 0x00301ebc
name: CheckVillagerSlotValidity
status: fully understood
tags: [save-system, villager, validation]
decompiled_at: Session 24
---

# [[FUN_00301ebc|CheckVillagerSlotValidity]] — Villager Slot Validity Gate

## Summary

Called from [[FUN_005c9a24]] (villager slot reader) as the per-slot gate that decides whether a villager slot is occupied/valid.

```c
undefined4 FUN_00301ebc(int param_1)  // param_1 = villager slot pointer
{
    uint species = *(ushort *)(param_1 + 0x2c);
    bool valid = species < DAT_00301ef4;   // species < max species count
    if (valid) {
        uint personality = *(byte *)(param_1 + 0x2e);
    }
    if (valid && personality < 8 && FUN_0076b6ec(param_1 + 0x16) != 0) {
        return 1;
    }
    return 0;
}
```

## Key Findings

> [!IMPORTANT] Slot is considered valid only if **all three** hold:
> - `slot+0x2C` (species id) `< DAT_00301ef4` (species table size / max species count)
> - `slot+0x2E` (byte) `< 8` — likely **personality type** (ACNL has 8 personality types)
> - `FUN_0076b6ec(slot+0x16)` returns non-zero — sub-struct validity check at offset `+0x16` (16 bytes before species field — possibly a name string or ID block)

## Open Questions

- `DAT_00301ef4` exact value (species table size) — not yet read from ELF
- What does `FUN_0076b6ec` validate at `slot+0x16`? (name string length? checksum?)

## See Also

- [[FUN_005c9a24]] — villager slot reader (caller)

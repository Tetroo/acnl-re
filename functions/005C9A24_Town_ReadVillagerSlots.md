---
address: 0x005c9a24
name: ReadVillagerSlots
status: partially decompiled
tags: [save-system, villager, slots, reader]
decompiled_at: Session 23
---

# [[FUN_005c9a24|ReadVillagerSlots]] — Villager Slot Reader (10 slots)

## Summary

Iterates over all 10 villager slots in the save buffer. For each slot, reads a uint16 field at offset +0x2c and writes it to a runtime output array. Also sets bit flags in a bitfield for valid/loaded villagers.

```c
void FUN_005c9a24(void) {
    int buf = *DAT_005c9a38;   // save buffer base
    *(uint16*)(DAT_001192f8 + 4) = 0;   // clear bitfield
    FUN_003091e0();
    int iVar2 = FUN_003091ec();
    uint16* out = (uint16*)DAT_00119300;  // output array
    int iVar3 = iVar2 + DAT_001192fc;

    for (uint i = 0; i < 10; i++) {
        out[i] = 0xFFFF;   // init to "empty"
        int slot = i * DAT_00119304 + buf + 0x292a4;   // villager struct ptr
        
        int iVar5 = FUN_00301ebc();
        if (iVar5 != 0) {
            out[i] = *(uint16*)(slot + 0x2c);   // read species/ID field
            if (iVar2 != 0) {
                int iVar4 = FUN_00309dcc(slot, iVar3);
                if (iVar4 != 0 && *(char*)(iVar4 + 0xbc) < 0) {
                    *(uint16*)(DAT_001192f8 + 4) |= (uint16)(1 << i);  // set bit i
                }
            }
        }
    }
}
```

## Key Findings

> [!IMPORTANT] **Villager array confirmed:**
> - Start offset in save buffer: `buffer + 0x292A4` (= 168,612 bytes from file start)
> - Slot count: **10** (indices 0–9)
> - Stride per slot: `DAT_00119304` bytes (needs ELF read)
> - Field at slot+0x2c: uint16 → likely **villager species ID**

## Runtime Globals

| Address | Role |
|---------|------|
| `DAT_005c9a38` | Pointer that holds save buffer base addr |
| `DAT_00119300` | Output array: 10 uint16 species IDs |
| `DAT_001192f8 + 4` | Bitfield: bit i = villager slot i is valid |
| `DAT_001192fc` | Date/context offset (passed to FUN_00309dcc) |
| `DAT_00119304` | Villager struct stride (bytes per slot) |

## Confirmed Constants (from ELF)

| DAT address | Value | Meaning |
|-------------|-------|---------|
| `DAT_00119304` | `0x2518` (9,496) | **Villager struct stride** |
| `DAT_005c9a38` | `0x0095AF74` | Ptr-to-ptr: holds save buffer pointer addr |
| `DAT_00119300` | `0x0095B230` | Output array (10 × uint16) runtime addr |
| `DAT_001192f8` | `0x0095B218` | Bitfield base; valid villager bits at +4 |
| `DAT_001192fc` | `0x55A6` (21,926) | Date/context constant |

> [!IMPORTANT] **Villager layout confirmed from ELF:**
> - Array start: `buffer + 0x292A4` = file offset 168,612
> - Stride: `0x2518` bytes per slot
> - Total: `10 × 0x2518 = 0x18EF0` = 94,960 bytes
> - Array end: `buffer + 0x292A4 + 0x18EF0 = buffer + 0x42194`
> - Species field: `slot + 0x2C` (uint16, 0xFFFF = empty)

## Open Questions

- ~~What does `FUN_00301ebc()` check?~~ **RESOLVED** — see [[FUN_00301ebc]]: slot valid iff species < max species count AND personality byte (+0x2E) < 8 AND `FUN_0076b6ec(slot+0x16)` sub-check passes.
- What does `FUN_00309dcc(slot, 0x55A6)` do? (date-context check)
- Is `slot + 0x2C` the villager species ID or internal ID?
- What is `*(char*)(iVar4 + 0xbc) < 0`? Signed byte flag

## See Also

- [[FUN_005c9dd4]] — save buffer constructor
- [[FUN_00300fdc]] — save buffer getter
- [[FUN_005c9760]] — sibling buffer accessor (lazy resource loader, unrelated to villagers)
- [[FUN_00301ebc]] — villager slot validity gate (fully understood)

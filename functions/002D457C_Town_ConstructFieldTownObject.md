---
address: 0x002d457c
name: ConstructFieldTownObject
status: partially decompiled
tags: [save-system, constructor, town, acres, field, buffer]
decompiled_at: Session 26
---

# [[FUN_002d457c|ConstructFieldTownObject]] — Town/Field Data Object Constructor (contains TownAcres)

## Summary

C++ placement constructor for a POD (no vtable) sub-object inside the [[FUN_005ca4f4]] save-buffer construction chain. Called as the 4th of six top-level sub-constructors, with `param_1 = save_buffer_base + 0x53430` (derived by exact arithmetic — see Evidence below).

> [!IMPORTANT] Confirmed exact match with community save-editor offset
> `param_1 + 0x54` = `save_buffer_base + 0x53484`, which is byte-exact identical to `marcrobledo/acnl-editor`'s `OffsetsPlus.MAP_ACRES` constant (`0x053484`) — the documented base address of `TownAcres[42]` (7×6 grid of `u16` AcreIDs, 84 bytes total). See `research/townacres-search-strategy.md` for the community source and cross-validation against [[FUN_005c9a24]]'s villager offset.

## Decompiled Body

```c
int FUN_002d457c(int param_1)
{
  int iVar1;

  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 8) = 0;
  iVar1 = DAT_002d45d8;             // = 0x7EC
  *(undefined4 *)(param_1 + 0x1c) = 0;
  *(undefined4 *)(param_1 + 0xc) = 0;
  *(undefined4 *)(param_1 + 0x10) = 0;
  *(undefined4 *)(param_1 + 0x14) = 0;
  *(undefined4 *)(param_1 + 0x18) = 0;
  *(undefined2 *)(iVar1 + param_1) = 0;       // +0x7EC
  *(undefined2 *)(iVar1 + 2 + param_1) = 0;   // +0x7EE
  *(undefined2 *)(param_1 + 0x7f0) = 0;       // +0x7F0
  *(undefined2 *)(iVar1 + 6 + param_1) = 0;   // +0x7F2
  FUN_002f8280(param_1 + 0x6c, 0x780);        // zero-fill [0x6c, 0x7EC)
  return param_1;                              // returns `this` unchanged
}
```

`DAT_002d45d8 = 0x000007EC` (resolved via `list_data_items`).

## Struct Layout (relative to param_1)

| Range | Content |
|-------|---------|
| `[0x0, 0x4)` | Untouched by this ctor (possibly implicit/inherited from outer zero-fill) |
| `[0x4, 0x20)` | Seven zeroed `u32` fields (0x4,0x8,0xc,0x10,0x14,0x18,0x1c) |
| **`[0x20, 0x6c)`** | **Untouched 76-byte gap — raw POD zone.** `TownAcres[42]` (84 bytes, `+0x54`–`+0xA8`) starts inside this gap and spills ~8 bytes into the following memset region (no conflict — both are zero at construction time). |
| `[0x6c, 0x7ec)` | Zero-filled via `FUN_002f8280(param_1+0x6c, 0x780)` (1920 bytes) |
| `0x7ec`, `0x7ee` | `u16` = 0 |
| `0x7f0` | `u16` = 0 |
| `0x7f2` | `u16` = 0 |
| slot end | `0x850` (reserved gap before next sub-object [[FUN_006127ac]] begins) — ~92 bytes beyond `0x7f2` unaccounted for by this ctor, likely other raw fields |

No vtable write at `+0x0` — this is a plain data struct, not an RTTI-tracked polymorphic class. This is consistent with (and explains) why all prior RTTI-based searches (`isfactory::VillageBuilder`, `dFieldFactory.cpp`/`fgobj`) never surfaced anything resembling TownAcres: those classes govern *dynamic placed objects* (holes/trees/presents), not this static save-grid struct.

## Evidence: Exact Offset Derivation

Traced from [[FUN_005ca4f4]] entry (`param_1_outer + 0x20`) through four sub-constructor calls. Key finding: [[FUN_00301d9c]] (the array-construct helper) and all six top-level sub-constructors in this chain (including this one) follow the **C++ ABI "constructor returns `this`" convention** — return value equals the exact input pointer, unchanged. This means 100% of the running offset accumulation in [[FUN_005ca4f4]] comes from explicit hex literals in that function, not from any callee's internal size.

```
iVar1 = 0x20                                    (param_1_outer + 0x20)
+ FUN_00112584(iVar1+0x29200) → dest=0x29220, returns unchanged → iVar1=0x29220
+ FUN_005ccdc8(iVar1+0x22be0) → dest=0x4c000,  returns unchanged → iVar1=0x4c000
+ FUN_002bf578(iVar1+0x44bc)  → dest=0x504bc,  returns unchanged → iVar1=0x504bc
+ FUN_002d457c(iVar1+0x28f4)  → dest=0x533b0,  returns unchanged → iVar1=0x533b0  ← this object
```

`0x533b0` is this object's `param_1`, relative to [[FUN_005ca4f4]]'s own `param_1`. Since [[FUN_005ca4f4]] is called with `param_1 = save_buffer_base + 0x80` (confirmed in [[FUN_005c9dd4]]):

```
object_base_absolute = save_buffer_base + 0x80 + 0x533b0 = save_buffer_base + 0x53430
target = object_base_absolute + 0x54 = save_buffer_base + 0x53484   ← exact match to MAP_ACRES
```

Full DAT_ constant resolution table and step-by-step trace: `meta/claude_note/claude-working-notes24.md`.

## Open Questions

- No reader/writer function for `TownAcres` has been located yet — only the caller chain (`FUN_005c9dd4` → [[FUN_005ca4f4]] → this) is known, and that's the *constructor* (zero-init at buffer allocation), not runtime game-logic access.
- What are the ~92 unaccounted bytes at the end of this object's `0x850`-byte slot (`0x7f2`–`0x850`)?
- Is `ISLAND_ACRES` (`buffer+0x06feb8`, per the same community offset table) inside one of the later sub-objects ([[FUN_006127ac]] or [[FUN_002ebdf4]])? Not yet checked.

## See Also

- [[FUN_005ca4f4]] — parent constructor, full chain
- [[FUN_00301d9c]] — array-construct helper (confirmed: returns `param_1` unchanged)
- [[FUN_005c9dd4]] — save buffer constructor, establishes `param_1 = buffer+0x80` convention
- [[FUN_005c9a24]] — villager reader, source of the cross-validation offset
- `research/townacres-search-strategy.md` — community offset source + cross-validation

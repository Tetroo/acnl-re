---
name: GetStateSubObjectFlag4000
tags: [function, state-system, getter]
status: fully understood
address: "0x006F7C54"
---

# FUN_006F7C54 — State Sub-Object Getter (flag 0x4000)

## Summary

Returns a state-specific sub-object if bit 0x4000 is set for the given state index.
Used in [[FUN_005B2FE0]] to handle weather type 0x15 (special case).

```c
int FUN_006f7c54(undefined4 param_1)  // param_1 = state_index
{
    iVar1 = FUN_006f8668();
    if (iVar1 - 2U < 0x18) FUN_002faec0(); // assert: panic if phase in [2,25]
    iVar1 = FUN_002f7574(0x4000, param_1); // check bit 0x4000 for this state
    if (iVar1 != 0) {
        FUN_005b38fc();
        iVar1 = FUN_006cab50();
        if (iVar1 != 0) return iVar1 + 0x10;
    }
    return 0;
}
```

> [!IMPORTANT]
> Returns `FUN_006cab50() + 0x10` if state has flag 0x4000 set, otherwise 0.
> The returned pointer is passed to [[FUN_0071F3EC]] for level checks.

## Known Fields

The returned object (base = FUN_006cab50() + 0x10) is used as the first arg to
FUN_0071F3EC, which reads: `*(byte*)(obj + param_2 * 0x302 + 0x36)`.
So the object starts an array of 0x302-byte structs; field 0x36 = level byte.

## References

- Called from: [[FUN_005B2FE0]] (state→weather type mapper, type 0x15 branch)
- Uses: [[FUN_002F7574]] (bit-flag checker)
- FUN_006f8668: phase getter (asserts phase NOT in [2,25]) — not yet documented
- FUN_005b38fc: unknown side-effect — not yet documented
- FUN_006cab50: main object getter — not yet documented

## Open Questions

- What is FUN_006cab50 — what singleton does it return?
- What is FUN_006f8668 — what phase does it check?
- What does bit 0x4000 signify in the state flag table?

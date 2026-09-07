---
address: 0x002F72BC
name: HandleStateTransition
tags: [function, state, transition]
status: understood — structure and globals fully mapped; vtable semantics inferred
---

# [[FUN_002F72BC|HandleStateTransition]] — State Transition Handler

## Signature
```c
void FUN_002F72BC(int *state_obj);
```

## Purpose

Handles state transitions for a game object. Checks active state via vtable calls,
updates state tracking fields at offsets `+0x1FC` / `+0x1FD`, and manages a linked
list of child objects. Rich vtable interface — this is a real, substantial game class.

## Decompiled (annotated)
```c
void FUN_002F72BC(int *state_obj) {
    // Check if state is "done" via vtable
    int done = (*vtable[+0x28])(state_obj);
    if (done == 0) done = (*vtable[+0x18])(state_obj);
    if (done == 0) {
        (*vtable[+0x24])(state_obj);  // some tick/update

        if ((state_obj[0x35] & 1) == 0) {
            // Read current state index from global object
            int ctx      = FUN_006D0EAC();
            uint cur_idx = *(DAT_002F7428 + 0x1FC);  // current state byte
            uint new_idx = state_obj[0x34] & 0x6;    // flags from object

            if (new_idx != 0) {
                if (*(ctx + 0x40) == 0) {
                    // Transition pending — check conditions, write new state
                    *(DAT_002F7428 + 0x1FC) = state_obj[0x33] - 1;
                    *(ctx + 0x40) = 1;
                } else {
                    // State already transitioning — advance or complete
                    if (cur_idx == state_obj[0x33] - 1) {
                        // Transition complete — write to DAT_002F67E8
                        *(DAT_002F67E8 + 0x1FC) = state_obj[0x33];
                        // ... wind/float params if flag 0x2 set
                        *(DAT_002F67E8 + 0x1FD) = *(state_obj + 0xD2);
                        // Walk linked list of child objects → activate next
                    }
                }
            }
        }
    }
}
```

## Key Globals

| Address | Role |
|---------|------|
| `DAT_002F7428` | "Pending" state object — written during transition setup |
| `DAT_002F67E8` | "Active" state object — written when transition completes |
| `FUN_006D0EAC` | Unknown getter — returns a context object with flag at `+0x40` |

## Vtable Methods Used (inferred from context)

| Offset | Role |
|--------|-------------------|
| `+0x18` | IsComplete / IsDone (inferred) |
| `+0x1C` | OnTransitionIn |
| `+0x20` | OnTransitionOut |
| `+0x24` | Tick / Update |
| `+0x28` | IsActive / IsEnabled |

## Open Questions

- [[FUN_006D0EAC]] — context getter, returns object with flag at `+0x40`; not yet decompiled
- Relationship between `DAT_002F7428` / `DAT_002F67E8` and `DAT_002F7568` — likely same state system, exact connection unclear

## See Also

[[FUN_002F755C]], [[FUN_002F7574]], [[FUN_006D0EAC]]

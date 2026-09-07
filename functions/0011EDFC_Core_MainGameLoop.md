---
address: 0x0011EDFC
name: MainGameLoop
tags: [function, main-loop, critical]
status: understood — structure and phases fully mapped; per-frame callees mostly unknown
---

# [[FUN_0011EDFC|MainGameLoop]] — Main Game Loop

## Signature
```c
uint FUN_0011edfc(char *app_state, int param_2);
```

## Purpose

The main game loop / application lifecycle function. Manages three initialization phases
(tracked via bytes in `app_state`), then runs the per-frame update cycle.
Contains direct 3DS SVC syscalls.

## Structure (three phases)

```c
// Phase 0: first-time init (app_state[0] == 0)
if (app_state[0] == 0) {
    SVC #0x28;                    // WaitSynchronization
    // ... init fields
    app_state[0] = 1;
    FUN_00135764 / FUN_00137EA8;  // unknown init calls
}

// Phase 1: wait for subsystem ready (app_state[1] == 0)  
if (app_state[1] == 0) {
    while (!FUN_00120CB8(app_state)) {
        thunk_FUN_00127D4C(DAT_0011EEC8, 0);  // sleep/yield
    }
}

// Phase 2: per-frame update (app_state[2] == 0 first time, then stays 1)
if (app_state[2] == 0) {
    // --- MAIN UPDATE ---
    puVar8 = *DAT_00120CA0;           // root game object
    if (puVar8 != NULL) {
        FUN_00100684(puVar8);          // state manager tick
        (*vtable[0])(puVar8);          // root object Update()
    }

    // Global update calls
    FUN_001262C4(); FUN_00126284();
    FUN_00122F94(); FUN_00123044();
    FUN_00122FF8(); FUN_0012354C();
    FUN_00123504(); FUN_001231B0();
    FUN_0012D4D0(); FUN_00313A64();
    FUN_00100608();

    // ... timing, sleep, SVC #0x3 (ExitThread / SleepThread)
}
```

## Critical Findings

### Root Game Object
`DAT_00120CA0` holds a pointer to the root game object. This object is passed to:
1. `FUN_00100684` — state manager tick (checks/advances state machine)
2. `(*vtable[0])(obj)` — the object's primary `Update()` virtual method

**This is the most important global pointer found so far.**
Finding who initializes `DAT_00120CA0` will likely reveal who sets `DAT_002F7568`
(the global state object used by weather and other systems).

### SVC Calls
| SVC | Hex | Likely meaning |
|-----|-----|----------------|
| `software_interrupt(0x28)` | SVC #0x28 | `WaitSynchronization` |
| `software_interrupt(0x3)` | SVC #0x3 | `SleepThread` or `ExitThread` |

## Open Questions

- Who calls `FUN_0011EDFC`? (need xref — likely `main()` or OS callback)
- What initializes `DAT_00120CA0`?
- What are the many unnamed global update calls (`FUN_001262C4` etc.)?

## See Also

[[FUN_00100684]], [[FUN_002F755C]], [[memory-map]]

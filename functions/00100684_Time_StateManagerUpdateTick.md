---
address: 0x00100684
name: StateManagerUpdateTick
tags: [function, state, update-tick]
status: understood — structure clear, subfunction details partial
---

# [[FUN_00100684|StateManagerUpdateTick]] — State Manager Update Tick

## Signature
```c
undefined4 FUN_00100684(int manager_obj);
```

## Purpose

Per-frame update tick for a state manager object. Flushes pending state transitions
across multiple subsystems, then checks the global state object for a condition (bit `0x40`).
Always returns `2`.

## Decompiled (annotated)
```c
undefined4 FUN_00100684(int manager_obj) {
    // Flush subsystem A (always)
    FUN_002F744C(manager_obj + DAT_0010075C);  // flush pending transition
    FUN_002F7510(manager_obj + DAT_00100764);  // release vtable object
    FUN_0012D418(manager_obj);                 // unknown — acts on whole manager

    // Flush subsystem B (if active)
    if (*(manager_obj + DAT_00100768 + 0x28) != 0) {
        FUN_002F744C(manager_obj + DAT_0010076C);
        FUN_002F7510(manager_obj + DAT_00100770);
    }

    // Flush subsystem C (if active)
    if (*(manager_obj + DAT_00100774 + 0x38) != 0) {
        FUN_002F744C(manager_obj + DAT_00100778);
        FUN_002F7510(manager_obj + DAT_0010077C);
    }

    // Flush subsystem D (if active, offset 0xEA08)
    if (*(manager_obj + 0xEA08) != 0) {
        FUN_002F744C(manager_obj + DAT_00100780);
        FUN_002F7510(manager_obj + DAT_00100784);
    }

    // Check dirty flags and process pending updates
    if (FUN_002F753C()) FUN_002F7544(manager_obj + DAT_00100788);
    if (FUN_002F753C()) FUN_002F7544(manager_obj + DAT_0010078C);
    for (int i = 0; i < 2; i++) {
        if (FUN_002F753C()) FUN_002F7544(i * 0x4C + manager_obj + DAT_00100790);
    }

    FUN_0012D1BC();  // unknown — no args, likely global tick/sync

    // Check global state bit 0x40
    uint8_t state = FUN_002F755C();
    if (FUN_002F7574(0x40, state)) {
        thunk_FUN_007B0BA0();  // action when bit 6 is set
    }

    return 2;
}
```

## Key Observations

- Uses the same `FUN_002F755C` / `FUN_002F7574` pattern as weather, but with mask `0x40` (bit 6)
  instead of `0x400000`. **Confirms the global state object is game-wide, not weather-specific.**
- Pattern `FUN_002F744C` + `FUN_002F7510` appears 4 times = flush + release for 4 subsystems
- Pattern `FUN_002F753C` + `FUN_002F7544` appears 3 times (one in loop ×2) = dirty-check + clear
- Hardcoded offset `0xEA08` stands out — subsystem D may be a special/optional component
- Always returns `2` — likely a state machine return code ("continue" / "still running")

## Unknown Subfunctions

| Function | Role |
|----------|------|
| `FUN_0012D418` | Acts on whole manager object — unclear |
| `FUN_0012D1BC` | No args — global tick or sync call |
| `thunk_FUN_007B0BA0` | Called when bit 6 is set — unknown action |

## What Bit 0x40 Means

Unknown. Bit 6 of the global state entry. Something triggers `thunk_FUN_007B0BA0` when active.
Could be: pause, menu open, cutscene, loading — anything that would stall the state manager.

## See Also

[[FUN_002F755C]], [[FUN_002F7574]], [[FUN_002F744C]], [[FUN_002F7510]], [[FUN_002F753C]], [[FUN_002F7544]]

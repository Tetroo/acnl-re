---
address: 0x001d3748
name: DispatchSaveLifecycle
tags: [lifecycle, resource-manager, callback-dispatcher]
status: partially decompiled
---

# [[FUN_001d3748|DispatchSaveLifecycle]] (Lifecycle Manager)

## Technical Analysis

This function implements a state machine for resource loading. It manages the transition from raw data loading to structured parsing.

### Data Structure (Resource Descriptor)
The function operates on a descriptor object located at `*(int *)(param_1 + 4)`.

| Offset | Type | Role | Notes |
|--------|------|------|-------|
| +0x08 | uint | Flags/Type | Controls the loading path and applet checks |
| +0x0C | uint | Extra Flags | Passed to the resource dispatcher |
| +0x10 | func*| Callback 1 | Pre-load/Post-load validation |
| +0x20 | func*| Callback 2 | **Target: Resource Parser (e.g., TownAcres parser)** |
| +0x24 | func*| Callback 3 | Post-parsing finalization |
| +0x28 | func*| Callback 4 | Cleanup/Deallocation |
| +0x34 | int | State/Status | Initialized to `0xffffffff` at start |

### Execution Flow
1. **State Reset**: Sets `descriptor + 0x34` to `0xffffffff`.
2. **Pre-load Checks**:
    - Checks `(descriptor + 8) & 0x800000`. If 0, updates global `DAT_001d3d10`.
    - Checks `(descriptor + 8) & 0x3c0000`. If set, calls `thunk_FUN_007b0ba0` to verify applet status.
3. **Resource Dispatch**:
    - Calls `[[FUN_001d3d3c]]` with the resource ID (`descriptor + 8`) and flags (`descriptor + 0xc`).
4. **Callback Sequence**:
    - Executes `(*descriptor + 0x10)()` if `(flags & 0x10000000) == 0`.
    - Executes `(*descriptor + 0x20)()`. **This is where the raw buffer is parsed into game structures.**
    - Executes `(*descriptor + 0x24)()`.

## Evidence Ladder
- **Call Flow**: Level 4 (Directly observed in decompilation).
- **Callback Offsets**: Level 4 (Explicitly used in the function logic).
- **Resource Descriptor Layout**: Level 3 (Inferred from accesses).

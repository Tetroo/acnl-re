---
address: 0x002b385c
name: LookupTownMapPane
status: partially decompiled
tags: [ui, menu, town-map]
decompiled_at: Session 24
---

# [[FUN_002b385c|LookupTownMapPane]] — Town Map Pane Lookup (`P_mp_00`)

## Summary

Looks up the single named layout pane `"P_mp_00"` within the loaded `map_village.arc` layout (state 8 of [[FUN_002b32f4]]'s init state machine).

```c
void FUN_002b385c(int param_1) {
    FUN_00220138(param_1 + 0xa7c, PTR_s_P_mp_00_002b3998);  // find pane by name "P_mp_00"
    FUN_002fb34c();  // tail call (Ghidra mis-flags as noreturn — see FUN_002FB34C note)
}
```

## Key Findings

> [!IMPORTANT] Only one pane name (`P_mp_00`) exists for the entire Town Map screen — confirms the acre/terrain grid is rendered as a single composited or procedurally-drawn texture, not 42 individual per-acre UI panes.

## See Also

- [[FUN_002b32f4]] — caller; Town Map screen init state machine
- [[FUN_002FB34C]] — tail-called at the end (unrelated time-singleton getter, coincidental)

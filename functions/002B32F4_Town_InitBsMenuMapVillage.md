---
address: 0x002b32f4
name: InitBsMenuMapVillage
status: partially decompiled
tags: [ui, menu, town-map, state-machine]
decompiled_at: Session 24
---

# [[FUN_002b32f4|InitBsMenuMapVillage]] — BsMenuMapVillage Init State Machine (Town Map screen)

## Summary

State machine (states 0–10) that loads and initialises the Town Map menu screen (Nook's Homes / Town Hall bulletin board map). Loads `Layout/Menu/map_village.arc`, binds `map_00_in_list.bclan`/`map_00_out_list.bclan` transition animations, and looks up a single named layout pane `"P_mp_00"` (see [[FUN_002b385c]]).

```c
case 1: FUN_00566e7c(param_1+0x2d0, "Layout/Menu/map_village.arc"); ...
case 2: FUN_002226f4(...); FUN_0056863c(..., "map_00_in_list.bclan", ...); FUN_0056863c(..., "map_00_out_list.bclan", ...); break;
case 10: *(char**)(param_1+0x18) = "BsMenuMapVillage"; ... // sets debug type name, dispatches via vtable
```

## Key Findings

> [!IMPORTANT] Only **one** named layout pane (`P_mp_00`) is referenced for the whole map screen — strongly suggests the acre/terrain grid is rendered as a single composited or procedurally-generated texture rather than 42 individual UI panes. This closes the "search via named UI panes" avenue for finding TownAcres.

## Open Questions

- What writes pixel/texture data into the `P_mp_00` texture? (would directly reveal the acre-grid read/render code)
- What do states 2–9 do exactly? (only sketched)

## See Also

- [[FUN_002b385c]] — looks up `P_mp_00` pane by name
- [[research/town-generation]] — broader context, TownAcres search

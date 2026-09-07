# Actor Engine & Entity Dispatch System

> [!NOTE] Audit 2026-09-07: spot-checked `Actor_AcObjectBase_UpdatePhysics` (`0x001F4B94`) — the real decompile matches in substance (velocity integration, terrain clamp via `Town_SampleFlatGroundHeight`, calls through vtable pointers); this is not a literal decompile dump but an honest simplification. The rest of the vtable table (35 entries, specific indices/offsets) and the lifecycle states (`+0xC6`) were not re-verified line by line in this audit — before using this for the port, cross-check `decompile_function` for each address individually.

## Overview

In *Animal Crossing: New Leaf — Welcome amiibo*, every active entity in the world (players, villagers, furniture, fish, insects, particles, and managers) is built on a unified object hierarchy rooted at `Base` (`0x008CB0C4`) and `AcObjectBase` (`0x008C9250`).

---

## 1. Class Inheritance Hierarchy

```
Base (0x008CB0C4)
 ├── Actor (0x008CB11C)
 │    ├── AcObjectBase (0x008C9250) [Primary World Entity]
 │    │    ├── AcFishCommon (0x008C920C)
 │    │    ├── AcInsectCommon (0x008C99B8)
 │    │    │    └── AcInsectFieldBase (0x008CA29C)
 │    │    │         ├── AcInsectFieldFly (0x008CA008)
 │    │    │         ├── AcInsectFieldStraight (0x008CAAB4)
 │    │    │         └── AcInsectFieldFlyPursue (0x008CABDC) [Bees/Wasps]
 │    │    ├── AcNpcNml (0x008CB2AC) [Town Villagers]
 │    │    ├── AcPlayer (0x008CB2B8) [Player Characters]
 │    │    └── AcFsFdShadow (0x008C9218) [Fish Water Shadow]
 │    └── DemoActor (0x008CB5FC)
 ├── 6BsRoot (0x008CB168) [Root Engine Scene]
 ├── 8BsNpcMgr (0x008CB328) [NPC Manager - 10 Villagers]
 ├── 11BsPlayerMgr (0x008C9114) [Player Manager - 4 Players]
 ├── 14BsFishFieldMgr (0x008C9A04) [River & Ocean Fish Spawner]
 ├── 16BsInsectFieldMgr (0x008CA04C) [Bug Spawner & Spatial Grid]
 ├── 13BsObjCheckMgr (0x008C97F4) [Object Interaction Manager]
 ├── 12BsGrRenderer (0x008C93B0) [Terrain Acre Grid Renderer]
 └── 14BsShadowMapMgr (0x008C9AEC) [Dynamic Shadow Map]
```

---

## 2. Virtual Table Layout (`AcObjectBase` at `0x0083CF98`)

The base entity vtable has 35 entries, establishing the lifecycle protocol for all derived objects:

| Vtable Index | Offset | Function Address | Name / Purpose | Description |
|---|---|---|---|---|
| `[00]` | `+0x00` | `0x001F50D0` | `AcObjectBase_DeletingDtor` | Calls dtor and deallocates memory |
| `[01]` | `+0x04` | `0x001F50C0` | `AcObjectBase_Dtor` | Base class destructor |
| `[03]` | `+0x0C` | `0x001F4CC4` | `AcObjectBase_Init` | 2-phase initialization and acre registration |
| `[06]` | `+0x18` | `0x001F4E6C` | `AcObjectBase_Destroy` | Cleanup before actor removal |
| `[09]` | `+0x24` | `0x001F4DE8` | `AcObjectBase_Update` | Main per-frame update loop |
| `[12]` | `+0x30` | `0x001F4B0C` | `AcObjectBase_Draw` | Model dispatch and shadow rendering |
| `[24]` | `+0x60` | `0x001F4CC0` | `AcObjectBase_UpdateAI` | Virtual hook for behavior & AI states |
| `[25]` | `+0x64` | `0x001F4B94` | `AcObjectBase_UpdatePhysics` | Velocity integration & terrain height clamp |
| `[29]` | `+0x74` | `0x001F4EBC` | `AcObjectBase_BuildMatrix` | Computes 4x3 affine world matrix |
| `[33]` | `+0x84` | `0x00710C00` | `AcObjectBase_GetShadowParams`| Retrieves shadow radius and alpha |
| `[34]` | `+0x88` | `0x001F46F4` | `AcObjectBase_CanSpawn` | Validates if actor can exist in current scene |

---

## 3. Entity Lifecycle (`AcObjectBase_Init`)

Every actor goes through the following lifecycle states (`lifecycle_state` at `+0xC6`):

1. **`Uninitialized (0)`:** Actor instance allocated in memory.
2. **`Initializing (1)`:**
   - Acre coordinate calculation:
     $$\text{AcreX} = \lfloor \frac{\text{PosX}}{16.0 \times 16.0} \rfloor, \quad \text{AcreZ} = \lfloor \frac{\text{PosZ}}{16.0 \times 16.0} \rfloor$$
   - World elevation sampling:
     $$\text{GroundY} = \text{Town\_SampleFlatGroundHeight}(\text{WorldPos}, 0)$$
   - Initial transform matrix constructed (`0x001F4EBC`).
3. **`Active (2)`:** Actor participates in the per-frame update and render cycles.
4. **`PausedOrDead (3)`:** Flagged for removal by `BsObjCheckMgr` during garbage collection.

---

## 4. Physics & Movement Integration (`AcObjectBase_UpdatePhysics`)

Executed every frame at `0x001F4B94`:

```c
// Velocity integration:
world_pos.x += velocity.x;
world_pos.y += velocity.y;
world_pos.z += velocity.z;

// Visual offset interpolation:
display_pos.x = world_pos.x + visual_offset.x;
display_pos.y = world_pos.y + visual_offset.y;
display_pos.z = world_pos.z + visual_offset.z;

// Terrain height clamping:
ground_height = Town_SampleFlatGroundHeight(&world_pos, 0);

// Model transform matrix update:
AcObjectBase_BuildWorldTransformMatrix(&matrix, display_pos, rotation);
```

---

## 5. Subsystem Managers & NPC Management

### `8BsNpcMgr` (0x008F77E0)
- Manages the **10 villager instances** in town.
- In `FUN_006a0c58`, loops through villager index `0..9`, updating their AI state machine:
  - Inside home / Outside wandering.
  - Pathfinding between interest points (Town Hall, Re-Tail, River, Bench, Coffee shop).
  - Social interactions with nearby players or other villagers.

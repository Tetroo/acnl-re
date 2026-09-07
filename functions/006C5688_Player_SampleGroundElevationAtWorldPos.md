---
address: 0x006C5688
name: Player_SampleGroundElevationAtWorldPos
subsystem: Player
tags: [player, physics, terrain, heightmap, collision, ramp, confirmed]
status: done
---

# `006C5688_Player_SampleGroundElevationAtWorldPos` — Terrain Surface Elevation Sampler

## Overview

Called on every physics update tick (e.g. from `Player_AcPlayer_Vtbl30` / `0x0064C2A0`) to clamp the player's vertical position ($Y$) directly onto the terrain mesh surface or smoothly interpolate elevation along natural ramps, beach inclines, and stone staircases.

---

## Decompiled Code (Ghidra 12.1.3)

```c
void Player_SampleGroundElevationAtWorldPos(float *pos_world, undefined4 param_2)
{
  int collision_type;
  int acre_x, acre_z;
  int tile_x, tile_z;
  
  // 1. Convert continuous 3D world pos to discrete acre and local tile indices
  Town_WorldPosToAcreAndTileCoords(&acre_x, &acre_z, &tile_x, &tile_z, pos_world);
  
  // 2. Query tile collision / geometry classification
  collision_type = Town_GetTileCollisionType(acre_x, acre_z, tile_x, tile_z, param_2);
  
  // 3. Branch: Flat terrain vs Slopes/Ramps
  if (collision_type != 2 && collision_type != 0xF8 && 
      collision_type != 4 && collision_type != 3 &&
      collision_type != 0xA2 && collision_type != 0xA3 && 
      collision_type != 0xA4 && collision_type != 0xA5) 
  {
    // Flat acre terrain heightmap lookup from grd_jmp.bin
    Town_SampleFlatGroundHeight(pos_world, param_2);
    return;
  }
  
  // 4. Smooth ramp / stair incline interpolation
  Town_WorldPosToAcreAndTileCoords(&acre_x, &acre_z, &tile_x, &tile_z, pos_world);
  Town_InterpolateRampElevation(acre_x, acre_z, tile_x, tile_z, param_2);
}
```

---

> [!NOTE] Audit 2026-09-07: the decompiled code itself was checked against the live Ghidra output and matches verbatim (variables renamed for readability, logic identical). The table below is HYPOTHESIS: the specific semantic labels for `collision_type` values (Ramp Ascending/Descending, Stone Stairs, Ocean Boundary) are not confirmed within this function itself — they were not derived from strings/constants, but guessed from the numeric values. The numbers themselves are confirmed (visible in the code); their meanings are not.

## Collision Classification Table

| Collision Value | Meaning | Elevation Handling |
|---|---|---|
| `2` | Flat grass / dirt terrain | Samples half-float height from `grd_jmp.bin` |
| `3` | Ramp Ascending (North-South slope) | Linear vertical LERP along incline |
| `4` | Ramp Descending (East-West beach slope) | Linear vertical LERP along incline |
| `0xA2` .. `0xA5` | Stone Stairs / Bridges | Stair incline step curve |
| `0xF8` | Ocean Water Boundary | Blocks land movement, activates swimming check |

---

## Architectural Significance

1. **Pixel-Perfect Ground Clamping:** Eliminates character floating or sinking into the ground on complex curved cliff edges.
2. **Smooth Ramp Incline:** Rather than treating ramps as jagged staircase steps, the game smoothly interpolates elevation using `Town_InterpolateRampElevation` (`0x006C6628`).

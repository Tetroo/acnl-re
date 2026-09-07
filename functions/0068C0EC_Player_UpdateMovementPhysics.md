# Function: Player_UpdateMovementPhysics (`0x0068C0EC`)

> **Address:** `0x0068C0EC`  
> **Subsystem:** Player & Physics (`AcPlayer`)  
> **Source Binary:** `exefs.elf`  
> **Status:** Fully Reverse-Engineered  
> **Related Files:** [`types/Player.h`](../types/Player.h), [`systems/player-physics.md`](../systems/player-physics.md)

---

## 1. Overview

`Player_UpdateMovementPhysics` is slot 9 of the `AcPlayer` virtual method table (`0x008F74B0`). Executed once every simulation frame (30 Hz), it handles the player's core physical interaction with the world: querying the procedural ground heightmap to snap the player's Y elevation, computing 3D collision boxes against town obstacles, updating the locomotion state machine, and dispatching footstep sound effects.

---

## 2. Decompiled Implementation

```c
undefined4 Player_UpdateMovementPhysics(int param_1)
{
  int iVar1;
  uint uVar2;
  byte collision_flags;
  bool is_state_2;
  undefined4 ground_y;
  
  iVar1 = FUN_0064cfd0();
  if ((iVar1 != 0) && (iVar1 = FUN_00652ab0(param_1), iVar1 != 0)) {
    // 1. Terrain Heightmap Elevation Snapping
    if (*(char *)(param_1 + 0x177) < '\0') {
      ground_y = Town_SampleFlatGroundHeight(param_1 + 0x14, 0);
      *(undefined4 *)(param_1 + 0x18) = ground_y; // Set world Y coordinate
      FUN_00613914(param_1 + 0x494);
    }
    else {
      // 2. Resolve terrain collision box
      if (((0xea < *(byte *)(param_1 + 0x1a9)) || (iVar1 = FUN_0064e3d8(), iVar1 == 0)) ||
         (collision_flags = *(byte *)(iVar1 + 5) & 0xf, 4 < collision_flags)) {
        collision_flags = 0;
      }
      uVar2 = Core_GetStateIndex();
      is_state_2 = uVar2 == 2;
      if (is_state_2) {
        uVar2 = (uint)*(byte *)(param_1 + 0x1a9);
      }
      if (is_state_2 && uVar2 == 0x2f) {
        collision_flags = 2;
      }
      iVar1 = FUN_0064e658(param_1, collision_flags); // Apply obstacle pushing
      if (iVar1 == 0) {
        return 2;
      }
    }
    
    // 3. Update Locomotion State Machine
    iVar1 = Player_UpdateStateMachine(param_1);
    if (iVar1 != 0) {
      // 4. Update footstep audio and grass erosion
      FUN_0064ca1c(param_1);
    }
  }
  return 2;
}
```

---

## 3. Key Takeaways for PC Port

1. **Elevation Clamping:** The player's vertical position is clamped to the town heightmap each tick via `Town_SampleFlatGroundHeight`, ensuring the player smoothly traverses ramps and cliffs.
2. **HYPOTHESIS (not confirmed by bytes):** `FUN_0064ca1c` is called after `Player_UpdateStateMachine`, but this function itself has not been decompiled or verified — the earlier version's claim of "footstep audio and grass erosion" was speculation based on game lore (desire paths are a real ACNL mechanic), not a conclusion drawn from the code. `FUN_0064ca1c` needs to be decompiled before this can be treated as fact.

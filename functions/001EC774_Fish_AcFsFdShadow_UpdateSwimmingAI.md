# 0x001EC774 — Fish_AcFsFdShadow_UpdateSwimmingAI

## Function Signature
```cpp
void Fish_AcFsFdShadow_UpdateSwimmingAI(AcFsFdShadow* this);
```

- **Address:** `0x001EC774`
- **Subsystem:** `Item` / `Fish`
- **Class:** `AcFsFdShadow` (RTTI: `12AcFsFdShadow`)
- **Vtable Slot:** `[24]` (offset `+0x60` in `0x008EACA4`)

---

## Purpose & Summary

This function drives the active per-frame AI behavior of fish shadows swimming in the town rivers and ocean. It executes the 12-state hierarchical state machine (`SpeedUp`, `MoveS`, `MoveR`, `SpeedDown`, `DriftS`, `DriftR`, `Escape`, `Pick`, `Hit`, `Pass`, `Leave`, `Delete`), calculates distance to the player's fishing bobber, and triggers the flee response when running footsteps are detected.

---

## State Transition Jump Table (`0x008C77DC`)

The function indexes state handlers via `DAT_008C77DC + state * 0x1C`:

```
Offset +0x00: const char* state_name
Offset +0x04: void (*OnEnter)(AcFsFdShadow* this)
Offset +0x0C: void (*OnUpdate)(AcFsFdShadow* this)
Offset +0x14: void (*OnExit)(AcFsFdShadow* this)
```

### State Behaviors:
- **`Pick` (`0x001EA010`):** Approaching the bobber. Evaluates bite probability based on `nibble_count * 20%`.
- **`Hit` (`0x001E9E68`):** Bobber submerged. Runs reaction timer up to `window_frames` ($30 \text{ fps}$).
- **`Escape` (`0x001EB52C`):** Swimming away at 3x speed and despawning.

---

## Player Proximity & Spooking Logic

The function queries all active players ($0..3$) in town via `FUN_005c1d38(player_idx, 1)`. If a player is running within radius $R = 48.0 \times 32.0$ units, the fish immediately cancels its current state and triggers `Escape`.

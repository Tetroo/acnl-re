# Fishing System & Fish AI State Machine

## Overview

Fishing in *Animal Crossing: New Leaf — Welcome amiibo* is managed across two primary components:
1. **`BsFishFieldMgr` (`0x008EDA24`):** The global town fish manager responsible for spawning, pooling, and tracking up to 8 active fish instances simultaneously.
2. **`AcFsFdShadow` (`0x008EACA4`):** The fish actor representing the shadow swimming in the water, driven by a 12-state AI state machine with dynamic player-distance detection and fishing hook interaction.

---

## 1. Fish Population & Spawning (`BsFishFieldMgr`)

- **Maximum Concurrency:** At any given time, a maximum of **8 active fish** can exist in the town:
  $$\text{ActiveRiverFish} + \text{ActiveOceanFish} < 8$$
- **Slot Storage:** The manager maintains 8 packed slots labeled `'T'`, `'U'`, `'V'`, `'W'`, `'X'`, `'Y'`, `'Z'`, `'['` (offset `0x00253f54`).
- **Packed Record Format:** Each slot is 6 bytes:
  - Byte 0: `SpeciesID & 0x7F` (106 total species: 72 fish + diving creatures + specials)
  - Bytes 1–3: Packed 10-bit X and Z coordinates in world space ($0..1023$)
  - Byte 4: Fish flags and orientation
  - Byte 5: Active slot index ($0..7$)
- **Spawn Selection:** Determined by location ($0 = \text{River}, 1 = \text{Ocean}$) and current calendar date/time (`GetTimeSingletonPrimary()`). Species are picked via a cumulative probability lookup table (`0x00614C14`).

---

## 2. The 12-State Fish AI State Machine (`0x008C77DC`)

Every fish shadow executes a hierarchical state machine defined in table `DAT_008C77DC`:

| ID | State Name | Enter Function | Update Function | Exit Function | Description |
|---|---|---|---|---|---|
| `0` | **`SpeedUp`** | `0x001EBD38` | `0x001EB828` | `0x001EB9A4` | Accelerates swimming speed |
| `1` | **`MoveS`** | `0x001EBA3C` | `0x001EAB58` | `0x001EB5E0` | Straight swimming motion |
| `2` | **`MoveR`** | `0x0027D75C` | `0x001EA848` | `0x001EB5DC` | Rotational turn swimming motion |
| `3` | **`SpeedDown`**| `0x001EBE18` | `0x001EBA88` | `0x001EBD34` | Decelerates back to idle drift |
| `4` | **`DriftS`** | `0x001EBBB8` | `0x001EB36C` | `0x001EB8E0` | Passive drift with river current (straight) |
| `5` | **`DriftR`** | `0x001EBB68` | `0x001EB114` | `0x001EB8DC` | Passive drift with river current (rotating) |
| `6` | **`Escape`** | `0x001EBC08` | `0x001EB52C` | `0x001EB8E4` | Fleeing rapidly and despawning |
| `7` | **`Pick`** | `0x001EB920` | `0x001EA010` | `0x001EADD8` | Approaching and nibbling on player's bobber |
| `8` | **`Hit`** | `0x001EB5E4` | `0x001E9E68` | `0x001EA558` | Bobber pulled underwater (bite splash) |
| `9` | **`Pass`** | `0x001EB8E8` | `0x001E9F9C` | `0x001EADD4` | Swims past hook without noticing |
| `10`| **`Leave`** | `0x001EB9A8` | `0x001EA6EC` | `0x0027D47C` | Loses interest in bobber and turns away |
| `11`| **`Delete`** | `0x0027DB58` | `0x001EB0C4` | `0x001EB8D8` | Despawns entity from manager |

---

## 3. The 20%-per-Nibble Bite Formula (`Fish_AcFsFdShadow_StatePick_Update`, `0x001EA010`)

> [!NOTE] Verified 2026-09-07 (second pass): the core threshold formula below is `[TOOL]`-confirmed exact — real decompile has `uVar8 = FUN_002ff788(100); if ((uVar8 & 0xff) < *(byte*)(param_1+0x1ce) * 0x14)`, i.e. literally `random(100) < NibbleCount * 20`. The **"Transition to State 8: Hit"** label and the **"-0.56 units retreat"** figure in the table below are `HYPOTHESIS` — not confirmed in this function. The success branch sets an internal sub-state field (`param_1+0x202 = 10`) and calls unanalyzed `FUN_0027dc4c`/`FUN_0071e6fc`/`FUN_0071e6b4`; whether/where this maps to the outer 12-state table's "state 8 (Hit)" wasn't traced. The retreat distance number doesn't appear anywhere in the decompiled function at all.

$$\text{Threshold} = \text{NibbleCount} \times 20$$

When the fish nudges the bobber, it rolls a random integer $R \in [0, 99]$:
$$\text{If } R < (\text{NibbleCount} \times 20) \implies \text{Bite (transition target unconfirmed, likely State 8: Hit)}$$

| Nibble Attempt | Bite Probability | Result if Failed |
|---|---|---|
| **Nibble 1** | **20%** ($R < 20$) | Nudges bobber, increments count to 2 (`HYPOTHESIS`: retreat distance not confirmed in code) |
| **Nibble 2** | **40%** ($R < 40$) | Nudges bobber, increments count to 3 |
| **Nibble 3** | **60%** ($R < 60$) | Nudges bobber, increments count to 4 |
| **Nibble 4** | **80%** ($R < 80$) | Nudges bobber, increments count to 5 |
| **Nibble 5** | **100%** ($R < 100$) | **Guaranteed Bite!** (transition mechanics unconfirmed) |

---

## 4. Reel-In Reaction Window (`FUN_001eb5e4`)

When entering the `Hit` state (`0x001EB5E4`):
```c
reaction_seconds = FUN_002554c0(); // Dependent on fish rarity (0.5s to 1.2s)
window_frames = (int)(reaction_seconds * 30.0f);
```
- A frame timer (`param_1 + 0x198`) increments every frame ($30 \text{ fps}$).
- If the player presses **A** before `timer >= window_frames`, the fish is successfully caught!
- If the timer expires without input, the fish transitions to state `6` (`Escape`) and vanishes.

---

## 5. Player Running / Flee Trigger (`FUN_001ec774`)

In `0x001EC774` lines 148–158:
$$\text{Dist}^2 = (X_{\text{fish}} - X_{\text{player}})^2 + (Y_{\text{fish}} - Y_{\text{player}})^2 + (Z_{\text{fish}} - Z_{\text{player}})^2$$
- If the player is running (locomotion mode $\ge 2$) within proximity:
  $$\text{Dist}^2 < (48.0 \times 32.0)^2$$
- The fish is startled and immediately transitions to state `6` (`Escape`).

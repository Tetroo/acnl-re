# Insect / Bug Catching System

## Overview

The Insect System in *Animal Crossing: New Leaf — Welcome amiibo* controls the spawning, movement, player detection, and net catching physics of all 72 bug species. It is managed by `BsInsectFieldMgr` (`0x008F0188`) and entity base `AcInsectFieldBase` (`0x008F0CC0`).

---

## 1. Population & Concurrency (`BsInsectFieldMgr`)

- **Maximum Active Bugs:** The town maintains up to **12 concurrent insect slots** labeled `'H'`, `'I'`, `'J'`, `'K'`, `'L'`, `'M'`, `'N'`, `'O'`, `'P'`, `'Q'`, `'R'`, `'S'`.
- **Weather Sensitivity (`FUN_002abfe0`):**
  - Flags `DAT_0094f13f` (Rain) and `DAT_0094f141` (Snow).
  - Flying insects (Category `'A'`) and ground insects (Categories `'F'` and `'G'`) immediately trigger their despawn callback (`+0xA8`) when rain or snow starts. Only rain-tolerant species (like Snails) remain.

---

## 2. Species Physics Table (`0x0086773C`)

Each of the 72 insect species has an **80-byte (`0x50`)** parameter record loaded in `0x0024F314`:
- `+0x00`: Collision / Net catch radius (e.g. 6.0 for Common Butterfly, 12.0 for Birdwing).
- `+0x08`: Player detection radius.
- `+0x0C`: Flight altitude above terrain.
- `+0x10`: Flight speed.
- `+0x14`: Turning rate.

---

## 3. Net Catching & Net Collision (`0x002C28B4`)

When the player swings the bug net:
1. The net hitbox checks overlap against `AcInsectFieldBase.current_coll_radius`.
2. If the net overlaps the insect before its `flee_state_flag` (`+0x1CB`) triggers, the insect is caught and added to the player's pocket!
3. If the player approaches while running or without sneaking, the bug enters the flee/scatter state and vanishes.

---

## 4. Wasp / Bee Attack Sequence (`AcInsectFieldFlyPursue`)

- **Class:** `AcInsectFieldFlyPursue` (`0x008F43BC`).
- **Trigger:** Shaking a non-fruit hardwood or cedar tree rolls a chance for a beehive (`0x32C96C`).
- **Pursuit AI:** Swarms around the player for 5 seconds. If the player fails to enter a building, catch them with a net, or jump into water/train tracks, the player is stung (swollen eye effect).

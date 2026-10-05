# Function: Insect_BsInsectFieldMgr_UpdateBugLoop (`0x002ACB7C`)

> **Address:** `0x002ACB7C`
> **Subsystem:** Insect & Critter Management
> **Class:** `BsInsectFieldMgr` (Vtable Slot 9 at `0x008F0188`)
> **Source Binary:** `exefs.elf`
> **Status:** Done (100% byte-verified via Ghidra MCP)
> **Related Files:** [`types/Critters.h`](../types/Critters.h), [`systems/insects.md`](../systems/insects.md)

---

## 1. Overview

`Insect_BsInsectFieldMgr_UpdateBugLoop` (`0x002ACB7C`) is the master per-tick update routine for all outdoor insects and bugs in Animal Crossing: New Leaf. It handles:
1. **Weather Despawn:** Checks rain/snow conditions and immediately despawns rain-sensitive flying insects (categories 'A', 'F', 'G').
2. **12 Field Slot Synchronization:** Synchronizes the 12 outdoor insect slots (ASCII identifiers `"HIJKLMNOPQRS"`) located at instance offset `+0x1370` (6 bytes per slot).
3. **Spawn Countdown:** Ticks the insect spawn timer at `+0x11FC`.
4. **Proximity Alert & Scare:** Every 11 frames (`+0x1205`), calculates Euclidean distance squared between all active insects and all 4 players; triggers scare behavior if player enters bug scare radius.

---

## 2. Decompiled Implementation (`decompile_function` verified)

```c
undefined4 Insect_BsInsectFieldMgr_UpdateBugLoop(int param_1)
{
  int iVar1;
  undefined4 uVar2;
  byte *pbVar3;
  int iVar4;
  uint uVar5;
  
  iVar1 = FUN_00300994();
  if (iVar1 == 0) {
    Insect_BsInsectFieldMgr_CheckWeatherDespawn(param_1);
    FUN_002aacec(param_1);
    Insect_BsInsectFieldMgr_TickSpawnTimer(param_1);
    Insect_BsInsectFieldMgr_CheckPlayerProximityAndScare(param_1);
  }
  else {
    iVar1 = FUN_006197e8();
    if (iVar1 == 0) {
      Insect_BsInsectFieldMgr_SyncSlotsAndDespawnMismatch(param_1);
    }
    else {
      Insect_BsInsectFieldMgr_CheckWeatherDespawn(param_1);
      if (*(char *)(param_1 + 0x1206) != '\0') {
        uVar5 = 0;
        do {
          uVar2 = FUN_00625940("HIJKLMNOPQRS"[uVar5]);
          FUN_00778604(param_1 + uVar5 * 6 + 0x1370, uVar2);
          uVar5 = uVar5 + 1;
        } while (uVar5 < 0xc);
      }
      *(undefined1 *)(param_1 + 0x1206) = 0;
      Insect_BsInsectFieldMgr_TickSpawnTimer(param_1);
      for (iVar1 = *(int *)(param_1 + 0x20); iVar1 != 0; iVar1 = *(int *)(iVar1 + 4)) {
        pbVar3 = (byte *)(param_1 + *(int *)(iVar1 + 0x14) * 6 + 0x1370);
        *pbVar3 = *pbVar3 & 0x80 | 0x53;
        iVar4 = *(int *)(iVar1 + 0x14);
        if (iVar4 < 0xc) {
          FUN_00624174("HIJKLMNOPQRS"[iVar4], param_1 + iVar4 * 6 + 0x1370, 1);
        }
      }
      Insect_BsInsectFieldMgr_CheckPlayerProximityAndScare(param_1);
    }
  }
  return 2;
}
```

---

## 3. Subsystem Architecture & Memory Offsets

| Offset | Type | Description |
|---|---|---|
| `+0x1C` | `void*` | Head of active insect actor linked list |
| `+0x20` | `void*` | Head of active insect slot linked list (stride 24B, node+0x4 = next, node+0x14 = slot index) |
| `+0x11F8` | `uint32_t` | Spawn timer reload interval (frames) |
| `+0x11FC` | `int32_t` | Spawn countdown timer (decrements each tick, spawns bug when $\le 0$) |
| `+0x1200` | `uint16_t` | Special critter timer (wasps, ants) |
| `+0x1205` | `uint8_t` | Player proximity check counter ($0 \dots 10$, triggers every 11 frames) |
| `+0x1206` | `uint8_t` | Slot reinitialization dirty flag |
| `+0x1207` | `uint8_t` | Active weather despawn flag |
| `+0x1220` | `InsectEntry[12]` | 12 active insect actor entries (24 bytes each, total 288 bytes) |
| `+0x1340` | `InsectEntry[2]` | 2 special critter actor entries (wasps, ants; 24 bytes each, total 48 bytes) |
| `+0x1370` | `InsectSlot[12]` | 12 insect slot descriptors (`"HIJKLMNOPQRS"`, 6 bytes each, total 72 bytes) |

---

## 4. Player Proximity & Scare Mechanics (`0x002AC5E0`) `[TOOL]`

`Insect_BsInsectFieldMgr_CheckPlayerProximityAndScare` executes every 11 frames:
1. Iterates all 4 player slots ($P \in [0, 3]$) via `FUN_005bfce8(pos, P)`.
2. For each active insect $I$ in `active_insects_list` (`+0x1C`):
   $$(X_I - X_P)^2 + (Z_I - Z_P)^2 < R_{\text{scare}}^2$$
   where $R_{\text{scare}}$ is a float loaded from insect object offset `+0x118` (index `0x46` as float array).
3. If the inequality holds, calls insect virtual method slot `+0xB0` (`vtable[44]`):
   `(**(code **)(*insect + 0xb0))(insect, player_pos, player_id);`
   triggering flee / fly-away animation.

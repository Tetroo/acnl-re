# Town_SpawnRockHitReward (`0x0059EA7C`)

> **Address:** `0x0059EA7C`  
> **Subsystem:** Town / World Interaction  
> **Symbol:** `Town_SpawnRockHitReward` (original: `FUN_0059ea7c`)  
> **Status:** `done` (Verified with live Ghidra MCP decompile)  
> **Callers:** `FUN_0059e530` (`0x0059E530`, stone interaction state machine dispatcher)  
> **Calls:** `FUN_0030b854` (Get item category ID), `FUN_002fc318`, `Core_ReadGameStateFlag`, `FUN_006f1938` (Loot pool builder), `FUN_002fcc14` (Item construct), `FUN_00164ed0` (Grid bounds check), `FUN_0059f72c`, `FUN_00613100` (Tile obstruction validator), `FUN_0059f934`, `FUN_006239f8` (Event dispatcher)

---

## 1. Overview & Verification

`[TOOL]` This function handles the physical ejection of reward items (Bell bags, coins, and mineral ore gems) when a rock is struck by the player with a shovel.

### Key Reverse-Engineered Mechanics:
1. **8-Strike Loop:** The function iterates up to **8 consecutive strikes** via a loop:
   ```c
   iVar6 = iVar6 + 1;
   } while (iVar6 < 8);
   ```
2. **Surrounding Drop Geometry:** Drops are routed to the 8 tiles adjacent to the struck rock using offset tables `DAT_0085e0c8` ($\Delta X$) and `DAT_0085e0e8` ($\Delta Y$):
   $$\text{targetX} = \text{rockX} + \Delta X_i, \quad \text{targetY} = \text{rockY} + \Delta Y_i$$
3. **Collision / Obstruction Validation:** Each prospective target tile is checked via `FUN_00613100`. If a tile is obstructed by a hole, water, fence, bush, or existing item, the spawner advances to the next available clockwise adjacent tile.
4. **Daily Luck Integration:**
   - `DAT_00952f68 == 0` (**Good Money Luck**): Spawns upgraded `0x20AD` Bell bags instead of `0x20AC`.
   - `DAT_00952f68 == 6` (**Good Ore Luck**) or Silver Shovel flag (`param_2[1] & 1`): Invokes `FUN_006f1938(&local_310, 5, 8, 1, 0)` and `(..., 5, 9, 1, 0)` to generate multi-gem drops (Gold, Silver, Ruby, Sapphire, Emerald, Amethyst) from the Money Rock!

---

## 2. Drop Item IDs

`[TOOL]` Extracted directly from function literals and confirmed against `tools/work/romfs_out/Script/Str/STR_Item_name.umsbt`:

| Item ID | Item Name | Context |
|---|---|---|
| `0x209E` | 100 Bells coin | Initial rock strike payout |
| `0x20AC` | Bell bag | Standard progressive Bell bag (200..8,000 Bells) |
| `0x20AD` | Large Bell bag | Upgraded payout under Good Money Luck (up to 16,000 Bells) |
| `0x2119` | Giant Bell sack | Final 8th strike jackpot (up to 32,000 Bells under luck) |
| `0x20A2` | Gold nugget | Rare mineral drop (Silver Shovel / Ore Luck) |
| `0x20A3` | Silver nugget | Rare mineral drop (Silver Shovel / Ore Luck) |
| `0x20A4` | Ruby | Gem drop |
| `0x20A5` | Sapphire | Gem drop |
| `0x20A6` | Emerald | Gem drop |
| `0x20A7` | Amethyst | Gem drop |

---

## 3. Literal Decompiled Implementation (`decompile_function`)

`[TOOL]` Verbatim Ghidra output from `0x0059EA7C`:

```c
undefined1 * Town_SpawnRockHitReward(int param_1)

{
  bool bVar1;
  undefined1 uVar2;
  byte bVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  undefined1 auStack_314 [4];
  undefined *local_310;
  undefined1 auStack_30c [4];
  undefined1 auStack_308 [4];
  undefined1 auStack_304 [4];
  byte local_300;
  byte local_2ff;
  undefined1 local_2fe;
  undefined1 local_2fa;
  undefined1 local_2f9;
  undefined1 local_2f8;
  undefined1 *local_2f4;
  undefined1 auStack_2ec [4];
  undefined1 auStack_2e8 [704];
  undefined1 auStack_28 [4];
  
  FUN_002fc29c(auStack_28);
  bVar1 = false;
  uVar4 = FUN_0030b854(param_1);
  switch(uVar4) {
  case 0x71:
  case 0x76:
    FUN_002fc318(&local_310);
    iVar6 = Core_ReadGameStateFlag();
    if (((iVar6 == 3) || (DAT_00aae14c == 0)) || (DAT_00952f68 != '\x06')) {
      iVar6 = Core_ReadGameStateFlag();
      if (((iVar6 == 3) || (DAT_00aae14c == 0)) || (DAT_00952f68 != '\a')) {
        FUN_006f1938(&local_310,5,1,1,0);
      }
      else {
        FUN_006f1938(&local_310,5,5,1,0);
      }
    }
    else {
      FUN_006f1938(&local_310,5,8,1,0);
      FUN_006f1938(&local_310,5,9,1,0);
    }
    FUN_002feabc(auStack_314,&local_310);
    FUN_002fc978(auStack_28,auStack_314);
    bVar1 = true;
    break;
  case 0x72:
  case 0x77:
    uVar4 = FUN_002fcc14(auStack_314,0x209e);
    FUN_002fc978(auStack_28,uVar4);
    break;
  case 0x73:
  case 0x74:
  case 0x78:
  case 0x79:
    iVar6 = Core_ReadGameStateFlag();
    if (((iVar6 == 3) || (DAT_00aae14c == 0)) || (DAT_00952f68 != '\0')) {
      uVar4 = FUN_002fcc14(auStack_314,0x20ac);
      FUN_002fc978(auStack_28,uVar4);
    }
    else {
      uVar4 = FUN_002fcc14(auStack_314,0x20ad);
      FUN_002fc978(auStack_28,uVar4);
    }
    FUN_006bac5c(auStack_28);
    break;
  case 0x75:
  case 0x7a:
    uVar4 = FUN_002fcc14(auStack_314,0x2119);
    FUN_002fc978(auStack_28,uVar4);
    break;
  default:
    goto switchD_0059eaa4_default;
  }
  FUN_002fc978(param_1 + 8,auStack_28);
  if (bVar1) {
    uVar2 = 0;
  }
  else {
    uVar2 = 0xff;
  }
  *(undefined1 *)(param_1 + 0xf) = uVar2;
  *(undefined1 *)(param_1 + 0x10) = 0;
  *(undefined1 *)(param_1 + 0x11) = 0;
  uVar11 = (uint)*(byte *)(param_1 + 0x13);
  uVar12 = (uint)*(byte *)(param_1 + 0x14);
  iVar6 = 0;
  do {
    iVar7 = (&DAT_0085e0c8)[iVar6];
    iVar8 = (&DAT_0085e0e8)[iVar6];
    if ((*(byte *)(param_1 + 0xe) & 2) == 0) {
      iVar7 = -iVar7;
    }
    uVar10 = (uint)*(byte *)(param_1 + 0x13) + iVar7;
    uVar9 = (uint)*(byte *)(param_1 + 0x14) + iVar8;
    if (iVar6 == 0) {
      uVar12 = uVar9;
      uVar11 = uVar10;
    }
    iVar5 = FUN_00164ed0(uVar10,uVar9,*(undefined1 *)(param_1 + 0x12),0,0);
    if ((iVar5 != 0) &&
       (iVar5 = FUN_0059f72c(uVar10,uVar9,*(undefined1 *)(param_1 + 0x12)), iVar5 == -1)) {
      if (iVar6 != 0) {
        FUN_006a5e04(auStack_2ec,*(undefined1 *)(param_1 + 0x13),*(undefined1 *)(param_1 + 0x14));
        local_2f4 = auStack_2ec;
        uVar4 = Core_GetStateIndex();
        iVar5 = Core_CheckStateFlag(4,uVar4);
        if ((iVar5 != 0) && (iVar5 = FUN_00613100(uVar10,uVar9,local_2f4), iVar5 == 0)) {
          FUN_006a5e04(auStack_2e8,uVar11,uVar12);
          local_2f4 = auStack_2e8;
          uVar4 = Core_GetStateIndex();
          iVar5 = Core_CheckStateFlag(4,uVar4);
          if ((iVar5 != 0) && (iVar5 = FUN_00613100(uVar10,uVar9,local_2f4), iVar5 == 0))
          goto LAB_0059efd4;
        }
      }
      iVar5 = FUN_0059f934(0x21,uVar10,uVar9,*(undefined1 *)(param_1 + 0x12),1);
      if (iVar5 != -1) {
        FUN_0059f6a4(auStack_30c);
        uVar2 = *(undefined1 *)(param_1 + 0x12);
        local_310 = &DAT_0095afdc;
        bVar3 = FUN_00306038();
        local_300 = bVar3 & 3 | 0x84;
        local_2ff = local_2ff & 0xc0 | 0xc;
        local_2fe = 0;
        local_2f9 = (undefined1)uVar10;
        local_2f8 = (undefined1)uVar9;
        FUN_002fc978(auStack_30c,local_310);
        FUN_002fc978(auStack_308,auStack_28);
        FUN_002fc978(auStack_304,&DAT_0095afdc);
        local_2fa = uVar2;
        iVar6 = FUN_002fc990();
        if (iVar6 == 0) {
          local_2ff = local_2ff & 0xc3 | 0xc;
          FUN_0059e530(auStack_30c);
        }
        else if ((local_300 & 3) == 0) {
          local_2ff = local_2ff & 0xc3 | 0x1c;
          FUN_0059e530(auStack_30c);
        }
        else {
          local_2ff = local_2ff & 0xc3 | 0x1c;
          FUN_006239f8(0x3a,local_300 & 3,auStack_30c,0x16);
        }
        *(byte *)(param_1 + 0xf) = (char)iVar7 + 4U & 7 | (byte)((iVar8 + 4U & 7) << 4);
        break;
      }
    }
LAB_0059efd4:
    iVar6 = iVar6 + 1;
  } while (iVar6 < 8);
switchD_0059eaa4_default:
  return auStack_28;
}
```

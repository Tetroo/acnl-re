# Function: PostOffice_UpdateMonthlyInterestAndSavingsRewards (0x0062F7D0)

- **Address:** `0x0062F7D0`
- **Subsystem:** `PostOffice` / `Save`
- **Binary:** `exefs.elf` (ARMv6K, CTR)
- **Status:** `done` (Byte-exact decompilation and full register verification)

---

## 📋 Summary

`PostOffice_UpdateMonthlyInterestAndSavingsRewards` processes bank interest accumulation and savings rewards for a player.
Specifically, it:
1. `[TOOL]` Reads the player's bank account structure at `param_1 + 0x6B8C`.
2. `[TOOL]` Checks the current achieved reward tier from `param_1 + 0x571E` (bits [4:1]) and granted tier (bits [7:5] of `0x571E` + bit [0] of `0x571F`).
3. `[TOOL]` Compares balance divided by 10,000 against the 8 threshold values in table `0x0083D838` (10, 50, 100, 500, 1000, 2000, 5000, 10000).
4. `[TOOL]` If a new savings milestone tier is reached, prepares and delivers a special system letter `"Mail_SP_Postoffice"` containing the milestone reward item from table `0x0083D848` (`0x2CA2`, `0x2CE0`, `0x2CA3`, `0x2CA5`, `0x2E2C`, `0x2C3E`, `0x287B`, `0x2CA4`).
5. `[TOOL]` Reads current date from `GetTimeSingletonPrimary()`, normalizes it via `NormalizeDate`, and compares against the last processed timestamp (`local_2C0`, `uStack_2BC`).
6. `[TOOL]` Computes elapsed months: $\Delta M = (\text{month} + 12 \times \text{year}) - (\text{lastMonth} + 12 \times \text{lastYear})$.
7. `[TOOL]` For each elapsed month ($\Delta M \ge 1$), accrues interest at 0.5% (`0.005f` at literal `0x0062FBC8`):
   $$\text{interest} = \lfloor \text{balance} \times 0.005 \rfloor$$
8. `[TOOL]` Clamps total accrued interest for the cycle to a maximum of 99,999 Bells (`0x1869F` at literal `0x0062FBD0`).
9. `[TOOL]` Deposits accrued interest via `FUN_00305AD8` (`Player_Abd_AddInterest`), which clamps total balance to 999,999,999 Bells.
10. `[TOOL]` Sends notice letter `"Mail_SP_Postoffice"` (template index 1) via `Mail_CreateSpecialLetter` (`0x005CB34C`).

---

## 🔬 Decompiled Implementation

> [!IMPORTANT]
> Literal pseudocode produced by Ghidra MCP `decompile_function` for `0x0062F7D0`.

```c
void PostOffice_UpdateMonthlyInterestAndSavingsRewards(int param_1,undefined4 param_2)

{
  uint uVar1;
  uint uVar2;
  undefined4 uVar3;
  int iVar4;
  int *piVar5;
  undefined4 uVar6;
  uint uVar7;
  int iVar8;
  bool bVar9;
  uint in_fpscr;
  float fVar10;
  int local_2dc;
  int local_2d8;
  int local_2d4;
  undefined1 auStack_2d0 [16];
  int local_2c0;
  undefined4 uStack_2bc;
  undefined4 uStack_2b8;
  undefined1 auStack_50 [4];
  undefined1 auStack_4c [4];
  int local_48;
  undefined1 auStack_3c [8];
  
  if (param_1 != 0) {
    uVar7 = (uint)(*(byte *)(param_1 + 0x571e) >> 5) + (*(byte *)(param_1 + 0x571f) & 1) * 8;
    FUN_002fc978(&DAT_0094c744,&DAT_0095afdc);
    local_48 = param_1 + 0x55a6;
    iVar8 = param_1 + 0x6b8c;
    if (uVar7 != 8) {
      uVar1 = FUN_00303700(iVar8);
      uVar2 = 0;
      do {
        bVar9 = uVar1 / 10000 <= (uint)(ushort)(&DAT_0083d838)[uVar2];
        if ((uint)(ushort)(&DAT_0083d838)[uVar2] <= uVar1 / 10000) {
          uVar2 = uVar2 + 1;
          bVar9 = 7 < uVar2;
        }
      } while (!bVar9);
      uVar1 = (*(byte *)(param_1 + 0x571e) & 0x1f) >> 1;
      if ((uVar1 < uVar2) && (uVar7 == uVar1)) {
        uVar2 = uVar1 + 1;
        uVar1 = uVar1 + 1;
        if (8 < uVar2) {
          uVar2 = 8;
        }
        *(byte *)(param_1 + 0x571e) =
             *(byte *)(param_1 + 0x571e) & 0xe1 | (byte)((uVar2 & 0xf) << 1);
      }
      if (8 < uVar1) {
        uVar1 = 8;
        *(byte *)(param_1 + 0x571e) = *(byte *)(param_1 + 0x571e) & 0xe1 | 0x10;
      }
      if (uVar1 != uVar7) {
        FUN_002ff264(auStack_2d0);
        FUN_002fcc14(auStack_4c,*(undefined2 *)(&UNK_0083d846 + uVar1 * 2));
        FUN_002fc978(&DAT_0094c744,auStack_4c);
        uVar3 = FUN_002fcc14(auStack_3c,0x227e);
        iVar4 = FUN_005cb34c(auStack_2d0,"Mail_SP_Postoffice",uVar1 + 1,local_48,uVar3,0xb,
                             FUN_0016c480);
        if (iVar4 != 0) {
          Villager_SetGiftExchangeStatus(auStack_2d0,auStack_4c,0);
          iVar4 = FUN_002ff3d0(auStack_2d0,8,2,0);
          if (iVar4 != 0) {
            uVar7 = (uint)(*(byte *)(param_1 + 0x571e) >> 5) + (*(byte *)(param_1 + 0x571f) & 1) * 8
            ;
            uVar2 = uVar7 + 1;
            if (uVar2 < 9) {
              uVar7 = uVar7 + 1;
            }
            if (8 < uVar2) {
              uVar7 = 8;
            }
            *(byte *)(param_1 + 0x571e) = *(byte *)(param_1 + 0x571e) & 0x1f | (byte)(uVar7 << 5);
            *(byte *)(param_1 + 0x571f) =
                 *(byte *)(param_1 + 0x571f) & 0xfe | (byte)((uVar7 & 8) >> 3);
          }
        }
        FUN_002f77dc(auStack_2d0);
      }
    }
    FUN_0056b4b4(&local_2c0);
    piVar5 = (int *)GetTimeSingletonPrimary();
    local_2dc = *piVar5;
    local_2d8 = piVar5[1];
    local_2d4 = piVar5[2];
    NormalizeDate(&local_2dc);
    uVar3 = FUN_00303700(iVar8);
    fVar10 = (float)VectorUnsignedToFloat(uVar3,(byte)(in_fpscr >> 0x16) & 3);
    DAT_0094c730 = 0;
    if ((((uint)(0.0 < fVar10 * 0.005) * (int)(fVar10 * 0.005) != 0) &&
        (iVar4 = FUN_0056b748(), iVar4 == 0)) &&
       (iVar4 = FUN_002fb4ac(local_2c0,uStack_2bc,uStack_2b8,local_2dc,local_2d8,local_2d4),
       iVar4 == 0)) {
      iVar4 = ((int)(char)local_2d8 + local_2dc * 0xc) - ((int)(char)uStack_2bc + local_2c0 * 0xc);
      if (iVar4 < 1) {
        bVar9 = false;
      }
      else {
        bVar9 = true;
        do {
          uVar3 = FUN_00303700(iVar8);
          iVar4 = iVar4 + -1;
          fVar10 = (float)VectorUnsignedToFloat(uVar3,(byte)(in_fpscr >> 0x16) & 3);
          DAT_0094c730 = (uint)(0.0 < fVar10 * 0.005) * (int)(fVar10 * 0.005) + DAT_0094c730;
          if (99999 < DAT_0094c730) {
            DAT_0094c730 = 99999;
          }
        } while (0 < iVar4);
      }
      if (bVar9) {
        FUN_00305ad8(param_1,DAT_0094c730);
        uVar3 = FUN_002fd0f8();
        FUN_006b566c(uVar3,param_2,DAT_0094c730);
        uVar3 = FUN_00303700(iVar8);
        uVar6 = FUN_002fd0f8();
        FUN_006b7510(uVar6,param_2,uVar3);
        FUN_002ff264(auStack_2d0);
        uVar3 = FUN_002fcc14(auStack_50,0x227e);
        iVar8 = FUN_005cb34c(auStack_2d0,"Mail_SP_Postoffice",1,local_48,uVar3,0xb,FUN_001656b4);
        if (iVar8 != 0) {
          FUN_002ff3d0(auStack_2d0,2,1,0);
        }
        FUN_002f77dc(auStack_2d0);
      }
    }
    return;
  }
  return;
}
```

---

## 🔢 Verified Binary Constants and Tables

| Property | Value | Source | Description |
|---|---|---|---|
| Bank Account Offset | `+0x6B8C` | `[TOOL]` line 27 | Offset in `PlayerSaveData` |
| Savings Flag Byte 1 | `+0x571E` | `[TOOL]` line 25 | Bits [4:1] = reached tier, [7:5] = granted tier [2:0] |
| Savings Flag Byte 2 | `+0x571F` | `[TOOL]` line 25 | Bit [0] = granted tier [3] |
| Threshold Table | `0x0083D838` | `[TOOL]` line 31 | 8 uint16 values: `{10, 50, 100, 500, 1000, 2000, 5000, 10000}` |
| Threshold Multiplier | `10,000` | `[TOOL]` line 31 | Division by 10,000 before comparing with table |
| Reward Items Table | `0x0083D848` | `[TOOL]` line 52 | `{0x2CA2, 0x2CE0, 0x2CA3, 0x2CA5, 0x2E2C, 0x2C3E, 0x287B, 0x2CA4}` |
| Interest Rate | `0.005f` (0.5%) | `[TOOL]` line 83 | Single-precision IEEE float `0x3BA3D70A` at `0x0062FBC8` |
| Max Monthly Interest | `99,999` | `[TOOL]` line 85 | Hardcoded clamp `0x0001869F` at `0x0062FBD0` |
| Max Account Balance | `999,999,999` | `[TOOL]` in `0x00612CD4` | Clamped balance limit inside `Player_Abd_DepositClamped` |
| Post Notice String | `"Mail_SP_Postoffice"` | `[TOOL]` lines 55, 96 | System letter identifier |

---

## 🔗 Called Functions

- `[TOOL]` `Player_Abd_GetBalance` (`0x00303700`): Decrypts current bank balance.
- `[TOOL]` `Player_Abd_AddInterest` (`0x00305AD8`): Deposits interest into player bank account.
- `[TOOL]` `Save_Mail_ConstructLetter640B` (`0x002FF264` / `0x002FF304`): Allocates / initializes `MailData` buffer.
- `[TOOL]` `Mail_CreateSpecialLetter` (`0x005CB34C`): Fills special template letter.
- `[TOOL]` `GetTimeSingletonPrimary` / `NormalizeDate`: Date calculation.

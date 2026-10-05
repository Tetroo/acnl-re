# Function: Town_CalculatePlayerRemainingDebt (0x00715550)

## 1. Metadata
- **Address:** `0x00715550`
- **Subsystem:** `Town` / `Save`
- **Status:** `done`
- **Source Binary:** `tools/ctr-elf/output/exefs.elf` (EUR Multi5 Welcome amiibo, SHA-256 `43b71edf...`)

---

## 2. Description & Summary
`Town_CalculatePlayerRemainingDebt` iterates over the player's 6 house room save records (`0x302` bytes each), accumulates the total mortgage debt from table `0x0088DED0` according to the current expansion tier of each room, adds the Welcome amiibo **Secret Storeroom** mortgage (158,000 Bells) if unlocked (`param_2 + 0x5727`), subtracts the 10,000 Bell down payment if applicable (`param_2 + 0x5704`), and deducts all repayments made to date via `Player_GetTotalLoanRepayments(param_2)`.

The final remaining mortgage balance is cached in the active session object `*(uint *)(iVar2 + 0xc)` and returned.

---

## 3. Disassembly & Key Constants `[TOOL]`
- Table base: `&DAT_0088decc` (offset `0x0088ded0`, stride 20 bytes = 5 x 4 bytes per room)
- Room save struct stride: `0x302` bytes (770 bytes)
- Room level byte offset: `+0x36` within room record
- Secret Storeroom fee: `0x26930` = **158,000 Bells** (`*(char *)(param_2 + 0x5727) < '\0'`)
- Down payment credit: `10,000 Bells` (`0x2710`) (`*(byte *)(param_2 + 0x5704) << 0x1b >= 0`)

---

## 4. Decompiled Implementation `[TOOL]`
```c
uint Town_CalculatePlayerRemainingDebt(int param_1, int param_2)
{
  byte bVar1;
  int iVar2;
  int *piVar3;
  uint uVar4;
  int *piVar5;
  uint total_debt;
  uint room_idx;
  int iVar8;
  int *piVar9;
  
  total_debt = 0;
  room_idx = 0;
  do {
    iVar8 = param_1 + room_idx * 0x302;
    iVar2 = FUN_0071f3d4(iVar8 + 0x18); // Checks if room is active (status != 1 && status != 5)
    if (iVar2 != 0) {
      bVar1 = *(byte *)(iVar8 + 0x36); // Room expansion level
      piVar3 = (int *)(&UNK_0088dec8 + room_idx * 0x14);
      if ((bVar1 & 1) == 0) {
        total_debt = total_debt + (&DAT_0088decc)[room_idx * 5];
        piVar3 = &DAT_0088decc + room_idx * 5;
      }
      iVar2 = 0;
      for (uVar4 = bVar1 + 1 >> 1; uVar4 != 0; uVar4 = uVar4 - 1) {
        piVar5 = piVar3 + 1;
        piVar9 = piVar3 + 2;
        piVar3 = piVar3 + 2;
        total_debt = total_debt + *piVar5;
        iVar2 = iVar2 + *piVar9;
      }
      total_debt = total_debt + iVar2;
    }
    room_idx = room_idx + 1;
  } while (room_idx < 6);

  // Welcome amiibo Secret Storeroom mortgage: 158,000 Bells
  if (*(char *)(param_2 + 0x5727) < '\0') {
    total_debt = total_debt + 158000; // 0x26930
  }

  // Down payment credit (10,000 Bells)
  if ((-1 < (int)((uint)*(byte *)(param_2 + 0x5704) << 0x1b)) && (9999 < total_debt)) {
    total_debt = total_debt - 10000;
  }

  // Deduct repayments
  room_idx = Player_GetTotalLoanRepayments(param_2);
  if (room_idx <= total_debt) {
    iVar2 = Player_GetTotalLoanRepayments(param_2);
    total_debt = total_debt - iVar2;
  }

  // Cache remaining debt in session manager
  iVar2 = FUN_006c8218();
  *(uint *)(iVar2 + 0xc) = total_debt;
  return total_debt;
}
```

---

## 5. Verification Checklist
- [x] Literal value `0x26930` = 158,000 Bells byte-exact for Welcome amiibo secret storeroom
- [x] Stride `0x302` (770 bytes) matches room save record length
- [x] 6-room loop strictly matches 6 house rooms

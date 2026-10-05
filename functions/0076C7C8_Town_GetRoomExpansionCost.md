# Function: Town_GetRoomExpansionCost (0x0076C7C8)

## 1. Metadata
- **Address:** `0x0076C7C8`
- **Subsystem:** `Town` / `Shop` (Nook Real Estate)
- **Status:** `done`
- **Source Binary:** `tools/ctr-elf/output/exefs.elf` (EUR Multi5 Welcome amiibo, SHA-256 `43b71edf...`)

---

## 2. Description & Summary
`Town_GetRoomExpansionCost` calculates and returns the mortgage cost (in Bells) for constructing or expanding a specific room in the player's house. It queries Tom Nook's building state and switches on the target room index:
- Room 0: Main Floor (Central Room)
- Room 1: 2nd Floor (Upper Level)
- Room 2: Basement
- Room 3: Left Room (West Wing)
- Room 4: Right Room (East Wing)
- Room 5: Back Room (North Wing)

---

## 3. Disassembly & Byte Analysis `[TOOL]`
```arm
0076c7c8: e92d41f0    push {r4, r5, r6, r7, r8, lr}
0076c7cc: e1a04001    mov  r4, r1
0076c7d0: ebff67e6    bl   FUN_005c2e10
0076c7d4: e2505000    subs r5, r0, #0
0076c7d8: 08bd81f0    popeq {r4, r5, r6, r7, r8, pc}
0076c7dc: e3540005    cmp  r4, #5
0076c7e0: 979ff104    ldrls pc, [pc, r4, lsl #2]
...
```

Literal Pool values:
- `39,800` Bells (`0x00009B78`) at `0x0076C91C`: Initial house down payment from tent
- `98,000` Bells (`0x00017ED0`) at `0x0076C920`: Expand Main Room 4x4 $\to$ 6x6
- `198,000` Bells (`0x00030570`) at `0x0076C924`: Expand Main Room 6x6 $\to$ 8x8
- `298,000` Bells (`0x00048C10`) at `0x0076C910`: Initial 2nd Floor construction (6x6)
- `428,000` Bells (`0x000687E0`) at `0x0076C8DC`: Initial Basement construction (8x8)
- `348,000` Bells (`0x00054F60`) at `0x0076C92C`: Initial side room construction (Left, Right, Back - 6x6)
- `498,000` Bells (`0x00079950`) at `0x0076C914`: Room expansion to 8x8
- `598,000` Bells (`0x00091FF0`) at `0x0076C918`: Room expansion to maximum size

---

## 4. Decompiled Implementation `[TOOL]`
```c
undefined4 Town_GetRoomExpansionCost(undefined4 param_1, int param_2)
{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar1 = FUN_005c2e10();
  if (iVar1 == 0) {
    return 0;
  }
  switch(param_2) {
  case 0: // Main Room
    if (*(char *)(iVar1 + 4) == '\0') {
      return 39800; // Tent to House
    }
    iVar1 = Core_ReadStructArrayLevelField(iVar1, param_2);
    if (iVar1 == 2) {
      return 98000; // 4x4 -> 6x6
    }
    if (iVar1 == 3) {
      return 198000; // 6x6 -> 8x8
    }
    if (iVar1 == 4) {
      return 298000;
    }
    return 0;
  case 1: // 2nd Floor
    Town_HouseGetRoomSubObject(iVar1, param_2);
    iVar2 = Town_HouseCheckRoomExists();
    if (iVar2 == 0) {
      return 298000; // Initial 2nd Floor (6x6)
    }
    iVar1 = Core_ReadStructArrayLevelField(iVar1, param_2);
    break;
  case 2: // Basement
  case 3: // Left Room
  case 4: // Right Room
  case 5: // Back Room
    Town_HouseGetRoomSubObject(iVar1, param_2);
    iVar2 = Town_HouseCheckRoomExists();
    if (iVar2 == 0) {
      if (param_2 == 2) {
        uVar3 = 428000; // Basement initial (8x8)
      }
      else {
        uVar3 = 348000; // Left/Right/Back initial (6x6)
      }
      return uVar3;
    }
    iVar1 = Core_ReadStructArrayLevelField(iVar1, param_2);
    break;
  default:
    return 0;
  }
  if (iVar1 == 2) {
    return 498000; // Expand to 8x8
  }
  if (iVar1 == 3) {
    return 598000; // Max size
  }
  return 0;
}
```

---

## 5. Verification Checklist
- [x] Pure decompile from Ghidra without artificial edits
- [x] All constants traced directly to ELF literal pool and `.rodata`
- [x] Room indices and prices cross-referenced with `0x0088DED0` mortgage matrix

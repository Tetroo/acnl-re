---
address: 0x00756F90
tags: [save-system, checksum, validation, crc32, field, town, acres, confirmed]
status: fully understood — Field/Town Region Checksum Validator
---

# [[FUN_00756f90]] — ValidateSubObjectChecksums

## ⭐ KEY FINDING

**This function verifies the CRC32 integrity of all save sub-objects on load, specifically verifying the Field/Town region containing `TownAcres[7×6]` (`0x53484`).**

- Returns `1` if all regions pass checksum validation, `0` if corrupted.
- Checks Field/Town payload `[0x53428, 0x71900)` against the 4-byte CRC at `0x53424`.

---

## Decompiled Code

```c
undefined4 ValidateSubObjectChecksums(int param_1, undefined4 param_2) // param_1 = save_buffer + 0x80
{
  byte bVar1;
  int iVar2;
  uint uVar3;
  undefined4 uVar4;
  
  iVar2 = FUN_00756d38();
  uVar4 = 0;
  if (((((iVar2 != 0) && (iVar2 = FUN_00719328(param_1 + 0x20, param_2), uVar4 = 0, iVar2 != 0)) &&
       (iVar2 = FUN_00119874(param_1 + 0x29220, param_2), uVar4 = 0, iVar2 != 0)) &&
      ((iVar2 = FUN_007582d8(param_1 + 0x4be00, param_2), uVar4 = 0, iVar2 != 0 &&
       (iVar2 = FUN_0071ca00(param_1 + 0x502bc), uVar4 = 0, iVar2 != 0)))) &&
     (iVar2 = FUN_0071ee80(param_1 + 0x52bb0), uVar4 = 0, iVar2 != 0)) {
     
    // Town Name flags check at +0x6214e (+0x80 = 0x621ce)
    bVar1 = *(byte *)(param_1 + 0x6214e);
    uVar3 = FUN_00301514();
    if (((bVar1 >> 4 == uVar3) &&
        (uVar3 = FUN_005d37e0(*(byte *)(param_1 + 0x6214e) & 0xf),
        uVar3 == *(byte *)(param_1 + 0x6214e) >> 4)) &&
        
       // Recalculate Field/Town CRC32 and verify equality with stored field:
       (iVar2 = CalculateCrc32_SaveBuffer(param_1 + 0x533a8, 0x1e4d8, 0xffffffff),
       *(int *)(param_1 + 0x533a4) == iVar2)) {
      uVar4 = 1;
    } else {
      uVar4 = 0;
    }
  }
  return uVar4;
}
```

---

## Callers

- Called from [[FUN_00756c54]] (`DeserializeSaveBuffer_ValidateChecksums`).
- Upstream caller: [[FUN_001d3d3c]] (`DispatchSaveLoadStateMachine`).

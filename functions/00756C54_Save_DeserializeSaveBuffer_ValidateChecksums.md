---
address: 0x00756C54
tags: [save-system, deserializer, validation, crc32, confirmed]
status: fully understood — Save Buffer Integrity Validator
---

# [[FUN_00756c54]] — DeserializeSaveBuffer_ValidateChecksums

## Summary

Top-level save deserializer validator. Called right after reading and XOR-decrypting `garden_plus.dat` in [[FUN_001d3d3c]] (`DispatchSaveLoadStateMachine`).

Verifies the integrity of all save regions by calling [[FUN_00756f90]] (`ValidateSubObjectChecksums`) and [[FUN_0070dfd0]]. Returns `1` on valid checksums, `0` on corruption.

## Decompiled Code

```c
undefined4 DeserializeSaveBuffer_ValidateChecksums(int save_buffer, undefined4 param_2)
{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = ValidateSubObjectChecksums(save_buffer + 0x80);
  uVar2 = 0;
  if ((iVar1 != 0) && (iVar1 = FUN_0070dfd0(save_buffer + 0x71900, param_2), uVar2 = 0, iVar1 != 0)) {
    uVar2 = 1;
  }
  return uVar2;
}
```

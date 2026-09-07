---
address: 0x00756F20
tags: [save-system, checksum, crc32, field, town, acres, confirmed]
status: fully understood — Field/Town Region Checksum Serializer
---

# [[FUN_00756f20]] — CalculateSubObjectChecksums

## ⭐ KEY FINDING

**This function calculates and writes the CRC32 checksum for the entire Field/Town region containing `TownAcres[7×6]` (`0x53484`) before saving.**

- **Target Region:** `buffer + 0x53428` through `0x71900`
- **Region Size:** `0x1E4D8` (124,120 bytes)
- **Checksum Storage:** `buffer + 0x53424` (4 bytes, preceding payload)
- **Algorithm:** Standard CRC32 via [[FUN_00319340]] (`CalculateCrc32_SaveBuffer`)

---

## Decompiled Code

```c
void CalculateSubObjectChecksums(int param_1) // param_1 = save_buffer + 0x80
{
  undefined4 uVar1;
  
  FUN_00756d14();
  FUN_007192c8(param_1 + 0x20);      // offset +0xa0
  FUN_00119858(param_1 + 0x29220);   // offset +0x292a0 (Villager array CRC)
  FUN_007582b0(param_1 + 0x4be00);   // offset +0x4be80
  FUN_0071c9dc(param_1 + 0x502bc);   // offset +0x5033c
  FUN_0071ee60(param_1 + 0x52bb0);   // offset +0x52c30
  
  // Field/Town region CRC calculation:
  uVar1 = CalculateCrc32_SaveBuffer(param_1 + 0x533a8, 0x1e4d8, 0xffffffff);
  *(undefined4 *)(param_1 + 0x533a4) = uVar1;
  return;
}
```

---

## Structure Derivation

1. `param_1` is called from [[FUN_00756c34]] with `save_buffer_base + 0x80`.
2. `param_1 + 0x533a4` = `0x80 + 0x533a4 = 0x53424` (CRC32 field).
3. `param_1 + 0x533a8` = `0x80 + 0x533a8 = 0x53428` (Data payload start).
4. `0x53428 + 0x1e4d8 = 0x71900` (Exact byte boundary of the Small Map State object).
5. The container object [[FUN_002d457c]] (`ConstructFieldTownObject`) is constructed at `buffer + 0x53430`.
6. `TownAcres[7×6]` sits at `buffer + 0x53484`, covered directly by this CRC32 block!

---

## Callers

- Called from [[FUN_00756c34]] (`SerializeSaveBuffer_CalculateChecksums`).
- Counterpart validator: [[FUN_00756f90]] (`ValidateSubObjectChecksums`).

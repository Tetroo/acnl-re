---
address: 0x00611D34
name: SetDefaultAcreId_109
tags: [town, acres, confirmed]
status: fully understood
---

# [[FUN_00611d34|SetDefaultAcreId_109]] — Set Default Acre ID (0x109)

## Summary

Simple setter. Sets a `uint16_t` Acre ID to `0x109` (decimal 265, standard empty border acre ID).

Called in a loop from [[FUN_00612820|InitTownGridAndAcres]] across all 42 entries of the $7 \times 6$ `TownAcres` array.

## Decompiled Code

```c
void SetDefaultAcreId_109(undefined2 *param_1)
{
  *param_1 = 0x109;
  return;
}
```

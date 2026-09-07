---
address: 0x0076dfac
name: VerifySaveFileCrc32
status: partial — verified against live decompile, caller sites for other file types not confirmed
tags: [save-system, checksum, crc32]
decompiled_at: Session 22
verified_at: audit-2026-09-07 (fork)
---

# [[FUN_0076dfac|VerifySaveFileCrc32]] — Save File CRC32 Verifier

## Summary

Verifies the CRC32 checksum of a save-file buffer.

## Decompiled Implementation (verbatim from Ghidra, 2026-09-07)

```c
bool VerifySaveFileCrc32(int *param_1)
{
  int iVar1;

  iVar1 = Save_CalculateCrc32_SaveBuffer(param_1 + 1, 0x25f8c, 0xffffffff);
  return *param_1 == iVar1;
}
```

## Buffer Layout Expected

```
[+0x00]  uint32  stored_checksum
[+0x04]  ...     0x25f8c (155,020) bytes of data
```

`Save_CalculateCrc32_SaveBuffer` computes CRC32 over the data with initial value `0xFFFFFFFF`. Returns `true` if the stored and computed checksums match.

> [!IMPORTANT] Correction (audit 2026-09-07)
> The previous version of this doc claimed the data length came from a variable `DAT_0076dfdc` "not yet read from ELF" and that it "varies per file type". That is **wrong** — the real decompile shows a hardcoded immediate `0x25f8c` baked into *this* call site. There is no evidence in this function of a variable/shared length symbol. If other file types (mail, friends, mvplayer) use a differently-sized call to the same `Save_CalculateCrc32_SaveBuffer`, each call site will have its own hardcoded length — this has NOT been individually verified per caller (see Callers section, unconfirmed).

## Callers

- [[FUN_001d3d3c]] state `0x06` — called after reading `/mydsgn*.dat`: `FUN_0076dfac(*DAT_001d43e0, 1)`

## Related

- `FUN_0076de58` — likely same pattern, used for mail files (state 0x08)
- `FUN_0071c4a4` — called after friend files (state 0x0A)
- `FUN_0070a7ec` — called after mvplayer files (state 0x0C)
- `FUN_00319340` — CRC32 implementation

## See Also

- [[FUN_001d3d3c]] — dispatcher that calls this post-read

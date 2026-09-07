---
address: 0x001b9428
name: CheckSaveMount
status: partially decompiled
tags: [save-system, filesystem, mount-check]
decompiled_at: Session 20
---

# [[FUN_001b9428|CheckSaveMount]] — Save Mount Check

## Summary

Pre-flight check before reading `garden_plus.dat`. Verifies the save archive is accessible and mounts `"data:"` if needed. Returns 1 on success, 8 on error.

## Decompiled Logic

```c
int FUN_001b9428(int param_1)
{
    if (param_1 != 0) {
        FUN_005c96a0();
        uVar1 = FUN_005c9d64();
        iVar2 = FUN_001b8e6c(*DAT_001b94c8, 0, uVar1, 8, 0);
        if (iVar2 != 1) return iVar2;   // propagate error
    }

    if ((*DAT_001b94cc == 0x01) && (DAT_001b94cc[1] != 0)) {
        if (param_1 != 0) FUN_002fb34c();
        iVar2 = FUN_00344d18("data:");  // mount "data:" archive
        if (iVar2 < 0) return 8;
    }
    return 1;
}
```

> [!IMPORTANT]
> `"data:"` is the 3DS virtual filesystem prefix for ExtData/SaveData via nn::fs.  
> `param_1` is a flag passed from [[FUN_001d3748]] derived from bit 30 of the mode field: `~(uVar2 >> 0x1e) & 1`.

## Sub-functions

| Function | Role |
|----------|------|
| `FUN_005c96a0` | Unknown pre-check |
| `FUN_005c9d64` | Returns some environment value |
| `FUN_001b8e6c` | Initiates load process (returns status) |
| `FUN_00344d18` | nn::fs mount — mounts `"data:"` archive |

## See Also

- [[FUN_001d3748]] — Save state machine; calls this with the mount flag
- [[FUN_001b927c]] — Lower-level archive mount (different path)

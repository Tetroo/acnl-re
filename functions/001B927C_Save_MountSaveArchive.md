---
address: 0x001b927c
name: MountSaveArchive
status: partially decompiled
tags: [save-system, filesystem, nn-fs, mount]
decompiled_at: Session 20
---

# [[FUN_001b927c|MountSaveArchive]] — Save Archive Mount

## Summary

Mounts the save data archive via `nn::fs` using the `"data:"` virtual drive. Sets a global busy flag before mounting. Returns a status code.

## Decompiled Logic

```c
int FUN_001b927c(int param_1, int param_2, int param_3)
{
    *DAT_001b9408 = 1;  // set busy flag

    if (no_existing_mount && param_3 == 0) {
        FUN_00344c4c("data:");          // mount ExtData/SaveData via nn::fs
    } else {
        FUN_00344cc8(&DAT_001b9410, DAT_001b940c);  // mount with explicit params
    }

    // nn::fs error code decoding: field [10:18] = module, field [0:10] = code
    // module == 0x11 (nn::fs):
    //   code 0x154–0x167 → return 6  (not found / no access)
    //   code range B     → return 7
    //   code 0x64–0xB3  → return 2  (corrupt save?)
    // Success            → DAT_001b9408[1] = 1, return 1
    // param_1 == 0 after error → return 3
}
```

> [!IMPORTANT]
> `DAT_001b940c` — likely holds the save ID or ExtData ID for ACNL's save partition.  
> Return code 2 (nn::fs codes 0x64–0xB3) is the probable corrupt-save detection path.

## Return Codes

| Code | Meaning |
|------|---------|
| 1 | Success — archive mounted |
| 2 | Corrupt / unreadable save |
| 3 | Error with param_1 == 0 |
| 6 | Save not found / no access |
| 7 | Other nn::fs error |

## Open Questions

- Exact semantics of `param_1`, `param_2`, `param_3` not confirmed
- `FUN_00344c4c` / `FUN_00344cc8` — nn::fs mount wrappers, not individually documented
- `DAT_001b940c` value (ExtData ID) — not yet read from ELF

## See Also

- [[FUN_001d3748]] — Save state machine dispatcher; calls this function
- [[FUN_001b8b9c]] — Low-level file reader; called after successful mount

---
address: 0x006e1d70
name: NetworkSessionObjectHandler
status: partially decompiled
tags: [save-system, network, multiplayer]
decompiled_at: Session 23
---

# [[FUN_006e1d70|NetworkSessionObjectHandler]] — Network/Session Object Handler

## Summary

Reads fields from a large object at `param_1` (offsets 0x2768–0x27c6) and interacts with network-layer functions. Calls [[FUN_00300fdc]] and `FUN_005c9a24` only inside a conditional branch, suggesting the save buffer access is incidental (e.g. sync on connection established).

```c
undefined4 FUN_006e1d70(int param_1)
{
    // Reads: param_1+0x2768, +0x2788, +0x278c, +0x276c, +0x27a4
    int iVar2 = FUN_0050c268(DAT_006e1f80,
                              *(param_1+0x2768), *(param_1+0x2788),
                              *(param_1+0x278c), *(param_1+0x276c),
                              DAT_006e1f7c, *(param_1+0x2768),
                              (int*)(param_1+0x27a4));
    if (iVar2 == 0) {
        // network check path
        if (FUN_00745fa4(DAT_006e1f80) == 0) return 0;
        uint uVar4 = FUN_00745f14(DAT_006e1f80);
        if (uVar4 > 1 && FUN_00745ee8(DAT_006e1f80) != 0) {
            *(param_1+0x27a8) = FUN_00745ee8(DAT_006e1f80);
            *(param_1+0x27c6) = 1;
        }
    } else if (*(param_1+0x27a4) == DAT_006e1f7c) {
        // connection established branch
        if (FUN_00756cd8(...) && FUN_007581a8(...) && FUN_00756cf0(...)) {
            FUN_00300fdc();      // save buffer access
            FUN_005c9a24();
            *DAT_006e1f84 = FUN_002ff8f4(4);
            FUN_002fb894();     // non-returning (error/exit)
        }
    }
    return 0xffffffff;
}
```

## Notes

- `FUN_007xxxxx` calls suggest network/UDS layer (local wireless play)
- `FUN_002fb894` is non-returning — likely an error handler or graceful shutdown
- **Not a TownAcres/Villager reader** — save buffer accessed only in error/connection path
- param_1 is a session or connection object (~0x27c7 bytes minimum)

## Open Questions

- What triggers this? Multiplayer session join?
- What does `FUN_005c9a24` do with the save buffer?

## See Also

- [[FUN_00300fdc]] — save buffer getter
- `FUN_005c9a24` — buffer accessor (not yet documented)

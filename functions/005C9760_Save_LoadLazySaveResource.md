---
address: 0x005c9760
name: LoadLazySaveResource
status: partially decompiled
tags: [save-system, resource, lazy-init]
decompiled_at: Session 24
---

# [[FUN_005c9760|LoadLazySaveResource]] — Lazy Resource Loader (buffer accessor, not TownAcres)

## Summary

```c
void FUN_005c9760(void) {
    puVar1 = DAT_005c97b4;      // struct ptr
    if (DAT_005c97b4[1] == 0) { // lazy-init check: not yet loaded
        iVar3 = vtable_call(*DAT_005c97b8, +0x18)(*DAT_005c97b8, DAT_005c97bc, 0x80);
        puVar1[1] = iVar3;
        if (iVar3 != 0) {
            FUN_002fbb9c(iVar3, *puVar1, DAT_005c97bc);
        }
    }
}
```

Reads the save buffer pointer (`0x0095AF74`) as one of its inputs but the `0x18` vtable call + `0x80`-byte allocation pattern is a generic **lazy resource/heap loader**, not a direct field read. Not a TownAcres candidate.

## Open Questions

- What object's vtable is at `*DAT_005c97b8`? (heap/resource manager?)
- What does `FUN_002fbb9c` do with the loaded 0x80-byte block?

## See Also

- [[FUN_005c9a24]] — villager slot reader (sibling accessor, same DAT_005c9a38 buffer)

---
address: 0x0012cc9c
name: CleanupSaveDataBuffer
status: partially decompiled
tags: [save-system, garden-plus, distributor, structure]
decompiled_at: Session 22
---

# [[FUN_0012cc9c|CleanupSaveDataBuffer]] — Save Data Distributor

## Summary

**Destructor / cleanup function for the garden_plus.dat save buffer.** Checks if the buffer is loaded, calls C++ destructors on all objects within it (via `FUN_002f7740`), then clears the buffer pointer at `0x0095AF74`.

```c
void FUN_0012cc9c(void) {
    int* piVar1 = DAT_0012d0d0;   // = 0x0095AF74 (ptr to buffer ptr)
    int  iVar3  = *DAT_0012d0d0;  // = *(0x0095AF74) = actual buffer address
    if (iVar3 == 0) return;       // nothing loaded, bail

    // Navigate to section within save buffer
    iVar2 = FUN_0012e868(iVar3 + 0x88ab8);   // offset near end of 563 KB file
    iVar2 = FUN_0012e868(iVar2 + -0x290);

    // Call destructors on each C++ object array in the buffer (reverse order)
    FUN_002f7740(iVar2 + -0xca30,  DAT_0012d0d4, 0x280, 0x50);  // dtor for 80 items × 640 B
    FUN_002f7740(iVar2 + -0xe0b0,  DAT_0012d0d8, 0x5a0, 4);     // dtor for 4 items × 1440 B
    FUN_002f7740(iVar2 + -0x14ed0, DAT_0012d0e0, DAT_0012d0dc, 4);
    // ... [many more — see Field Offsets table]

    (**(code **)(*piVar4 + 0x1c))(piVar4, *piVar1);  // vtable dtor call
    *piVar1 = 0;  // CLEAR buffer ptr at 0x0095AF74
}
```

> [!IMPORTANT]
> `FUN_002f7740(base, fn_ptr, elem_size, count)` iterates **backwards** calling `fn_ptr(elem_addr)` for each element — classic C++ destructor pattern. The `DAT_0012d0d4/d8/e0...` are **function pointers to destructors**, not data destinations.

> [!IMPORTANT]
> This function CLEARS `0x0095AF74` at the end. The actual setter (who writes the buffer address into `0x0095AF74`) is [[FUN_005c9dd4]].

## Navigation Helpers

Before field distribution, the function walks the save buffer using:
- `FUN_0012e868` — likely alignment navigator (pointer arithmetic within save structure)
- `FUN_0012eaf0`, `FUN_0012e87c`, `FUN_0012eb4c` — similar navigation helpers
- `FUN_00135008`, `FUN_00132960`, `FUN_001325c4`, `FUN_00100a8c` — additional offset resolvers

The starting point `iVar3 + 0x88ab8` (= buffer + 559,288) points near the **end** of the 563,968-byte file, suggesting sections are located by working backwards.

## Field Offsets Observed (relative to computed `iVar2`)

All via `FUN_002f7740(src, dst_global, elem_size, count)`:

| Offset from iVar2 | Elem size | Count | Total bytes | Candidate |
|-------------------|-----------|-------|-------------|-----------|
| `+0x6add4` | 4 | 2 | 8 | ? |
| `+0x6adcc` | 4 | 2 | 8 | ? |
| `+0x6ad38` | 4 | 1 | 4 | ? |
| `+0x6acd2` | 0x18 | 2 | 48 | ? |
| `+0x6ac78` | 4 | 0x12 | 72 | ? |
| `+0x6ac4e` | 4 | 8 | 32 | ? |
| `+0x6ac3e` | 4 | 3 | 12 | ? |
| `+0x6689c` | 0x870 | 8 | 17,280 | **Villager structs?** (8×2160 B) |
| `+0x66834` | 4 | 8 | 32 | ? |
| `+0x66820` | 4 | 5 | 20 | ? |
| `+0x667dc` | 4 | 1 | 4 | ? |
| `+0x667cc` | 4 | 1 | 4 | ? |
| `+0x667ae` | 4 | 6 | 24 | ? |
| `+0x666b0` | 4 | 1 | 4 | ? |
| `+0x66676` | 4 | 0xb | 44 | ? |
| `+0x66638` | 4 | 7 | 28 | ? |
| `+0x6226c` | 4 | 0xd | 52 | ? |
| `+0x621f0` | 4 | 0x18 | 96 | ? |

Early offsets (relative to iVar2 before second navigation step):

| Offset | Elem size | Count | Total bytes | Candidate |
|--------|-----------|-------|-------------|-----------|
| `−0xca30` | 0x280 | 0x50 | 32,768 | **TownAcres / map data?** |
| `−0xe0b0` | 0x5a0 | 4 | 5,760 | **Player structs?** (4×1440 B) |
| `−0x14ed0` | DAT_0012d0dc | 4 | ? | ? |

## Open Questions

- `FUN_002f7740` signature: is it `(src, dst, size, count)` or `(dst, src, size, count)`? Needs verification.
- Exact base address of `iVar2` at runtime — determines absolute save-file offsets.
- Which offset holds TownAcres[42]? Best candidate: `−0xca30` (32 KB).
- 8 × 0x870 at `+0x6689c` — are these 8 villager slots (game has 10 total)?

## Callers

Unknown — xref search needed.

## See Also

- [[FUN_005c9d64]] — getter for the buffer ptr at `0x0095AF74`
- [[FUN_001b96c8]] — loads the raw file into the buffer before this runs
- `FUN_002f7740` — field copy helper (needs doc)
- `FUN_0012e868`, `FUN_0012eaf0` — save structure navigation helpers

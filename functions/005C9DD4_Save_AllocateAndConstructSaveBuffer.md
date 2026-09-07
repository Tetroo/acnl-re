---
address: 0x005c9dd4
name: AllocateAndConstructSaveBuffer
status: partially decompiled
tags: [save-system, garden-plus, constructor, allocator, structure]
decompiled_at: Session 22
---

# [[FUN_005c9dd4|AllocateAndConstructSaveBuffer]] — Save Buffer Constructor / Allocator

## Summary

Allocates the runtime save buffer, constructs C++ objects in-place, then initializes their fields. Paired with [[FUN_0012cc9c]] (destructor).

Only runs if `*0x0095AF74 == 0` (buffer not yet allocated).

```c
void FUN_005c9dd4(void) {
    piVar5 = DAT_005c9f30;   // = 0x0095AF74 (ptr to buffer ptr)
    if (*piVar5 != 0) return;  // already allocated

    // Allocate buffer via vtable +0x18 (heap alloc)
    iVar3 = (*vtable[+0x18])(heap_obj, DAT_005c9f34, 0x80);  // size 0x80 alignment?
    *piVar5 = iVar3;   // WRITE: store new buffer ptr to 0x0095AF74

    FUN_002f8280(iVar3, ...);  // constructor init
    if (iVar3 != 0) {
        // Construct C++ sub-objects at specific offsets:
        iVar3 = FUN_005ca4f4(iVar3 + 0x80);
        iVar3 = FUN_001e0340(iVar3 + 0x71880);
        iVar3 = FUN_002d3f54(iVar3 + 0x24);
        iVar3 = FUN_002a6b48(iVar3 + 0xbe8);
        // Forward-iterate constructors (FUN_00301d9c = constructor-iterator):
        iVar3 = FUN_00301d9c(iVar3 + 0x144c, DAT_005c9f3c, size, 4);
        iVar3 = FUN_00301d9c(iVar3 + 0x6e20, DAT_005c9f40, 0x5a0, 4);  // 4×1440B
        uVar6 = FUN_00301d9c(iVar3 + 0x1680, DAT_005c9f44, 0x280, 0x50); // 80×640B
        iVar3 = (int)uVar6;
        // more sub-object inits...
    }
    piVar5 = (int *)*piVar5;  // piVar5 now = buffer itself

    // Init header fields
    *piVar5     = <computed>;   // [0]
    piVar5[1]   = <computed>;   // [1]
    piVar5[2]   = 0;            // [2]
    FUN_005ca2c4(piVar5 + 0x20, ...);

    FUN_002d3f00(piVar5 + 0x1c649);

    // 4-player loop
    for (int i = 0; i < 4; i++) {
        FUN_006f4234(piVar5 + i * DAT_001df4e8 * 2 + 0x1ce56);
        FUN_006cbff8(piVar5 + i * 0x168 + 0x1e9de);
    }

    FUN_001e4418(piVar5 + 0x1ef7e);
    FUN_002a6604(piVar5 + 0x1c943);
    FUN_00279ea0(piVar5 + 0x22354);

    // Clear tail fields (near end of 563 KB buffer)
    piVar5[0x226ae] = 0;  piVar5[0x226af] = 0;
    piVar5[0x226b1] = 0;  piVar5[0x226b2] = 0;
    piVar5[0x226b3] = 0;
}
```

## Key Insight — Save Buffer Architecture

> [!IMPORTANT]
> `DAT_005c9f34 = 0x89B00 = 563,968` — the allocation size **equals the file size**. The buffer holds the entire `garden_plus.dat` content verbatim. No separate copy — C++ objects are constructed in-place within the same memory block.

`FUN_00301d9c(base, ctor_ptr, elem_size, count)` = forward constructor-iterator (mirror of `FUN_002f7740` which is the backward destructor-iterator).

## Save Buffer Structure — Confirmed Offsets

Offsets relative to buffer base (byte offsets):

| Byte offset | Size | Description |
|-------------|------|-------------|
| `0x00` | 4 | Header field [0] |
| `0x04` | 4 | Header field [1] |
| `0x08` | 4 | 0 |
| `0x80` | ? | Sub-object (`FUN_005ca4f4`) |
| `0x71900` | ? | Sub-object (`FUN_001e0340`) — large offset, likely map/acre region |
| `0x71924` | ? | Sub-object (`FUN_002d3f54`) |
| `0x7250c` | ? | Sub-object (`FUN_002a6b48`) |
| `0x72038` (`0x1c00e×4`) | ? | `FUN_002a6604` approx |
| **`0x73958`** | 7048 | **Player 0** primary (`FUN_006f4234`); stride = 7048 bytes (0x1B88) |
| **`0x75540`** | 7048 | **Player 1** primary |
| **`0x77128`** | 7048 | **Player 2** primary |
| **`0x78CB0`** | 7048 | **Player 3** primary |
| `0x7A778` (`0x1e9de×4`) | 0x168×4=1440 | Player 0 secondary (`FUN_006cbff8`) |
| `0x7A778 + 0x1B0×i` | 1440 | Player i secondary (stride 0x5A0) |
| `0x7BDF8` (`0x1ef7e×4`) | ? | `FUN_001e4418` |
| `0x88D50` (`0x22354×4`) | ? | `FUN_00279ea0` |
| `0x89AB8` (`0x226ae×4`) | 4 | 0 (tail field) |
| `0x89ACC` (`0x226b3×4`) | 4 | 0 (tail field — 0x54 bytes before file end) |

**Confirmed:** `DAT_001df4e8 = 881 (0x371)`. Player stride = `881 × 2 × 4 = 7048 bytes`.

## Open Questions

- `FUN_001e0340` at buffer+0x71880 — large sub-object, possible map/acre data section
- Exact layout within the 7048-byte player struct (from `FUN_006f4234`)
- 4 × 0x5a0 region at buffer+? (from `FUN_00301d9c`) — primary player struct? 
- Villager slots offset — not yet identified in this function
- `FUN_00301d9c` forward vs backward confirmed?

## See Also

- [[FUN_0012cc9c]] — paired destructor; clears `0x0095AF74`
- [[FUN_005c9d64]] — getter for `0x0095AF74`
- [[FUN_001b96c8]] — loads file into the buffer
- `FUN_00301d9c` — constructor-iterator (needs doc)
- `FUN_006f4234` — per-player init called in loop (needs doc)

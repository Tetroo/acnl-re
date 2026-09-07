---
address: 0x001b8b9c
name: ReadSaveFileRaw
status: partially decompiled
tags: [save-system, filesystem, io, nn-fs]
decompiled_at: Session 20
---

# [[FUN_001b8b9c|ReadSaveFileRaw]] — Save File Raw Reader

## Summary

Generic low-level file reader. Implements the full open → seek → read → verify → close cycle against the nn::fs API. Returns 1 on success, 8 on any failure.

## Inferred Signature

```c
int FUN_001b8b9c(path_obj param_1, int offset, void* buf, int size);
// returns: 1 = ok, 8 = error
```

## Execution Flow

```
1. FUN_001b85e8()              → file handle (0 = fail → return 8)
2. FUN_0053d728(handle, &ctx, &path_struct, 2)  → seek/open on path
3. FUN_00748480(...)           → unknown pre-read step
4. FUN_001b8a30(...)           → result check (1 = ok)
5. if (param_2 != 0):
       FUN_0053da34(&ctx, param_2, 0)  → seek to offset
6. FUN_0053da08(&ctx, out_buf, param_3, param_4)  → READ bytes
7. if (local_38[0] != param_4): → short read check → error
8. FUN_0053d9e8(&ctx)          → close / flush
9. FUN_0013ef5c(...)           → cleanup / destructor
```

> [!IMPORTANT]
> Step 7 (short-read check) is the integrity guard: if fewer bytes were read than requested, the function treats it as an error. This is the primary `garden_plus.dat` read primitive.

## Sub-functions (not individually documented)

| Function | Role |
|----------|------|
| `FUN_001b85e8` | File open — returns handle |
| `FUN_0053d728` | Seek/open on path struct |
| `FUN_0053da08` | Read bytes |
| `FUN_0053da34` | Seek to offset |
| `FUN_0053d9e8` | Close / flush |
| `FUN_0013ef5c` | Cleanup / destructor |

## See Also

- [[FUN_001d3d3c]] — Resource dispatcher; routes to this function for raw reads
- [[FUN_001b96c8]] — Thin wrapper that calls this with pre-set globals
- [[FUN_001b927c]] — Archive mount; must succeed before this is called

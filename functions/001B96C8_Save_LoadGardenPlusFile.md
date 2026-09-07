---
address: 0x001b96c8
name: LoadGardenPlusFile
status: fully understood
tags: [save-system, garden-plus, loader]
decompiled_at: Session 22
---

# [[FUN_001b96c8|LoadGardenPlusFile]] — garden_plus.dat File Reader

## Summary

Reads the entire `garden_plus.dat` save file into the runtime save buffer.

```c
void FUN_001b96c8(void)
{
    void* buf = FUN_005c9d64();   // get runtime buffer ptr from *0x0095AF74
    FUN_001b8b9c(*DAT_001b96f4,  // path ptr → *0x0095AF70 → "/garden_plus.dat"
                 0,               // offset = 0 (read from start)
                 buf,             // destination buffer
                 *DAT_001b96f0); // size = 0x89B00 = 563,968 bytes
}
```

## Confirmed Constants

| Symbol | Value | Meaning |
|--------|-------|---------|
| `DAT_001b96f4` | `0x0095AF70` | Pointer to the path pointer |
| `*0x0095AF70` | `0x00930480` | String `"/garden_plus.dat"` in .rodata |
| `DAT_001b96f0` | `0x0088D448` | Pointer to the size constant |
| `*0x0088D448` | `0x00089B00` | File size = **563,968 bytes** |

> [!IMPORTANT]
> The runtime buffer pointer is stored at `0x0095AF74` (.data), initialised to NULL, written by [[FUN_0012cc9c]] before this is called. [[FUN_005c9d64]] is the getter. The buffer holds the raw file bytes — no inline decryption happens here.

## Callers

- [[FUN_001d3d3c]] — state `uVar4 == 0x02`; called once per relevant tick until read completes

## See Also

- [[FUN_001b8b9c]] — raw file reader implementation
- [[FUN_005c9d64]] — garden_plus.dat buffer getter (`*0x0095AF74`)
- [[FUN_001b9428]] — pre-flight check; also calls [[FUN_005c9d64]]
- [[FUN_0012cc9c]] — allocates / sets the buffer pointer at `0x0095AF74`

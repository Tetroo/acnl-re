---
address: 0x005c9d64
name: GetGardenPlusBufferPointer
status: fully understood
tags: [save-system, garden-plus, getter, singleton]
decompiled_at: Session 22
---

# [[FUN_005c9d64|GetGardenPlusBufferPointer]] — garden_plus.dat Buffer Getter

## Summary

Trivial getter. Returns the current runtime pointer to the `garden_plus.dat` in-memory buffer.

```c
void* FUN_005c9d64(void)
{
    return *DAT_005c9d70;   // = *(0x0095AF74)
}
```

## Key Addresses

| Address | Segment | Role |
|---------|---------|------|
| `0x005c9d70` | .text (literal pool) | Holds value `0x0095AF74` |
| `0x0095AF74` | .data | Runtime ptr to save buffer; NULL in ELF, set by [[FUN_0012cc9c]] |

> [!IMPORTANT]
> `0x0095AF74` is adjacent in .data to other save-file path pointers:
> - `0x0095AF6C` → `/garden.dat` (base-game save)
> - `0x0095AF70` → `/garden_plus.dat` (Welcome Amiibo save path string)
> - `0x0095AF74` → runtime buffer ptr (this getter)

## Callers

| Address | Function | Context |
|---------|----------|---------|
| `0x001b96cc` | [[FUN_001b96c8]] | Gets buffer before calling [[FUN_001b8b9c]] |
| `0x001b9438` | [[FUN_001b9428]] | Pre-flight check |

## See Also

- [[FUN_001b96c8]] — reads garden_plus.dat into this buffer
- [[FUN_0012cc9c]] — writes the buffer pointer (allocator)
- [[FUN_005c9dd4]] — also reads/writes `0x0095AF74` (unknown role)

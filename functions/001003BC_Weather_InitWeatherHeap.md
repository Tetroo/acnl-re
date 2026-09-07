---
address: 0x001003BC
tags: [function, weather, heap, init]
status: understood
---
# [[FUN_001003BC]] — WeatherHeap Init

**Address:** `0x001003BC`  
**Section:** `.text`  
**Type:** Initialization  

---

## Signature
```c
undefined4 FUN_001003BC(undefined4 param_1);
```

## Decompiled
```c
undefined4 FUN_001003BC(undefined4 param_1)
{
    int iVar1;
    undefined4 uVar2;
    undefined1 auStack_18[12];

    uVar2 = FUN_002F6E48(auStack_18, "WeatherHeap");      // heap descriptor init
    uVar2 = FUN_002F6E5C(0x7600, uVar2, param_1, 1, 0);  // allocate 30208 bytes
    iVar1 = DAT_001003FC;
    *(undefined4 *)(DAT_001003FC + 0x10) = uVar2;         // store heap ptr at object+0x10
    FUN_002F6E70(auStack_18);                              // heap descriptor cleanup
    return *(undefined4 *)(iVar1 + 0x10);                 // return stored ptr
}
```

## Analysis

Allocates WeatherHeap (30208 bytes / `0x7600`). The pointer is **not** stored in a bare global — it is written into `DAT_001003FC + 0x10`, meaning WeatherHeap is a field at offset `+0x10` inside some parent object pointed to by `DAT_001003FC`. That parent object is likely a game-manager or scene-manager struct.

### Heap allocation pattern
`FUN_002F6E48` / `FUN_002F6E5C` / `FUN_002F6E70` appear to be a 3-part heap API:
- `48` — init a local heap descriptor
- `5C` — allocate from it (`size, descriptor, allocator?, flag, flag`)
- `70` — release the local descriptor

## Parameters

| | Type | Notes |
|-|------|-------|
| `param_1` | `undefined4` | Passed to allocator as 3rd arg — likely parent allocator or arena |

## Returns

Pointer to allocated WeatherHeap block.

## Writes

`DAT_001003FC + 0x10` = WeatherHeap pointer

## Calls

| Address | Role |
|---------|------|
| `FUN_002F6E48` | Heap descriptor init |
| `FUN_002F6E5C` | Heap allocate |
| `FUN_002F6E70` | Heap descriptor cleanup |

## Open Questions

- Who calls [[FUN_001003BC]]? Xrefs not yet traced.

## See also

[[FUN_001009E4]] — paired cleanup  
[[weather]] — system overview

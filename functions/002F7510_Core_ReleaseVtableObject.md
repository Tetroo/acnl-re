---
address: 0x002F7510
name: ReleaseVtableObject
tags: [function, state, helper]
status: understood
---

# [[FUN_002F7510|ReleaseVtableObject]] — Release vtable Object

## Signature
```c
void FUN_002f7510(int slot);
```

## Purpose

Releases an object stored in a slot by calling its destructor via vtable `+0x10`,
then nulls the pointer. Identical pattern to [[FUN_001009E4]] (WeatherHeap cleanup).
This is a generic vtable-object release helper.

## Decompiled
```c
void FUN_002f7510(int slot) {
    if (*(slot + 4) != NULL) {
        (*vtable[+0x10])();   // call destructor
        *(slot + 4) = 0;      // null the pointer
    }
}
```

## See Also

[[FUN_001009E4]], [[FUN_002F744C]], [[FUN_00100684]]

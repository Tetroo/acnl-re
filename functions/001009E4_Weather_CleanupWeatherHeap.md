---
address: 0x001009E4
tags: [function, weather, heap, cleanup]
status: understood
---
# [[FUN_001009E4]] — WeatherHeap Cleanup

**Address:** `0x001009E4`  
**Section:** `.text`  
**Type:** Cleanup / Destructor call  

---

## Signature
```c
void FUN_001009E4(void);
```

## Decompiled
```c
void FUN_001009E4(void)
{
    int iVar1;
    iVar1 = DAT_001009F8;
    (**(code **)(**(int **)(DAT_001009F8 + 0x10) + 0x10))(); // vtable[+0x10]()
    *(undefined4 *)(iVar1 + 0x10) = 0;                       // null out the pointer
    return;
}
```

## Analysis

Calls a virtual destructor on the WeatherHeap object via vtable, then nulls the pointer.

Pointer chain:
```
DAT_001009F8          → parent object
  +0x10               → WeatherHeap pointer (same slot as written by FUN_001003BC)
  → dereference       → WeatherHeap object
  → dereference       → vtable pointer
  +0x10               → vtable slot 4 (destructor or Cleanup())
  → call
```

vtable slot `+0x10` = 4th function pointer (0-indexed, 4 bytes each). Likely `Destroy()` or `~WeatherData()`.

## Writes

`DAT_001009F8 + 0x10` = `0` (nulled after destroy)

## Notes

`DAT_001009F8` and `DAT_001003FC` resolve to the same address — both equal `0x00951C34` (HeapManager).
Confirmed from data_items analysis. WeatherHeap slot is at `HeapManager + 0x10` in both cases.

## See also

[[FUN_001003BC]] — paired init  
[[weather]]

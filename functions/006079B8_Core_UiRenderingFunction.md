---
address: 0x006079b8
name: UiRenderingFunction
status: partially decompiled
tags: [save-system, rendering, ui]
decompiled_at: Session 23
---

# [[FUN_006079b8|UiRenderingFunction]] — UI/Rendering Function (incidental save buffer read)

## Summary

Performs rendering or UI operations using a sub-object at `param_1 + 0x18`. Calls [[FUN_00300fdc]] but discards the return value — the save buffer is not the primary concern here.

```c
void FUN_006079b8(int param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4)
{
    int sub = *(int*)(*(int*)(param_1 + 0x18) + 0x18);
    FUN_00600f68(sub, 0, param_3, param_4, param_4);
    FUN_00602564(sub, 5, 0, 0, 3);
    FUN_00602eac(sub);
    FUN_00300fdc();       // return value unused
    FUN_00250b38();
    FUN_0081b0b8(param_1);
}
```

## Notes

- `FUN_00600f68`, `FUN_00602564`, `FUN_00602eac` — graphics/shader or UI calls (addresses 0x006xxxxx)
- `FUN_0081b0b8` — high address, likely vtable dispatch or object method
- **Not a TownAcres/Villager reader** — [[FUN_00300fdc]] return value is discarded

## See Also

- [[FUN_00300fdc]] — save buffer getter (called here but result unused)

---
name: ReadStructArrayLevelField
tags: [function, state-system, accessor]
status: fully understood
address: "0x0071F3EC"
---

# FUN_0071F3EC — Struct Array Level Field Reader

## Summary

Reads a single byte from a struct array: field `+0x36` of element `param_2`.
Used to check "level" values for state sub-objects.

```c
undefined1 FUN_0071f3ec(int param_1, int param_2)
{
    return *(undefined1 *)(param_1 + param_2 * 0x302 + 0x36);
}
```

> [!IMPORTANT]
> `param_1` = base pointer (from [[FUN_006F7C54]] return value)
> `param_2` = element index (3, 4, or 5 in practice)
> `0x302` = struct element stride (770 bytes)
> `+0x36` = byte field within each element ("level" or "count")

## Usage in FUN_005B2FE0

Called three times with param_2 = 3, 4, 5. All results compared `> 1`.
If all three level bytes are >= 2 → weather type 0x15 resolves to type 3 (not 2).

## References

- Called from: [[FUN_005B2FE0]] (weather type 0x15 branch)
- param_1 comes from: [[FUN_006F7C54]]

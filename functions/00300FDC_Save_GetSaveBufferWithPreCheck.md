---
address: 0x00300fdc
name: GetSaveBufferWithPreCheck
status: partially decompiled
tags: [save-system, buffer, getter]
decompiled_at: Session 22c
---

# [[FUN_00300fdc|GetSaveBufferWithPreCheck]] — Save Buffer Getter (with pre-check)

## Summary

Second getter for the garden_plus.dat runtime buffer. Calls [[FUN_006064bc]] as a pre-flight check before returning the buffer pointer.

```c
int* FUN_00300fdc(void) {
    FUN_006064bc();
    return *(int**)(0x0095AF74);
}
```

## Notes

- Functionally equivalent to [[FUN_005c9d64]] but with an added pre-check
- `FUN_006064bc` — purpose unknown; likely asserts buffer is initialised
- Called by: [[FUN_006079b8]], [[FUN_006e1d70]], [[FUN_0071ee44]], [[FUN_0028de1c]], [[FUN_0022eba4]], [[FUN_0070d894]]

## See Also

- [[FUN_005c9d64]] — simpler getter: `return *(0x0095AF74)` with no check
- [[FUN_005c9dd4]] — save buffer constructor (allocates 563,968 bytes)

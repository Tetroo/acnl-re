---
address: 0x005c96a0
name: IncrementNonZero64BitCounter
status: fully understood
tags: [save-system, counter, utility]
decompiled_at: Session 24
---

# [[FUN_005c96a0|IncrementNonZero64BitCounter]] — Non-Zero 64-bit Counter Increment

## Summary

Increments a 64-bit counter (`*DAT_005c96dc` as `{uint32 lo; uint32 hi;}`) by 1, with special-case logic to skip the value `0` on overflow (if result is exactly 0, increments again). Not related to TownAcres or any buffer offset read — it's a generic non-zero ID/tick generator, likely used to mint unique save-session or timestamp IDs.

## See Also

- [[FUN_005c9a24]] — villager slot reader (found via same xref search on save buffer pointer)

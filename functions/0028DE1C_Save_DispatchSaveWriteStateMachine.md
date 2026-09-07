---
address: 0x0028de1c
name: DispatchSaveWriteStateMachine
status: partially decompiled
tags: [save-system, write, state-machine]
decompiled_at: Session 22c
---

# [[FUN_0028de1c|DispatchSaveWriteStateMachine]] — Save Write State Machine

## Summary

State machine dispatcher with a save write path. At case 1, calls [[FUN_00300fdc]] to access the save buffer — likely the write/flush entry point.

## Known Behaviour

| Case | Action |
|------|--------|
| 1 | Calls [[FUN_00300fdc]] — save buffer access (write path) |
| others | Not yet analysed |

## Open Questions

- Full state table (how many cases?)
- Does it call the write analog of [[FUN_001b8b9c]]?
- Is this the function that marks the save as dirty before flush?

## See Also

- [[FUN_00300fdc]] — save buffer getter (with pre-check)
- [[FUN_001b8b9c]] — raw file reader (low-level I/O)
- [[FUN_001d3748]] — save lifecycle manager

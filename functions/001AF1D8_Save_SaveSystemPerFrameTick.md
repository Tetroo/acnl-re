---
address: 0x001af1d8
name: SaveSystemPerFrameTick
status: partially decompiled
tags: [save-system, task, async]
decompiled_at: Session 20
---

# [[FUN_001af1d8|SaveSystemPerFrameTick]] — Save System Per-Frame Tick

## Summary

Per-frame wrapper around the save state machine. Prepares context, calls the dispatcher, then executes a virtual callback. Structured as an async **Task object** with a vtable.

## Execution Flow

```c
void FUN_001af1d8(obj* param_1)
{
    FUN_005cddd4();                          // unknown pre-tick
    // ... prepare context ...
    FUN_001d3748(param_1);                   // save state machine tick
    (*vtable[+0x1C])(param_1, 0);           // virtual post-tick callback
}
```

> [!IMPORTANT]
> The `vtable[+0x1C]` call pattern identifies this as a **Task / Job object** — the save operation is implemented as an async task with lifecycle callbacks, not a blocking call.

## Call Chain Position

```
[main loop / scheduler]
    └── FUN_001af1d8    ← this function (per-frame tick)
            └── FUN_001d3748   (save state machine dispatcher)
                    ├── FUN_001b9428   (mount check)
                    │       └── FUN_001b8b9c   (file reader)
                    └── FUN_001b927c   (archive mount)
```

## Open Questions

- Who calls `FUN_001af1d8`? xrefs upstream not yet traced
- `FUN_005cddd4` — unknown pre-tick function
- vtable layout at `param_1` not mapped

## See Also

- [[FUN_001d3748]] — Save state machine; called by this function each frame
- `FUN_005cc01c` — Second caller of [[FUN_001d3748]] (wrapper, returns 1)

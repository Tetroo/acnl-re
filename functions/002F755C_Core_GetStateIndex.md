---
address: 0x002F755C
name: GetStateIndex
tags: [function, state, getter]
status: understood
---

# [[FUN_002F755C|GetStateIndex]] — State Index Getter

## Signature
```c
uint8_t FUN_002f755c(void);
```

## Purpose

Returns the current **state index** of a global game object. This is a simple getter — no logic, no RNG. The byte at offset `+2` of the object pointed to by `DAT_002F7568`.

## Decompiled
```c
uint8_t FUN_002f755c(void) {
    return *(uint8_t *)(DAT_002F7568 + 2);
}
```

## Key Facts

- Called from **~100 sites** across the entire binary — this is a heavily used global accessor
- Returns a `uint8_t` — so at most 256 possible states (sentinel at `0xA5 = 165`)
- The return value is always passed into [[FUN_002F7574]] as the `index` parameter
- `DAT_002F7568` is a pointer to some global object — **who initializes it is unknown**

## What it is NOT

Not a random number generator. The name "RNG" was a wrong early hypothesis — discarded.

## Open Questions

- What object does `DAT_002F7568` point to?
- What system owns this state? (weather, time-of-day, season?)
- Who writes offset `+2` of that object?

## Call Graph

```
FUN_002F755C()
    └── reads *(DAT_002F7568 + 2)
            └── passed as index → FUN_002F7574(mask, index)
```

## See Also

[[FUN_002F7574]], [[weather]]

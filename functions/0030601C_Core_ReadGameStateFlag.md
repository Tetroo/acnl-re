---
address: 0x0030601C
name: ReadGameStateFlag
tags: [function, state, flag]
status: understood
---

# [[FUN_0030601C|ReadGameStateFlag]] — Game State Flag Reader

## Signature
```c
undefined1 FUN_0030601c(void);
```

## Purpose

Reads a single byte from the main game state object at a fixed offset.
Used as a condition gate in `FUN_0062eee8` — if result == 3, normal
period calculation is bypassed.

## Logic

```c
return *(byte*)(*DAT_00306030 + 0x1329d);
```

- `DAT_00306030` — pointer to a large global game object (likely main GameSingleton)
- Offset `0x1329d` (78,493 bytes in) — a status/phase byte deep inside that structure
- Return value 3 means something like "special mode" or "tutorial/loading phase"

## Meaning of Return Value 3

When this returns 3, `FUN_0062eee8` skips date comparison entirely and
falls through to `FUN_002fb34c` — i.e., uses the current period singleton as-is.
Likely a game phase where weather is not yet initialized or is locked.

## See Also

[[FUN_0062EEE8]]

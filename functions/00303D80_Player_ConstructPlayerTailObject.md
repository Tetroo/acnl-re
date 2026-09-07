---
address: 0x00303d80
name: ConstructPlayerTailObject
status: not yet decompiled
tags: [save-system, player, struct-init]
decompiled_at: —
---

# [[FUN_00303d80|ConstructPlayerTailObject]] — Player Region Tail Object Constructor

## Summary

Initialises a different-typed object at offset `+0x1b80` within the player region. Called once per player by [[FUN_006f4234]] after the 10+1 × 640-byte sub-structs.

## Layout Context

Within each player region (base = buffer + 0x73958 + i×0x1B88):

| Offset | Size | Description |
|--------|------|-------------|
| +0x0000 | 0x1B80 | 11 × 0x280 sub-structs ([[FUN_002ff304]]) |
| +0x1b80 | ? | This object (initialised by FUN_00303d80) |

## Open Questions

- What type is this? Pocket inventory? Letter data? House exterior?
- Size of this object (stride from 0x1B80 to end of player region = 0x1B88 − 0x1B80 = 8 bytes... too small?)

> [!NOTE] The player stride is 7048 bytes (0x1B88). 11 × 0x280 = 0x1B80, leaving only 8 bytes for this object. Possible the object is larger and stride was misread — needs verification.

## See Also

- [[FUN_006f4234]] — caller; loops 4 players, calls this at +0x1b80
- [[FUN_002ff304]] — the 640-byte sub-struct constructor

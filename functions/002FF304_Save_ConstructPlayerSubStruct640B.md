---
address: 0x002ff304
name: ConstructPlayerSubStruct640B
status: partially decompiled
tags: [save-system, player, struct-init]
decompiled_at: Session 22
---

# [[FUN_002ff304|ConstructPlayerSubStruct640B]] — 640-Byte Save Sub-Struct Constructor

## Summary

Zero-initialises and constructs a 640-byte (`0x280`) save sub-struct. Called 10+1 times per player region by [[FUN_006f4234]].

## Struct Layout (0x280 bytes)

| Byte offset | Size | Description |
|-------------|------|-------------|
| `0x00` | 48 (12×4) | uint32 fields, zeroed |
| `0x30` | 1 | byte, zeroed |
| `0x34`–`0x60` | 4×12 | more uint32 fields, zeroed |
| `0x64` | 1 | byte, zeroed |
| `0x68`–`0xA9` | 66 | `FUN_002f8280(..., 0x42)` — zeroed block |
| `0xAA` | ? | `FUN_003014d4(ptr, DAT_002ff3c8)` — sub-object init |
| `0x22C`–`0x26D` | 66 | `FUN_002f8280(..., 0x42)` — zeroed block |
| `0x26E`–`0x272` | 5 | bytes, zeroed |
| `0x274` | ? | `FUN_002fc978(ptr, DAT_002ff3cc)` — sub-object init |
| `0x278` | ? | `FUN_006bb094(ptr)` — sub-object init |
| end | — | total = **0x280 bytes** |

## Open Questions

- Struct identity: 10+1 per player — villager relationship data? letter slots? storage items?
- `FUN_003014d4` at +0xAA and `FUN_002fc978` at +0x274 — what types do they construct?

## See Also

- [[FUN_006f4234]] — caller; loops this 11 times per player

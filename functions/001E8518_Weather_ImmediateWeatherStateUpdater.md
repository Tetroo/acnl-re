---
address: 0x001e8518
name: ImmediateWeatherStateUpdater
status: partially decompiled
tags: [weather, rendering, update, state-change]
decompiled_at: Session 23
---

# [[FUN_001e8518|ImmediateWeatherStateUpdater]] — Immediate Weather State Updater

## Summary

Called on weather state transitions (not periodically). Resets weather object fields, sets cloud parameters, then calls [[FUN_001e63dc]] and [[FUN_001e6fe0]] immediately.

## Key Behaviour

- Zeroes out cloud/fog arrays in `DAT_001e8864` (5×5 matrix of cloud weights)
- Reads state bits `0x80` and `0x4000` → sets cloud types via `FUN_001e6044(10/11, ...)`
- Sets fog near/far distances at `obj+0x14` and `obj+0x18`:
  - If state==0x66 (indoor?): `fVar5` (default fog)
  - Otherwise: `DAT_001e886c` (different fog range)
- Checks state==0x17: force-sets weather type 0xb (snow?) via `FUN_001e6044`
- Calls [[FUN_001e63dc]] → [[FUN_001e6fe0]] — full immediate repaint
- Copies 12 floats from one buffer to another (double-buffering weather state)

## Notable State Values

| Value | Context |
|-------|---------|
| `0x66` (102) | Likely indoor state — uses default fog params |
| `0x17` (23) | Special weather → force type 0xb |
| bit `0x80` | Cloud active flag |
| bit `0x4000` | Secondary cloud flag |
| bit `0x40` | Unknown weather modifier |

## See Also

- [[FUN_001e63dc]] — weather interpolation
- [[FUN_001e6fe0]] — sky compositor
- [[FUN_001e5ef4]] — periodic updater (every 300 frames)
- `FUN_001e6044` — cloud/weather type setter

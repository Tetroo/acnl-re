---
address: 0x001d3d3c
name: DispatchSaveLoadStateMachine
status: partially decompiled
tags: [save-system, resource-manager, dispatcher]
decompiled_at: Session 22
---

# [[FUN_001d3d3c|DispatchSaveLoadStateMachine]] — Save Load State Machine Dispatcher

## Summary

Per-tick dispatcher for the save system state machine. Extracts state bits `[1:5]` from `param_3` (`uVar4 = param_3 & 0x3e`) and routes to the appropriate load handler. Each state reads one file (or a batch of files) from the save archive.

## Full State Routing Table

| `uVar4` | File(s) | Size | Post-read call | Notes |
|---------|---------|------|----------------|-------|
| `0x02` | `/garden_plus.dat` | 563,968 bytes | — | via [[FUN_001b96c8]], sets status flag before/after |
| `0x04` | `/exhibition.dat` | 1,555,984 bytes | — | via [[FUN_002cd508]]; checks [[FUN_002cd508]] result |
| `0x06` | `/mydsgn1-4.dat` | ? | `FUN_0076dfac` | array at `DAT_001d43d4` = `0x0098106C`; index in `DAT_001d43d8` |
| `0x08` | `/mail1-4.dat` | ? | `FUN_0076de58` | array at `DAT_001d43e4` = `0x00981044` |
| `0x0A` | `/friend1-4.dat` | ? | `FUN_0071c4a4` | array at `DAT_001d43f4` = `0x0098101C` |
| `0x0C` | `/mvplayer_pls.dat` ×4 | ? | `FUN_0070a7ec` | array at `DAT_001d4404` = `0x00980FF4` |
| `0x0E` | — | — | `thunk_FUN_007b0ba0` | state-machine query, no file read |
| `0x10` | (4-byte header?) | 4 bytes | — | reads from `*(DAT_001d4414+4)` into `DAT_001d4414` buffer |
| `0x12` | — | — | `thunk_FUN_007b0ba0` | same as 0x0E |
| `0x14` | `/amiibo.dat` | 0x128 bytes | `FUN_0047dac0` (XOR) | XOR decrypts, then verifies checksum vs `*piVar5` |

> [!IMPORTANT]
> `/garden_plus.dat` is handled exclusively at `uVar4 == 0x02` via [[FUN_001b96c8]]. There is **no** inline XOR decrypt at this stage — the raw 563 KB buffer is read directly. Verification happens at a higher layer.

> [!IMPORTANT]
> `/amiibo.dat` (state 0x14, 0x128 = 296 bytes) is the only file explicitly XOR-decrypted inline here via [[FUN_0047dac0]]. Structure: `[4-byte checksum][0x124 bytes data]`.

## File Array Layout in .data

The file path arrays for states 0x06–0x0C are static pointer arrays in `.data`:

| State | Array VA | Files |
|-------|----------|-------|
| 0x06 | `0x0098106C` | `/mydsgn1.dat` … `/mydsgn4.dat` |
| 0x08 | `0x00981044` | `/mail1.dat` … `/mail4.dat` |
| 0x0A | `0x0098101C` | `/friend1.dat` … `/friend4.dat` |
| 0x0C | `0x00980FF4` | `/mvplayer_pls.dat` ×4 |

Array entries use `0xFFFF0000` as a sentinel (end of array).

## Parameters

| Param | Description |
|-------|-------------|
| `param_1` | State machine context pointer |
| `param_2` | Flags (secondary) |
| `param_3` | State + flags; bits [1:5] = state |
| `param_4` | Additional flags (`& 2` = batch mode) |

## Open Questions

- Who advances the state (`param_3` bits)? Likely [[FUN_001d3748]]
- Post-read calls for states 0x06–0x0C (FUN_0076dfac etc.) — are they CRC verifiers or parsers?
- State 0x10: what file is at `*(DAT_001d4414+4)`? (reads only 4 bytes — probably a version/magic header)

## See Also

- [[FUN_001d3748]] — Lifecycle manager; drives this dispatcher per tick
- [[FUN_001b96c8]] — garden_plus.dat load thunk (state 0x02)
- [[FUN_002cd508]] — exhibition.dat reader (state 0x04)
- [[FUN_001b8b9c]] — Raw file reader (called by all file-reading states)
- [[FUN_0047dac0]] — XOR decryptor (state 0x14 only)
- [[FUN_0076dfac]] — CRC32 verifier (called after states 0x06, possibly others)

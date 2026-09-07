---
address: 0x005ca4f4
name: SaveObjectMasterConstructor
status: partially decompiled
tags: [save-system, constructor, buffer, structure, town, acres]
decompiled_at: Session 26
---

# [[FUN_005ca4f4|SaveObjectMasterConstructor]] — Save Buffer Main Object Constructor

## Summary

The primary C++ object constructor for the garden_plus.dat buffer. Called from [[FUN_005c9dd4]] with `param_1 = buffer + 0x80`. Chains dozens of [[FUN_00301d9c]] (forward iterator / array constructor) calls to build out the entire C++ object hierarchy within the 563,968-byte buffer.

> [!IMPORTANT] TownAcres located (Session 26)
> The 4th top-level sub-constructor call, [[FUN_002d457c]], builds a POD struct at `param_1 + 0x533b0` (= `save_buffer_base + 0x53430`). `TownAcres[42]` (community-sourced offset `buffer+0x53484`, cross-validated against the independently-derived villager offset) sits at exactly `+0x54` inside that struct — a byte-exact arithmetic match, not an approximation. Full derivation: [[FUN_002d457c]] and `meta/claude_note/claude-working-notes24.md`.
>
> **Key methodological finding**: every sub-constructor in this chain (verified for all 6 of the first-half calls: [[FUN_00112584]], [[FUN_005ccdc8]], [[FUN_002bf578]], [[FUN_002d457c]], [[FUN_006127ac]], [[FUN_002ebdf4]]) follows the C++ ABI "constructor returns `this`" convention — return value equals the input `param_1`, unchanged. [[FUN_00301d9c]] (the array-construct helper) does too. This means **all offset accumulation in this function is fully determined by its explicit hex literals** — no need to know a sub-constructor's internal size to trace positions past it, only the literal gaps between calls matter.

## Key Structure Observations

The function walks forward through the buffer via `iVar1`, constructing arrays of typed objects:

> [!WARNING] Correction (audit 2026-09-07): gap table was off-by-one
> Live re-decompile of `Save_SaveObjectMasterConstructor` (0x005CA4F4) shows the gap immediately before `FUN_002d457c`/`Town_ConstructFieldTownObject` is **`+0x28f4`**, not `+0x44bc` (that was a duplicate of the prior row — transcription error). Every row from `FUN_002d457c` onward was shifted by one in the old table. Corrected table below, verbatim from Ghidra:

| Call (in order) | Gap before it | Notes |
|---|---|---|
| `FUN_00301d9c(param_1+0x20, ..., 0xa480, 4)` | — | array ctor, precedes the sub-ctor chain |
| [[FUN_00112584]] (`Town_GridSubObjectConstructor`) | `+0x29200` | matches prior finding |
| [[FUN_005ccdc8]] (`Save_SaveBufferSubRegionConstructor`) | `+0x22be0` | matches prior finding |
| `FUN_002bf578` | `+0x44bc` | matches prior finding |
| [[FUN_002d457c]] (`Town_ConstructFieldTownObject`) | **`+0x28f4`** (was wrongly `+0x44bc`) | **the claimed TownAcres carrier — see caveat below** |
| `FUN_006127ac` | **`+0x850`** (was wrongly `+0x28f4`) | shifted in old table |
| `FUN_002ebdf4` | **`+0xa480`** (was wrongly `+0x850`) | shifted in old table |

> [!WARNING] "TownAcres at +0x54 inside Town_ConstructFieldTownObject" — NOT re-verified in this audit
> This function's own gap table is now corrected against live Ghidra output, but the deeper claim (that `Town_ConstructFieldTownObject`, i.e. `FUN_002d457c`, contains a POD struct with `TownAcres[42]` at internal offset `+0x54`) depends on decompiling *that* function, which is outside this audit pass's scope (it's a `Town_*` function, not `Save_*`/`Core_*`). Treat the absolute save-buffer offset for TownAcres as **unconfirmed** until `functions/002D457C_Town_ConstructFieldTownObject.md` is itself checked against a live decompile.
| later | 0x870 | 8 | **8 × 2160B** — villager/NPC array (matches 0x6689c ref in [[FUN_0012cc9c]]) |
| later | 0x870 | 8 | Second 8 × 2160B array (same type) |
| later | 0x2e | 3 | Structs (46B each) × 3 |
| later | 0x2e | 3 | Same type × 3 |
| later | 0xc | 4 | Small structs × 4 |

## Notable Size: 8 × 0x870

```
FUN_00301d9c(iVar1 + 0x40, DAT_005cac24, 0x870, 8)
```

`0x870 = 2160 bytes` per element, `8` elements = `17,280 bytes` total. This appears twice.
From [[FUN_0012cc9c]] destructor analysis: `0x6689c` = villager candidate region `8 × 0x870`.

> [!IMPORTANT] 8 × 0x870 is NOT 10 villagers. Either only 8 slots are C++ objects, or this is a different entity type (e.g. special NPCs, museum exhibits, etc.).

## Also Observed

- `*(undefined2*)(iVar1 + 0x70e60) = 0` — zero-initialises a uint16 at a large relative offset (possible acre-related?)
- Field at computed offset references DAT_005cac88/8c/90 — unknown structure end region
- Accumulator returns `iVar1 + DAT_005cac90` — total constructed region end

## Open Questions

- What are the 8 × 0x870 objects? (villagers? special characters?)
- What does [[FUN_005ccdc8]] (167,936-byte region) contain?
- What is the base buffer offset of each major array?

## See Also

- [[FUN_005c9dd4]] — caller; passes buffer+0x80 as param_1
- [[FUN_00301d9c]] — forward iterator (array constructor); confirmed returns `param_1` unchanged
- [[FUN_0012cc9c]] — destructor; destructs in reverse order
- [[FUN_005ccdc8]] — first large sub-region constructor
- [[FUN_002d457c]] — 4th sub-constructor; **contains TownAcres at +0x54**
- `research/townacres-search-strategy.md` — community offset source + cross-validation
- `meta/claude_note/claude-working-notes24.md` — full DAT_ constant resolution + arithmetic trace

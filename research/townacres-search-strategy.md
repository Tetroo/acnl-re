---
tags: [research, town, acres, save-system, strategy, resolved]
aliases: [TownAcres strategy, acre search rethink]
---

# TownAcres Search — Strategy Rethink (Session 25d) / RESOLVED (Session 26)

> [!IMPORTANT] RESOLVED (Session 26) — byte-exact confirmation
> Path A (manual trace of [[FUN_005ca4f4]]) was carried through to completion. Result: `buffer + 0x53484` = **exactly** [[FUN_002d457c]]'s constructed object `+ 0x54` — an independently re-derived arithmetic match to the byte, not just "same region." Full derivation: [[FUN_002d457c]], `meta/claude_note/claude-working-notes24.md`, and `research/town-generation.md` Session 26 section.
>
> **Still open:** the runtime reader/writer function. The offset is now proven correct structurally, but no code path that *reads* it at runtime (map rendering, town map UI, terrain generation) has been located yet — see `research/town-generation.md` → Next Steps for the two remaining candidate search surfaces.

**Purpose of this document:** the TownAcres search (Sessions 24–25c) tried five RE-only avenues and closed all of them without finding the reader/writer. Per user request, this is a full reconsideration of the approach — including questioning the earliest assumption — rather than another blind sweep.

---

## What was tried and closed (summary — see `meta/changelog.md` / `meta/changelog-archive.md` for full detail)

| Session | Approach | Result |
|---------|----------|--------|
| 24 | Exhaust all 15 direct-literal-pool readers of `*(void**)0x0095AF74` | Closed — none read TownAcres |
| 24 | String search for "Acre"/"acre" in ELF + all `.cro` modules | Zero hits anywhere |
| 24 | `.cro` module hypothesis (generation logic lives in `ModuleRealVillage.cro`) | Disproven — raw string-dumped all 3 Village/Prologue `.cro` files directly, none contain relevant class names |
| 24 | RTTI backward-chase on `isfactory::VillageBuilder` type_info via `get_xrefs_to` | Dead end — only DATA xrefs (sibling type_info structs), no CODE xref to constructor |
| 24 | Town Map UI (`BsMenuMapVillage`) resource chase | Dead end — map renders as one composited texture (`P_mp_00`), not per-acre panels |
| 25 | Reopened via [[FUN_002fb34c]] (found to be the *real* central save-buffer-pointer accessor, 200+ callers, previously misfiled as a "time singleton" due to an uncorrected Session 5 error) | New surface opened |
| 25b | Swept 39/247 callers of [[FUN_002fb34c]] | 100% were compiler trap-pattern tail-calls in unreachable branches, not real data access — surface abandoned |
| 25c | Investigated `/garden.dat` (second save file found in rodata) | Closed — single xref is a UI resource-lookup key (error dialog text), not a real `nn::fs` file open |
| 25d (killed mid-run) | RTTI chase attempt #2, using correct Itanium ABI methodology (`type_info → vtable+8 → CODE xref` instead of xref-ing the type_info itself) | Interrupted by user before completion, not yet conclusive |

## Where was the earliest possible error?

Looking back, the entire search assumed TownAcres would be found either (a) via **code archaeology** (xrefs, RTTI, constructors) starting from zero prior knowledge, or (b) inside the **raw block** (`0x00`–`0x292A4`, confirmed to have no C++ constructors) purely because that's the only region without C++ objects and therefore looked like the natural home for "simple" grid data.

Neither assumption was re-examined until now:

1. **Nobody checked whether the ACNL save format is already publicly documented.** This is a 2013 game with over a decade of active homebrew/modding history (save editors, cheat tools). A quick web search immediately surfaced multiple open-source, actively-maintained save editors: [marcrobledo/acnl-editor](https://github.com/marcrobledo/acnl-editor) (JS), [Cuyler36/ACSE](https://github.com/Cuyler36/ACSE) (C#), [kwsch/NLSE](https://github.com/kwsch/NLSE). This should have been the *first* move, not the sixth session's afterthought — cross-referencing known community RE work is standard practice and doesn't compromise the originality of verifying facts against the actual binary.
2. **The "raw block only" assumption about where TownAcres lives was wrong.** See below — the community-sourced offset lands in the middle "Unknown C++ objects" region (`0x42194`–`0x71900`), which Session 23 flagged but never explored, having assumed (without checking) that C++-constructed regions wouldn't contain something as simple as a POD grid. C++ objects can absolutely contain POD array members.

---

## New Finding: Community-Sourced Offset, Cross-Validated Against Our Own RE

From `marcrobledo/acnl-editor`, `js/editor_garden.js` (`OffsetsPlus` object — the Welcome Amiibo / Plus format, matching our exact binary):

```javascript
MAP_ACRES:    0x053484   // 341,124 decimal
ISLAND_ACRES: 0x06feb8
VILLAGERS:    0x0292d0
TOWN_NAME:    0x0621ba
```

Acre grid: 7×6 = 42 entries, 2 bytes each (contiguous `u16` array), confirmed by the editor's own iteration logic (`i % 7` row-boundary checks, `n*2` per-entry stride).

### Cross-validation against our independently-derived facts

Our own RE (Session 23, [[FUN_005c9a24]]) found: villager array starts at `buffer + 0x292A4`, and the **species field sits at `slot + 0x2C`** within each villager record.

```
0x292A4 (our villager array base) + 0x2C (our species field offset) = 0x292D0
editor's VILLAGERS constant                                          = 0x0292D0
```

**Exact match.** This is strong evidence that:
- Our buffer-base alignment and the editor's offsets refer to the identical byte layout — same game revision/region (EUR Welcome Amiibo), same struct packing.
- The editor's `VILLAGERS` constant points at the first meaningful field of the record (species ID), not the record's raw struct start — a convention worth remembering when interpreting its other offsets too.
- **`MAP_ACRES = 0x053484` should be treated as a high-confidence candidate for TownAcres**, not a hypothesis needing independent re-derivation from scratch.

### Where this places TownAcres in our buffer map

```
0x292A4 (168,612)  villager array start
0x42194 (270,740)  "Unknown C++ objects" region start (FUN_005ca4f4 chain) — never explored for grid data
0x53484 (341,124)  ← MAP_ACRES candidate — INSIDE the "Unknown" region, ~70,384 bytes (0x11330) into it
0x71900 (465,152)  small map state object start
```

TownAcres is **not** in the raw block — it's inside the C++-constructed middle region. This explains why every raw-block-focused and direct-literal-pool-reader search came up empty: we were looking at the wrong region and the wrong accessor pattern.

---

## Revised Next Steps

1. ~~**Primary — find the accessor for `buffer + 0x53484`.**~~ **RESOLVED — offset confirmed byte-exact (Session 26).** Manually traced [[FUN_005ca4f4]]'s constructor chain from entry through all four sub-constructors preceding the target, resolving every `DAT_005cacXX` literal-pool constant via `list_data_items`. Key unlock: every sub-constructor in the chain (and [[FUN_00301d9c]] itself) returns its own `param_1` unchanged (C++ ABI "constructor returns `this`" convention) — so the running offset is a pure sum of explicit hex literals, no guesswork needed. Result: `buffer+0x53484` = **exactly** [[FUN_002d457c]]'s object `+0x54`, landing in a 76-byte untouched raw-data gap. Full trace: `meta/claude_note/claude-working-notes24.md`, [[FUN_002d457c]], `research/town-generation.md` Session 26.
2. ~~If the offset doesn't resolve cleanly — re-verify via checksum.js/data_types.js~~ **Moot** — offset resolved with independent byte-exact arithmetic confirmation; no need to re-verify against the editor's own source further.
3. ~~Deprioritized but not closed: RTTI chase on `isfactory::VillageBuilder`~~ **CLOSED (Session 25d)** — re-attempted with correct Itanium ABI methodology (`type_info → vtable+8 → CODE xref`, two hops deep), still zero CODE xrefs at any depth; confirmed the target address isn't even part of any recognized function. Three independent attempts across sessions now agree: this is a genuine tooling limitation (ARM32 PIC constant materialization not resolved by `get_xrefs_to`), not a methodology error. Full detail: `research/town-generation.md` Session 25d section.
4. **`fgobj`/`dFieldFactory.cpp` per-acre placed-object classes** — deprioritized further now that the save-offset (the actual Godot-port blocker) is resolved; these govern *dynamic placed objects* (holes/trees/presents), a separate system from the static TownAcres grid confirmed in Session 26.
5. **New primary — find the runtime reader/writer for `buffer+0x53484`.** The offset is now proven, but no code path reads it at runtime yet. Ruled out (Session 26): all 15 direct dereferencers of `0x0095AF74`, all 12 callers of [[FUN_005c9d64]]/`FUN_00300fdc`, and [[FUN_002d457c]]'s sole caller (the constructor itself). Remaining candidates: a not-yet-located dedicated "Field/Town object" getter, or the ~208 still-unswept callers of [[FUN_002fb34c]] — now searchable much more precisely, since any real candidate must contain an add-immediate landing near `0x53430`–`0x534d8` (object base through TownAcres array end). See `research/town-generation.md` → Next Steps.

## Lesson for future searches in this vault

**Check for public prior art before spending sessions on RE-only archaeology**, especially for well-known commercial games with active modding communities. Web search + cross-validation against our own independently-confirmed offsets (like the villager check above) is fast, cheap, and doesn't compromise rigor — every externally-sourced claim still gets verified against the actual ELF before being written up as confirmed fact in `functions/*.md`.

## See Also

- `research/town-generation.md` — main TownAcres research log (Sessions 24–26)
- [[FUN_005c9a24]] — villager reader, source of the cross-validation offset
- [[FUN_005ca4f4]] — main save object constructor, contains the `0x42194`–`0x71900` region
- [[FUN_002d457c]] — **confirmed container of TownAcres, at `+0x54`** (Session 26)
- `meta/claude_note/claude-working-notes24.md` — full Session 26 derivation, all resolved `DAT_` values
- `meta/changelog.md` — session-by-session log

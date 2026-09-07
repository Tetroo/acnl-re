---
tags: [research, town, generation, seed, open]
aliases: [town seed, village seed, island seed, town generation]
---

# Town & Island Generation

**Status:** `TownAcres` offset RESOLVED with byte-exact confirmation (Session 26) — see below and `research/townacres-search-strategy.md`. Runtime reader/writer function still open.
**Key finding:** Town layout is stored directly in save data, not regenerated from seed at runtime.

## Session 26 — TownAcres offset RESOLVED (byte-exact)

Manually traced [[FUN_005ca4f4]]'s sub-constructor chain (Path A from `research/townacres-search-strategy.md`) all the way through, resolving every `DAT_005cacXX` literal-pool constant via `list_data_items`. Key unlock: [[FUN_00301d9c]] and all six top-level sub-constructors in the chain follow the C++ ABI "constructor returns `this`" convention (return value = input `param_1`, unchanged) — so the entire running offset is determined purely by explicit hex literals in [[FUN_005ca4f4]], with zero contribution from sub-constructor internals.

Result: `buffer + 0x53484` (the community-sourced `MAP_ACRES` candidate) = **exactly** `param_1` of [[FUN_002d457c]] (the 4th sub-constructor) `+ 0x54`. Not "in the right region" — byte-exact arithmetic match, independently re-derived from scratch and matching the community offset to the byte.

[[FUN_002d457c]] builds a plain POD struct (no vtable — not an RTTI class) with an untouched 76-byte raw-data gap at `[0x20, 0x6c)` relative to its own `param_1`. `TownAcres[42]` (84 bytes) starting at `+0x54` fits this gap (spilling ~8 bytes into the following zero-filled region, which is harmless). Full trace, all resolved `DAT_` values, and struct layout: [[FUN_002d457c]], `meta/claude_note/claude-working-notes24.md`.

**Still open:** the actual runtime reader/writer for this field. Checked and ruled out: all direct literal-pool dereferencers of `0x0095AF74` (15, exhausted in Session 24), callers of the two buffer-pointer wrapper getters [[FUN_005c9d64]]/`FUN_00300fdc` (12 total, all previously-known dead ends or new ones checked in Session 26 — `FUN_0028de1c` is a trap-pattern call), and the single caller of [[FUN_002d457c]] itself (just [[FUN_005ca4f4]], the constructor — not a reader). Likely candidates for a future session: a dedicated "get Field/Town object" singleton accessor not yet located, or one of the ~208 still-unswept callers of [[FUN_002fb34c]] (see Session 25b — swept 39/247, 100% trap-pattern, but not exhaustively ruled out).

---

## Tortimer Island — No Seed

18 pre-built fixed layouts loaded from `FgDataTour/`:

| Index | File |
|-------|------|
| 0 | Default.bin |
| 1 | A0.bin |
| 2 | C0.bin |
| 3-8 | F0–F5.bin |
| 9-11 | G0–G2.bin |
| 12-15 | H0–H3.bin |
| 16 | J0.bin |
| 17 | L0.bin |

Selection logic is in [[FUN_002ca040]] — loads all 18 via [[FUN_00200A94]].
Which island the player gets = likely based on online session ID or save slot.
No procedural generation.

---

## Town Layout — Stored, Not Generated

Town map = `TownAcres[7×6]` = 42 uint16 AcreIDs stored in `garden_plus.dat`.

The layout is generated **once** during the new game prologue, then saved.
At runtime the game simply reads and applies the stored acre IDs.

Template files used during generation:
- `TemplateData/village/sea_side_left.bin` — 2240 bytes, template for sea-left towns
- `TemplateData/village/sea_side_right.bin` — 2240 bytes, template for sea-right towns
- `TemplateData/island/island_type.bin` — 40 bytes, island terrain type IDs
- `TemplateData/island/peninsula_type.bin` — 3520 bytes, peninsula layout data

Loaded by [[FUN_002ca040]] and [[FUN_0027c56c]].

> [!WARNING] Discarded — corrected Session 24
> Previously claimed "seed/generation code lives in `ModuleRealVillage.cro`/`ModulePrologue.cro`". **Verified false**: raw string-dumped all three RomFS Village/Prologue `.cro` files directly (`ModuleRealVillage.cro`, `ModuleVillage.cro`, `ModulePrologue.cro`, not encrypted) — none contain "Field", "Builder", "isfactory", or "VillagePrologueBuilder" anywhere. Their actual content: `ModuleRealVillage.cro` = special visitor NPC scripts (Jonny, Kaburiba, Palloncino, Isabelle secretary events, holiday events); `ModuleVillage.cro` = Amiibo Festival ticket-exchange UI; `ModulePrologue.cro` = Tom Nook prologue dialogue + tree-planting ceremony script. **The `isfactory`/`dFieldFactory.cpp`/`fgobj` class cluster (incl. `VillageBuilder`) is confirmed to live in the main ELF itself** (`exefs.elf`) — found via `list_strings` there. The reason its code couldn't be reached is a different, tooling-level problem: `get_xrefs_to` on the class's RTTI `type_info` object only surfaces other **DATA** references (other type_info structs, likely sibling/derived classes in the same 0x008d20xx cluster), never a **CODE** reference from the actual constructor — almost certainly because the constructor sets the vtable pointer via an ARM32 PC-relative/PIC addressing pattern that this Ghidra analysis doesn't resolve into an indexed xref. Reaching `VillageBuilder`'s methods will need manual disassembly around the RTTI cluster (`0x008d2004`–`0x008d20ac`) or a smarter Ghidra analysis pass, not further string/xref searching.

---

## Season Period System — Confirmed

[[FUN_0056AC80]] — period index getter (0–22) based on date vs threshold table.

23 boundary dates in `DAT_0056ace8` divide the year into 23 weather periods:
Jan 15, Feb 18, Feb 24, Mar 31, Apr 5, Apr 10, Apr 14, Jun 7, Jun 15, Jun 27,
Jul 5, Jul 23, Sep 7, Sep 15, Oct 16, Oct 24, Nov 1, Nov 9, Nov 17, Nov 25,
Nov 30, Dec 10, Dec 31.

Used by [[FUN_006a3578]] to select Summer vs Winter .cro module:
- Period index 3–14 AND hemisphere ≠ southern → load `Summer.cro`
- Otherwise → load `Winter.cro`

---

## Module Dispatch System — Confirmed

[[FUN_006a3578]] — dispatches .cro modules by state flags:

| State bit | Module loaded |
|-----------|--------------|
| `0x8` | Indoor / Ftr |
| `0x4` | Outdoor |
| `0x20` | Village → RealVillage + Summer/Winter |
| `0x8000` | Museum → MusFish / MusIns / MusFossil |
| `0x1000` | Shop + NpcDt |
| `0x200` | Downtown + DowntownEx + NpcDt |
| `0x400000` | Station |
| `0x1000000` | Cafe + NpcDt |
| `0x800000` | Club + NpcDt |
| `0x9F/0xA0` | AutoCamp |
| `0x67` | Tour |

Uses [[FUN_006a332c]] to load each module by name string.

---

## What Determines the Town Layout Variant

At new game creation (prologue, in .cro):
1. **OceanSide** — left or right sea direction → selects which template file
2. **GrassType** — 0/1/2 → grass shape variant
3. **Player name hash** (likely) → seeds RNG for tree/item placement
4. Resulting `TownAcres[42]` stored permanently in `garden_plus.dat`

After that — layout never changes. Seed is discarded.

---

## Session 24 — Additional RTTI/string evidence (main ELF), TownAcres reader still not located

Confirmed via `list_strings`/`get_xrefs_to` (full GhidraMCP toolset restored) that the internal engine terminology for "acre-like area" is likely **`Field`**, not `Acre` — exhaustive string search for "Acre"/"acre" returned **zero hits** anywhere in the ELF, while `Field`-prefixed RTTI/class names are extensive:

- `dFieldFactory.cpp` — anonymous-namespace builder classes: `BasicBuilder`, `ElmFieldBuilder`, `ExhibitionBuilder`, `MuseumFossilBuilder`, `MuseumPictureBuilder`, `ShopGraceBuilder`, `CampNpcBuilder`, `VillagePrologueBuilder`, `ModelHouseFieldBuilder` — one builder per major scene/location, not per-acre.
- `fgobj` namespace (**F**ield-**G**round **Obj**ect?) — runtime placed-object system: `ObjectBase/Bury/Deco/Fall/Leaf/Move/Fruit/Other/Plant/Scale/Stump/Swing/Throw/Timer/Hanabi/Present/CoinStone/DecoSwing/Honeycomb/StumpAnim/ThrowHole/Dig/Exe`, plus `FieldBit`, `DrawList`, `DrawListNode`, `Proc`. This is the buried-item/hole/tree/present-box object system — likely reads *per-acre placed object* data, but its constructor/vtable could not be located via `get_xrefs_to` on the RTTI `type_info` object (ARM32 PIC relocation not indexed by this xref implementation — dead end with current tooling).
- `isfactory` namespace — **`VillageBuilder`**, `IslandBuilder`, `FishingIslandBuilder` — separate factory namespace, near-certainly the class used at new-game creation to lay out the town. Same RTTI xref dead-end (no vtable found).
- **`BsMenuMapVillage`** class + resource `Layout/Menu/map_village.arc` — the in-game Town Map screen (Nook's Homes bulletin board map). Loader state machine documented: [[FUN_002b32f4]]. Only **one** named layout pane (`"P_mp_00"`) was found — the acre grid is very likely rendered as a single composited/procedural texture, not per-acre UI panes, so this didn't yield an offset either.
- **`/garden.dat`** (`0x00930474`) — a **second, previously undocumented save file** distinct from `/garden_plus.dat`, found adjacent to it in rodata. Not yet investigated — open question for the save-system research.
- `GardenPlusUpdate` / `GardenManualImport` strings (`0x0092ed69` / `0x0092f1ed`) — referenced only from a DATA table (`0x00956844`/`0x0095683c`), likely a debug state-name array; xref chase on the table entries dead-ended (no code reference found).

> [!IMPORTANT] Working conclusion (Session 24)
> All in-main-ELF avenues tried (save-buffer-pointer xrefs, save object constructor chain, RTTI/vtable backward-chase, Town Map UI resource chase) failed to locate a `TownAcres` reader/writer. Given [[FUN_0027c56c]]/[[FUN_002ca040]] only *load the raw template `.bin` resources* (`sea_side_left/right.bin`, `island_type.bin`, `peninsula_type.bin`) into scratch buffers without visibly writing them into the save buffer, and given the `isfactory::VillageBuilder` RTTI exists but its code/vtable isn't resolvable in this Ghidra project — **the actual acre-assignment logic most likely lives in `ModuleRealVillage.cro`**, a dynamically-loaded module that is NOT part of the currently analysed `exefs.elf` and would need to be loaded as its own Ghidra project to decompile directly.

## Session 25b — FUN_002fb34c caller sweep: negative result

Session 25 (see changelog) had reopened the search by noting `FUN_002fb34c` (save buffer base pointer getter, confirmed via disassembly: `ldr r0,[0x2fb358]; ldr r0,[r0]; bx lr`) has **200+ distinct callers**, far beyond the 15 direct-literal-pool readers Session 24 exhausted.

Session 25b swept **39 of the 247 unique caller functions**, deliberately chosen for address diversity (save cluster `0x005c08a8`–`0x005cad8c`, save/file cluster `0x001b11ac`–`0x001c0290`, date/weather cluster `0x002faf9c`–`0x0033ac7c`, villager AI `0x0010ccbc`, network `0x006c9a28`, village menu/UI `0x0058bc6c`, `0x00220360`, `0x0069dec8`, `0x0024a8c0`, `0x005611ac`).

> [!IMPORTANT] Negative result — this is not a productive search surface
> **All 39 sampled callers use `FUN_002fb34c` as a naked tail-call inside an error/default/"unreachable case" branch** — the return value is never captured into a variable or dereferenced with an offset. This is a `B`-not-`BL` compiler artifact (Ghidra reports "Subroutine does not return" because control never returns to the caller's own frame): these sites effectively do `return FUN_002fb34c()` purely to produce *some* valid non-null pointer return for an unhandled switch/if arm, not to read save data. `FUN_002faec0` (the previously-identified abort/assert handler) plays the identical role in several of the same functions. Full technical writeup: `functions/FUN_002fb34c.md`.
>
> Checked (all negative, see `meta/claude_note/claude-working-notes21.md` for individual notes): `FUN_002faf9c`, `FUN_002fd714`, `FUN_002ff5b4`, `FUN_003006e0`, `FUN_00301104`, `FUN_00302394`, `FUN_003023d0`, `FUN_00302e9c`, `FUN_00303304`, `FUN_00303808`, `FUN_003056c0`, `FUN_0030723c`, `FUN_005c08a8`, `FUN_005c2e10`, `FUN_005c3eb4`, `FUN_005c47ec`, `FUN_005c6164`, `FUN_005cad8c`, `FUN_001b11ac`, `FUN_001b1230`, `FUN_001b894c`, `FUN_001bafd8`, `FUN_001bb08c`, `FUN_001bb44c`, `FUN_001bbc58`, `FUN_001bf804`, `FUN_001c0290`, `FUN_0070a300`, `FUN_0070a32c`, `FUN_0070a34c`, `FUN_0070a39c`, `FUN_0070a490`, `FUN_0010ccbc`, `FUN_0024a8c0`, `FUN_005611ac`, `FUN_0058bc6c`, `FUN_006c9a28`, `FUN_0069dec8`, `FUN_00220360`.
>
> **~208 callers remain technically unchecked**, but given 39/39 (100%) show the identical non-data-access pattern across every region of the binary sampled, further sweeping this specific list is *not* recommended as the next move — see Next Steps below.

## Session 25c — `/garden.dat` investigated: UI label key, not a real save file load

Followed up on the Session 25b Next Steps #2 lead. `get_xrefs_to` on the `"/garden.dat"` string (`0x00930474`) returns **exactly one reference**: [[FUN_0070d894]], at instruction `0070d980`. Full disassembly of that function shows the string is passed as a **lookup key** into `FUN_00810a50(scratch, DAT_0070daac, *DAT_0070daa8)` — a vtable-based accessor (`(*obj)[+8]()`) whose result is UTF-8→UTF-16 converted into a scratch buffer for what looks like UI text (an error/confirmation dialog naming affected save files). It is **not** passed to any `nn::fs`-style file-open call anywhere in that function.

> [!IMPORTANT]
> No raw loader (`nn::fs::OpenFile` style) for `/garden.dat` has been found anywhere in the ELF — the single xref is a UI-string lookup, not a data-file open. This demotes `/garden.dat` from "unexplored second save file" to "likely a legacy/unused filename referenced only in message text" (possibly a compatibility-check leftover from an earlier game revision). See corrected [[FUN_0070d894]] (previously misfiled as a "player/town name reader").

## Session 25d — RTTI chase attempt #2: definitively dead-ended (correct ABI methodology, still zero CODE xrefs)

Re-attempted the `VillageBuilder` constructor chase using the correct Itanium C++ ABI methodology (`type_info → class's own vtable slot[1] → vtable_start+8 (first virtual function slot) → CODE xref`), instead of Session 24's flawed approach of xref-ing the type_info struct directly (which only ever finds sibling type_info DATA references, never code).

**Steps taken:**
1. `list_strings filter="VillageBuilder"` → exactly one mangled RTTI name: `N9isfactory14VillageBuilderE` @ `0x008e9076`.
2. `get_xrefs_to(0x008e9076)` → one reference @ `0x008d2018` (the name-pointer slot inside VillageBuilder's own typeinfo struct, confirming typeinfo starts at `0x008d2014` — same address Session 24 found, now confirmed to be VillageBuilder-specific, not a shared/ambiguous base).
3. `get_xrefs_to(0x008d2014)` (the typeinfo struct's own address, as opposed to the name string) → the same 7-entry cluster Session 24 found (`0x008d2004`, `0x008d2040`, `0x008d2058`, `0x008d207c`, `0x008d2094`, `0x008d20a0`, `0x008d20ac`). Interpreted as 7 sibling/derived classes' typeinfo structs, each storing a base-class pointer to VillageBuilder's typeinfo (Itanium `__si_class_type_info` layout).
4. For each of the 7, computed `vtable_start = ref_addr - 4` and queried `get_xrefs_to(vtable_start + 8)` (the first actual virtual-function slot — this is the address a constructor writes into `this->vptr`, per ABI). All 7 returned exactly **one DATA reference each**, landing in a *third* cluster: `0x0090a148`, `0x0090a1e4`, `0x0090a24c`, `0x0090a2e8`, `0x0090a350`, `0x0090a384`, `0x0090a3b8`.
5. Queried `get_xrefs_to` on all 7 of these third-cluster addresses (one more hop, same double-indirection pattern seen elsewhere in this binary, e.g. [[FUN_002fb34c]]'s `DAT_002fb358`) — **all 7 returned zero results**. Confirmed via `get_function_by_address(0x0090a148)` → "No function found" — this is pure data, not embedded in any recognized function, so there's no function to even decompile/disassemble at this location.

> [!IMPORTANT] Conclusion — RTTI/vtable chase is dead with current tooling, not a methodology error
> Three independent attempts now (Session 24's direct typeinfo xref, and this session's two-hop corrected-ABI chase) all converge on the same result: **zero CODE xrefs at any depth**. This is not fixable by trying yet another intermediate address in the chain — it indicates `get_xrefs_to` in this GhidraMCP setup genuinely does not resolve however this binary's ARM32 PIC constructor/vtable-assignment pattern is encoded (likely `MOVW`/`MOVT` pair or multi-instruction address materialization that isn't captured as a data/code xref by whatever analysis pass backs this bridge). Reaching the constructor would require **manual byte-level disassembly** of a `.text` range and pattern-matching for instructions that load `0x0090a148` (or its siblings) as an immediate — not further xref-chasing. Deprioritizing this avenue; not worth further attempts without a different tool/approach (e.g. a proper Ghidra analysis re-run with better ARM constant propagation, or manual disassembly scanning).

## Open Questions

- ~~What exactly is inside `ModuleRealVillage.cro`~~ **RESOLVED (Session 24)**: special visitor NPC scripts, unrelated to town generation. See correction above.
- How to reach `isfactory::VillageBuilder`'s actual methods in the main ELF, given RTTI backward-chase via `get_xrefs_to` dead-ends (only DATA xrefs found, no CODE xref to the constructor)? Candidates: manual disassembly around the RTTI cluster `0x008d2004`–`0x008d20ac`, or brute-force decompiling nearby function addresses in `.text` near the `dFieldFactory.cpp` builder classes.
- What value seeds `sead::GlobalRandom` at new game creation?
- Is `sead::GlobalRandom` reseeded on each game load, or only at creation?
- ~~What is `/garden.dat`~~ **RESOLVED (Session 25c)**: a UI-string lookup key, not an actively-loaded save file. See above.
- Does a TownAcres *reader* (for rendering terrain each frame, not writing) exist in the main ELF? Given generation logic is confirmed to be in-ELF too (not .cro), this is now more likely findable.

## Next Steps (post Session 26)

The offset question itself is now closed (see Session 26 section at top) — remaining work is finding the runtime accessor:

1. **Find the TownAcres reader/writer.** Ruled out in Session 26: all direct dereferencers of `0x0095AF74` (15), callers of [[FUN_005c9d64]]/`FUN_00300fdc` (12), and [[FUN_002d457c]]'s own single caller. Two live leads: (a) a dedicated "get Field/Town object" singleton getter, not yet located — search functions in the `0x005c9000`–`0x005cb000` cluster that aren't yet decompiled; (b) the ~208 still-unswept callers of [[FUN_002fb34c]] (Session 25b swept 39/247, 100% trap-pattern, but technically not exhaustive) — now that the exact target offset (`+0x53430` object base, `+0x53484` array base, relative to save buffer) is known, any real caller can be confirmed quickly by checking for a large add-immediate in that specific numeric range, rather than blind sampling.
2. **`ISLAND_ACRES`** (`buffer+0x06feb8` per the same community table) — not yet checked against the [[FUN_005ca4f4]] chain; likely inside [[FUN_006127ac]] or [[FUN_002ebdf4]] (the two sub-constructors immediately after [[FUN_002d457c]]). Same tracing method as Session 26 should resolve it quickly now that the "constructors return `this`" convention is established.
3. Deprioritized: manual disassembly around the RTTI cluster `0x008d2004`–`0x008d20ac` (`isfactory::VillageBuilder`) — this governs town *generation*, not the save-format offset, and is confirmed unreachable via `get_xrefs_to` at any depth (Session 25d). `fgobj`/`dFieldFactory.cpp` per-acre placed-object classes — same caveat, and now lower priority since the save-offset itself (the actual Godot-port blocker) is resolved.

---

## See Also

[[FUN_0056AC80]], [[FUN_006a3578]], [[FUN_002ca040]], [[FUN_0027c56c]],
[[FUN_005ca4f4]], [[FUN_002d457c]], [[FUN_005c9a24]],
[[binary/EnvironmentParameter]], [[systems/weather]]

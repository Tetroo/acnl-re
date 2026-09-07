---
tags: [research, npc, villager, system]
aliases: [NPC system, villager system, villager selection]
---

# NPC / Villager System

**Status:** Partially investigated — initial selection mechanism understood at high level
**Total villagers:** 399 (333 original + 66 added in Welcome amiibo update)

---

## Data Files

| File | Size | Content |
|------|------|---------|
| `Npc/Normal/ConstPack.bin` | 13,568 bytes | All 399 normal villager codes + parameters |
| `Npc/Normal/Package.bin` | 35,136 bytes | Per-villager detailed parameters |
| `Npc/Special/ConstPack.bin` | — | Special NPC parameters (Isabelle, Blathers, etc.) |

Both loaded at startup by [[FUN_00109A90]] (main asset loader).

### Villager Species Codes (from ConstPack.bin)
35 species: `ant`, `bea`, `brd`, `bul`, `cat`, `cbr`, `chn`, `cow`, `crd`, `der`,
`dog`, `duk`, `elp`, `flg`, `goa`, `gor`, `ham`, `hip`, `hrs`, `kal`, `kgr`,
`lon`, `mnk`, `mus`, `oct`, `ost`, `pbr`, `pgn`, `pig`, `rbt`, `rhn`, `shp`,
`squ`, `tig`, `wol`

Format: 3-letter species code + 2-digit index (e.g. `cat22`, `dog10`).

---

## Initial Villager Selection

**5 villagers** are present when player first arrives. Maximum is 10.

> [!IMPORTANT] Town layout determines initial villagers
> In ACNL, the player picks from **4 pre-generated town layouts** on the train.
> Each layout likely has pre-set initial villager personality slots.
> From the player's perspective this feels random — they don't know which
> villagers come with each layout until they move in.

The actual RNG selection within each layout slot uses `sead::GlobalRandom`
(confirmed present via RTTI string at `0x008e1f2d`).

### Constraints
- Certain villagers **cannot** be starting villagers (blacklist exists)
- Personality balance is maintained in the initial set
- After the initial 5, additional villagers move in randomly up to 10

---

## Key Classes Found

| Class | RTTI Address | Role |
|-------|-------------|------|
| `VillageBook` | `0x008c9204` | Village data container (vtable) |
| `VillageSpBook` | `0x008c99b0` | Village special data |
| `AcNpcNml` | `0x008cb2b0` | Normal villager NPC class |
| `BsNpcMgr` | `0x008d6857` | NPC manager |
| `sead::GlobalRandom` | `0x008e1f2d` (RTTI) | RNG engine used for selection |

---

## Module Assignments

| Module | Content |
|--------|---------|
| `ModuleRealVillage.cro` | Special NPC visit events (Grace, Shrunk, etc.), seasonal festivals |
| `ModuleNpcDt.cro` | NPC in downtown/shops |
| `ModulePrologue.cro` | Tom Nook dialogue, secretary plants trees (new game intro) |
| `ModuleVillage.cro` | Ticket exchange UI, balloon manager |

The actual villager placement/generation code is in the main ELF,
called from the prologue layout selection sequence.

---

## Open Questions

- [ ] Exact function that selects initial 5 villagers — not found (deep in prologue path)
- [ ] What seeds `sead::GlobalRandom` for villager selection?
- [ ] Does each of the 4 layout options have fixed personality slots or fixed villagers?
- [ ] Where is the "cannot be starting villager" blacklist stored?

---

## See Also

[[research/town-generation]], [[FUN_006A3578]], [[FUN_00109A90]]

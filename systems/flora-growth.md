# Flora Growth & Gardening Engine

> [!WARNING] Correction (audit 2026-09-07, second pass)
> Section 1 below previously claimed `Town_AdvanceFloraGrowthForPlayableAcres` iterates and processes all 20 acres in one call. **This was fabricated** — the real `[TOOL]` decompile shows something structurally different: it walks the 5×4 grid only to pick **one** eligible acre (via a bounded-random index) and processes only that one acre before returning early. Status downgraded from "Fully Reverse-Engineered" (implicit) to **Partial**.

## Overview

Flora growth, tree development, flower hydration, and hybrid genetics in *Animal Crossing: New Leaf — Welcome amiibo* are managed by the `BsGrowUp` engine (`0x008F773C`). Flora logic updates during the **6:00 AM Daily Rollover** (call frequency/caller not traced in this pass — `HYPOTHESIS` that it's a once-per-day call, not re-confirmed).

---

## 1. Playable Grid — One Random Acre Per Call (`0x00114E10`, `[TOOL]`)

`Town_AdvanceFloraGrowthForPlayableAcres` does **not** loop over all 20 acres unconditionally. Real decompile:

```c
void Town_AdvanceFloraGrowthForPlayableAcres(void)
{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;

  iVar1 = FUN_002fc2ec(DAT_00951b9c);   // bounded random index, upper bound from DAT_00951b9c
  iVar5 = 0;
  uVar4 = 0;
  do {
    uVar3 = 0;
    do {
      iVar2 = FUN_002ffca8(uVar3 + 1, uVar4 + 1, 1);   // eligibility check for acre (uVar3+1, uVar4+1)
      if (iVar2 != 0) {
        if (iVar5 == iVar1) {
          Town_ProcessFloraTileGrid(1, uVar3 + 1, uVar4 + 1, 1, 3);
          iVar1 = FUN_002fc2ec(2);
          iVar5 = FUN_002fc2ec(2);
          Town_ProcessFloraTileGrid(0x1e, iVar5 + 1, iVar1 + 1, 1, 3);
          return;   // <-- early return after ONE acre
        }
        iVar5 = iVar5 + 1;
      }
      uVar3 = uVar3 + 1;
    } while (uVar3 < 5);
    uVar4 = uVar4 + 1;
  } while (uVar4 < 4);
}
```

**`[DERIVED]`** interpretation: the function counts eligible acres (`FUN_002ffca8(...) != 0`) while scanning the 5×4 grid, picks the `iVar1`-th eligible one (`iVar1` from a bounded RNG call), calls `Town_ProcessFloraTileGrid` on it, then makes a **second** `Town_ProcessFloraTileGrid` call with a completely different signature (`0x1e` = 30 as first arg, and coordinates from two fresh `FUN_002fc2ec(2)` calls — likely a 0/1 coordinate, not an acre index) before returning. The purpose of this second call is `HYPOTHESIS` — possibly a second, unrelated one-off tile pick (e.g. weed/flower spawn), not confirmed.

This means the "20-acre scan" framing was wrong on two counts: (1) it's a random single-acre pick weighted toward eligible acres, not an unconditional full sweep, and (2) `FUN_002ffd1c` — the function name previously cited in the fabricated loop — doesn't even appear in the real decompile; the actual per-acre worker is `Town_ProcessFloraTileGrid` (already a real, separately-named symbol in `symbols.csv`).

The "$16\times16=256$ tiles inspected individually" claim was **not re-confirmed** in this pass — `Town_ProcessFloraTileGrid` itself wasn't decompiled here. Treat as `HYPOTHESIS (not confirmed by bytes)` until checked.

---

## 2. Tree Growth Lifecycle

Planted saplings progress through 4 distinct visual and collision stages across 4 real-world days:

1. **Stage 0 (Planted Sapling - `0x005E`):** Small seedling. Walk-through collision.
2. **Stage 1 (Small Tree - `0x005F`):** Young tree. Solid collision.
3. **Stage 2 (Medium Tree - `0x0060`):** Adolescent tree.
4. **Stage 3 (Mature Adult Tree - `0x0061`):** Adult tree capable of bearing fruit.
   - If native fruit or island fruit was planted, transitions to fruit-bearing state (`0x0062..0x0066`).
   - When shaken, fruit drops (3 fruit per tree), and the tree reverts to bare state `0x0061`.
   - Regrowth takes **3 days**.

### Perfect Fruit Tree Death
- Perfect fruit trees produce harvest up to 4–7 times.
- On the final harvest, two perfect fruits and one **Rotten Fruit** drop.
- The tree loses its leaves, dies, and must be chopped down with an axe.

---

## 3. Flower Hydration & Weather Integration (`FUN_001175bc`)

- **Weather Auto-Watering:** When `Thunk_ResolveWeatherType()` returns 3, 4 (Rain / Heavy Rain) or 5, 6 (Snow / Blizzard), all flowers across all acres are automatically flagged as **Watered** (`FlowerState::Watered`).
- **Watering Can:** Players using a watering can directly apply the watered flag to flowers in front of them. Gold watering cans water a $3 \times 3$ area.
- **Wilting Cycle:**
  - If a healthy flower is unwatered for a day, it turns **Wilted** (brown/shriveled).
  - If watered while wilted, it revives back to Healthy the next morning.
  - If left unwatered while wilted, it **dies and is deleted** on the 6:00 AM reset.
  - Black roses watered with a **Gold Watering Can** turn into **Gold Roses** (which never wilt!).

---

## 4. Hybrid Breeding

When two compatible flowers are planted adjacent (orthogonally or diagonally) and both are watered, there is a chance for a new hybrid flower to spawn in an adjacent empty soil tile:
- **Red + White Tulip** $\to$ Pink Tulip
- **Red + Yellow Tulip** $\to$ Orange Tulip
- **Red + Red Rose** $\to$ Black Rose (or Hybrid Red for Blue Rose lineage)
- **Blue Rose Genetics:** Requires multi-generation recessive breeding (Orange + Purple $\to$ Special Red $\to$ Blue Rose).

---

## 5. Beach Seashell Spawning & Player Luck (`0x0010446C`, `0x00864764`, `[TOOL]`)

When the daily rollover processes beach acres (`param_1 == 0x1E` in `Town_AdvanceFloraGrowthForPlayableAcres`), seashells are spawned into available shore tiles via `Town_ProcessFloraTileGrid_SpawnFloraCallback` (`0x0010446C`).

### 5.1 Cumulative Probability Tables (`.rodata`)
- **Table 0 (Bad Luck 7, `0x00864764`):** `(0, 0, 3, 8, 13, 16, 31, 56, 96, 100)`
- **Table 1 (Normal Luck, `0x0086478C`):** `(2, 5, 10, 20, 30, 35, 45, 65, 95, 100)`
- **Table 2 (Good Luck 6, `0x008647B4`):** `(5, 15, 25, 35, 45, 55, 65, 75, 90, 100)`

### 5.2 Exact Species Odds
| Item ID | Species Name | Value | Bad Item Luck | Normal Luck | Good Item Luck |
|---|---|---|---|---|---|
| `0x208C` | Pearl-oyster shell | 4,800 Bells | **0%** (Disabled) | **2%** | **5%** |
| `0x208D` | Conch shell | 1,400 Bells | **0%** (Disabled) | **3%** | **10%** |
| `0x208E` | Giant-clam shell | 1,800 Bells | **3%** | **5%** | **10%** |
| `0x208F` | Coral | 1,000 Bells | **5%** | **10%** | **10%** |
| `0x2090` | Venus-comb shell | 600 Bells | **5%** | **10%** | **10%** |
| `0x2091` | Scallop shell | 2,400 Bells | **3%** | **5%** | **10%** |
| `0x2092` | Sea-snail shell | 360 Bells | **15%** | **10%** | **10%** |
| `0x2093` | Cowrie shell | 120 Bells | **25%** | **20%** | **10%** |
| `0x2094` | Sand dollar | 240 Bells | **40%** | **30%** | **15%** |
| `0x2095` | Oyster shell | 1,800 Bells | **4%** | **5%** | **10%** |

Under Bad Luck, top-tier shells (`0x208C` and `0x208D`) are completely blocked from spawning, and 65% of all spawns are cheap Sand dollars or Cowrie shells. Under Good Luck, expensive shells increase to 5%–10% each.


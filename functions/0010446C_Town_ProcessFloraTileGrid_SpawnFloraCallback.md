# Function: Town_ProcessFloraTileGrid_SpawnFloraCallback (0x0010446C)

## 1. Metadata
- **Address:** `0x0010446C`
- **Subsystem:** `Town` / `Flora`
- **Status:** `done`
- **Source Binary:** `tools/ctr-elf/output/exefs.elf` (EUR Multi5 Welcome amiibo, SHA-256 `43b71edf...`)

---

## 2. Description & Summary
`Town_ProcessFloraTileGrid_SpawnFloraCallback` is the tile item spawn callback invoked by `Town_ProcessFloraTileGrid` (`0x002FFD1C`) through `FUN_002FDD80`. When processing flora acre spawns (specifically daily beach seashell spawning where acre category = `0x1E` / subcategory = `0x18`), it:
1. Queries the current player's daily Item Luck tier (`FUN_002fc2b4()` reading `DAT_00952f68`):
   - Tier 7 (Bad Item Luck): Table 0 (`0x00864764`)
   - Tier Normal: Table 1 (`0x0086478C`)
   - Tier 6 (Good Item Luck): Table 2 (`0x008647B4`)
2. Rolls a percentage $R \in [0, 99]$ via bounded PRNG `FUN_002fc2ec(100)`.
3. Performs a cumulative threshold scan across the 10 entries of the selected probability table to pick a species index $0 \dots 9$.
4. Maps the index to the corresponding seashell item ID in category `0x3C`, subcategory `0x18` (IDs `0x208C`..`0x2095` in `Item.bin`).
5. Places the spawned item onto the map grid at coordinate $(X, Y)$ and marks the tile dirty.

---

## 3. Probability Tables & Exact Seashell Odds `[TOOL]`

### Cumulative Tables in `.rodata`
- **Table 0 (Bad Luck, `0x00864764`):** `(0, 0, 3, 8, 13, 16, 31, 56, 96, 100)`
- **Table 1 (Normal Luck, `0x0086478C`):** `(2, 5, 10, 20, 30, 35, 45, 65, 95, 100)`
- **Table 2 (Good Luck, `0x008647B4`):** `(5, 15, 25, 35, 45, 55, 65, 75, 90, 100)`

### Derived Species Spawn Probabilities `[DERIVED]`
| Item ID | Seashell Species | Sell Price | Bad Item Luck (7) | Normal Luck | Good Item Luck (6) |
|---|---|---|---|---|---|
| `0x208C` | Pearl-oyster shell | 4,800 Bells | **0%** (Disabled!) | **2%** | **5%** |
| `0x208D` | Conch shell | 1,400 Bells | **0%** (Disabled!) | **3%** | **10%** |
| `0x208E` | Giant-clam shell | 1,800 Bells | **3%** | **5%** | **10%** |
| `0x208F` | Coral | 1,000 Bells | **5%** | **10%** | **10%** |
| `0x2090` | Venus-comb shell | 600 Bells | **5%** | **10%** | **10%** |
| `0x2091` | Scallop shell | 2,400 Bells | **3%** | **5%** | **10%** |
| `0x2092` | Sea-snail shell | 360 Bells | **15%** | **10%** | **10%** |
| `0x2093` | Cowrie shell | 120 Bells | **25%** | **20%** | **10%** |
| `0x2094` | Sand dollar | 240 Bells | **40%** | **30%** | **15%** |
| `0x2095` | Oyster shell | 1,800 Bells | **4%** | **5%** | **10%** |

---

## 4. Decompiled Implementation `[TOOL]`
```c
void Town_ProcessFloraTileGrid_SpawnFloraCallback(undefined4 param_1, int param_2, int *param_3)
{
  int table_ptr;
  uint roll;
  undefined4 map_mgr;
  undefined4 uVar4;
  int table_idx;
  uint species_idx;
  undefined4 local_308;
  undefined1 auStack_304 [4];
  undefined1 auStack_300 [736];
  undefined1 item_out [4];
  undefined1 auStack_1c [8];
  
  // Clear candidate bit in acre grid bitmap
  *(ushort *)(param_2 + *param_3 * 2) =
       *(ushort *)(param_2 + *param_3 * 2) & ~*(ushort *)(param_3 + 4);
  *(int *)(param_2 + 0x280) = *(int *)(param_2 + 0x280) - 1;

  FUN_002fc29c(item_out);

  // Select luck table (0 = Bad, 1 = Normal, 2 = Good)
  table_idx = 1;
  species_idx = FUN_002fc2b4(); // Player luck from DAT_00952f68
  if (species_idx == 6) {
    table_idx = 2; // Good Luck
  }
  else if (species_idx == 7) {
    table_idx = 0; // Bad Luck
  }

  table_ptr = (&DAT_00951bf4)[table_idx];
  roll = FUN_002fc2ec(100); // R in [0, 99]

  species_idx = 0;
  do {
    if (roll < *(uint *)(table_ptr + species_idx * 4)) break;
    species_idx = species_idx + 1;
  } while (species_idx < 10);

  // Initialize item category bitmap for subcategory 0x18
  FUN_002fc318(&local_308);
  FUN_002fc334(&local_308, 0x18);
  FUN_001acdec(auStack_1c, species_idx, &local_308); // Picks species_idx-th item ID (0x208C..0x2095)
  FUN_002fc978(item_out, auStack_1c);
  FUN_002f77a0(auStack_1c);
  FUN_002fc98c(&local_308);

  // Play particle/sound effect if active
  table_ptr = FUN_002fc990();
  if (table_ptr != 0) {
    table_ptr = thunk_FUN_006f84c4(DAT_00951b99);
    thunk_FUN_005933b0(auStack_300);
    uVar4 = FUN_002fc9c4(auStack_304, item_out);
    local_308 = table_ptr;
    thunk_FUN_00593388(auStack_300, uVar4, param_3[2], param_3[3]);
    FUN_002f77a0(auStack_304);
    thunk_FUN_00593374(auStack_300);
    thunk_FUN_002f77a0(auStack_300);
  }

  // Commit item to map tile at (param_3[2], param_3[3])
  map_mgr = FUN_002fc9d8();
  local_308 = 0;
  FUN_002fca24(map_mgr, item_out, param_3[2], param_3[3]);
  thunk_FUN_0059a648(param_3[2], param_3[3]); // Mark tile dirty
  FUN_002f77a0(item_out);
  return;
}
```

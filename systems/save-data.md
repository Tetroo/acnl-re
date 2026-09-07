# Save Data Subsystem (`garden_plus.dat`)

> [!WARNING] Correction (audit 2026-09-07)
> Two errors found and fixed against a live Ghidra re-decompile: (1) file size hex below was wrong — `0x89AB8` is the start of the *tail marker region* (see `systems/save-buffer-architecture.md`), not the total size; the actual read size used throughout the save code (`Save_ReadSaveFileRaw(..., 0x89b00)`) is `0x89B00`. (2) The "Villagers Block" initializer in the table below was misattributed to `FUN_0011253c`; the real function (confirmed live in `Town_ResetSaveBuffer_AllSubObjects` / `0x005CA2C4`) is `Save_InitVillagersBlock`. Other function names in the table below were **not** individually re-verified in this audit pass — treat them as unconfirmed until cross-checked the same way.

## 1. Overview & Architecture

Animal Crossing: New Leaf — Welcome amiibo stores the entire town, player profiles, villagers, and items in a monolithic save file named `garden_plus.dat` (563,968 bytes / `0x89B00`).

The save data is structured as an array of discrete C++ sub-objects managed by `SaveObjectMasterConstructor` (`0x005CA4F4`) and initialized to defaults by `Town_ResetSaveBuffer_AllSubObjects` (`0x005CA2C4`).

---

## 2. Master Sub-Object Memory Map

| File Offset Range | Size (Bytes) | Sub-Object / Component | Ghidra Initializer | Description |
|---|---|---|---|---|
| `0x00020` .. `0x2921F` | 168,448 | `Tiles & Items` | `FUN_0027aa8c` | Town items, buried fossils, flowers, custom patterns |
| `0x29220` .. `0x4BDFF` | 142,284 | `Villagers Block` | `Save_InitVillagersBlock` (was wrongly `FUN_0011253c`) | 10 Villager slots (9,496 bytes each) + moving queue |
| `0x4BE00` .. `0x502BB` | 17,596 | `Field Foliage` | `FUN_005ccc50` | Bushes, trees, bamboo, rocks, weeds |
| `0x502BC` .. `0x52BAF` | 10,484 | `Exhibition` | `FUN_002bf46c` | Museum custom exhibition rooms 1..4 |
| `0x52BB0` .. `0x533FF` | 2,128 | `Field Town Object`| `FUN_002d4524` | Mayor status, town ordinance, environmental rating |
| `0x53424` .. `0x53427` | 4 | `Field CRC32` | `Save_CalculateCrc32` | CRC32 checksum protecting town geography |
| `0x53484` .. `0x534D7` | 84 | `TownAcres[7×6]` | `Town_InitTownGridAndAcres` (`0x00612820`) | 42 Acre IDs for the full map. **Note (2026-09-07, second pass):** `systems/save-buffer-architecture.md` had a *different* claim attributing this same offset to `Town_ConstructFieldTownObject`'s internal layout — that specific claim is refuted (the constructor never touches `+0x54`). This row's attribution to `Town_InitTownGridAndAcres` is not contradicted by that finding, but it also hasn't been independently confirmed here — `Town_InitTownGridAndAcres` itself was verified as a real function in the Town/Player/Villager audit pass, but its absolute write offset into the save buffer was not traced back to `0x53484` specifically. Treat as plausible, not confirmed. |
| `0x534D8` .. `0x5D87F` | 41,896 | `Playable Acres` | `InitTownGridAndAcres` | 20 Inner acres ($5 \times 4$), 16×16 tile arrays |
| `0x5D880` .. `0x621CB` | 18,508 | `Public Works` | `FUN_002ebd84` | Bridges, fountains, benches, lamps (PWP) |
| `0x621CC` .. `0x6AE37` | 35,948 | `Town Infrastructure`| `FUN_001b1b54` | Train station, town gates, flags, bulletin board |
| `0x6AE38` .. `0x6E1F3` | 13,244 | `Main Street` | `FUN_006bb620` | Shop upgrades, museum specimens, catalog |
| `0x6E1F4` .. `0x70E5F` | 11,372 | `Town Hall & Plaza` | `FUN_00612668` | Town Tree growth stage, town tune, town flag |
| `0x70E60` .. `0x717EB` | 2,444 | `RV Campground` | `Player_ConstructPlayerTailObject` | Harvey's Welcome amiibo campground & RVs |
| `0x717EC` .. `0x73957` | 8,556 | `Wisp & amiibo` | `FUN_0024a52c` | Magic lamp state, scanned cards, coupons |
| `0x73958` .. `0x89AB7` | 90,464 | `Players (4 Slots)` | `ConstructPlayerSubStruct` | 4 Player slots (pockets, house, mail, bank) |

---

## 3. Checksum Verification Protocol

Each major section has an independent 32-bit CRC checksum:
1. Field / Town Checksum at offset `0x53424`:
   $$\text{CRC32}(0x53428 \dots 0x718FF, \text{seed} = 0xFFFFFFFF)$$
2. Global Save Checksum calculated before writing to SD card archive via `MountSaveArchive` (`0x001B927C`).

---

## 4. C++ Port Integration

The complete binary structures matching `garden_plus.dat` 1:1 are defined in [`types/SaveFile.h`](file:///c:/Users/user/Documents/acnl_re/types/SaveFile.h) and ready for deserialization in the native C++20 engine.

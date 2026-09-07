# `0x006BB660` — `Museum_DonateSpecimen`

## Overview
Processes the player's donation of an identified specimen (fossil, insect, fish, diving creature, painting, or sculpture) to the town Museum. Validates the item index, records the active player ID (1..4) into the donation flag table, timestamps the donation with the current in-game RTC date, and evaluates whether the corresponding wing and entire museum have reached 100% completion.

---

## Function Prototype
```cpp
bool Museum_DonateSpecimen(MuseumDonationsBlock* save_block, uint16_t item_id);
```

---

## Arguments
- `save_block` (`MuseumDonationsBlock*`): Pointer to the 1,372-byte (`0x55C`) Museum donation block located at `+0x6AE38` in the Main Street save buffer.
- `item_id` (`uint16_t`): The global 16-bit item ID being donated.

---

## Internal Logic & Algorithm

1. **Category & Index Extraction:**
   - Calls `FUN_002fcbc4(item_id)` to extract internal category type:
     - `0x2C`: Fossils (count: 67 / `0x43`)
     - `0x24`: Insects (count: 72 / `0x48`)
     - `0x27`: Fish (count: 72 / `0x48`)
     - `0x28`: Deep-Sea Diving creatures (count: 30 / `0x1E`)
     - `0x54`: Art Paintings (count: 25 / `0x19`)
     - `0x58`: Art Sculptures (count: 8 / `0x08`)
   - Calls `FUN_002ff994(item_id)` to extract category-relative index.
   - Calls `Museum_ValidateSpecimenIndex(save_block, category, index)`. If invalid, aborts.

2. **Donator Flag Registration:**
   - Retrieves active player index `0..3` via `FUN_002ff8f4(4)`.
   - Writes `(player_index + 1)` (value `1..4`) to the corresponding flag array offset:
     - Fossils: `+0x448 + index`
     - Fish: `+0x48B + index`
     - Diving: `+0x4D3 + index`
     - Insects: `+0x4F1 + index`
     - Paintings: `+0x539 + index`
     - Sculptures: `+0x552 + index`

3. **Donation Date Timestamping:**
   - Obtains current in-game calendar date from primary time singleton via `GetTimeSingletonPrimary()` and `FUN_0030f73c`.
   - Writes packed 4-byte date (`uint16_t year`, `uint8_t month`, `uint8_t day`) to date array:
     - Fossils: `+0x000 + index * 4`
     - Fish: `+0x10C + index * 4`
     - Diving: `+0x22C + index * 4`
     - Insects: `+0x2A4 + index * 4`
     - Paintings: `+0x3C4 + index * 4`
     - Sculptures: `+0x428 + index * 4`

4. **Completion Evaluation:**
   - Re-evaluates wing completion statuses:
     - `Museum_CheckAquariumComplete` (`0x0076B8D0`) for fish and diving.
     - `Museum_CheckArtGalleryComplete` (`0x0076C14C`) for paintings and sculptures.
   - If all wings are fully completed, returns true indicating grand opening/museum complete state.

---

## Clean C++20 Reconstruction

```cpp
#include "types/Museum.h"

namespace ACNL {

bool Museum_DonateSpecimen(MuseumDonationsBlock* save, uint8_t category, uint32_t index, uint8_t player_idx, MuseumDonationDate current_date) {
    if (!save || player_idx >= 4) return false;
    const uint8_t donator_flag = player_idx + 1; // 1..4

    switch (category) {
    case 0x2C: // Fossils
        if (index >= kMuseumFossilsCount) return false;
        save->fossil_flags[index] = donator_flag;
        save->fossil_dates[index] = current_date;
        break;
    case 0x27: // Fish
        if (index >= kMuseumFishCount) return false;
        save->fish_flags[index] = donator_flag;
        save->fish_dates[index] = current_date;
        break;
    case 0x28: // Deep-sea Diving
        if (index >= kMuseumDivingCount) return false;
        save->diving_flags[index] = donator_flag;
        save->diving_dates[index] = current_date;
        break;
    case 0x24: // Insects
        if (index >= kMuseumInsectsCount) return false;
        save->insect_flags[index] = donator_flag;
        save->insect_dates[index] = current_date;
        break;
    case 0x54: // Paintings
        if (index >= kMuseumArtPaintingsCount) return false;
        save->painting_flags[index] = donator_flag;
        save->painting_dates[index] = current_date;
        break;
    case 0x58: // Sculptures
        if (index >= kMuseumArtSculpturesCount) return false;
        save->sculpture_flags[index] = donator_flag;
        save->sculpture_dates[index] = current_date;
        break;
    default:
        return false;
    }
    return true;
}

} // namespace ACNL
```

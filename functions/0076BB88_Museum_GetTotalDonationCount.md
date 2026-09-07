# `0x0076BB88` — `Museum_GetTotalDonationCount`

## Overview
Iterates through all 274 specimen slots across the four wings of the town Museum and returns the total number of items that have been successfully donated (i.e. specimen status flag $\ne 0$).

---

## Function Prototype
```cpp
uint32_t Museum_GetTotalDonationCount(const MuseumDonationsBlock* save_block);
```

---

## Arguments
- `save_block` (`const MuseumDonationsBlock*`): Pointer to the 1,372-byte Museum donations block in the save file (`+0x6AE38`).

---

## Returns
- `uint32_t`: Total donation count across all categories, ranging from `0` to `274`.

---

## Internal Logic & Assembly Decomposition

The routine executes six consecutive counting loops, corresponding to each specimen category:

1. **Paintings (`0x54`):** Iterates 25 entries (`0x19`).
2. **Sculptures (`0x58`):** Iterates 8 entries (`0x08`).
3. **Fish (`0x27`):** Iterates 72 entries (`0x48`).
4. **Deep-Sea Diving (`0x28`):** Iterates 30 entries (`0x1E`).
5. **Insects (`0x24`):** Iterates 72 entries (`0x48`).
6. **Fossils (`0x2C`):** Iterates 67 entries (`0x43`).

Total counted items: $25 + 8 + 72 + 30 + 72 + 67 = 274$.

---

## Clean C++20 Reconstruction

```cpp
#include "types/Museum.h"

namespace ACNL {

uint32_t Museum_GetTotalDonationCount(const MuseumDonationsBlock* save) {
    if (!save) return 0;

    uint32_t count = 0;

    // 1. Fossils (67)
    for (uint32_t i = 0; i < kMuseumFossilsCount; ++i) {
        if (save->fossil_flags[i] != 0) ++count;
    }

    // 2. Fish (72)
    for (uint32_t i = 0; i < kMuseumFishCount; ++i) {
        if (save->fish_flags[i] != 0) ++count;
    }

    // 3. Deep-Sea Creatures (30)
    for (uint32_t i = 0; i < kMuseumDivingCount; ++i) {
        if (save->diving_flags[i] != 0) ++count;
    }

    // 4. Insects (72)
    for (uint32_t i = 0; i < kMuseumInsectsCount; ++i) {
        if (save->insect_flags[i] != 0) ++count;
    }

    // 5. Paintings (25)
    for (uint32_t i = 0; i < kMuseumArtPaintingsCount; ++i) {
        if (save->painting_flags[i] != 0) ++count;
    }

    // 6. Sculptures (8)
    for (uint32_t i = 0; i < kMuseumArtSculpturesCount; ++i) {
        if (save->sculpture_flags[i] != 0) ++count;
    }

    return count;
}

} // namespace ACNL
```

# Town Structures & Public Works Projects (PWP)

> [!WARNING] ⚠️ External game knowledge (audit 2026-09-07): this entire file is general well-known ACNL gameplay facts (NPC names, ordinances, shop mechanics), no statement is tied to a specific address/decompile in this document. Useful as a reference, but not a result of reverse engineering.

## Overview

In *Animal Crossing: New Leaf — Welcome amiibo*, static and interactive buildings, facilities, and mayor public works projects are derived from the `AcStrc` base class inside `ModuleOutdoor.cro`.

---

## 1. Primary Village Facilities

1. **Town Hall (`AcStrcOffice`):**
   - Inside: Isabelle (`szo.bcres`) manages town satisfaction ratings, ordinances (Early Bird, Night Owl, Bell Boom, Beautiful Town), town tune, and flag designs.
   - Mayor chair: initiates Public Works Projects (PWP).
2. **Plaza Symbol Tree (`AcStrcSymboltree`):**
   - Planted by the player during the arrival prologue ceremony.
   - Grows dynamically across 7 distinct visual stages based on cumulative play history:
     - **Stage 4 (100 Days / 100 Hours):** The stone plaza rim becomes interactive; player can sit down on the edge.
     - **Stage 5 (180 Days / 180 Hours):** Sitting on the rim plays the town history credits roll (listing dates of villagers moving in, museum donations, shop upgrades).
3. **Re-Tail (`AcStrcRecycleShop`):**
   - Reese (`alp.bcres`): buys items for 4x Timmy & Tommy price, manages flea market tables, checks turnip prices twice daily.
   - Cyrus (`alw.bcres`): furniture customization, mineral ore jewel plating, fossil miniatures, and music boxes.
4. **Train Station (`AcStrcTrain` & `AcStrcCrossing`):**
   - Crossing gates flash red lights and play bell chimes when a train arrives.
   - Station Porter (`enj.bcres`): manages local wireless and Nintendo Network online visitor gates.

---

## 2. Public Works Projects (PWP) Footprint & Reservation

When the mayor selects a PWP location with Isabelle:
- Footprint verification ensures no overlap with river banks, cliff edges, villager door entry tiles ($3 \times 1$ front clearance), or rock positions.
- Gyroid Lloyd (`AcStrcRentalHaniwa`) appears at the building site to accept community bell donations until funded.

# Museum Subsystem Specification (`AcNpcSpFuta` & `AcNpcSpShopFuko`)

## 1. Overview & Architecture

The Museum in *Animal Crossing: New Leaf — Welcome amiibo* is the central cultural and collection institution of the player's town. Located on the Main Street (Shopping District / `SceneId 0x3A`), it consists of two distinct floors:
1. **Ground Floor (1F) — Public Exhibits:** Managed by curator **Blathers** (`AcNpcSpFuta`, Actor ID `0x1A1` / `417`). Houses four main exhibition wings divided across 11 spatial 3D rooms:
   - **Aquarium** (3 rooms: `0x4A`, `0x4B`, `0x4C`)
   - **Insectarium** (3 rooms: `0x4D`, `0x4E`, `0x4F`)
   - **Paleontology / Fossils** (3 rooms: `0x50`, `0x51`, `0x52`)
   - **Art Gallery** (2 rooms: `0x53`, `0x54`)
2. **Second Floor (2F) — Museum Shop & Custom Exhibit Rooms:** Managed by **Celeste** (`AcNpcSpShopFuko`). Unlocked via a Public Works Project proposal from Blathers. Features a gift shop and four rentable exhibition rooms (`0x502BC`, 10,484 bytes).

---

## 2. Binary Save Architecture

### 2.1 Public Exhibits Save Block (`0x6AE38`, 1,372 bytes / `0x55C`)
Located in the monolithic Main Street save data buffer (`0x6AE38` in `garden_plus.dat`), this block tracks donation status and timestamps for every collectible specimen in the game.

```
+0x0000 -------------------------------------------------------------+
| 274 Specimen Donation Dates (274 * 4 bytes = 1,096 bytes / 0x448)   |
|   +0x000: Fossils (67 entries * 4B = 268B)                         |
|   +0x10C: Fish (72 entries * 4B = 288B)                            |
|   +0x22C: Deep-Sea Creatures (30 entries * 4B = 120B)              |
|   +0x2A4: Insects (72 entries * 4B = 288B)                         |
|   +0x3C4: Art - Paintings (25 entries * 4B = 100B)                 |
|   +0x428: Art - Sculptures (8 entries * 4B = 32B)                  |
+0x0448 -------------------------------------------------------------+
| 274 Specimen Donator Status Flags (274 * 1 byte = 274 bytes)       |
|   +0x448: Fossil Flags (67B)                                       |
|   +0x48B: Fish Flags (72B)                                         |
|   +0x4D3: Deep-Sea Flags (30B)                                     |
|   +0x4F1: Insect Flags (72B)                                       |
|   +0x539: Painting Flags (25B)                                     |
|   +0x552: Sculpture Flags (8B)                                     |
+0x055A -------------------------------------------------------------+
| Padding / Alignment (2 bytes: 0x0000)                              |
+0x055C -------------------------------------------------------------+
```

### 2.2 Specimen Entry Formats

#### Donation Date Record (4 bytes, Little-Endian)
```cpp
struct MuseumDonationDate {
    uint16_t year;  // Little-endian year (default: 0x07DC = 2012)
    uint8_t  month; // 1-12 (default: 0x01)
    uint8_t  day;   // 1-31 (default: 0x01)
};
```

#### Donation Flag Byte (1 byte)
- `0x00` — **Uncollected / Not Donated**: Item plaque is blank; exhibit model is not spawned in the room.
- `0x01` — **Donated by Player 1 (Mayor / Resident 0)**.
- `0x02` — **Donated by Player 2 (Resident 1)**.
- `0x03` — **Donated by Player 3 (Resident 2)**.
- `0x04` — **Donated by Player 4 (Resident 3)**.
- `0x05` — **Special / Town Donation**: Pre-populated donation or special event donation.

When a player interacts with a specimen plaque in an exhibit room, `Museum_GetSpecimenDonatorName` (`0x0076B970`) resolves the player's UTF-16 name from the player save profile at `+0x55A6` based on this flag byte.

---

## 3. Specimen Categorization & Census

Total specimens: **274**.

| Category ID | Category Name | Count | Date Offset Range | Flag Offset Range | Exhibit Rooms |
|---|---|---|---|---|---|
| `0` | **Fossils** | 67 | `+0x000` .. `+0x10B` | `+0x448` .. `+0x48A` | `0x50`, `0x51`, `0x52` |
| `1` | **Fish** | 72 | `+0x10C` .. `+0x22B` | `+0x48B` .. `+0x4D2` | `0x4A`, `0x4B` |
| `2` | **Deep-Sea Diving** | 30 | `+0x22C` .. `+0x2A3` | `+0x4D3` .. `+0x4F0` | `0x4C` |
| `3` | **Insects** | 72 | `+0x2A4` .. `+0x3C3` | `+0x4F1` .. `+0x538` | `0x4D`, `0x4E`, `0x4F` |
| `4` | **Paintings** | 25 | `+0x3C4` .. `+0x427` | `+0x539` .. `+0x551` | `0x53` |
| `5` | **Sculptures** | 8 | `+0x428` .. `+0x447` | `+0x552` .. `+0x559` | `0x54` |

---

## 4. Exhibit Room Scene Mapping (`Item_GetMuseumExhibitRoomId` `0x0076AC9C`)

The engine dynamically determines which 3D room an exhibit item is placed in based on category, item flags, and dinosaur group IDs:

| Scene ID | Room Description | Contents & Placement Logic |
|---|---|---|
| `0x4A` | **Aquarium Room 1** | River fish, pond fish, small water creatures (Entrance). |
| `0x4B` | **Aquarium Room 2** | Large ocean fish, pelagic tank, sharks, sunfish, tuna. |
| `0x4C` | **Aquarium Room 3** | Deep-sea creatures (all 30 diving specimens), deep ocean trench. |
| `0x4D` | **Insectarium Room 1** | Tropical palm tree beetles (Goliath, Hercules, Golden Stag). |
| `0x4E` | **Insectarium Room 2** | Butterflies, grasshoppers, cicadas, forest floor insects. |
| `0x4F` | **Insectarium Room 3** | Subterranean insects, centipedes, scorpions, tarantulas, ants. |
| `0x50` | **Paleontology Room 1** | Small standalone fossils (Amber, Ammonite, Trilobite, Dinosaur Egg). |
| `0x51` | **Paleontology Room 2** | Mesozoic Hall: Large multi-part dinosaurs (T-Rex, Triceratops, Apato). |
| `0x52` | **Paleontology Room 3** | Cenozoic Hall: Prehistoric mammals (Mammoth, Smilodon, Megacero). |
| `0x53` | **Art Gallery Room 1** | 25 genuine paintings displayed along wall fixtures. |
| `0x54` | **Art Gallery Room 2** | 8 genuine statues/sculptures on central pedestals. |

---

## 5. Completion Logic & Mathematics

### 5.1 Wing Completion Checks
- **Aquarium Complete:** `Museum_CheckAquariumComplete` (`0x0076B8D0`): Evaluates all 72 fish + 30 diving specimens ($72 + 30 = 102$). Returns `1` if all 102 flags $\ne 0$.
- **Fossils Complete:** `Museum_CheckFossilsComplete` (`0x0076BD24`): Evaluates all 67 fossils. Returns `1` if all 67 flags $\ne 0$.
- **Insects Complete:** `Museum_CheckInsectsComplete` (`0x0076BE80`): Evaluates all 72 insects. Returns `1` if all 72 flags $\ne 0$.
- **Art Gallery Complete:** `Museum_CheckArtGalleryComplete` (`0x0076C14C`): Evaluates all 25 paintings + 8 sculptures ($25 + 8 = 33$). Returns `1` if all 33 flags $\ne 0$.
- **Total Completed Wings:** `Museum_GetCompletedWingCount` (`0x0076C2E4`): Sums the 4 boolean flags (returns `0..4`).

### 5.2 Completion Percentage Formulas
ACNL uses integer arithmetic for percentage displays on dialogue screens and player passcards:
$$\text{Overall Completion \%} = \frac{\text{Total Donations} \times 100}{266}$$
*(Note: Code at `0x0076C1EC` uses divisor `266` for overall completion)*.

- **Aquarium Completion:** $\frac{\text{Donated Fish \& Diving} \times 100}{102}$ (`0x0076C22C`)
- **Fossil Completion:** $\frac{\text{Donated Fossils} \times 100}{67}$ (`0x0076C448`)
- **Insect Completion:** $\frac{\text{Donated Insects} \times 100}{72}$ (`0x0076C4C4`)
- **Art Completion:** $\frac{\text{Donated Art} \times 100}{33}$ (`0x0076C5C8`)

### 5.3 Multi-Part Dinosaur Skeleton Completion (`Museum_CheckDinosaurSkeletonComplete` `0x0076C540`)
- Large dinosaurs consist of 2 to 5 distinct fossil parts (Skull, Torso, Tail, Left Wing, Right Wing, etc.).
- Function reads Dinosaur Group ID ($\le 30$) from item metadata at `+0x12` (`Item_GetFossilDinosaurGroupId` `0x00767B44`).
- Scans all fossils in category 0 that match the same group ID.
- Returns `true` only if every piece of that specific skeleton has a non-zero donation flag, triggering Blathers' celebratory dialogue ("A complete skeleton is now assembled!").

---

## 6. Second Floor Custom Exhibits (`0x502BC`, 10,484 bytes)

Managed by Celeste (`AcNpcSpShopFuko`):
- **4 Custom Exhibition Rooms** for rent (10,000 Bells each).
- Each room contains:
  - `furniture_layout`: 2,520 bytes (16x16 placement grid for customized display furniture).
  - `room_title`: 54 bytes (UTF-16 custom string for room door sign).
- CRC32 checksum verified at offset `+0x00` of the `0x502BC` sub-block.

---

## 7. C++20 Clean Implementation Reference

Header definition available at [`types/Museum.h`](file:///c:/Users/user/Documents/acnl_re/types/Museum.h). Fully interoperable with 3DS save data `garden_plus.dat` at offset `+0x6AE38`.

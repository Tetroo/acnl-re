# Mail, Post Office & ABD Banking Subsystem

> **Target Title:** Animal Crossing: New Leaf — Welcome amiibo (CTR, Title ID `0004000000198F00`)  
> **Source Files (Reverse-Engineered):** `exefs.elf`, `ModuleIndoor.cro`, `ModuleRealVillage.cro`, `dSvMail.cpp`  
> **Status:** `done` (Byte-exact decompilation, verified structures, and Ghidra MCP synchronization)

---

## 1. Overview & Architecture

The postal and banking subsystem in Animal Crossing: New Leaf provides:
1. **Mail System (`dSvMail`):** Standardized 640-byte binary format for player-to-player, player-to-villager, and system-to-player letters.
2. **Mail Storage Hierarchy:**
   - **Pocket Letters:** 10 slots per player ($10 \times 640 = 6,400$ bytes).
   - **Incoming/Work Buffer:** 1 slot per player (640 bytes).
   - **Post Office Mail Storage:** 80 persistent letters preserved at the post office counter ($80 \times 640 = 51,200$ bytes).
3. **Automatic Bell Dispenser (ABD / Bank):**
   - 8-byte obfuscated balance record per player (`+0x6B8C`).
   - Monthly interest calculation (0.5%, capped at 99,999 Bells).
   - 8-tier savings milestone reward gifts delivered via special mail `"Mail_SP_Postoffice"`.
   - Maximum bank account balance: **999,999,999 Bells**.
4. **Post Office Actors:**
   - **Pelly (`AcNpcSpPeriko`, Actor ID 471):** Day clerk (07:00 – 22:00).
   - **Phyllis (`AcNpcSpPerimi`, Actor ID 472):** Night clerk (22:00 – 07:00).
   - **Pete (`AcNpcSpPerioNormal`, Actor ID 467):** Town mail carrier (deliveries at 09:00 and 17:00).

---

## 2. Binary Mail Structure (`MailData`, 640 Bytes / 0x280)

`[TOOL]` Constructed by `Save_Mail_ConstructLetter640B` (`0x002FF304`) and `FUN_002FF264`.

| Offset | Type | Field | Description |
|---|---|---|---|
| `+0x000` | `uint8_t[104]` | `headerFlags` | Internal flags, template IDs, layout state |
| `+0x068` | `char16_t[33]` | `recipientName` | Recipient name (UTF-16, 66 bytes) |
| `+0x0AA` | `char16_t[193]` | `messageBody` | Message text content (UTF-16, 386 bytes) |
| `+0x22C` | `char16_t[33]` | `senderName` | Sender name & town name (UTF-16, 66 bytes) |
| `+0x26E` | `uint8_t` | `stationeryId` | Stationery / letter paper ID |
| `+0x26F` | `uint8_t` | `flags` | Bit 0: Read/Unread; Bit 1: Gift opened |
| `+0x270` | `uint8_t` | `category` | Mail category (0=Villager, 1=Player, 2=Special/Bank) |
| `+0x271` | `uint8_t` | `moodSubtype` | Emotional tone / message variant |
| `+0x272` | `uint8_t` | `deliveryFlags` | Delivery queue status |
| `+0x273` | `uint8_t` | `padding` | Alignment padding |
| `+0x274` | `uint16_t` | `presentItemId` | Attached gift item ID (`0x7FFE` = Empty / None) |
| `+0x276` | `uint16_t` | `presentFlags` | Gift wrapping paper color and modifiers |
| `+0x278` | `uint64_t` | `timestamp` | Unique delivery ID / RTC timestamp |

---

## 3. Save Data Layout (`garden_plus.dat`)

`[TOOL]` Verified from `Save_AllocateAndConstructSaveBuffer` (`0x005C9DD4`).

### Player Region Mail Offsets (`OFFSET_PLAYERS = 0x73958`):
For each of the 4 players ($4 \times \text{0x1B88} = \text{0x6E20}$ bytes):
- `+0x0000` .. `+0x18FF`: 10 Pocket Letters ($10 \times 640 = 6,400$ bytes).
- `+0x1900` .. `+0x1B7F`: 1 Incoming / Draft Work Buffer (640 bytes).
- `+0x1B80` .. `+0x1B87`: Player Mail Tail Object (8 bytes: `0xFFFFFFFF`, `0x7FFFFFFF`).
- `+0x6B8C`: ABD Bank Savings Account (`AbdAccount`, 8 bytes).

### Post Office Counter Storage (`OFFSET_POSTOFFICE_STORAGE = 0x7BDF8`):
- `0x7BDF8` .. `0x885F7`: 80 Stored Letters ($80 \times 640 = 51,200$ bytes / `0xC800`).

---

## 4. ABD Banking & Cryptographic Obfuscation

`[TOOL]` Functions: `Player_Abd_GetBalance` (`0x00303700`), `Player_Abd_SetBalance` (`0x003035C4`), `Player_Abd_DepositClamped` (`0x00612CD4`).

### Structure `AbdAccount` (8 Bytes at `+0x6B8C`)
```c
struct AbdAccount {
    uint32_t encBalance;   // +0x00: Bit-rotated encrypted balance
    uint16_t xorKey;       // +0x04: Random 16-bit key
    uint8_t  shiftAmount;  // +0x06: Random bit shift (0..25)
    uint8_t  checksum;     // +0x07: Checksum byte
};
```

### Checksum & Decryption Algorithm (`0x00303700`)
1. **Checksum Validation:**
   $$\text{checksum} \stackrel{?}{=} \left(\sum_{i=0}^3 \text{byte}_i(\text{encBalance}) + \text{0xBA}\right) \pmod{256}$$
   If checksum is invalid, balance returns `0`.
2. **Bit Rotation:**
   $$\text{rot} = \text{ROL32}(\text{encBalance}, 28 - \text{shiftAmount})$$
3. **Additive Key Subtraction:**
   $$\text{balance} = \text{rot} - (\text{xorKey} + \text{0x8F187432})$$

### Encryption Algorithm (`0x003035C4`)
1. Generate random 16-bit key: `xorKey = rand() & 0xFFFF`.
2. Generate random shift: `shiftAmount = rand() % 26`.
3. $\text{preVal} = \text{balance} + \text{xorKey} + \text{0x8F187432}$.
4. $\text{encBalance} = \text{ROL32}(\text{preVal}, \text{shiftAmount} + 4)$.
5. $\text{checksum} = \left(\sum_{i=0}^3 \text{byte}_i(\text{encBalance}) + \text{0xBA}\right) \pmod{256}$.

---

## 5. Interest Calculation & Savings Milestones

`[TOOL]` Function: `PostOffice_UpdateMonthlyInterestAndSavingsRewards` (`0x0062F7D0`).

### Monthly Interest Math
- Evaluated on date advance when $\Delta M \ge 1$:
  $$\Delta M = (\text{month} + 12 \times \text{year}) - (\text{lastMonth} + 12 \times \text{lastYear})$$
- Monthly Rate: **0.5%** (`0.005f` from literal `0x0062FBC8`).
- Monthly Clamp: $\min(\text{interest}, 99\,999)$ Bells (`0x0001869F`).
- Account Limit: Clamped to **999,999,999 Bells** (`0x00612CD4`).
- Upon interest payout, a letter `"Mail_SP_Postoffice"` (template index 1) is queued.

### Savings Milestone Tiers & Rewards
`[TOOL]` Tables at `0x0083D838` (Thresholds / 10,000) and `0x0083D848` (Item IDs):

| Tier | Threshold (Bells) | Table Value | Reward Item ID | Reward Item Name (`[RECALL]`) |
|---|---|---|---|---|
| **1** | 100,000 | 10 | `0x2CA2` | Box of tissues |
| **2** | 500,000 | 50 | `0x2CE0` | Letter set |
| **3** | 1,000,000 | 100 | `0x2CA3` | Piggy bank |
| **4** | 5,000,000 | 500 | `0x2CA5` | Aluminum briefcase |
| **5** | 10,000,000 | 1,000 | `0x2E2C` | Post-office poster |
| **6** | 20,000,000 | 2,000 | `0x2C3E` | Safe |
| **7** | 50,000,000 | 5,000 | `0x287B` | Mailman's hat |
| **8** | 100,000,000 | 10,000 | `0x2CA4` | Automatic Bell Dispenser (ABD) |

---

## 6. Post Office Staff & Delivery Schedule

`[TOOL]` Identified in Actor Registry (`0x0095459C`..`0x009545D8`), `ModuleIndoor.cro`, and `ModuleRealVillage.cro`:

| Actor Name | Actor ID | Module | Role & Schedule |
|---|---|---|---|
| `AcNpcSpPeriko` | 471 | `ModuleIndoor.cro` | **Pelly:** Day shift clerk (07:00 – 22:00) |
| `AcNpcSpPerimi` | 472 | `ModuleIndoor.cro` | **Phyllis:** Night shift clerk (22:00 – 07:00) |
| `AcNpcSpPerioNormal` | 467 | `ModuleRealVillage.cro` | **Pete:** Mail delivery carrier (09:00 AM & 05:00 PM) |
| `AcNpcSpPerioSpecial` | 468 | `ModuleRealVillage.cro` | **Pete Special:** Overfull mailbox visit / special delivery |
| `AcNpcSpPerioTutorial`| 469 | `ModuleRealVillage.cro` | **Pete Tutorial:** Prologue letter delivery sequence |
| `AcNpcSpPerioWarning` | 470 | `ModuleRealVillage.cro` | **Pete Warning:** Mailbox full alert |

---

## 7. C++ Header Reference

Include headers in the PC port:
- [`types/Mail.h`](file:///c:/Users/user/Documents/acnl_re/types/Mail.h): Contains `MailData`, `PlayerMailRegion`, and `PostOfficeMailStorage`.
- [`types/BankABD.h`](file:///c:/Users/user/Documents/acnl_re/types/BankABD.h): Contains `AbdAccount`, encryption/decryption routines, and milestone arrays.

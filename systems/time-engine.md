# Time Engine & Daily 6:00 AM Rollover

> **⚠️ AUDIT 2026-09-07 — serious problems found in this file.**
> 1. All 11 functions in section 3 ("Reverse-Engineered Functions") **have not been renamed in Ghidra** — checked via `get_function_by_address`, all of them are still called `FUN_0056xxxx` rather than their claimed names, despite `done` status in symbols.csv. This is a protocol violation (renaming in Ghidra is a mandatory step before `done` status).
> 2. Section 2 ("Daily 6:00 AM Boundary") contains **fabricated pseudocode**: the real decompile of `0x0056BFD8` has nothing to do with an hour/6-AM check — it's a check of `year >= 2012` and month/day fields with a call to `Core_ErrorAbortHandler()` on failure. This section has been corrected below.
> 3. Sections 1 and 4 (turnip prices, fossil/ore spawns, time-travel mechanics) are **well-known gameplay mechanics from the wiki**, not tied to specific functions verified in this session. Kept as context, but marked as unconfirmed by code.
>
> The actually-confirmed time logic (2000 epoch, the 39-year 2012↔2051 wrap, ticks, Julian Day) lives in `functions/00124120_*.md`, `functions/00126514_*.md`, `functions/0056BBD4_*.md` and other articles with addresses in the `0x0012xxxx`/`0x0056Axxx-0x0056BCxx` range — that part of the base passed audit successfully and can be trusted.

## Overview

The Time Engine in *Animal Crossing: New Leaf — Welcome amiibo* controls the progression of in-game time, calendar calculations, seasonal transitions, turnip price fluctuations, shop stock restocking, flora growth, villager schedules, and the critical **Daily 6:00 AM Rollover**.

---

## 1. Clock Architecture & Timestamps

ACNL operates on a hybrid system combining the 3DS hardware Real-Time Clock (RTC) and internal software time tracking:

1. **Hardware RTC Base (`0x0095B4E0`):** Polled from the CTR OS kernel via `ConvertTicksToWallClock` (`0x0056B820`).
2. **User Time Offset (`0x0095B4E8`):** Tracks in-game time travel adjustments made via the title screen (with Isabelle) or system settings without desynchronizing the physical 3DS hardware.
3. **Current Game Clock (`0x00AD46B0`):** 9-byte structure representing the current in-game calendar date and time:
   - Base Year: **2012** (`0x7DC`)
   - Upper Limit Year: **2050** (`0x802` / `0x803` clamped in `NormalizeYearRange_2050Limit`)

### Binary Structure: `GameDateTime` (9 Bytes)
Stored in `garden_plus.dat` at offset `0x55A8`:

```cpp
#pragma pack(push, 1)
struct GameDateTime {
    uint16_t year;        // 2012..2050
    uint8_t  month;       // 1..12
    uint8_t  day;         // 1..31
    uint8_t  day_of_week; // 0 = Sunday, 1 = Monday, ..., 6 = Saturday
    uint8_t  hour;        // 0..23
    uint8_t  minute;      // 0..59
    uint8_t  second;      // 0..59
    uint8_t  subsecond;   // Fractional tick
};
#pragma pack(pop)
```

At `0x55BA`, bit 0 flags Daylight Saving Time (Summer Time) toggle.

---

## 2. `FUN_0056bfd8` — actual content (not about 6 AM)

**⚠️ Fixed during the 2026-09-07 audit.** The old version claimed this was an hour-vs-6:00-AM check. The real decompile:

```c
void FUN_0056bfd8(int *param_1)
{
  int iVar1; int iVar2; int iVar3; bool bVar4; bool bVar5;

  iVar3 = *param_1;           // year
  iVar2 = param_1[1];         // packed month/day/etc
  bVar5 = SBORROW4(iVar3,0x7dc);   // compare year vs 2012 (0x7dc)
  iVar1 = iVar3 + -0x7dc;
  bVar4 = iVar3 == 0x7dc;
  if (iVar3 < 0x7dd) {
    iVar3 = (int)(char)iVar2;      // month byte
    bVar5 = SBORROW4(iVar3,1);
    iVar1 = iVar3 + -1;
    bVar4 = iVar3 == 1;
  }
  if (bVar4 || iVar1 < 0 != bVar5) {
    iVar1 = (iVar2 << 0x10) >> 0x18;   // another packed byte
    bVar4 = SBORROW4(iVar1,1);
    iVar3 = iVar1 + -1;
    if (iVar1 < 2) {
      bVar4 = SBORROW4(iVar2 >> 0x18,6);
      iVar3 = (iVar2 >> 0x18) + -6;
    }
    if (iVar3 < 0 != bVar4) {
      DAT_0095b4d2 = DAT_0095b4d2 | 1;
      Core_ErrorAbortHandler();
      return;
    }
  }
  DAT_0095b4d2 = DAT_0095b4d2 & 0xfe;
  Core_ErrorAbortHandler();
  return;
}
```

Based on its structure, this is a **date-validity check against the 2012-01-01 epoch** (year>=2012, month>=1, ...), which either sets an error bit in `DAT_0095b4d2` and jumps to `Core_ErrorAbortHandler`, or clears it and also jumps to `Core_ErrorAbortHandler` (looks like a shared finalizer, not necessarily "abort" in the crash sense — unverified). There is no "hour - 6" or "previous game day" logic here at all. `HYPOTHESIS`: the actual 6-AM daily-rollover check — if it exists as a separate function at all — was not found within this audit.

### Consequences of the 6:00 AM Rollover (⚠️ general gameplay mechanic, not tied to the code below):
When the clock transitions from `05:59:59` to `06:00:00`:

1. **Turnip Price Curve Re-roll:**
   - On **Sunday at 6:00 AM**, Joan appears, buy price is rolled (`90..110` bells), and the weekly pattern is chosen:
     - Pattern 0: **Fluctuating** (50..140 bells, high volatility)
     - Pattern 1: **Large Spike** (monotonic drop for 2-7 half-days, then sudden spike to 200..600 bells)
     - Pattern 2: **Decreasing** (drops each half-day by 0.9x to 0.8x, guaranteed loss)
     - Pattern 3: **Small Spike** (gradual rise peaking at 140..200 bells)
   - On **Monday to Saturday**, prices update twice daily:
     - **AM Price:** Active 6:00 AM to 11:59 AM
     - **PM Price:** Active 12:00 PM to 2:00 AM (closing time)
2. **Daily Ground Spawns:**
   - **Fossils:** Up to 4 new buried cracks (`0x005A..0x005D`) spawn in random soil tiles across acres. Total buried fossils capped at 4.
   - **Pitfall Seed:** 1 buried crack.
   - **Gyroids:** Up to 3 buried cracks spawn if it rained or snowed the previous day.
   - **Money Rock:** 1 existing rock is selected as the daily money rock (spawns 100, 200, 300, 500, 1000, 2000, 4000, 8000 bells on shovel strikes).
   - **Ore Rock:** 1 fake rock spawns containing mineral ore (Gold, Silver, Ruby, Sapphire, Emerald, Amethyst) or 7 bells if hit with a silver shovel.
3. **Flora & Weather Updates:**
   - **Flowers:** Wilted flowers from yesterday that were not watered die and disappear. Watered flowers revive. Adjacent compatible flowers test for hybrid color generation.
   - **Trees:** Shaken fruit trees advance 1 day towards the 3-day regrowth cycle. Saplings advance 1 growth stage (Seedling -> Small -> Medium -> Full).
   - **Weeds:** 2 to 3 new weeds spawn randomly (suppressed under Beautiful Town Ordinance).
4. **Villager Schedules:**
   - Sleeping villagers despawn from public acres into their homes.
   - Villagers scheduled to move in or move out update their house state (Plot reserved -> House built -> Unpacked -> Settled -> In boxes -> Departed).

---

## 3. Reverse-Engineered Functions

> **⚠️ Not one of these 11 functions has been renamed in Ghidra** (checked 2026-09-07 via `get_function_by_address` — all are still `FUN_0056xxxx`). The table below is a set of working name/purpose hypotheses, not fact. The only row where the real decompile was checked and turned out to be **wrong** is `0x0056BFD8` (see section 2 above). The rest have not been re-examined line by line in this session, except `0x0056AA90` and `0x0056C288`, which look structurally plausible but are unconfirmed by xref to calling code.

| Address | Original Ghidra Name | Identified Symbol | Description |
|---|---|---|---|
| `0x0056AA90` | `FUN_0056aa90` | `Time_ConvertHardwareRtcToGameDate` | Converts raw CTR RTC ticks to GameDateTime format |
| `0x0056ABE0` | `FUN_0056abe0` | `Time_GetEpochDate2012` | Sets baseline epoch to 2012-01-01 |
| `0x0056ADD4` | `FUN_0056add4` | `Time_CalculateMonthDayFromJulian` | Julian day subfield conversion |
| `0x0056B034` | `FUN_0056b034` | `Time_AddDurationAndClamp2050` | Adds timestamp offset with 2050 boundary clamp |
| `0x0056B448` | `FUN_0056b448` | `Time_NormalizeDateStructure` | Normalizes year, month, day, and leap years |
| `0x0056B5D4` | `FUN_0056b5d4` | `Time_InitializeClockSubsystem` | Initializes 2000, 2012, and 2051 tick constants |
| `0x0056B820` | `FUN_0056b820` | `Time_UpdateCurrentGameClock` | Main clock tick function called once per frame |
| `0x0056BFD8` | `FUN_0056bfd8` | `Time_Check6AmDailyRolloverBoundary` | Checks if current hour is past 6 AM boundary |
| `0x0056C1F4` | `FUN_0056c1f4` | `Time_ApplyClockOffsetAndSave` | Updates time offset variables after time change |
| `0x0056C288` | `FUN_0056c288` | `Time_SetPlayerTimeTravelOffset` | Applies user time travel offset |
| `0x0056C344` | `FUN_0056c344` | `Save_ReadClockHeaderRecord` | Reads 9-byte clock struct at 0x55A8 in garden_plus.dat |

---

## 4. Time Travel Detection & Penalties

> **⚠️ Not confirmed by code in this session.** This is well-known time-travel mechanics from gameplay experience/wiki, not tied to a specific verified function. Requires separate reverse engineering before treating it as fact for the port.

When loading a save file:
1. The game compares current game time to `SaveClockRecord.last_saved_time` (`0x55A8`).
2. If `current_time < last_saved_time`:
   - **Backwards Time Travel Flag** is triggered.
   - All held/stored turnips immediately spoil into item ID `0x2035` (Spoiled Turnips).
   - Massive weed growth penalty applied.
   - Resetti / Isabelle dialogue triggers special reactions.

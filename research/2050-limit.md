---
tags: [research, date, limit, resolved]
aliases: [2050 limit, year limit, date wrap, date reset]
---

# The 2050 Limit

**Status:** ✅ RESOLVED (Session 10)
**Confirmed:** Maximum date is December 31, 2050. After midnight → resets to January 1, 2012.
**Source of limit:** `FUN_0056BBD4` — year range normalizer. 39-year modular wrap. Pivot = 2051.
**Community source:** Nookipedia, BellTree forums, community testing — consistent across sources.

> Previously labelled "2030 limit" — incorrect assumption. Corrected 2026-03-04.

---

## What We Now Know

### Time System Architecture

The game does **not** call RTC on each time query. Instead:

1. At startup: RTC is read once (location TBD), a base timestamp is stored.
2. Each frame: `FUN_0012994C` calls `SVC 0x28` (`svcGetSystemTick`) to read CPU
   ticks, then derives current time by adding elapsed ticks to the base timestamp.
3. `FUN_002FB39C` returns a pointer to the global time object (singleton).
4. Date arithmetic uses `FUN_001296BC` → `FUN_0012F8A0` (Julian Day, epoch 2000).

### Weather Lookup Chain

```
FUN_001E63DC (per-frame weather update)
  └── FUN_002FB39C()              → time object
  └── FUN_0062E094()              → weather type byte (52×24 table, week × hour)
  └── FUN_0062EEE8()              → period index (3 paths: flag/state/normal)
        ├── Path A (flag 0x20000): FUN_0062EFD4 → JulianDay % 3
        ├── Path B (state == 3):   FUN_002FB34C → *DAT_002FB358
        └── Path C (normal):
              FUN_0062EF1C()       → validates date against current time
                └── FUN_00308F5C() → date normalizer; applies 6-month offset + FUN_0056BBD4 year wrap
                └── FUN_002FD4A0() → period delta (signed count)
                └── FUN_0031872C() → shift −6 months → FUN_002FB34C
              └── FUN_001296BC()   → timestamp
                    └── FUN_0012F8A0() → Julian Day (epoch 2000, no year cap)
  └── FUN_001E82E0()              → (weather_type, period_index) → group_index
        └── record_index = group_index * 0x18 + hour_of_day → EnvironmentParameter.bin
```

### The 52×24 Weather Table

`FUN_0062E084` uses a **52 × 24 lookup table**:
- Axis 0: week of year (0–51), bounds-checked `< 52`
- Axis 1: normalised hour (0–23), shifted by -6 (day starts at 06:00)
- `DAT_0062E1AC` = array of 52 row pointers

This table is annual (52 weeks). It does **not** encode year, so it cannot be the
direct source of the 2050 limit.

### What Does Not Cause the 2050 Limit

- `FUN_0012F8A0` — no year cap, unbounded Julian Day calculation
- `FUN_001296BC` — no cap, pure arithmetic
- `FUN_0012994C` — reads ticks, no cap on date range
- `FUN_00124120` — date→ticks converter, no clamp (confirmed Session 10)
- `FUN_00126514` — ticks→date converter, no clamp (confirmed Session 10)
- 3DS RTC hardware — uint8 year-from-2000 supports up to year 2255, no hardware limit

### ✅ Confirmed Source: [[FUN_0056BBD4]]

Called from `FUN_00308F5C` Phase 1 (and from `FUN_0030A594`).
Year range normalizer — **39-year modular wrap**:

```c
int pivot = DAT_0056bc3c;  // = 2051
int lower = pivot - 39;    // = 2012

if (year >= pivot) year -= 39;  // 2051→2012, 2052→2013, ...
if (year < lower)  year += 39;  // forward correction for years before 2012
```

Pivot `DAT_0056bc3c = 2051` confirmed from ELF at file offset `0x47bc3c`.

### Why Nintendo Chose This Design

This is **not** a hardware limitation and not a simple oversight. It is a deliberate
design decision, likely driven by a combination of factors:

1. **Year 2012 = Japan release date.** The valid window [2012, 2050] is exactly
   the game's intended lifespan from launch. Nintendo never expected the game to
   be played 40+ years after release.

2. **Event tables are annual, not multi-year.** The weather/event lookup table
   (`FUN_0062E094`) is 52×24 — 52 weeks × 24 hours. Year is not encoded at all.
   The game works in a repeating annual cycle by design.

3. **Memory budget.** Storing per-year event data for 100+ years would be
   wasteful on a device with 128MB RAM. The 39-year wrap keeps all tables compact.

4. **One normalizer, everywhere.** By applying `FUN_0056BBD4` at two entry points
   (`FUN_00308F5C` and `FUN_0030A594`), any date reaching the game's logic is
   silently normalized. No bounds checks scattered through the codebase.

---

## Hypotheses — Resolved

### H0 — [[FUN_00308F5C]] clamps in tick-space ❌ WRONG LOCATION, RIGHT CHAIN
`FUN_00308F5C` is indeed where year normalization happens, but not via tick arithmetic.
The actual clamp is delegated to `FUN_0056BBD4` called at Phase 1.
`FUN_00124120` and `FUN_00126514` inside have no clamping.

### H1 — Cap in initial RTC reader / SetDateTimeBg ❌ NOT THE SOURCE
Downstream arithmetic functions have no caps. The wrap is applied by `FUN_0056BBD4`
inside the date normalizer, not at the UI or RTC ingestion layer.

### H2 — [[FUN_002FB34C]] is the reset-to-2012 handler ❌ DISCARDED
Plain getter: `return *DAT_002FB358`. Ghidra non-returning was a false positive.

### H3 — [[FUN_0011F340]] field +0x4 cap at 300 ❌ UNRELATED
Field `+0x4` capped at 300 is unrelated to the year limit. Semantics still unknown
but not a factor in the 2050 mechanism.

### ✅ CONFIRMED — [[FUN_0056BBD4]] (Session 10)
See "Confirmed Source" section above.

---

## Open Questions (post-resolution)

These are no longer blocking but remain interesting:

| Question | Status |
|----------|--------|
| Where is RTC initially read? Who writes `DAT_002FB3A4`? | Open — unrelated to 2050 limit |
| What is field `+0x4` in time object (capped at 300)? | Open — semantics unknown |
| Who writes `DAT_002FB358` (second singleton)? | Open |

---

## Investigation Log

| Date | Finding |
|------|---------|
| 2026-03-03 | Initial (wrong) hypothesis: weather seed from date, limit at 2030 |
| 2026-03-03 | [[FUN_002F755C]] confirmed NOT RNG — state index getter |
| 2026-03-03 | State table sentinel = 0xA5 (165) — possible table size |
| 2026-03-03 | [[FUN_001E5BC8]] found — weather init, loop 0x18×9 from data source |
| 2026-03-03 | [[FUN_001E63DC]] found — indexes source as `time_field + type * 0x18` |
| 2026-03-04 | **Year confirmed: 2050** (not 2030). Reset target: January 1, 2012. |
| 2026-03-04 | SVC 0x28 = GetSystemTick — time is tick-derived, not direct RTC |
| 2026-03-04 | 52×24 weather table in [[FUN_0062E084]] — annual, no year encoding |
| 2026-03-04 | [[FUN_0012F8A0]] has no year cap — limit not here |
| 2026-03-04 | [[FUN_002FB34C]] is a non-returning function called on date range failure (CORRECTED 2026-03-04 session 5: plain getter) |
| 2026-03-09 | [[FUN_0062EF1C]] fully understood: normalizes dates via [[FUN_00308F5C]], compares periods via [[FUN_002FD4A0]], dispatches to [[FUN_0031872C]] or [[FUN_002FB34C]] |
| 2026-03-09 | [[FUN_002FD4A0]] returns signed period-count delta (not void); scale=1000000, magic shift=45 |
| 2026-03-09 | [[FUN_00308F5C]] decomp: tick-space normalization with DAT_003090f0=0x30B8A000 — **leading candidate for 2050 clamp** |
| 2026-03-09 | [[FUN_00134CD8]] = 64-bit logical right shift (param_3 = bit count); [[FUN_00133080]] = 64-bit multiply+shift helper |
| 2026-03-21 | [[FUN_00126514]] decompiled: no clamp, pure ticks→date via [[FUN_0012952c]] |
| 2026-03-21 | [[FUN_00124120]] decompiled: no clamp, pure date→ticks |
| 2026-03-21 | **[[FUN_0056BBD4]] identified as the 2050 limit source.** Modular year wrap: year >= 2051 → year -= 39. Pivot DAT_0056bc3c = 2051 confirmed from ELF offset 0x47bc3c. Valid range [2012, 2050]. Investigation CLOSED. |

---

## See Also

[[FUN_002FB39C]], [[FUN_0062E094]], [[FUN_0062EEE8]], [[FUN_0062EF1C]], [[FUN_002FD4A0]],
[[FUN_001296BC]], [[FUN_0012F8A0]], [[FUN_0012994C]], [[FUN_0011F340]],
[[FUN_001E63DC]], [[FUN_001E5BC8]], [[FUN_001E82E0]], [[FUN_00308F5C]],
[[FUN_00312A5C_0030A594]], [[FUN_0031872C]], [[binary/EnvironmentParameter]]

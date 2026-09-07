---
address: 0x0056BBD4
name: NormalizeYearRange_2050Limit
tags: [function, date, year, limit-2050, CONFIRMED]
status: fully understood — SOURCE OF THE 2050 LIMIT
---

# [[FUN_0056BBD4|NormalizeYearRange_2050Limit]] — Year Range Normalizer (2050 Limit Source)

## ⭐ KEY FINDING

**This function is the direct source of the 2050 date limit.**

- Valid year range: **[2012, 2050]** (39 years)
- Year 2050 → untouched (2050 < 2051 → in range)
- Year 2051 → mapped to **2012** (2051 − 39 = 2012)
- Year 2052 → mapped to 2013, etc.

The limit is NOT a clamp — it is a **modular wrap with a 39-year period**.

---

## Signature
```c
void FUN_0056bbd4(int *param_1);
// param_1[0] = year field of date struct (absolute year, e.g. 2024)
```

---

## Decompiled (annotated)

```c
void FUN_0056bbd4(int *param_1) {
    int pivot = DAT_0056bc3c;       // = 2051 (confirmed from ELF)
    int year  = *param_1;
    int lower = pivot - 0x27;       // = 2051 - 39 = 2012

    if (year < pivot) {             // year < 2051
        if (year >= lower) return;  // year in [2012, 2050] → valid, no change

        // year < 2012: shift forward by 39
        year += 39;
        *param_1 = year;
        if (year >= lower) return;  // now in range

        year += 39;                 // original + 78
        if (lower < year) year = lower; // cap
        *param_1 = year;

    } else {                        // year >= 2051
        year -= 39;                 // 2051→2012, 2052→2013, ...
        *param_1 = year;
        if (year < pivot) return;   // now < 2051 → done

        year -= 39;                 // try again (year was >= 2090)
        *param_1 = year;
        if (year < pivot) return;

        // fallthrough: set to lower bound
        *param_1 = lower;           // = 2012
    }
}
```

---

## Constants (confirmed from ELF)

| Address | Value | Meaning |
|---------|-------|---------|
| `DAT_0056bc3c` | `0x803` = **2051** | First invalid year (pivot) |
| `0x27` | 39 | Cycle length in years |
| `pivot - 0x27` | **2012** | Reset target / lower bound |

ELF raw read: `file_offset = 0x47bc3c`, `[BitConverter]::ToUInt32($bytes, 0x47bc3c) = 2051`

---

## Mechanism

```
year ∈ [2012, 2050]  →  unchanged          ← valid game range
year = 2051           →  2051 - 39 = 2012   ← RESET (Jan 1 2051 → Jan 1 2012)
year = 2052           →  2052 - 39 = 2013
year = 2090           →  2090 - 39 - 39 = 2012 (two shifts)
year < 2012           →  year + 39 or year + 78 (forward correction)
```

---

## Why This Matches Community Observations

Community reports: game resets to **January 1, 2012** after **December 31, 2050**.

Exact match:
- Dec 31, 2050: year=2050 < 2051 → function returns unchanged → valid
- Jan 1, 2051 (midnight roll): year=2051 → function fires → year becomes 2012
- Result: date becomes January 1, 2012 ✓

---

## Call Contexts

Called from two places:
1. **`FUN_00308F5C`** — date normalizer (Phase 1 post-conversion fixup)
2. **`FUN_0030A594`** — date offset calculator (final fixup after arithmetic)

Both paths apply this correction, so any date arithmetic that produces a year ≥ 2051
gets silently wrapped back.

---

## See Also

[[FUN_00308F5C]], [[FUN_00312A5C_0030A594]], [[FUN_00126514]], [[research/2050-limit]]

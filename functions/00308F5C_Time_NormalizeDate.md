---
address: 0x00308F5C
name: NormalizeDate
tags: [function, time, date, normalization]
status: fully understood — date normalizer; 2050 limit applied via [[FUN_0056BBD4]]
---

# [[FUN_00308F5C|NormalizeDate]] — Date Normalizer

## Signature
```c
void FUN_00308f5c(uint *param_1);
```

## Purpose

Normalizes a date object in-place. Called by `FUN_0062EF1C` on both the input
date and the current time before comparison. Does two things:
1. Converts through tick-space to apply a 6-month offset (season alignment)
2. Applies year range correction via `FUN_0056BBD4` — **this is where the 2050 limit lives**

All sub-functions fully analyzed as of Session 10.

## Structure (from decompile)

### Phase 1 — Tick-space conversion + year wrap
```c
FUN_00124120(&local, param_1);   // date → 64-bit ticks (no clamp)
local += DAT_003090f0 * (-6);    // subtract 6-month offset in tick space
FUN_00126514(&local, &result);   // 64-bit ticks → date fields (no clamp)
*param_1 = result;
FUN_0056bbd4(param_1);           // ⭐ year range normalizer — applies [2012,2050] wrap
```

### Phases 2–5 — Sub-field carry propagation
Four passes normalize overflow in sub-day fields (seconds→minutes→hours→days):
```c
FUN_0012f998(&tmp, year, month, day_field);  // Julian Day from sub-fields
FUN_00126514(&out, &tmp);                    // back to date struct
*param_1 = out;
```

## Sub-functions (all resolved)

| Function | Role | Status |
|----------|------|--------|
| `FUN_00124120` | Date → 64-bit ticks | Fully understood — no year clamp |
| `FUN_00126514` | 64-bit ticks → date fields | Fully understood — no year clamp |
| `FUN_0056BBD4` | **Year range normalizer — 2050 limit source** | Fully understood |
| `FUN_0012F998` | Julian Day from (year, month, day sub-field) | Fully understood |

## Constants
| Address | Value | Notes |
|---------|-------|-------|
| `DAT_003090f0` | `0x30B8A000` | Tick-space 6-month offset base |
| `DAT_003090f4` | `0xD7B634DB` | Fixed-point magic constant (268 MHz tick arithmetic) |
| `DAT_003090f8` | `0x431BDE82` | Fixed-point magic constant (268 MHz tick arithmetic) |

`DAT_003090f4` and `DAT_003090f8` appear in `FUN_00133080` — fixed-point helpers
for converting between ticks and time units at 268 MHz 3DS clock rate.

## The 2050 Limit — Resolved

The 2050 limit is **not** in the tick-space arithmetic of this function.
`FUN_00124120` and `FUN_00126514` are pure converters with no clamping.
The limit is applied by `FUN_0056BBD4` (Phase 1 fixup):

```
year ∈ [2012, 2050]  →  unchanged
year >= 2051         →  year -= 39  (2051→2012, 2052→2013, ...)
year < 2012          →  year += 39  (forward correction)
```

Pivot constant `DAT_0056bc3c = 2051`, confirmed from ELF at file offset `0x47bc3c`.
This is a deliberate **39-year modular wrap**, not overflow protection.
Valid game range [2012, 2050] = 39 years from Japan release date.

## See Also

[[FUN_0062EF1C]], [[FUN_00124120]], [[FUN_00126514]], [[FUN_0056BBD4]], [[FUN_0012F998]], [[research/2050-limit]]

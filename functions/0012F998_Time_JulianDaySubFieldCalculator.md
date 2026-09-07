---
address: 0x0012F998
name: JulianDaySubFieldCalculator
tags: [function, time, date, julian-day]
status: fully understood — Julian Day variant for sub-field carry propagation
---

# [[FUN_0012F998|JulianDaySubFieldCalculator]] — Julian Day Sub-field Calculator

## Signature
```c
void FUN_0012F998(uint64_t* out, uint year, uint month, uint day_field);
```

## Purpose

Computes a tick/Julian Day value from (year, month, day_field).
Variant of [[FUN_0012F8A0]] used specifically for sub-field normalization.

## Call Context ([[FUN_00308F5C]])

Called **4 times** in Phases 2–5 of [[FUN_00308F5C]], performing carry propagation
up through date sub-fields (seconds→minutes→hours→days):

```c
FUN_0012f998(&tmp, year_field, month_field, day_variant);
FUN_00126514(&out, &tmp);
*param_1 = out;
```

Each pass normalizes one level of the date struct — overflow in seconds
propagates to minutes, then hours, then days, then months/years.

## Findings (Session 10)

No year clamp. Pure arithmetic, same pattern as [[FUN_0012F8A0]].

## See Also

[[FUN_00308F5C]], [[FUN_0012F8A0]], [[FUN_001296BC]], [[FUN_00126514]]

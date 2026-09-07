---
address: 0x0062EF1C
name: ResolveWeatherPeriodIndex
tags: [function, time, date, period-index]
status: understood — period-index resolver with date range validation
---

# [[FUN_0062EF1C|ResolveWeatherPeriodIndex]] — Period Index Resolver

## Signature
```c
int FUN_0062ef1c(undefined4 *param_1);
```
Ghidra marks return as void — incorrect. Returns period index in r0.

## Purpose

Given a date object (param_1), resolves and returns a **period index** used
by the weather table lookup system (`FUN_0062E094`). Validates the input date
against the current game time before computing.

## Full Control Flow

```c
int FUN_0062ef1c(int *param_1) {

    // 1. Early exit: special game state (tutorial/loading, etc.)
    if (FUN_0030601c() == 3)
        return FUN_002fb34c();  // tail-call → *DAT_002FB358

    // 2. Normalize input date into local copy
    local_date = copy(param_1);
    FUN_00308f5c(&local_date);   // normalize (clamp? validate?)

    // 3. Get current time + normalize
    int *cur = FUN_002fb39c();   // primary time singleton
    local_cur = copy(cur[0..2]);
    FUN_00308f5c(&local_cur);

    // 4. Compare periods
    int cmp = FUN_002fd4a0(&local_date, &local_cur, 1);
    // cmp = signed period-count difference (param_1 ahead of current)

    // 5. Dispatch
    if (cmp == 0) return FUN_002fb34c();   // same period → fallback
    if (cmp == 1) return FUN_002fb34c();   // 1 period ahead → fallback
    // cmp != 0 && cmp != 1 (far future or past):
    FUN_0031872c(param_1);   // shift -6 months, then → *DAT_002FB358
    return;
}
```

## Return Value

All paths ultimately return `*DAT_002FB358` via `FUN_002FB34C` (directly or via
`FUN_0031872C`). The object behind `DAT_002FB358` determines the actual period index.

## [[FUN_00308f5c]] — Date Normalizer

Called twice: once for input date, once for current time.
Converts to 64-bit ticks, applies 6-month offset arithmetic, converts back,
then calls [[FUN_0056BBD4]] which applies the **39-year modular wrap [2012, 2050]**.
`DAT_003090f0 = 0x30B8A000` — tick-space 6-month offset base.

> [!IMPORTANT] 2050 limit resolved
> [[FUN_00308F5C]] itself does not clamp. The wrap is applied by [[FUN_0056BBD4]]
> called at Phase 1. See [[research/2050-limit]].

## Semantics of cmp == 0 and cmp == 1

Both cause fallback via `FUN_002FB34C`. Interpretation:
- `cmp == 0`: input date in same period as now → no need to compute
- `cmp == 1`: input date 1 period ahead → boundary case, fallback safe
- Other: normal resolution via −6 month shift

## Open Questions

- What is `DAT_002FB358`? Who writes to the object it points to?
- Why does cmp==1 fall back instead of taking the normal path?

## See Also

[[FUN_0062E094]], [[FUN_002FD4A0]], [[FUN_002FB34C]], [[FUN_00308F5C]], [[FUN_0031872C]], [[FUN_0030601C]]

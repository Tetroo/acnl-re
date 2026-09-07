---
address: 0x0062EEE8
name: ResolvePeriodIndexFromDate
tags: [function, weather, period, date]
status: fully understood — period index resolver, 3 paths
---

# [[FUN_0062EEE8|ResolvePeriodIndexFromDate]] — Period Index Resolver

## Signature
```c
void FUN_0062eee8(date_obj* param_1);
// Returns via FUN_002fb34c — result stored in DAT_002FB358 singleton
```

## Purpose

Computes the current weather **period index** from a date object and stores it
in the singleton `DAT_002FB358`. Always terminates by calling `FUN_002fb34c()`
(the getter for that singleton). The result is used as `param_2` in `FUN_001E82E0`.

## Logic

```c
void FUN_0062eee8(date_obj* param_1) {
    // Path A: flag 0x20000 set → use JulianDay % 3 period
    if (FUN_002f7574(0x20000, FUN_002f755c()) != 0) {
        FUN_0062efd4();  // JulianDay % 3 → period
        return;
    }

    // Path B: FUN_0030601c() == 3 → use singleton as-is
    if (FUN_0030601c() == 3) {
        FUN_002fb34c();
        return;
    }

    // Path C: Normal path
    date_copy_a = *param_1;
    FUN_00308f5c(&date_copy_a);   // normalize date

    date_copy_b = *FUN_002fb39c(); // get current date
    FUN_00308f5c(&date_copy_b);   // normalize current date

    int cmp = FUN_002fd4a0(&date_copy_a, &date_copy_b, 1);
    if (cmp == 0 || cmp == 1) {
        FUN_002fb34c(); return;   // date not past threshold → use current period
    }
    // cmp is other → normal period calc
    FUN_0031872c(param_1);        // → shifts date by -6 units, then FUN_002fb34c
    return;
}
```

## Key Dependencies

| Function | Role |
|---|---|
| [[FUN_002F7574]]`(0x20000, ...)` | Checks flag bit 0x20000 |
| [[FUN_0062EFD4]] | JulianDay % 3 period path |
| [[FUN_0030601C]] | Game state reader — value 3 = special phase (tutorial/loading), bypasses normal lookup |
| [[FUN_00308F5C]] | Date normalization — applies 6-month offset + 2050 year wrap via [[FUN_0056BBD4]] |
| [[FUN_002FD4A0]] | Period-count delta: returns signed count of how far param_1 leads param_2 |
| [[FUN_0031872C]] | Normal period compute: shifts date by −6 months → [[FUN_002FB34C]] |
| [[FUN_002FB34C]] | Singleton getter: returns `*DAT_002FB358` |

## DAT_002FB358 is a Pointer

`FUN_002fb34c` returns `*DAT_002FB358` — it dereferences a pointer.
`DAT_002FB358` holds a **pointer to an object**, not the period_index itself.
Who writes to this object is still unknown.

## See Also

[[FUN_001E82E0]], [[FUN_002FB34C]], [[FUN_0031872C]], [[FUN_0030601C]], [[FUN_00308F5C]], [[FUN_0056BBD4]]

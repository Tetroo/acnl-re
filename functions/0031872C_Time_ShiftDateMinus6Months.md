---
address: 0x0031872C
name: ShiftDateMinus6Months
tags: [function, date, period, utility]
status: understood
---

# [[FUN_0031872C|ShiftDateMinus6Months]] — Normal Period Calculation (date shift -6 months)

## Signature
```c
void FUN_0031872c(date_obj* param_1);
// Returns via FUN_002fb34c
```

## Purpose

The "normal path" period calculator. Copies the input date, shifts it back
by 6 months via `FUN_00312a5c`, then falls through to `FUN_002fb34c`
(the DAT_002FB358 singleton getter).

## Logic

```c
void FUN_0031872c(date_obj* param_1) {
    date_copy = *param_1;
    FUN_00312a5c(&date_copy, 0, 0xfffffffa, 0, 0);  // shift by -6 months
    FUN_002fb34c();  // return *DAT_002FB358
}
```

## Notes

- `0xfffffffa` = -6 as unsigned — passed as `months` offset to `FUN_00312a5c`
- After the shift, result ends up in the DAT_002FB358 singleton object
- Ghidra marks [[FUN_002fb34c]] as "non-returning" here — confirmed incorrect,
  it's a plain getter

## See Also

[[FUN_0062EEE8]], [[FUN_00312A5C_0030A594]], [[FUN_002FB34C]]

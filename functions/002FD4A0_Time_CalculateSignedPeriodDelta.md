---
address: 0x002FD4A0
name: CalculateSignedPeriodDelta
tags: [function, time, date, delta, period-count]
status: better understood — returns period-count delta between two timestamps
---

# [[FUN_002FD4A0|CalculateSignedPeriodDelta]] — Date Period-Count Delta

## Signature
```c
int FUN_002fd4a0(undefined4 *param_1, undefined4 *param_2, int param_3);
```
Ghidra marks return as void — incorrect. Returns signed period-count in r0.

## Purpose

Converts two time objects to 64-bit millisecond timestamps via `FUN_001296BC`,
subtracts them, and divides by a period length via fixed-point arithmetic.
Returns the signed number of periods by which param_1 leads param_2.

## Key Logic

```c
// Convert both dates to 64-bit ms timestamps (epoch 2000)
uint64 ts1 = FUN_001296bc(param_1);
uint64 ts2 = FUN_001296bc(param_2);

// Optional DST/leap adjustment (if param_3 && DAT_002FD594 flag bit set
// AND ts1 < ts2 — i.e. param_1 is earlier):
if (param_3 && (*(DAT_002fd594 + 2) & 1) && ts1 < ts2) {
    ts1 += FUN_00133080(DAT_002fd598, ...);  // add adjustment (DST offset?)
}

// Compute delta scaled by 1000000
uint64 delta = (ts1 - ts2) * 1000000;  // DAT_002fd5a4 = 1000000

// Divide by period length via magic-number multiplication >> 45
int result = (delta * magic) >> 45;
// magic = DAT_002fd5a8:DAT_002fd5ac, shift = 0x2D = 45
```

## Period Length

The divisor is encoded as a magic number for multiplication + shift-45.
`DAT_002fd5a8 = 0x48F948E3`, `DAT_002fd5ac = 0x683FFF6F`.
Exact period length (in ms) not yet decoded — likely 7 days (604800000 ms)
or 1 day. Decoding requires knowing exact input unit from `FUN_001296BC`.

## Return Value Semantics (inferred from [[FUN_0062EF1C]] usage)

| Return | Meaning |
|--------|---------|
| `0`    | param_1 is in the same period as param_2 |
| `1`    | param_1 is 1 period ahead of param_2 |
| `>=2`  | param_1 is far ahead — normal path |
| `<0`   | param_1 is in the past |

In `FUN_0062EF1C`, returns 0 and 1 both lead to `FUN_002FB34C` (fallback);
other values take the normal period-calculation path via `FUN_0031872C`.

## Constants
| Address | Value | Role |
|---------|-------|------|
| `DAT_002FD5A4` | `0x000F4240` = 1000000 | Scale factor |
| `DAT_002FD5A8` | `0x48F948E3` | Magic multiplier low |
| `DAT_002FD5AC` | `0x683FFF6F` | Magic multiplier high |
| `DAT_002FD594` | unknown | Flag register (DST?) |
| `DAT_002FD598/9C/A0` | unknown | DST adjustment value |

## See Also

[[FUN_001296BC]], [[FUN_0062EF1C]], [[FUN_00134CD8]], [[FUN_00133080]]

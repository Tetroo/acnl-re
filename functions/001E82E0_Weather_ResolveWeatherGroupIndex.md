---
address: 0x001E82E0
name: ResolveWeatherGroupIndex
tags: [function, weather, mapping, understood]
status: fully understood
---

# [[FUN_001E82E0|ResolveWeatherGroupIndex]] — Weather Group Index Resolver

## Signature
```c
int FUN_001E82E0(int weather_type, int period_index);
```

## Purpose

Maps `(weather_type, period_index)` → `group_index` into EnvironmentParameter.bin.

Called from `FUN_001E63DC` twice — once for the current date, once for date+1
(to enable smooth interpolation across day/period boundaries).

## Call Context (from [[FUN_001E63DC]])

```c
uVar5 = FUN_0062eee8(&local_84);   // period_index for current date
uVar6 = FUN_0062e084();            // weather_type (thunk)
iVar2 = FUN_001e82e0(uVar6, uVar5); // → group_index for current

uVar5 = FUN_0062eee8(&local_78);   // period_index for date+1
iVar7 = FUN_001e82e0(uVar4, uVar5); // → group_index for next
```

`param_2` is a **period index**, not raw hour. Produced by `FUN_0062eee8`.

## Logic (decompiled)

```c
int FUN_001e82e0(int weather_type, int period_index) {
    if (period_index == 0x32) return 7;  // special period → Sakura group
    if (period_index == 0x33) return 8;  // special period → fallback/placeholder group

    switch (weather_type) {
    case 0:
    case 1:
        if (period_index - 0x20 < 3)  return 3;  // periods 32–34
        if (period_index - 0x26 < 6)  return 5;  // periods 38–43
        if (period_index - 0x2c < 6)  return 6;  // periods 44–49
        return 0;                                  // default: Clear
    case 2:
        if (period_index - 0x23 < 3)  return 4;  // periods 35–37
        // fallthrough to case 3
    case 3:
    case 5:
        return 1;
    case 4:
    case 6:
        return 2;
    default:
        return 0;
    }
}
```

## Group Index Meanings (confirmed)

| Group | Index | Notes |
|-------|-------|-------|
| Clear | 0 | Default/sunny |
| Overcast/Rain A | 1 | weather_type 3 or 5 |
| Overcast/Rain B | 2 | weather_type 4 or 6 |
| Clear variant | 3 | weather_type 0/1, periods 32–34 |
| Rain special | 4 | weather_type 2, periods 35–37 |
| Overcast (mid) | 5 | weather_type 0/1, periods 38–43 |
| Overcast/clear mix | 6 | weather_type 0/1, periods 44–49 |
| Sakura | 7 | period_index == 0x32 (50) |
| Fallback/placeholder | 8 | period_index == 0x33 (51) |

Matches EnvironmentParameter.bin analysis from Session 6:
- G7 (group 7): all 24 hours identical — Sakura confirmed
- G8 (group 8): all 24 hours identical — fallback/placeholder confirmed

## Record Index Formula

```
record_index = group_index * 0x18 + hour_of_day
```
Where `0x18 = 24`. Final index into 216-record EnvironmentParameter.bin table.

## New Function Discovered

`FUN_0062eee8` — extracts period_index from a date object.
Called on both current date and date+1. Must be reversed next.

## See Also

[[FUN_001E63DC]], [[FUN_0062eee8]], [[FUN_0062E094]], [[binary/EnvironmentParameter]]

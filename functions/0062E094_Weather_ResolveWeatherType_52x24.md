---
address: 0x0062E094
name: ResolveWeatherType_52x24
tags: [function, weather, lookup, critical]
status: understood - real weather type resolver
---

# [[FUN_0062E094|ResolveWeatherType_52x24]] - Weather Type Resolver (52x24 Table Lookup)

## Signature
`c
char [[FUN_0062e094]](int *date_obj);
`

## Purpose

Resolves weather type byte from a 52x24 lookup table indexed by
**period index** (0-51) and **normalised hour** (0-23).
Called twice per frame by [[FUN_001E63DC]]: once for current date, once for date+1.

Previously misattributed to [[FUN_0062E084]] (which is a thunk).

## Key Logic

```c
char [[FUN_0062e094]](int *date_obj) {
    int state = [[FUN_002f755c]]();
    if (state == 0x5D || state == 0x99 || state == 0x9B) return 0;

    char base_type = *DAT_0062e1a8;
    if (base_type != 7) return base_type;

    // Get period index
    uint week;
    if ([[FUN_002f7574]](0x20000, state) == 0)
        week = [[FUN_0062ef1c]](date_obj);   // normal path
    else
        week = [[FUN_0062efd4]]();           // alternate: JulianDay % 3

    // Normalised hour: day boundary at 06:00
    uint hour = *(char*)(date_obj + 7) - 6;
    if ((int)hour < 0) hour += 18;

    // Table lookup (52 weeks x 24 hours)
    if (week < 52 && hour < 24)
        base_type = *(DAT_0062e1ac[week] + hour);

    // Heavy rain promotion (flag 8)
    if ([[FUN_002f7574]](0x20000, state) == 0 && [[FUN_001e5eb0]](8) == 1) {
        if (base_type == 3) base_type = 5;
        else if (base_type == 4) base_type = 6;
    }
    return base_type;
}
```


## Table Structure

DAT_0062E1AC = array of 52 pointers (one per week/period).
Each row: 24 bytes, one per normalised hour.
Table is annual — does NOT encode year.

## Special States

0x5D, 0x99, 0x9B -> return 0 immediately (indoors / cutscene).

## Heavy Rain

Types 3->5, 4->6 when [[FUN_001E5EB0]](8)==1 (precipitation intensity flag).

## Flag 0x20000 (bit 17)

Gates source of period index:
- Clear: [[FUN_0062EF1C]] (normal week-based lookup)
- Set: [[FUN_0062EFD4]] (JulianDay % 3, 3-cycle)

## See Also

[[FUN_0062E084]] — thunk entry point  
[[FUN_0062EF1C]] — period index calculator  
[[FUN_0062EFD4]] — alternate period (JulianDay % 3)  
[[FUN_001E63DC]] — caller

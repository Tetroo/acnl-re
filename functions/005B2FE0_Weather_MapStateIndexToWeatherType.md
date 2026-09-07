---
address: 0x005B2FE0
name: MapStateIndexToWeatherType
tags: [function, weather, state, mapping]
status: fully understood — state index → weather type mapper
---

# FUN_005B2FE0 — State Index → Weather Type Mapper

## Signature
```c
uint FUN_005b2fe0(uint state_index);
```

## Purpose

Converts a raw state index (from [[FUN_002F755C]]) into a weather type integer
used by the record index formula in [[FUN_001E63DC]].

Core lookup: `DAT_005b3068[state_index]` — a byte table of 165 entries (0xA5 max).

## Logic

```c
uint FUN_005b2fe0(uint param_1) {
    if (param_1 >= 0xa5) return 0xFFFFFFFF;  // invalid state

    uint type = DAT_005b3068[param_1];  // table lookup

    if (type == 0x15) {
        // Special case: type 21 — check additional conditions
        // Get an object for this state index via FUN_006f7c54
        int obj = FUN_006f7c54(param_1);
        if (obj != 0
            && FUN_0071f3ec(obj, 3) > 1
            && FUN_0071f3ec(obj, 4) > 1
            && FUN_0071f3ec(obj, 5) > 1) {
            return 3;  // promote to type 3 (heavy variant)
        }
        return 2;  // stay at type 2
    }

    return type;
}
```

## Key Data

| Address | Description |
|---------|-------------|
| `DAT_005b3068` | Byte table [165 entries]: `state_index → weather_type` |
| `DAT_005b306c` | Related state object (checked at `+2` for state match) |

## Return Values (used in FUN_001E63DC)

| Return | Meaning |
|--------|---------|
| `3` | → record index forced to `0x16` (22) |
| `0xB` (11) | → record index forced to `0x16` |
| `0x18` (24) | → record index forced to `0x16` |
| other | → normal: `record = time_field + type * 0x18` |
| `0xFFFFFFFF` | → invalid/out-of-range state |

## Special Type 0x15

When the table returns `0x15` (21), the function runs three additional checks
via `FUN_0071f3ec(obj, 3/4/5)`. If all return > 1 → promotes to type 3.
Otherwise stays at type 2. Likely: "is this a heavy rain/snow state?"

## Open Questions

- Contents of `DAT_005b3068` table not read — would reveal full state→type mapping
- `FUN_006f7c54` — object getter by state index, not yet decompiled
- `FUN_0071f3ec` — returns a count or level for a given parameter (3/4/5), not yet decompiled

## See Also

[[FUN_001E63DC]], [[FUN_002F755C]], [[FUN_002F7574]]

---
address: 0x002960D0
tags: [function, weather, particle, sakura]
status: understood
---
# [[FUN_002960D0]] — Sakura Update

**Address:** `0x002960D0`  
**Section:** `.text`  
**Type:** Weather particle update  

---

## Signature
```c
bool FUN_002960D0(int particle_context);
```

## Decompiled
```c
bool FUN_002960D0(int param_1)
{
    *(int *)(DAT_00296174 + 0xC) = param_1;

    // RNG #1 — particle state (same as Rain/Snow/Paper)
    uVar2 = FUN_002F755C();
    uVar1 = FUN_002F7574(0x400000, uVar2);
    *(undefined1 *)(DAT_00296178 + param_1) = uVar1;

    // RNG #2 — wind direction (unique to Sakura)
    uVar2 = FUN_002F755C();
    iVar3 = FUN_002F7574(0x10000000, uVar2);         // bit 28 as boolean
    puVar4 = (iVar3 == 0) ? DAT_00296180 + 2         // variant B
                          : DAT_00296180;             // variant A
    *(char *)(DAT_0029617C + param_1) = (char)iVar3;
    *(undefined4 *)(param_1 + 0x661C) = puVar4[0];  // wind_x
    *(undefined4 *)(param_1 + 0x6620) = puVar4[1];  // wind_y

    local_c = "BsWeatherSakura";
    iVar3 = FUN_003175C8(param_1 + 0x124,
                         DAT_0029619C,
                         *DAT_00296184,
                         &local_10,
                         1, 0);
    return iVar3 != 0;
}
```

## Analysis

Most complex of the four Update functions. Performs **two** RNG calls.

**RNG #2 — wind direction:**  
Mask `0x10000000` (bit 28). Effectively a random boolean. Selects one of two `uint32[2]` pairs from `DAT_00296180`:
- `iVar3 != 0` → `DAT_00296180[0]` and `[1]`
- `iVar3 == 0` → `DAT_00296180[2]` and `[3]`

Written into the particle object at `+0x661C` and `+0x6620` — likely float velocity or direction vectors for the petal drift.

### Object layout (partially known from Sakura)
| Offset | Size | Description |
|--------|------|-------------|
| `+0x124` | — | BehaviourScript node |
| `+0x661C` | uint32 | wind_x param |
| `+0x6620` | uint32 | wind_y param |

The object is large (offset 0x6620+ is non-trivial). This might be the main weather object, not just a per-particle struct.

## See also

[[FUN_00229684]] (Rain), [[FUN_0022A9C8]] (Snow), [[FUN_002663F0]] (Paper)  
[[weather]]

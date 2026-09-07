---
address: 0x00229684
tags: [function, weather, particle, rain]
status: understood
---
# [[FUN_00229684]] — Rain Update

**Address:** `0x00229684`  
**Section:** `.text`  
**Type:** Weather particle update  

---

## Signature
```c
bool FUN_00229684(int particle_context);
```

## Decompiled
```c
bool FUN_00229684(int param_1)
{
    undefined1 uVar1;
    undefined4 uVar2;
    int iVar3;
    undefined4 local_10;
    char *local_c;

    *(int *)(DAT_002296F4 + 4) = param_1;           // store context at DAT+4
    uVar2 = FUN_002F755C();                          // get RNG value
    uVar1 = FUN_002F7574(0x400000, uVar2);           // normalize: mask 0x400000
    *(undefined1 *)(DAT_002296F8 + param_1) = uVar1; // write state byte

    local_10 = DAT_00229710;
    local_c = "BsWeatherRain";
    iVar3 = FUN_003175C8(param_1 + 0x124,
                         DAT_00229714,
                         *DAT_002296FC,
                         &local_10,
                         1, 0);                      // launch BehaviourScript
    return iVar3 != 0;
}
```

## Analysis

Follows a shared pattern across all four weather Update functions:
1. Store `particle_context` into a DAT slot
2. Read current state index via [[FUN_002F755C]], check bit via [[FUN_002F7574]], write result to state array
3. Launch BehaviourScript by name on the particle object

`param_1 + 0x124` — offset to the BehaviourScript node within the particle object.  
`0x400000` — probability mask (~1-in-64 chance if used as threshold).

## Parameters

| | Type | Notes |
|-|------|-------|
| `param_1` | `int` | Particle object pointer or index |

## Returns

`bool` — BehaviourScript launch result

## Calls

| Address | Role |
|---------|------|
| `FUN_002F755C` | RNG — get value |
| `FUN_002F7574` | RNG — normalize with mask |
| `FUN_003175C8` | BehaviourScript launcher |

## See also

[[FUN_0022A9C8]] (Snow), [[FUN_002663F0]] (Paper), [[FUN_002960D0]] (Sakura)  
[[weather]]

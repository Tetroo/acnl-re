---
address: 0x002663F0
tags: [function, weather, particle, paper]
status: understood
---
# [[FUN_002663F0]] — Paper Update

**Address:** `0x002663F0`  
**Section:** `.text`  
**Type:** Weather particle update  

---

## Signature
```c
bool FUN_002663F0(int particle_context);
```

## Decompiled
```c
bool FUN_002663F0(int param_1)
{
    *(int *)(DAT_00266460 + 0xC) = param_1;          // store context at DAT+0xC
    uVar2 = FUN_002F755C();
    uVar1 = FUN_002F7574(0x400000, uVar2);
    *(undefined1 *)(DAT_00266464 + param_1) = uVar1;

    local_c = "BsWeatherPaper";
    iVar3 = FUN_003175C8(param_1 + 0x124,
                         0x19C0,               // literal constant, not a DAT pointer
                         *DAT_00266468,
                         &local_10,
                         1, 0);
    return iVar3 != 0;
}
```

## Open Questions

- Second argument to [[FUN_003175C8]] is a **literal `0x19C0`** (6592), not a DAT reference. Rain and Snow pass a DAT pointer there. Possible interpretations: particle count, update interval, hardcoded resource ID. Needs [[FUN_003175C8]] to be reversed.

## See also

[[FUN_00229684]] (Rain), [[FUN_0022A9C8]] (Snow), [[FUN_002960D0]] (Sakura)  
[[weather]]

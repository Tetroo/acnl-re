---
address: 0x0022A9C8
tags: [function, weather, particle, snow]
status: understood
---
# [[FUN_0022A9C8]] — Snow Update

**Address:** `0x0022A9C8`  
**Section:** `.text`  
**Type:** Weather particle update  

---

## Signature
```c
bool FUN_0022A9C8(int particle_context);
```

## Decompiled
```c
bool FUN_0022A9C8(int param_1)
{
    *(int *)(DAT_0022AA38 + 8) = param_1;            // store context at DAT+8
    uVar2 = FUN_002F755C();
    uVar1 = FUN_002F7574(0x400000, uVar2);
    *(undefined1 *)(DAT_0022AA3C + param_1) = uVar1;

    local_c = "BsWeatherSnow";
    iVar3 = FUN_003175C8(param_1 + 0x124,
                         DAT_0022AA58,
                         *DAT_0022AA40,
                         &local_10,
                         1, 0);
    return iVar3 != 0;
}
```

## Differences from Rain

Context stored at DAT `+8` (Rain uses `+4`). Likely separate slot in a shared weather-state struct.  
BehaviourScript: `"BsWeatherSnow"`.

## See also

[[FUN_00229684]] (Rain), [[FUN_002663F0]] (Paper), [[FUN_002960D0]] (Sakura)  
[[weather]]

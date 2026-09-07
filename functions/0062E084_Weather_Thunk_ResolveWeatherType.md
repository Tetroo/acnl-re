---
address: 0x0062E084
name: Thunk_ResolveWeatherType
tags: [function, weather, thunk]
status: partial — needs further investigation (⚠️ the old "4 instructions only" description was not confirmed on re-check 2026-09-07)
---

# [[FUN_0062E084|Thunk_ResolveWeatherType]] - Weather Type Resolver Entry Point

## ⚠️ Audit 2026-09-07

The old version of this article claimed this was a 4-instruction thunk that only called `0x002fb39c` (time singleton) and returned the result. **The current `decompile_function` output for this function shows full logic**, nearly identical to `Weather_ResolveWeatherType_52x24` (`0x0062E094`) — with the same `0x5d/0x99/0x9b` state checks, the same table lookup via `PTR_DAT_0095cf9c`, the same season-parameter check. The discrepancy is either because the function was merged/de-thunked during a Ghidra re-analysis, or the old description was simply wrong to begin with — the source of the discrepancy has not been established.

## Signature
```c
char Thunk_ResolveWeatherType(void);
```

## Decompiled Implementation (real Ghidra output, 2026-09-07)

```c
char Thunk_ResolveWeatherType(void)
{
  char cVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;

  iVar2 = GetTimeSingletonPrimary();
  iVar3 = Core_GetStateIndex();
  if (((iVar3 == 0x5d) || (iVar3 = Core_GetStateIndex(), iVar3 == 0x99)) ||
     (iVar3 = Core_GetStateIndex(), iVar3 == 0x9b)) {
    cVar1 = '\0';
  }
  else {
    cVar1 = DAT_0095cf98;
    if (DAT_0095cf98 == '\a') {
      uVar4 = Core_GetStateIndex();
      iVar3 = Core_CheckStateFlag(0x20000,uVar4);
      if (iVar3 == 0) {
        uVar5 = ResolveWeatherPeriodIndex(iVar2);
      }
      else {
        uVar5 = Weather_ResolveIslandWeatherWeekIndex();
      }
      cVar1 = DAT_0095cf98;
      if (DAT_0095cf98 == '\a') {
        cVar1 = '\0';
        uVar6 = (int)*(char *)(iVar2 + 7) - 6;
        if ((int)uVar6 < 0) {
          uVar6 = (int)*(char *)(iVar2 + 7) + 0x12;
        }
        if (uVar5 < 0x34 && uVar6 < 0x18) {
          cVar1 = (&PTR_DAT_0095cf9c)[uVar5][uVar6];
        }
      }
      uVar4 = Core_GetStateIndex();
      iVar2 = Core_CheckStateFlag(0x20000,uVar4);
      if (iVar2 == 0) {
        uVar4 = Core_GetStateIndex();
        iVar2 = Core_CheckStateFlag(0x20000,uVar4);
        if ((iVar2 == 0) && (iVar2 = Weather_GetSeasonParameter(8), iVar2 == 1)) {
          if (cVar1 == '\x03') {
            cVar1 = '\x05';
          }
          else if (cVar1 == '\x04') {
            cVar1 = '\x06';
          }
        }
      }
      return cVar1;
    }
  }
  return cVar1;
}
```

This is effectively a duplicate of `Weather_ResolveWeatherType_52x24` (see [[FUN_0062E094]]) — that one is called with an explicit `date_obj*` parameter, this one uses `GetTimeSingletonPrimary()` instead of a parameter. Neither is a thunk; both contain the full 52×24 table lookup.

## Purpose

Weather type resolver for the *current* time (as opposed to `0x0062E094`, which accepts an arbitrary date_obj). Used as the primary weather-type source in most call sites (`Weather_BsThunderMgr_GenerateMinuteStrikes` and others).

## See Also

[[FUN_0062E094]] — same algorithm, with an explicit date parameter
[[FUN_002FB39C]] — time singleton getter

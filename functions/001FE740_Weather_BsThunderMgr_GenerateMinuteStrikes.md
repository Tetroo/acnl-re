# Function: Weather_BsThunderMgr_GenerateMinuteStrikes (`0x001FE740`)

> **Address:** `0x001FE740`
> **Subsystem:** Weather & Environment
> **Source Binary:** `exefs.elf`
> **Status:** Partial — needs further investigation (⚠️ previously marked "Fully Reverse-Engineered", which was wrong)
> **Related Files:** [`types/Weather.h`](../types/Weather.h), [`systems/weather.md`](../systems/weather.md)

---

## 1. Overview

`Weather_BsThunderMgr_GenerateMinuteStrikes` is called by `BsThunderMgr`. The function clears a 0x3C-byte buffer at `param_1+0x13`, checks the weather type via `Thunk_ResolveWeatherType()`, and depending on the result either calls `DateOffsetCalculator` (-6 months) and returns the save-buffer pointer, or simply caches the current time into fields `+0x50/+0x54/+0x58`.

**⚠️ Audit 2026-09-07:** The previous version of this article contained a fully fabricated "Decompiled Implementation" block — a 60-second loop with an LCG lightning generator and a seed formula `(Year<<9 + Month<<5 + Day)<<8 + ...`. None of that exists in the real code. Below is the actual `decompile_function` output.

---

## 2. Decompiled Implementation (real Ghidra output, 2026-09-07)

```c
void Weather_BsThunderMgr_GenerateMinuteStrikes(int param_1)
{
  int iVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 local_3c;
  undefined4 uStack_38;
  undefined4 local_34;

  FUN_003014d4(param_1 + 0x13,0x3c);
  iVar1 = Thunk_ResolveWeatherType();
  if (iVar1 == 4) {
    puVar2 = (undefined4 *)GetTimeSingletonPrimary();
    local_3c = *puVar2;
    uStack_38 = puVar2[1];
    local_34 = puVar2[2];
    DateOffsetCalculator(&local_3c,0,0xfffffffa,0,0);
    Save_GetSaveBufferPointer();
    return;
  }
  puVar2 = (undefined4 *)GetTimeSingletonPrimary();
  uVar3 = *puVar2;
  uVar4 = puVar2[1];
  *(undefined4 *)(param_1 + 0x58) = puVar2[2];
  *(undefined4 *)(param_1 + 0x50) = uVar3;
  *(undefined4 *)(param_1 + 0x54) = uVar4;
  return;
}
```

There is no per-second loop, no PRNG, and no lightning bitmap in this function.

## 3. What's actually known

- `FUN_003014d4(param_1 + 0x13, 0x3c)` — zeroes 60 bytes of the buffer at `+0x13`. The logic inside `FUN_003014d4` has not been examined (probably memset, but unconfirmed).
- If `Thunk_ResolveWeatherType() == 4` (weather type 4 = thunderstorm) — the current time is fetched, shifted by -6 months via `DateOffsetCalculator`, and `Save_GetSaveBufferPointer()` is called. What happens with the result is **unexamined**: the return value of `Save_GetSaveBufferPointer()` is unused in this function; the actual strike generation (if any) must either be in another function or use this 60-byte buffer at `+0x13` somewhere further down the call chain — not traced.
- If the weather type is not 4 — it simply caches the current time into `+0x50/+0x54/+0x58` (3 words).

## 4. Downstream — `Weather_BsThunderMgr_TickLightning` (0x001FE8DC), verified 2026-09-07

The real decompile of this function confirms part of the old (fabricated) article — but not the formulas, only the downstream effects:

```c
undefined4 Weather_BsThunderMgr_TickLightning(int param_1)
{
  ...
  if ((*(char *)(param_1 + 0x57) != *(char *)(time+7)) ||
      (*(char *)(param_1 + 0x58) != *(char *)(time+8))) {
    Weather_BsThunderMgr_GenerateMinuteStrikes(param_1);   // called once per hour/minute on change
  }
  cVar1 = *(char *)(time + 9);                              // current minute (0..59)
  if (*(char *)(param_1 + 0x60) != '\0') {                  // manual trigger?
    *(undefined1 *)(param_1 + cVar1 + 0x13) = 1;
    *(undefined1 *)(param_1 + 0x60) = 0;
  }
  if (*(char *)(param_1 + cVar1 + 0x13) != '\0') {          // strike_bitmap[current_minute]
    *(undefined1 *)(param_1 + cVar1 + 0x13) = 0;
    FUN_001e6304(1,0,10,100);                                // screen flash — CONFIRMED
    if (FUN_00582c40() != 0) {
      FUN_0058c40c(0, (indoor) ? 0x100077f : 0x1000780);     // thunder sound — CONFIRMED (addresses match the old article text)
    }
  }
  return 2;
}
```

Confirmed by bytes: the buffer at `+0x13` is **60 bytes, indexed by minute-of-hour (0..59)**, not by second as the old version claimed. The screen flash `FUN_001e6304(1,0,10,100)` and thunder sounds `0x1000780`/`0x100077f` are real and match the old article text — this is the one part that was not fabricated.

## 5. TODO

`HYPOTHESIS (not confirmed by bytes)`: the actual strike-pattern generation (which minutes get `strike_bitmap[m]=1`) still hasn't been found — `GenerateMinuteStrikes` as it stands doesn't fill the buffer at all (only clears it via `FUN_003014d4`, presumably memset). The fill may happen inside `FUN_003014d4` (not confirmed to be a plain memset — unverified) or in unexamined code. Needed:
1. Decompile/disassemble `FUN_003014d4` — confirm whether it's really just a memset, not memset+generator.
2. Find the real source of randomness (if any) for the strike pattern — do not invent a seed formula until a real PRNG call is found.
3. Figure out field `+0x60` (manual/forced strike trigger).

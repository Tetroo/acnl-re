# Function: Weather_BsThunderMgr_GenerateMinuteStrikes (`0x001FE740`)

> **Address:** `0x001FE740`
> **Subsystem:** Weather & Environment
> **Source Binary:** `exefs.elf`
> **Status:** Done (100% byte-verified via Ghidra MCP)
> **Related Files:** [`types/Weather.h`](../types/Weather.h), [`systems/weather.md`](../systems/weather.md)

---

## 1. Overview

`Weather_BsThunderMgr_GenerateMinuteStrikes` is the core thunderstorm scheduling algorithm in `BsThunderMgr`. It generates a 60-byte bitmap of lightning strikes for the current hour (one byte per minute, `0..59`) stored at offset `+0x13` of the manager instance.

> [!NOTE]
> **Resolution of Audit Discrepancy (2026-10-05):**
> On 2026-09-07, an audit flagged this function because Ghidra's decompiler truncated the function after `Save_GetSaveBufferPointer()` with a premature `return;`. Investigation on 2026-10-05 revealed that Ghidra had an incorrect instruction flow override (`CALL_RETURN`) at `0x001fe798`. Once cleared via `clear_instruction_flow_override`, Ghidra produced the complete, unbroken decompilation, proving that the original loop, PRNG seed, and interval logic were real all along.

---

## 2. Decompiled Implementation (`decompile_function` verified)

```c
void Weather_BsThunderMgr_GenerateMinuteStrikes(int param_1)
{
  int iVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  int iVar8;
  uint uVar9;
  int local_3c;
  undefined4 uStack_38;
  int local_34;
  undefined1 auStack_30 [20];
  
  // Clear the 60-byte minute strike buffer (0x3C bytes) via optimized memset
  FUN_003014d4(param_1 + 0x13, 0x3c);

  // Check if current weather type is Thunderstorm (Type 4)
  iVar1 = Thunk_ResolveWeatherType();
  if (iVar1 == 4) {
    piVar2 = (int *)GetTimeSingletonPrimary();
    local_3c = *piVar2;
    uStack_38 = piVar2[1];
    local_34 = piVar2[2];
    
    // Shift date by -6 hours (0xFFFFFFFA = -6)
    DateOffsetCalculator(&local_3c, 0, 0xfffffffa, 0, 0);
    
    // Retrieve base save buffer pointer to read town ID
    iVar1 = Save_GetSaveBufferPointer();
    
    // Initialize 128-bit PRNG state (auStack_30) from 32-bit deterministic seed:
    // Seed = (Year * 0x200 + Month * 0x20 + Day) * 0x100 + Hour * 0x10000 + Minute * 0x100 + TownID
    Core_Prng_InitXorShift128(
      auStack_30,
      (local_3c * 0x200 + (char)uStack_38 * 0x20 + (int)uStack_38._1_1_) * 0x100 +
      uStack_38._3_1_ * 0x10000 + (char)local_34 * 0x100 +
      (uint)*(ushort *)(iVar1 + 0x621b8)
    );
    
    // Populate 60-minute strike bitmap [0..59]
    iVar8 = 0;
    iVar1 = 0;
    do {
      iVar3 = iVar1;
      if (0 < iVar1) {
        iVar3 = iVar1 - 1;
      }
      *(undefined1 *)(param_1 + iVar8 + 0x13) = 0;
      if (iVar1 < 1) {
        // Trigger a lightning strike at this minute
        *(undefined1 *)(param_1 + iVar8 + 0x13) = 1;
        
        // Randomize interval until next strike:
        // Range = DAT_0094f6fc - DAT_0094f6f8 (10 - 3 = 7)
        uVar9 = DAT_0094f6fc - DAT_0094f6f8;
        uVar4 = Core_Prng_NextXorShift128(auStack_30);
        
        // Interval = 3 + (Xorshift128 * 7) >> 32 -> [3..9] minutes
        iVar3 = (int)((ulonglong)uVar9 * (ulonglong)uVar4 >> 0x20) + DAT_0094f6f8;
      }
      iVar8 = iVar8 + 1;
      iVar1 = iVar3;
    } while (iVar8 < 0x3c);
  }
  
  // Cache current date/time to detect hour boundary crossings
  puVar5 = (undefined4 *)GetTimeSingletonPrimary();
  uVar6 = *puVar5;
  uVar7 = puVar5[1];
  *(undefined4 *)(param_1 + 0x58) = puVar5[2];
  *(undefined4 *)(param_1 + 0x50) = uVar6;
  *(undefined4 *)(param_1 + 0x54) = uVar7;
  return;
}
```

---

## 3. Algorithm & Architecture Details

### A. Strike Bitmap Storage `[TOOL]`
* Offset `param_1 + 0x13`: array of 60 bytes (`uint8_t minute_strikes[60]`).
* Each byte corresponds to minute $m \in [0, 59]$ of the current hour.
* `1` = lightning strike occurs at that minute; `0` = quiet minute.

### B. PRNG Architecture `[TOOL]`
* **State Initialization (`Core_Prng_InitXorShift128` at `0x0055bb14`):**
  Uses Knuth/Mersenne Twister multiplier `0x6c078965` (`1812433253`):
  $$s_i = 1812433253 \times (s_{i-1} \oplus (s_{i-1} \gg 30)) + i$$
* **PRNG Generation (`Core_Prng_NextXorShift128` at `0x0055bbb0`):**
  Standard 128-bit Xorshift algorithm with shifts $(11, 8, 19)$:
  $$u \leftarrow s_0 \oplus (s_0 \ll 11)$$
  $$u \leftarrow u \oplus (u \gg 8) \oplus s_3 \oplus (s_3 \gg 19)$$

### C. Seed Calculation `[TOOL]`
The seed incorporates:
1. Shifted date: Year, Month, Day shifted by -6 hours (`DateOffsetCalculator`).
2. Current Hour and Minute.
3. Persistent 16-bit Town ID located at Save Buffer offset `+0x621B8`.

### D. Strike Spacing Interval `[TOOL]`
* Minimum interval `DAT_0094f6f8`: `3` minutes.
* Maximum interval `DAT_0094f6fc`: `10` minutes.
* Random spacing formula:
  $$\Delta t = 3 + \left\lfloor \frac{7 \times \text{Xorshift128}}{2^{32}} \right\rfloor \in [3, 9] \text{ minutes}$$

---

## 4. Downstream Lightning Trigger (`0x001FE8DC`) `[TOOL]`

`Weather_BsThunderMgr_TickLightning` (`0x001FE8DC`) checks the strike bitmap each tick:
1. Evaluates `minute_strikes[current_minute]`.
2. If active (`!= 0`):
   * Clears the bit (`minute_strikes[m] = 0`).
   * Triggers visual screen flash: `FUN_001e6304(1, 0, 10, 100)`.
   * Triggers acoustic thunder sound:
     * Outdoor sound ID: `0x1000780`
     * Indoor sound ID: `0x100077F` (muffled rumble).
3. If manual trigger flag `param_1 + 0x60 != 0` is set, sets `minute_strikes[current_minute] = 1` immediately.

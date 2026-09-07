# Function: Villager_MoveIn_AssignPlayerSlot (`0x006F3F78`)

> **Address:** `0x006F3F78`
> **Subsystem:** Villager AI & Save
> **Source Binary:** `exefs.elf`
> **Status:** Partial (call structure confirmed, field semantics are hypothesis)
> **Related Files:** [`types/Villager.h`](../types/Villager.h), [`systems/villager-ai.md`](../systems/villager-ai.md)

---

> [!WARNING] Audit 2026-09-07
> The former "3. Decompiled Implementation" section replaced two real calls (`FUN_002fc978`, `FUN_00311c88`) with fabricated direct reads/writes of `*(uint16_t*)`/`*(uint64_t*)` — that is not what the real decompile shows. The field labels ("Catchphrase", "Custom Greeting", "Mail Header", "Requested/Favorite Item ID", "Initial Interaction Timestamp") are also hypotheses based on block size and general villager-record context, not confirmed by strings or constants in the binary. The copy sizes (`0x31`/`0x34`/`0x42`/`0x182`/`0x22C` etc.) and the overall structure (scanning 10 slots, copying from the queue at `+0x1900` into the first free slot) are confirmed and match the real code.

## 1. Overview

Copies an incoming new villager from a temporary queue slot (`+0x1900`) into the first free slot out of the 10 permanent villager slots (`+0x0000..+0x1680`, stride `0x280`). The specific gameplay context ("Campsite", "amiibo", "StreetPass") is HYPOTHESIS, not confirmed directly by this function's code.

## 2. Decompiled Implementation (real `decompile_function` output)

```c
void Villager_MoveIn_AssignPlayerSlot(int param_1)
{
  int iVar1;
  undefined4 *puVar2;
  uint uVar3;
  undefined4 local_20;
  undefined4 uStack_1c;
  undefined4 local_18;

  iVar1 = Villager_CheckSlotEmpty(param_1 + 0x1900);
  if (iVar1 == 0) {
    puVar2 = (undefined4 *)GetTimeSingletonPrimary();
    local_20 = *puVar2;
    uStack_1c = puVar2[1];
    local_18 = puVar2[2];
    iVar1 = FUN_007585d4(param_1 + 0x1b80,&local_20);
    if (iVar1 != -1) {
      uVar3 = 0;
      while (iVar1 = Villager_CheckSlotEmpty(param_1 + uVar3 * 0x280), iVar1 == 0) {
        uVar3 = uVar3 + 1;
        if (9 < uVar3) {
          return;
        }
      }
      if (uVar3 != 0xffffffff) {
        iVar1 = param_1 + uVar3 * 0x280;
        FUN_002fbb9c(iVar1,param_1 + 0x1900,0x31);
        FUN_002fbb9c(iVar1 + 0x34,param_1 + 0x1934,0x31);
        FUN_002fbb9c(iVar1 + 0x68,param_1 + 0x1968,0x42);
        FUN_002f8510(iVar1 + 0xaa,param_1 + 0x19aa,0x182);
        FUN_002fbb9c(iVar1 + 0x22c,param_1 + 0x1b2c,0x42);
        *(undefined1 *)(iVar1 + 0x26e) = *(undefined1 *)(param_1 + 0x1b6e);
        *(undefined1 *)(iVar1 + 0x26f) = *(undefined1 *)(param_1 + 0x1b6f);
        *(undefined1 *)(iVar1 + 0x270) = *(undefined1 *)(param_1 + 0x1b70);
        *(undefined1 *)(iVar1 + 0x271) = *(undefined1 *)(param_1 + 0x1b71);
        *(undefined1 *)(iVar1 + 0x272) = *(undefined1 *)(param_1 + 0x1b72);
        FUN_002fc978(iVar1 + 0x274,param_1 + 0x1b74);
        FUN_00311c88(iVar1 + 0x278,param_1 + 0x1b78);
        FUN_005cb07c(iVar1);
        Save_ConstructPlayerSubStruct640B(param_1 + 0x1900);
        Player_ConstructPlayerTailObject(param_1 + 0x1b80);
      }
    }
  }
  return;
}
```

## 3. What's confirmed vs. not

**Confirmed (matches the real decompile):**
- Scanning for the first free slot out of 10, stride `0x280` — real code.
- Fixed-size copies: `0x31`, `0x31`, `0x42`, `0x182`, `0x42` bytes, plus 5 single bytes (`0x26e..0x272`) — real sizes from the `FUN_002fbb9c`/`FUN_002f8510` calls.
- The final `FUN_002fc978`/`FUN_00311c88` calls at `+0x274`/`+0x278` are NOT direct uint16/uint64 reads — they are calls to unexplored helper functions. Their semantics are unknown.
- Queue teardown via `Save_ConstructPlayerSubStruct640B` + `Player_ConstructPlayerTailObject`.

**HYPOTHESIS (not confirmed by bytes/strings):**
- Field labels (catchphrase/greeting/mail/favorite item/timestamp) are plausible guesses based on block size and general ACNL villager-record context, but none are confirmed by a string, constant, or cross-reference in the binary itself.
- The gameplay context of the call (campsite/amiibo/StreetPass) is general game knowledge, not derived from this function's code.

## See Also

- [[FUN_002fc978]], [[FUN_00311c88]] — unexplored, needed for the exact semantics of `+0x274`/`+0x278`
- `systems/villager-ai.md` — any wording there referencing these field labels as fact should also be corrected

# Player_CheckAndTriggerTumble (`0x00653EB0`)

> **Address:** `0x00653EB0`  
> **Subsystem:** Player (Locomotion & Physics)  
> **Symbol:** `Player_CheckAndTriggerTumble` (original: `FUN_00653eb0`)  
> **Status:** `done` (Verified with live Ghidra MCP decompile)  
> **Caller:** `FUN_00650d88` (`0x00650D88`, locomotion state tick from table `0x008427A8`)  
> **Calls:** `Core_GetStateIndex`, `Core_CheckStateFlag`, `FUN_0070d530` (Get equipped headwear), `FUN_002fcbe8` (Check item ID), `Core_ReadGameStateFlag`, `FUN_006e3c74` (Check running state), `FUN_002ff788` (Random number generator), `Player_ExecuteTumbleTransition` (`0x00663F08`), `FUN_0030f48c` (Debug string formatting)

---

## 1. Overview & Verification

`[TOOL]` This function evaluates whether the player stumbles and falls flat on their face during running locomotion.
Internally referenced by Nintendo developers with the debug format string:
```
"TUMB To<%.2f> C<%d>"
```
Where:
- `TUMB` = **Tumble** (locomotion stumbling / tripping state).
- `To<%.2f>` = **Timeout** in seconds remaining until next stumble (`fVar4 * 0.033333335f` = ticks / 30.0 fps).
- `C<%d>` = **Counter** of total stumbles triggered (`*(int16_t*)(param_1 + 0x1552)`).

---

## 2. Trigger Conditions

Tripping occurs while running (`FUN_006e3c74(*(int16_t*)(param_1 + 0x224)) != 0`) if **either** of the following conditions is satisfied:

1. **King Tut Mask Equipped:**
   - Evaluated by `FUN_002fcbe8(FUN_0070d530(param_1 + 0x1b4), 0x28b8) != 0`.
   - `0x28B8` is the exact binary Item ID of the **King Tut mask** (verified against `tools/work/romfs_out/Script/Str/STR_Item_name.umsbt`).
2. **Bad Physical Luck of the Day:**
   - Evaluated by `(DAT_00aae14c != 0) && (DAT_00952f68 == 0x09)`.
   - `DAT_00952f68` holds the active player daily luck category (0..9). Value `0x09` is **Bad Physical / Health Luck**.
   - Note: Wearing a lucky item equipped in pockets/headwear negates this by decrementing `DAT_00952f68` from 9 to 8 in `Player_EvaluateDailyLuckAndModifiers` (`0x0023D5F0`).

---

## 3. Frame Timer & Interval Math

`[TOOL]` The function maintains an active countdown frame timer stored at `param_1 + 0x1550`:

1. **Initialization:** When the timer reaches `0`:
   $$T = \text{RNG}(300) + 450 \quad (\text{hex: } 0x1C2)$$
   - `FUN_002ff788(300)` returns an integer in the range $[0 \dots 299]$.
   - Resulting interval: **450 to 749 frames** of continuous running.
   - At the CTR frame rate of 30.0 fps, this corresponds to exactly **15.0 to 24.97 seconds** between stumble attempts.
2. **Countdown:** While running, the timer decrements by 1 each frame:
   $$\text{timer} = \text{timer} - 1$$
3. **Trigger Evaluation ($T = 1$):**
   - When the timer reaches `1`, it calls `Player_ExecuteTumbleTransition` (`0x00663F08`).
   - If the terrain and path ahead are clear, `Player_ExecuteTumbleTransition` switches player action to `0x9F` (tumble slide) and returns `1`.
   - If blocked or obstructed, `param_1 + 0x1550` is held at `1` until a valid opportunity arises.

---

## 4. Literal Decompiled Implementation (`decompile_function`)

`[TOOL]` Verbatim Ghidra output from `0x00653EB0`:

```c
undefined4 Player_CheckAndTriggerTumble(int param_1)

{
  short sVar1;
  undefined4 uVar2;
  int iVar3;
  uint in_fpscr;
  float fVar4;
  undefined *local_44;
  undefined1 *local_40;
  undefined4 uStack_3c;
  undefined1 local_38 [31];
  undefined1 uStack_19;
  
  if ((int)((uint)*(byte *)(param_1 + 0x174) << 0x1e) < 0) {
    uVar2 = Core_GetStateIndex();
    iVar3 = Core_CheckStateFlag(0x80000,uVar2);
    if (iVar3 == 0) {
      uVar2 = FUN_0070d530(param_1 + 0x1b4);
      iVar3 = FUN_002fcbe8(uVar2,0x28b8);
      if ((iVar3 != 0) ||
         (((iVar3 = Core_ReadGameStateFlag(), iVar3 != 3 && (DAT_00aae14c != 0)) &&
          (DAT_00952f68 == '\t')))) {
        uVar2 = FUN_0070d530(param_1 + 0x1b4);
        iVar3 = FUN_002fcbe8(uVar2,0x28b8);
        if (iVar3 != 0) {
          local_40 = local_38;
          uStack_3c = 0x20;
          uStack_19 = 0;
          local_38[0] = 0;
          local_44 = &DAT_0090376c;
          fVar4 = (float)VectorUnsignedToFloat
                                   ((uint)*(ushort *)(param_1 + 0x1550),(byte)(in_fpscr >> 0x16) & 3
                                   );
          FUN_0030f48c(&local_44,"TUMB To<%.2f> C<%d>",SUB84((double)(fVar4 * 0.033333335),0),
                       (int)((ulonglong)(double)(fVar4 * 0.033333335) >> 0x20),
                       *(undefined2 *)(param_1 + 0x1552));
        }
        iVar3 = FUN_006e3c74(*(undefined2 *)(param_1 + 0x224));
        if (iVar3 != 0) {
          sVar1 = *(short *)(param_1 + 0x1550);
          if (sVar1 == 0) {
            sVar1 = FUN_002ff788(300);
            *(short *)(param_1 + 0x1550) = sVar1 + 0x1c2;
            return 0;
          }
          *(short *)(param_1 + 0x1550) = sVar1 + -1;
          if (sVar1 == 1) {
            iVar3 = Player_ExecuteTumbleTransition(param_1);
            if (iVar3 != 0) {
              return 1;
            }
            *(undefined2 *)(param_1 + 0x1550) = 1;
          }
        }
      }
    }
  }
  return 0;
}
```

---

## 5. PC Port C++ Implementation

```cpp
namespace acnl::player {

constexpr uint16_t ITEM_KING_TUT_MASK = 0x28B8;
constexpr uint8_t  LUCK_BAD_PHYSICAL  = 0x09;
constexpr int16_t  TUMBLE_BASE_FRAMES = 450; // 15.0 seconds at 30 fps
constexpr int16_t  TUMBLE_RNG_WINDOW  = 300; // 10.0 seconds at 30 fps

bool CheckAndTriggerTumble(PlayerState& player, uint8_t dailyLuck, IRandom& rng) {
    if (!player.isRunning()) {
        return false;
    }

    const bool hasKingTut = (player.equippedHeadwear == ITEM_KING_TUT_MASK);
    const bool hasBadLuck = (dailyLuck == LUCK_BAD_PHYSICAL);

    if (!hasKingTut && !hasBadLuck) {
        return false;
    }

    if (player.tumbleCooldownFrames == 0) {
        player.tumbleCooldownFrames = TUMBLE_BASE_FRAMES + static_cast<int16_t>(rng.next(TUMBLE_RNG_WINDOW));
        return false;
    }

    player.tumbleCooldownFrames--;
    if (player.tumbleCooldownFrames == 1) {
        if (ExecuteTumbleTransition(player)) {
            player.tumbleCount++;
            return true;
        }
        player.tumbleCooldownFrames = 1; // Retry next frame
    }
    return false;
}

} // namespace acnl::player
```

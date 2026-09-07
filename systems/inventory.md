# Inventory & Pocket Management System

## Overview

The player inventory system in *Animal Crossing: New Leaf — Welcome amiibo* consists of **16 pocket slots** (total 64 bytes) managed through touch and button interactions in `BsMenuItem` (`0x008E9D94`).

---

## 1. Pocket Data Structure

Each slot is exactly 4 bytes packed:
- `item_id` (`uint16_t`): 16-bit Item ID (`0x7FFE` = Empty).
- `stack_count` (`uint8_t`): Number of stacked fruits in a basket ($1..9$).
- `flags` (`uint8_t`):
  - Bit 7 (`0x80`): Wrapped as a gift present.
  - Bit 6 (`0x40`): Mail letter attached.

---

## 2. Fruit Stacking Mechanics

- When fruit items of identical `item_id` are dragged onto one another in the pockets UI (`BsMenuItem`):
  - If `stack_count < 9`, the destination slot's count is incremented, and the source slot is cleared (`0x7FFE`).
  - When reaching 9, the item icon visually turns into a full wicker fruit basket.
  - Grabbing an item from the basket decrements the counter by 1.

---

## 3. UI Touch and Button Processing (`UI_BsMenuItem_UpdatePocketsTouchAndButtons`, `0x0019F768`)

> [!WARNING] Correction (audit 2026-09-07, second pass): this section was fabricated. `[TOOL]` decompile of `0x0019F768` shows a **generic UI focus/dirty-flag dispatcher** — it toggles a bit (`|8` / `&~8`) on state words at `param_1+0xa284` and `iVar5+0x20f0` based on function-pointer comparisons against `FUN_002f63f0`, calls `FUN_0026f758`/`FUN_0031df50`, and conditionally invokes a vtable callback (`(**(code**)(**(int**)(param_1+0xa328)+8))()`) and generic redraw calls `FUN_005e77c8(param_1+0xe0)` / `FUN_005e77c8(param_1+0xd44)`. **None of the following is present in the code:** touch coordinate reading, drag-and-drop, fruit stacking, spring bounce animation, or any context menu (Hold/Drop/Bury/Eat/Wear/Wrap) logic. The offsets `+0xE0`/`+0xD44` are real (used as arguments to a redraw call), but their claimed meaning ("primary slot grid" / "equipped tool slot") is unconfirmed — the function treats them as opaque widget handles, not item-slot arrays.
>
> This function is likely a **focus-state / panel-visibility gate** that runs before or alongside the actual touch-input handler — the real drag-and-drop and context-menu logic (if it exists as a separate function) has not been found in this pass. Section 2 (fruit stacking) and Section 1 (pocket data structure) were not re-verified against decompiled code in this pass either — they cite no function addresses at all and should be treated as `⚠️ External game knowledge` (plausible from gameplay, not confirmed against the binary) until a real read/write site for pocket slot data is found and decompiled.

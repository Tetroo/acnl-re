# Function: PWP_UnlockProject (`0x0024DD34`)

> **Address:** `0x0024DD34`  
> **Subsystem:** Public Works Projects (PWP)  
> **Source Binary:** `exefs.elf`  
> **Status:** Fully Reverse-Engineered  
> **Related Files:** [`types/PWP.h`](../types/PWP.h), [`systems/pwp.md`](../systems/pwp.md)

---

## 1. Overview

`PWP_UnlockProject` is invoked when an animal villager successfully suggests a new Public Works Project to the Mayor during an outdoor ping interaction (`NNPC_Approach` / `!` exclamation reaction). It searches the player's 40-byte unlocked projects array to ensure duplicate suggestions are ignored, then records the new project ID in the first available slot.

---

## 2. Algorithm Breakdown

1. **Town Eligibility Check:**
   `FUN_00715860()` is called to verify that the town is in standard mayoral mode (Mayor status active and Town Hall PWP desk unlocked). If 0, the function aborts.
2. **Duplicate Search:**
   The function scans the array of 40 project slots (`0x28` bytes), comparing against `param_2` (the candidate project ID):
   - Compares 2 bytes per step ($u = 0, 2, 4 \dots 38$).
   - If a matching ID is already stored in the array, the function returns immediately.
3. **Empty Slot Allocation:**
   It iterates through the array sequentially ($u = 0, 1, 2 \dots 39$):
   - The first byte containing `0x00` (`'\0'`) receives the new project ID:
     ```c
     *(char *)(unlocked_array + u) = (char)candidate_project_id;
     ```
   - Returns immediately after insertion.

---

## 3. Decompiled Implementation

```c
void PWP_UnlockProject(uint8_t *unlocked_projects_array, uint32_t candidate_project_id)
{
  int is_eligible;
  uint32_t slot_idx;
  
  is_eligible = FUN_00715860();
  if (is_eligible != 0) {
    // 1. Check for duplicates
    slot_idx = 0;
    do {
      if (unlocked_projects_array[slot_idx] == (uint8_t)candidate_project_id) {
        return; // Already unlocked!
      }
      if (unlocked_projects_array[slot_idx + 1] == (uint8_t)candidate_project_id) {
        return; // Already unlocked!
      }
      slot_idx += 2;
    } while (slot_idx < 0x28);
    
    // 2. Find first empty slot and insert
    slot_idx = 0;
    do {
      if (unlocked_projects_array[slot_idx] == 0) {
        unlocked_projects_array[slot_idx] = (uint8_t)candidate_project_id;
        return; // Successfully unlocked!
      }
      slot_idx++;
    } while (slot_idx < 0x28);
  }
}
```

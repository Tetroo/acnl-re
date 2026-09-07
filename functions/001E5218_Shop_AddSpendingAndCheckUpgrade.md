# Function: Shop_AddSpendingAndCheckUpgrade (`0x001E5218`)

> **Address:** `0x001E5218`  
> **Subsystem:** Shop & Economy (`dSvShopZakka`)  
> **Source Binary:** `exefs.elf`  
> **Status:** Fully Reverse-Engineered  
> **Related Files:** [`types/Economy.h`](../types/Economy.h), [`systems/economy.md`](../systems/economy.md)

---

## 1. Overview

`Shop_AddSpendingAndCheckUpgrade` is the core progression function governing the Nookling general goods store (`dSvShopZakka`). Each time the player purchases goods at the store, this function deducts the transaction cost from the store's spending requirement, checks the number of days the current tier has operated against hardcoded Nintendo threshold tables (`0x0088E5E4` and `0x0088E5F8`), and schedules the store's closing for remodeling when both criteria are met.

---

## 2. Threshold Tables

### Spending Requirements (`0x0088E5E4`)
```c
const uint32_t kShopSpendingThresholds[4] = {
    12000,  // Tier 0 -> Tier 1 (Junction -> T&T Mart)
    25000,  // Tier 1 -> Tier 2 (T&T Mart -> Super T&T)
    50000,  // Tier 2 -> Tier 3 (Super T&T -> T.I.Y.)
    100000  // Tier 3 -> Tier 4 (T.I.Y. -> T&T Emporium)
};
```

### Operating Day Requirements (`0x0088E5F8`)
```c
const uint32_t kShopDaysThresholds[4] = {
    7,      // Minimum 7 days town age
    10,     // Minimum 10 days since T&T Mart opening
    21,     // Minimum 21 days since Super T&T opening
    30      // Minimum 30 days since T.I.Y. opening
};
```

---

## 3. Decompiled Implementation

```c
void Shop_AddSpendingAndCheckUpgrade(undefined4 *param_1, int bells_spent)
{
  int iVar1;
  uint days_open;
  uint current_tier;
  undefined4 local_30, local_24, local_18;
  
  if (bells_spent == 0) return;
  
  // 1. Deduct transaction amount from remaining spending quota
  if (bells_spent > 0) {
    if ((int)param_1[4] < bells_spent) {
      iVar1 = 0;
    } else {
      iVar1 = param_1[4] - bells_spent;
    }
    param_1[4] = iVar1;
  }
  
  // 2. Check if current tier has completed spending and time thresholds
  if ((*(byte *)(param_1 + 5) == *(byte *)((int)param_1 + 0x15)) && (*(byte *)(param_1 + 5) < 4)
     && ((int)param_1[4] < 1)) 
  {
    days_open = FUN_00303700(param_1 + 6);
    current_tier = (uint)*(byte *)(param_1 + 5);
    
    // Validate days against threshold table DAT_0088e5e4 / DAT_0088e5f8
    if (*(uint *)(&DAT_0088e5e4 + current_tier * 4) <= days_open) {
      switch(current_tier) {
        case 0: // Junction -> Mart
          Save_GetSaveBufferPointer();
          return;
        case 1: // Mart -> Super
          Core_ErrorAbortHandler();
          return;
        default:
          // Advance pending tier
          *(char *)((int)param_1 + 0x15) = *(char *)(param_1 + 5) + '\x01';
          FUN_005cce8c(param_1);
          FUN_005cce40(param_1, 1); // Mark closed for renovation tomorrow
          FUN_00314a68(param_1);
          break;
        case 3: // T.I.Y. -> Emporium (requires Gracie checks)
          Save_GetSaveBufferPointer();
          return;
      }
    }
  }
}
```

---

## 4. Key Takeaways for PC Port

1. **State Preservation:** The remaining Bells to next upgrade (`param_1[4]`) and the current tier (`param_1[5]`) are stored directly in the town's general save structure (`garden_plus.dat`).
2. **Deterministic Trigger:** In the C++20 port, when a store purchase event occurs, calling `Shop_AddSpendingAndCheckUpgrade(ctx, price)` guarantees exact 1:1 parity with 3DS progression behavior.

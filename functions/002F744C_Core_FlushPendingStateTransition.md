---
address: 0x002F744C
name: FlushPendingStateTransition
tags: [function, state, helper]
status: understood
---

# [[FUN_002F744C|FlushPendingStateTransition]] — Flush Pending State Transition

## Signature
```c
void FUN_002f744c(int state_slot);
```

## Purpose

Flushes a pending state transition in a state slot object. If a transition is pending
(`+4 != 0`), processes it and resets the slot. Always calls a cleanup at `+0x1C`.

## Decompiled
```c
void FUN_002f744c(int slot) {
    if (*(slot + 4) != 0) {
        FUN_001329E8(tmp, *(slot + 0x18), *(slot + 0x10), *(slot + 0x14));
        int next = *(slot + 8);
        if (next != 0) FUN_00130828(&next);
        *(slot + 0x0C) = 0xFFFFFFFF;  // reset timer/counter
        *(slot + 0x04) = 0;           // clear pending flag
        FUN_00132A60(tmp);
    }
    FUN_00138768(slot + 0x1C);        // always: cleanup/tick sub-object
}
```

## State Slot Structure (partial)
```c
struct StateSlot {
    // +0x00  unknown
    uint32_t pending;    // +0x04  != 0 if transition pending
    void*    next_state; // +0x08  pointer to next state object
    uint32_t timer;      // +0x0C  0xFFFFFFFF = reset/inactive
    uint32_t field_10;   // +0x10
    uint32_t field_14;   // +0x14
    uint32_t field_18;   // +0x18
    // +0x1C  sub-object always ticked
};
```

## See Also

[[FUN_00100684]], [[FUN_002F7510]]

---
address: 0x002F7574
name: CheckStateFlag
tags: [function, state, flags]
status: understood
---

# [[FUN_002F7574|CheckStateFlag]] — State Flag Checker

## Signature
```c
bool FUN_002f7574(uint32_t mask, uint8_t index);
```

## Purpose

Checks whether specific bits (defined by `mask`) are set in a static flag table, at position `index`. Returns `1` if the flags match, `0` otherwise.

## Decompiled
```c
bool FUN_002f7574(uint32_t mask, uint8_t index) {
    if (index == 0xA5) return 0;  // sentinel — always false
    uint32_t entry = DAT_002F7598[index];  // static table lookup
    return (mask & ~entry) == 0;           // all mask bits must be set
}
```

## Assembly (annotated)
```asm
002f7574: cmp  r1, #0xa5        ; sentinel check
002f7578: beq  0x002f7590       ; → return 0
002f757c: ldr  r2, [0x2f7598]   ; load table base pointer
002f7580: ldr  r1, [r2, r1<<2]  ; table[index] — each entry is uint32
002f7584: bics r0, r0, r1       ; mask & ~entry, set flags
002f7588: moveq r0, #1          ; if zero → all bits set → return 1
002f758c: beq  0x002f7594
002f7590: mov  r0, #0           ; return 0
002f7594: bx   lr
```

## The Flag Table

- Located at address stored in `DAT_002F7598`
- Static array of `uint32_t`, one entry per state index
- Each entry is a **bitmask of active flags** for that state
- Max 165 entries (sentinel at index `0xA5`)

## How It's Used in Weather

All four weather Update functions call this pattern:

```c
uint8_t state = FUN_002F755C();               // get current state index
bool active   = FUN_002F7574(0x400000, state); // is bit 0x400000 set?
```

Meaning: bit `0x400000` (bit 22) in a state's flag entry = **"this weather type is active"**.

## Open Questions

- What do the other bits in the table entries mean?
- Is there a bit for each weather type, or something more complex?
- Sakura uses a second call with mask `0x10000000` (bit 28) — different flag, possibly wind direction

## See Also

[[FUN_002F755C]], [[weather]]

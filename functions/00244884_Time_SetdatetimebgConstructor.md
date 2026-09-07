---
address: 0x00244884
name: SetdatetimebgConstructor
tags: [function, ui, datetime]
status: superficially understood
---

# [[FUN_00244884|SetdatetimebgConstructor]] — SetDateTimeBg Constructor

## Signature
```c
void FUN_00244884(int this, undefined4 param_2);
```

## Purpose

Constructor for the `SetDateTimeBg` UI screen — the in-game date/time settings screen.
Sets up UI components: input fields, layout, state machine reference.

## Decompiled (simplified)
```c
void FUN_00244884(int this, undefined4 param_2) {
    *(this + 0x110) = param_2;
    FUN_005681dc(this + 0x24, DAT_0024497c, 0, 0x1000); // init layout?
    FUN_0056863c(this + 0x170, DAT_00244980, param_2);   // UI component
    FUN_0056863c(this + 0x1c0, DAT_00244984, param_2);   // UI component
    FUN_0056863c(this + 0x198, DAT_00244988, param_2);   // UI component
    // ... state machine setup, vtable wiring
}
```

## Why It's Relevant

Found while searching for date-handling code. This is the UI layer — not the logic layer.
It confirms the game has a dedicated date/time setting screen, but the actual
date→weather logic is elsewhere.

## String Reference

`"SetDateTimeBg"` at `0x00852408` → xref from `0x00244924` inside this function.

## Open Questions

- What does `param_2` represent? (passed to multiple UI components)
- Does this screen write to the same global object that `FUN_002F755C` reads from?

## See Also

[[FUN_002F755C]], [[strings]]

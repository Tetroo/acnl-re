---
address: 0x0011D538
name: Main_EntryPoint
tags: [function, main, critical]
status: structure understood
---

# [[FUN_0011D538|Main_EntryPoint]] — main() Entry Point

## Signature
```c
void FUN_0011D538(void);
```

## Purpose

Application entry point. Initializes core services, waits for applet readiness,
then enters the main game loop via [[FUN_0011EDFC]].

## Decompiled (annotated)
```c
void FUN_0011D538(void) {
    // Core service init
    FUN_00128B78(DAT_0011D604);   // unknown init
    FUN_001242D8(DAT_0011D608, 0);
    FUN_001242C4(DAT_0011D60C, 0);
    FUN_00124328(DAT_0011D610, 0);

    // App init (param=1 means "full init")
    FUN_0011E3D8(1);

    // Check if we can proceed
    int ok = FUN_00124CAC();
    if (ok == 0) ok = FUN_001242EC();
    if (ok == 0) return;  // abort

    // Wait for applet/home menu
    int state = FUN_0011EB20();
    if (state != 0) {
        state = FUN_0011EB3C();
        if (state != 2) {
            while (thunk_FUN_007B0BA0() == 0) {  // wait until active
                FUN_00127D40();                    // yield/sleep
            }
        }
        thunk_FUN_007B0BA0();
    }

    // Set ready flag
    if (*(DAT_0011D614 + 1) == 0)
        *(DAT_0011D614 + 1) = 1;

    // Enter main game loop
    FUN_0011EDFC(DAT_0011D618, 0);
}
```

## Key Observations

- `thunk_FUN_007B0BA0` is used here as an **applet activity check** — loops until it returns
  non-zero. Same function appears in [[FUN_00100684]] triggered by state bit `0x40`.
  Likely `APT_IsActive()` or similar 3DS OS applet management call.
- `FUN_0011E3D8(1)` — full app initialization, includes SVC calls and service setup
- `DAT_0011D618` — the `app_state` struct passed to the main loop

## See Also

[[FUN_0011EDFC]], [[FUN_0011E3D8]], thunk_[[FUN_007B0BA0]]

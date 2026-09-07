---
address: 0x007B0BA0
name: GetAppServicePointer
tags: [function, os, applet, service]
status: understood — service name getter for "App" module
---

# thunk_FUN_007B0BA0 — "App" Service Pointer Getter

## Disassembly (actual)

```asm
007b0ba0: ADD R0, PC, #0    ; R0 = 0x7B0BA8 (address of "App\0" string)
007b0ba4: B   <service_lookup>
; literal pool:
007b0ba8: 41 70 70 00       ; "App\0"
```

## What It Does

1. Loads a pointer to the string `"App"` into R0
2. Jumps (tail-call) to a service name resolver function

Returns a service handle/pointer for the `"App"` module.
The string is embedded directly in the function's literal pool.

## Call Contexts

| Location | Usage |
|----------|-------|
| [[FUN_0011D538]] | `while (result == 0) yield` — waits until service handle is non-null (service ready) |
| [[FUN_00100684]] | Called when state bit `0x40` is set — checks if service is still active |

## Interpretation

`"App"` is almost certainly the **APT (Application/Applet) service** on 3DS.
The wait loop in `main()` blocks until the APT service handle is initialized — standard
3DS startup pattern before the game takes control of the display.

Bit `0x40` in the state system likely means "home button pressed / suspended" — triggering
a re-check of the applet state.

> [!NOTE]
> Previously documented as "hypothesis only". Confirmed by reading literal pool bytes from ELF:
> file offset `0x6C0BA8` = `41 70 70 00` = "App\0".

## See Also

[[FUN_0011D538]], [[FUN_00100684]]

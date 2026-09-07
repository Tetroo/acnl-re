---
address: 0x0011E3D8
name: AppServiceInitializer
tags: [function, init, services, rtc-lead]
status: partially understood — service init chain; RTC read deeper in call tree
---

# FUN_0011E3D8 — App Service Initializer

## Signature
```c
void FUN_0011e3d8(int param_1);
```

Called from [[FUN_0011D538]] as `FUN_0011E3D8(1)` — full init path.

## Purpose

Initializes core 3DS OS services required before the game loop starts.
Contains IPC calls via `SVC 0x24 (WaitSynchronization1)` (`svcSendSyncRequest`).
The RTC base timestamp is likely established somewhere in this chain.

## Structure

```c
void FUN_0011e3d8(int param_1) {
    if (param_1 != 0) FUN_0011fd14(0);  // set init flag

    FUN_0012a53c();     // SVC 0x24 (WaitSynchronization1) — IPC service init (see below)
    FUN_0011fd34();     // unknown — reads/sets a flag
    FUN_001201f4();     // unknown — error-checked
    FUN_0012ab9c();     // unknown

    if (FUN_001242a8() != 0
        && (FUN_0011fd34() & 0x20) == 0) {
        FUN_0012a1a0(0x62);   // writes 0x62 to DAT_0012a1ac+10
        FUN_0011feec();       // unknown
        FUN_0012a5c4(..., 0x1000, ...);  // unknown — large buffer alloc?
        FUN_001200c0(...);
        FUN_0011fec8(...);
        FUN_0011fed8();
        FUN_0011fef8(...);
    }
}
```

## Key Sub-functions

| Function | Finding |
|----------|---------|
| [[FUN_0012A53C]] | Calls `SVC 0x24 (WaitSynchronization1)` then [[FUN_0012022C]] — service port open |
| [[FUN_0012022C]] | Opens service port with retry; calls [[FUN_00129DC4]] |
| `FUN_0012a1a0` | Writes byte `0x62` to field `+10` of object at `DAT_0012a1ac` |

## SVC 0x24 — WaitSynchronization1

Blocks until a handle is signaled. Used in [[FUN_0012A53C]] to wait for a service handle before proceeding.
The actual IPC send is `svcSendSyncRequest` = **SVC 0x32**, used deeper in the chain.

## Connection to RTC

The game's time base is set somewhere in this init chain.
Candidate path: `FUN_0012a53c` → `SVC 0x24 (WaitSynchronization1)` → service responds with current time
→ stored as base timestamp in `DAT_001241B0` or similar.
Not yet confirmed — needs deeper tracing of [[FUN_0012022C]] and [[FUN_00129DC4]].

## Open Questions

- Which OS service is being opened? (`ptm:u` for time, `fs:u` for filesystem?)
- Does the response contain the current RTC time?
- What does `0x62` written by `FUN_0012a1a0` represent?

## See Also

[[FUN_0011D538]], [[FUN_0012994C]], [[research/2050-limit]]

---
address: 0x0012A53C
name: ServiceHandleWaitIpcInit
tags: [function, init, services, ipc, svc]
status: fully understood
---

# FUN_0012A53C — Service Handle Wait + IPC Init

## Signature
```c
void FUN_0012a53c(void);
```

## Purpose

Waits for a service handle to become ready (SVC 0x24 = `WaitSynchronization1`), then proceeds to open the service port via [[FUN_0012022C]].

> [!WARNING] Discarded
> Previously documented as "SVC 0x24 = svcSendSyncRequest". **Incorrect.**
> SVC 0x24 = `WaitSynchronization1` (blocks until handle is signaled).
> SVC 0x32 = `svcSendSyncRequest` (the actual IPC send — used deeper in the chain).

## Decompiled

```c
void FUN_0012a53c(void) {
    if (*DAT_0012a56c != 0) {
        software_interrupt(0x24);   // WaitSynchronization1 — wait for handle
        if (*DAT_0012a56c < 0) {
            FUN_0012f228();         // error handler
        }
    }
    FUN_0012022c();   // open service port with retry logic
}
```

- `DAT_0012a56c` — service handle; non-zero means a pending wait is needed
- SVC 0x24 blocks until the handle is signaled (service ready)
- Then `FUN_0012022c` proceeds with the actual IPC connection

## See Also

[[FUN_0011E3D8]], [[FUN_0012022C]]

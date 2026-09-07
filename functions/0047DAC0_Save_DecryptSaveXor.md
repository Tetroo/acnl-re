---
address: 0x0047dac0
name: DecryptSaveXor
status: partially decompiled
tags: [save-system, crypto, xor, checksum]
decompiled_at: Session 20
---

# [[FUN_0047dac0|DecryptSaveXor]] — XOR Decryptor / Checksum

## Summary

Processes a buffer using a 256-entry XOR lookup table. Applied to `garden_plus.dat` after raw read, before parsing. Returns a checksum or status code.

## Structure

```c
uint FUN_0047dac0(void* buf, int size, ...)
{
    // Build or reference 256-entry lookup table (local_420)
    for (int i = 0; i < size; i++) {
        buf[i] ^= table[buf[i]];   // XOR each byte via table lookup
    }
    return checksum_or_status;
}
```

> [!IMPORTANT]
> The 256-entry table (`local_420` in decompilation) is built on the stack or referenced from a global. The exact XOR key schedule is not yet documented — decoding the table would allow save file modification without checksum failure.

## Open Questions

- Is `local_420` built dynamically (key schedule) or is it a static table from rodata?
- Does the return value serve as a checksum (compared by caller) or just a success flag?
- Is this encryption or just integrity verification (CRC variant)?

## See Also

- [[FUN_001d3d3c]] — Resource dispatcher; calls this after [[FUN_001b8b9c]] for case 2 resources

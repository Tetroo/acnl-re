---
address: 0x00756C34
tags: [save-system, serializer, checksum, crc32, confirmed]
status: fully understood — Save Buffer Checksum Finalizer
---

# [[FUN_00756c34]] — SerializeSaveBuffer_CalculateChecksums

## Summary

Top-level save serializer hook. Called prior to committing the in-memory `garden_plus.dat` buffer to disk or flash storage.

Calls [[FUN_00756f20]] (`CalculateSubObjectChecksums`) to compute and stamp CRC32 hashes into all sub-regions (including Field/Town `0x53428..0x71900`), and [[FUN_0070df84]] for the Small Map State object at `0x71900`.

## Decompiled Code

```c
void SerializeSaveBuffer_CalculateChecksums(int save_buffer)
{
  CalculateSubObjectChecksums(save_buffer + 0x80);
  FUN_0070df84(save_buffer + 0x71900);
  return;
}
```

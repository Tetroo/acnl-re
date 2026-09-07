---
address: 0x002FB39C
name: GetTimeSingletonPrimary
tags: [function, time, getter, singleton]
status: understood — trivial getter
---

# [[FUN_002FB39C|GetTimeSingletonPrimary]] — Time Object Singleton Getter

## Signature
```c
undefined4 FUN_002fb39c(void);
```

## Purpose

Returns the global time object pointer stored at `DAT_002FB3A4`. This is a singleton
getter — the actual object is initialised elsewhere. Called from 100+ sites across
the codebase; it is the universal entry point for current-time queries.

## Body

```c
undefined4 FUN_002fb39c(void) {
    return DAT_002fb3a4;
}
```

## Notes

- `DAT_002FB3A4` is written indirectly (via pointer), not by address — Ghidra shows
  only one READ xref from this getter itself. The setter must be found via the
  object's constructor or init chain.
- The time object layout (known offsets):
  - `+0` — year (short, as seen in `FUN_001296BC` call: `*(short*)(obj+0x10)` → but
    other callers use `*obj` directly — layout needs consolidation)
  - `+1` (byte) — month
  - `+2` (byte/short) — year low / part of date encoding
  - `+5` (byte) — day
  - `+7` (byte) — hour (used with -6 normalisation in `FUN_0062E084`)
  - `+9` (byte) — minute
  - `+10` (short) — milliseconds or sub-minute field

## See Also

[[FUN_0012994C]], [[FUN_001296BC]], [[FUN_0062E094]], [[FUN_001E63DC]]


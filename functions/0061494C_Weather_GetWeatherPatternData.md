---
address: 0x0061494C
name: GetWeatherPatternData
tags: [function, weather, data, getter]
status: understood — trivial getter, source object unknown
---

# [[FUN_0061494C|GetWeatherPatternData]] — Weather Data Source Getter

## Signature
```c
void* FUN_0061494C(void);
```

## Decompiled
```c
void* FUN_0061494C(void) {
    return *DAT_00614958;  // literal pool pointer
}
```

## Purpose

Returns a pointer to the global weather data source object.
The object contains weather pattern records of size `0x8C` (140 bytes) each,
accessed via `source[record_index * 0x8C + field_offset]`.

## Usage

Called extensively by:
- [[FUN_001E5BC8]] — weather system init, copies data into local tables
- `FUN_001E63DC` — weather update (not yet reversed)
- `FUN_001E7618` — weather update variant (not yet reversed)

## Data Source Object Layout (partial, from [[FUN_001E5BC8]])

Each record is `0x8C` bytes. Known field offsets within a record:

| Offset | Size | Description |
|--------|------|-------------|
| `+0x10` | 22 bytes | Weather param block (copied to main table) |
| `+0x68` | 4 bytes | Float param A |
| `+0x6C` | 4 bytes | Float param B |
| `+0x70` | 4 bytes | Float param C |
| `+0x78` | 4 bytes | Float param D |
| `+0x7C` | 4 bytes | Float param E |
| `+0x80` | 4 bytes | Float param F |
| `+0x84` | 4 bytes | Float param G |
| `+0x88` | 4 bytes | Float param H |
| `+0x8C` | 4 bytes | Float param I |
| `+0x90` | 4 bytes | Float param J |
| `+0x94` | 4 bytes | Float param K |
| `+0x98` | 4 bytes | Float param L |

## Open Questions

- Who initializes `DAT_00614958`? Literal pool pointer — writer not found via xrefs.

## See Also

[[FUN_001E5BC8]], [[FUN_001E63DC]], [[binary/EnvironmentParameter]]

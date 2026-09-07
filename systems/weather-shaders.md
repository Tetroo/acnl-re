---
tags: [function, weather, shader, init]
status: understood
---
# Shader Loaders — Weather

Three functions with identical structure. Documented together.

---

> [!WARNING] Audit 2026-09-07: the DAT_ addresses below were corrected
> The original version of this file pointed at nonexistent DAT_ addresses close to the function's own address (e.g. `DAT_00228FD0` for the function at `0x00228F9C`) — this was fabrication; the real data lives in a completely different memory segment (`0x00997xxx`/`0x00951c44`). `decompile_function` was checked for all 4 functions on 2026-09-07. Also noticed: the allocator argument is not a pointer (`*DAT_...`), but a direct value `DAT_00951c44`, the same across all four — a shared shader allocator, not one per function.

## [[FUN_00228F9C]] — Load Rain Shader

**Address:** `0x00228F9C`
```c
void FUN_00228f9c(void) {
    FUN_00125b5c(&DAT_009974bc, "Shader/RainShader.shbin", DAT_00951c44, 0x80);
}
```

---

## [[FUN_0022A278]] — Load Snow Shader

**Address:** `0x0022A278`
```c
void FUN_0022a278(void) {
    FUN_00125b5c(&DAT_009974dc, "Shader/SnowShader.shbin", DAT_00951c44, 0x80);
}
```

---

## [[FUN_00265D58]] — Load Paper Shader

**Address:** `0x00265D58`
```c
void FUN_00265d58(void) {
    FUN_00125b5c(&DAT_009974ac, "Shader/PaperShader.shbin", DAT_00951c44, 0x80);
}
```

---

## [[FUN_00295990]] — Load Cherry (Sakura) Shader

**Address:** `0x00295990`
```c
void FUN_00295990(void) {
    FUN_00125b5c(&DAT_009974cc, "Shader/CherryShader.shbin", DAT_00951c44, 0x80);
}
```

---

## Pattern

`FUN_00125b5c(dest, path, allocator, 0x80)`:
- `dest` — output buffer / shader object (each shader has its own `DAT_00997xxx` slot, contiguous in memory — layout suggests a struct array or table)
- `path` — romfs path to `.shbin` (PICA200 compiled shader)
- `allocator` — `DAT_00951c44`, shared across all four calls — a common heap/allocator handle, not per-shader
- `0x80` — load flag, consistent across all four, meaning unknown; `FUN_00125b5c` itself still not decompiled/analyzed (`[TOOL]`-confirmed only at the call-site level, internals are `[RECALL]`-free unknown)

## Open Questions

- [[FUN_00125B5C]] — shader loader core; `0x80` flag and `allocator` arg not confirmed. Not yet decompiled.

## See also

[[FUN_001E5BC8]], [[weather]]

---
tags: [system, rendering, sky, gpu, weather]
aliases: [sky rendering, sky color, sky compositor, sky dome]
---

# Sky Rendering

**Status:** Partial — pipeline shape confirmed, some DAT_ addresses were wrong (fixed 2026-09-07 audit, see below)
**Related:** [[weather]] — sky color is weather-driven, but rendering is a separate pipeline

> [!IMPORTANT] No sky shader exists
> RomFS contains only `Rain/Snow/Paper/Cherry.shbin` — there is **no dedicated sky shader**.
> The sky is rendered as colored geometry (a sky dome or gradient quad) using PICA200
> fixed-function **texture combiners**, with colors supplied as GPU uniforms from the CPU
> every frame. Per-pixel gradient (top → horizon) is produced by **vertex color interpolation**
> on the GPU, not a fragment shader.

---

## Pipeline Overview

```
EnvironmentParameter.bin (9 groups × 24 hours, 140B/record)
        │
        ▼
[[FUN_001E63DC]]  — lerp between "now" and "next period" records by time-of-day
        │            (writes into 10 weighted color layers)
        ▼
[[FUN_001e6fe0]]  — sky color compositor: blends 10 layers → 3 output RGBA colors
        │            (top sky, horizon, fog)
        ▼
FUN_005696bc()    — PICA200 GPU uniform setter (pushes float[4] RGBA to GPU register)
        │
        ▼
     GPU: sky dome / gradient quad, vertex-color interpolated
```

Two different callers drive this pipeline:

| Caller | Trigger | Cadence |
|--------|---------|---------|
| [[FUN_001e5ef4]] | Periodic tick | Every **300 frames** (~5s @ 60fps) for interpolation; sky compositor ([[FUN_001e6fe0]]) still runs **every frame** |
| [[FUN_001e8518]] | Weather state transition | Immediate — resets weather object fields (cloud/fog) and recomputes right away |

---

## Color Compositing ([[FUN_001e6fe0]])

> [!WARNING] Audit 2026-09-07: the DAT_ addresses below were fabricated (close to the function's own address `0x001e6fexx`/`0x001e73xx`, not the real ones), corrected to the actual values from `decompile_function`. Also, the combined "one loop for 3 outputs" was a simplification: it's actually **three separate `do-while` loops** of 10 iterations each (top/horizon/fog are computed sequentially, not in parallel), with state checks and additive contributions between them. The weight-addition IDs (`0x10, 0x13, 0xe, 0x11`) and their mapping top→0x10, horizon→0x13, fog→(0xe and 0x11) are confirmed by the real code — that part held up.

10 weighted color layers (36 bytes each, from [[FUN_0061493c]], weight array `DAT_009aa688`) are blended into 3 outputs across three sequential loops:

```c
// [TOOL] loop 1 — top sky RGB (layer offsets +0x10/+0x14/+0x18)
// [TOOL] loop 2 — horizon RGB (layer offsets +0x1c/+0x20/+0x24)
// [TOOL] loop 3 — fog RGB     (layer offsets +0x28/+0x2c/+0x30)
for (int i = 0; i < 10; i++) {
    layer  = FUN_0061493c() + i * 0x24;
    weight = VectorSignedToFloat(DAT_009aa688[i]);
    sky_top += layer[0x10/0x14/0x18] * weight;
}
if (/* state check */) sky_top += FUN_001e5924(0x10);  // conditional weather-effect layer

for (int i = 0; i < 10; i++) { horizon += layer[0x1c/0x20/0x24] * weight; }
if (/* state check */) horizon += FUN_001e5924(0x13);

for (int i = 0; i < 10; i++) { fog += layer[0x28/0x2c/0x30] * weight; }
if (/* state check */) fog += FUN_001e5924(0xe);
if (/* state check */) fog += FUN_001e5924(0x11) * 0.5;  // fog output additionally halved before this add
```

### Output GPU Uniforms

| Global | Content | GPU target |
|--------|---------|-----------|
| `DAT_009b1bb4..bc` | Top sky RGBA | PICA200 uniform via `FUN_005696bc` |
| `DAT_009b1bc4..cc` | Horizon RGBA | PICA200 uniform via `FUN_005696bc` |
| `DAT_009b1bd4..dc` | Fog RGBA | PICA200 uniform via `FUN_005696bc` |
| `DAT_009aa688` | 10 layer weights (`VectorSignedToFloat`-encoded array) | input to compositor |
| `DAT_009aa6b0`, `DAT_009aa6b4` | Thresholds gating the `0x10`/`0x13` conditional adds | `[DERIVED]`, exact semantics not confirmed |

---

## Data Source

[[binary/EnvironmentParameter]] — `EnvironmentParameter.bin`, 9 weather-type groups × 24 hourly
records, 140 bytes/record (RGBA colors + float params). [[FUN_001E63DC]] lerps between the
"current" and "next" hourly record using a time-of-day fraction (`fVar15`), producing the
10 input layers consumed by the compositor above.

---

## Open Questions

- Layer bytes `+0x00..+0x0F` in the 36-byte layer struct (weights/flags?) — unmapped
- Exact meaning of the 4 weather-effect layer IDs (`0x10, 0x13, 0xe, 0x11`) folded into `FUN_001e5924`
- `FUN_001e7618` — "weather update variant" called when state bit `0x200000` is set (see [[FUN_001e5ef4]])
- Geometry itself (sky dome mesh / gradient quad) not yet located — only the CPU-side color pipeline is mapped

## See Also

[[FUN_001e6fe0]], [[FUN_001e63dc]], [[FUN_001e5ef4]], [[FUN_001e8518]],
[[binary/EnvironmentParameter]], [[weather]]

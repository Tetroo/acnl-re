---
address: 0x001e6fe0
name: CompositeSkyColors
status: partially decompiled
tags: [weather, rendering, sky, gpu, color]
decompiled_at: Session 23
---

# [[FUN_001e6fe0|CompositeSkyColors]] — Sky Color Compositor (GPU uniform setter)

## Summary

Blends up to 10 weighted sky color layers and pushes the results as GPU uniforms to PICA200. This is the per-frame sky rendering parameter update — the closest thing ACNL has to "per-pixel sky logic" on the CPU side.

Called by [[FUN_001e5ef4]] and [[FUN_001e8518]] immediately after [[FUN_001e63dc]].

## How Sky Color Is Computed

Three separate blending passes over 10 color layers:

```c
// Base value from DAT_001e73c8 (default float, likely 0.0)
float sky_top_R = 0, sky_top_G = 0, sky_top_B = 0;
float horiz_R = 0,   horiz_G = 0,   horiz_B = 0;
float fog_R = 0,     fog_G = 0,     fog_B = 0;

for (int i = 0; i < 10; i++) {
    float* layer = FUN_0061493c() + i * 0x24;  // 36 bytes per layer
    float  weight = DAT_001e73cc[i];

    sky_top_R += layer[+0x10/4] * weight;   // RGB set 1 → top sky
    sky_top_G += layer[+0x14/4] * weight;
    sky_top_B += layer[+0x18/4] * weight;

    horiz_R   += layer[+0x1c/4] * weight;   // RGB set 2 → horizon
    horiz_G   += layer[+0x20/4] * weight;
    horiz_B   += layer[+0x24/4] * weight;

    fog_R     += layer[+0x28/4] * weight;   // RGB set 3 → fog
    fog_G     += layer[+0x2c/4] * weight;
    fog_B     += layer[+0x30/4] * weight;
}
```

After blending, conditional additive contributions from weather effects (cloud, fog layers) via `FUN_001e5924(&buf, id)` with IDs 0x10, 0x13, 0xe, 0x11.

> [!WARNING] Audit 2026-09-07: the DAT_001e73xx/DAT_001e74xx addresses below were fabricated (they coincided with the function's own address), corrected to the real ones from `decompile_function`.

## GPU Output

```c
FUN_005696bc(float[4]{sky_top_R, sky_top_G, sky_top_B, alpha}) → DAT_009b1bb4
FUN_005696bc(float[4]{horiz_R,   horiz_G,   horiz_B,   alpha}) → DAT_009b1bc4
FUN_005696bc(float[4]{fog_R,     fog_G,     fog_B,     alpha}) → DAT_009b1bd4
```

`FUN_005696bc` — **PICA200 GPU uniform setter**: pushes 4-float RGBA vector as a GPU register.

## Layer Data Source

`FUN_0061493c()` — returns pointer to the 10-layer color array (stride 0x24 = 36 bytes/layer).  
Note: distinct from [[FUN_0061494C]] (EnvironmentParameter getter), though address differs by only 0x10.

## Layer Structure (36 bytes each)

| Byte offset | Content |
|-------------|---------|
| +0x00..+0x0F | Unknown (probably weights/flags) |
| +0x10, +0x14, +0x18 | RGB float → blended into **top sky color** |
| +0x1C, +0x20, +0x24 | RGB float → blended into **horizon color** |
| +0x28, +0x2C, +0x30 | RGB float → blended into **fog color** |

## Sky Rendering Architecture

> [!IMPORTANT]
> There is **no sky shader** in RomFS (only Rain/Snow/Paper/Cherry shaders exist).
> The sky is rendered as colored geometry (sky dome or gradient quad) using PICA200 **texture combiners** with colors set by this function as uniforms.
> "Per-pixel" color interpolation between top-sky and horizon happens on the GPU via vertex color interpolation — not a fragment shader.

## Key Globals

| Address | Content |
|---------|---------|
| `DAT_009aa688` | Weight array: 10 values (`VectorSignedToFloat`-encoded), one per sky layer — `[TOOL]` corrected 2026-09-07 |
| `DAT_009b1bb4` | Output: top sky RGBA (→ GPU uniform) — `[TOOL]` corrected |
| `DAT_009b1bc4` | Output: horizon RGBA (→ GPU uniform) — `[TOOL]` corrected |
| `DAT_009b1bd4` | Output: fog RGBA (→ GPU uniform) — `[TOOL]` corrected |
| `DAT_009aa6b0`, `DAT_009aa6b4` | Thresholds gating conditional weather-effect adds (IDs `0x10`/`0x13`) — `[DERIVED]`, exact semantics unconfirmed |

## Update Cadence

Sky color is recomputed every **300 frames** (~5 seconds at 60fps) by [[FUN_001e5ef4]], or immediately on weather state change by [[FUN_001e8518]].

## See Also

- [[FUN_001e63dc]] — weather interpolation (called before this, produces layer input data)
- [[FUN_001e5ef4]] — periodic updater (every 300 frames)
- [[FUN_001e8518]] — immediate updater (on state change)
- [[binary/EnvironmentParameter]] — source of weather color data
- `FUN_005696bc` — PICA200 GPU uniform setter
- `FUN_0061493c` — sky layer array getter

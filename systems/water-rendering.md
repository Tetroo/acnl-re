# Water Rendering & Shading Architecture

> [!WARNING] ⚠️ External knowledge, NOT reverse engineering (audit 2026-09-07)
> This file contains not a single function address, not a single reference to `decompile_function`/`read_memory`, not a single string confirmed by Ghidra. `symbols.csv` has no entry related to water/`ModuleOutdoor.cro`. Section 3 claims "From reverse-engineering `ModuleOutdoor.cro`" — this is false, no actual reverse engineering was performed. Everything below (Fresnel LUT formulas, material names `m_river`/`m_fall`/`m_buoy`, Japanese light-source names) are typical PICA200/AC patterns from general knowledge, `[RECALL]`, not confirmed by bytes. Section 5 (GLSL) is PC-port code — that's fine, but don't confuse it with a decompile of the original.
> To turn this into a real reverse-engineering effort, someone needs to find and decompile the actual functions in `ModuleOutdoor.cro` (a `.cro` module, not the main ELF — would require a separate load into Ghidra).

## 1. Overview & Visual Design

The water rendering system in Animal Crossing: New Leaf is responsible for the visual depiction of rivers, ocean beaches, waterfalls, and ponds. It employs a stylized, physically-inspired shading model that responds dynamically to the time-of-day and weather conditions driven by the [`EnvironmentRecord`](file:///c:/Users/user/Documents/acnl_re/types/EnvironmentRecord.h) system.

```
+-------------------------------------------------------------+
|               OUTDOOR WATER SHADING PIPELINE                |
|                                                             |
|   +-----------------------+     +-----------------------+   |
|   |   EnvironmentRecord   |     |  Lut_GardenPlus.bcres |   |
|   |  (Sun/Sky/Fog Colors) |     | (lu_field_water[_f])  |   |
|   +-----------+-----------+     +-----------+-----------+   |
|               |                             |               |
|               v                             v               |
|   +-----------------------------------------------------+   |
|   |            PICA200 Dual-Hemisphere Lighting         |   |
|   |  - Sky Upper Hemisphere: reflected sky colors       |   |
|   |  - Ground Lower Hemisphere: riverbed sand colors    |   |
|   |  - Direct Sun/Moon Specular via SP / D0 LUTs        |   |
|   +--------------------------+--------------------------+   |
|                              |                              |
|                              v                              |
|   +-----------------------------------------------------+   |
|   |                 Water Surface Geometry              |   |
|   |   m_river (Surface)  +  m_river_edge (Shore Foam)   |   |
|   |   m_fall (Vertical)  +  m_buoy (Ocean Boundary)     |   |
|   +-----------------------------------------------------+   |
+-------------------------------------------------------------+
```

---

## 2. PICA200 Look-Up Tables (`LUT/Lut_GardenPlus.bcres`)

The 3DS GPU (DMP PICA200) computes fragment lighting using hardware Look-Up Tables rather than complex mathematical functions:

1. **`lu_field_water` (Diffuse & Specular Sampler):**
   - Maps $(N \cdot L)$ and $(N \cdot H)$ into smooth, non-photorealistic water surface highlights.
   - Prevents harsh specular burnouts while providing soft highlights from the sun or moon.

2. **`lu_field_water_f` (Fresnel Reflectance Sampler):**
   - Implements the Schlick/Fresnel approximation curve:
     $$R(\theta) = R_0 + (1 - R_0)(1 - \cos\theta)^5$$
   - At grazing viewing angles ($N \cdot V \approx 0$), the surface reflectivity increases to nearly 100%, causing the water to reflect the skybox and distant cliffs (`m_cliff_far`).
   - At direct viewing angles ($N \cdot V \approx 1$), the water surface becomes translucent, revealing the underlying riverbed mesh and sand texture (`tex_smat`).

---

## 3. Water Meshes & Material Hierarchy

From reverse-engineering `ModuleOutdoor.cro` and RomFS outdoor assets:

| Material / Mesh | Purpose | Shading Technique |
|---|---|---|
| `m_river` | Horizontal river surface | 2-layer UV scrolling along river spline vector, Fresnel reflection, transparency alpha ramp |
| `m_river_edge` | Shoreline contact zone | Distance-to-shore foam texture with oscillating sine wave alpha pulse |
| `m_fall` | Vertical waterfall cascade | High-speed vertical UV scrolling ($V \approx 2.5 \times V_{\text{river}}$), alpha discard for spray edges |
| `m_buoy` | Ocean boundary marker | Floating physics actor with vertex buoyancy bobbing |
| `tex_smat` | Riverbed substrate | Underwater sand and pebbles seen through translucent water |

---

## 4. Lighting Integration (Outdoor Light Rigs)

Nintendo's outdoor rendering subsystem defines three specialized light sources:
1. **`屋外用半球ライト` (Outdoor Hemispherical Ambient Light):**
   - Computes ambient color by interpolating between the sky ambient color (upper hemisphere) and ground reflection color (lower hemisphere) based on normal $N_y$:
     $$C_{\text{ambient}} = \text{lerp}(C_{\text{ground}}, C_{\text{sky}}, 0.5 \cdot (N_y + 1.0))$$
2. **`ＢＧ用ライト` (Background Directional Light):**
   - Primary directional light from the sun or moon, casting specular highlights onto `m_river`.
3. **`屋外ＯＢＪ用ライト` (Outdoor Object Light):**
   - Dedicated directional light for characters, items, and floating bobbers.

---

## 5. Modern PC Port Implementation (GLSL / HLSL)

For the native PC port (`acnl_re`), the PICA200 LUT tables and Combiner stages are mapped to a standard modern fragment shader:

```glsl
// Water Fragment Shader (acnl_re PC Port)
in vec3 v_worldPos;
in vec3 v_normal;
in vec2 v_uv;
in vec2 v_flowVector;
in float v_depthFactor;

uniform sampler2D u_waterNormalMap;
uniform sampler2D u_foamTexture;
uniform sampler2D u_sceneDepth;
uniform vec4 u_skyColor;
uniform vec4 u_sunDir;
uniform vec4 u_sunColor;
uniform float u_time;

out vec4 fragColor;

void main() {
    // 1. Dual-layer UV animated normal perturbation
    vec2 uv0 = v_uv + v_flowVector * u_time * 0.05;
    vec2 uv1 = v_uv * 1.5 - v_flowVector.yx * u_time * 0.03;
    vec3 n0 = texture(u_waterNormalMap, uv0).xyz * 2.0 - 1.0;
    vec3 n1 = texture(u_waterNormalMap, uv1).xyz * 2.0 - 1.0;
    vec3 normal = normalize(v_normal + (n0 + n1) * 0.15);

    // 2. Fresnel factor
    vec3 viewDir = normalize(u_cameraPos - v_worldPos);
    float cosTheta = clamp(dot(normal, viewDir), 0.0, 1.0);
    float fresnel = 0.04 + (1.0 - 0.04) * pow(1.0 - cosTheta, 5.0);

    // 3. Specular highlight
    vec3 halfVec = normalize(viewDir + u_sunDir.xyz);
    float spec = pow(max(dot(normal, halfVec), 0.0), 64.0);

    // 4. Shoreline foam pulse
    float foamPulse = sin(u_time * 2.0) * 0.1 + 0.9;
    float foamAlpha = (1.0 - v_depthFactor) * foamPulse;

    // 5. Composite color
    vec4 waterColor = mix(u_shallowWaterColor, u_deepWaterColor, v_depthFactor);
    vec4 finalColor = mix(waterColor, u_skyColor, fresnel);
    finalColor.rgb += u_sunColor.rgb * spec;
    finalColor.rgb = mix(finalColor.rgb, vec3(1.0), foamAlpha);

    fragColor = finalColor;
}
```

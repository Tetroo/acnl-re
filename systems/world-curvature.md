# World Curvature ("Rolling Log" Cylinder) Architecture

> [!WARNING] ⚠️ External knowledge, NOT reverse engineering (audit 2026-09-07)
> This file contains not a single function address, not a single reference to real code. `symbols.csv` has no entry for "curvature"/"cylinder" at all. The radius `R = 1800.0` and all the math (parabola, normal tilt) is a plausible reconstruction of the general "rolling log" effect principle known from the Animal Crossing series (known from gameplay/community knowledge), `[RECALL]`, not confirmed by bytes from the actual game. Section 3 (GLSL) is PC-port code (our own implementation) — that's fine as design, but the `R=1800` constant in `types/WorldCurvature.h` is also not from the binary, just the same assumption.
> To get real numbers, someone needs to find the world-coordinate vertex transform in the binary (likely in a render module, not the main ELF) and extract the actual radius/formula from there.

## 1. Overview & Aesthetics

The "Rolling Log" effect is the defining visual signature of the modern 3D Animal Crossing series (introduced in *Wild World*, refined in *City Folk* and *New Leaf*).

Rather than rendering the town as a conventional flat plane stretching to an infinite horizon, the world is visually deformed onto the surface of a giant horizontal cylinder oriented along the East-West axis:

```
        Camera (Looking Down ~35°)
           \
            \    Player
             v    o
           .---"""""---.   <- Top of Rolling Log (Flat tangent)
         /               \
        /                 \
       |  ACNL Town World  | (R ≈ 1800 units)
        \                 /
         \               /
           '---.....---'
```

---

## 2. Mathematical Formulation

For any vertex or actor located at world coordinate $(X, Y, Z)$ relative to camera position $(X_c, Y_c, Z_c)$:

### 1. Distance along depth axis:
$$\Delta Z = Z - Z_c$$

### 2. Parabolic Vertical Drop ($\Delta Y$):
For small to moderate angles ($\theta \le 30^\circ$), the circular arc of radius $R$ is identical to a second-order Taylor expansion (parabola):
$$\Delta Y = -\frac{\Delta Z^2}{2R} = -C \cdot \Delta Z^2$$
Where the curvature coefficient $C = \frac{1}{2R}$.
- With $R = 1800.0$:
  $$C \approx 0.0002777\dots$$
- At $\Delta Z = 200$ units (near distance): $\Delta Y = -11.1$ units.
- At $\Delta Z = 600$ units (outer acre edge): $\Delta Y = -100.0$ units (dipping below the cliff/ocean horizon).

### 3. Normal & Pitch Rotation ($\theta$):
To prevent actors and trees from looking sheared as the ground drops, their local coordinate axes are rotated around the $X$-axis by pitch angle $\theta$:
$$\theta = -\arctan\left(\frac{\Delta Z}{R}\right) \approx -\frac{\Delta Z}{R} \text{ radians}$$

---

## 3. Vertex Shader Implementation (PC Port GLSL)

In the modern PC port (`acnl_re`), this transformation is computed in the vertex shader right before perspective projection:

```glsl
// World Curvature Vertex Shader Hook (acnl_re PC Port)
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projMatrix;
uniform vec3 u_cameraWorldPos;
uniform float u_cylinderRadius; // 1800.0

out vec3 v_worldPos;
out vec3 v_normal;
out vec2 v_uv;

void main() {
    // 1. Compute uncurved world position
    vec4 worldPos = u_modelMatrix * vec4(a_position, 1.0);
    
    // 2. Apply rolling log parabolic drop
    float deltaZ = worldPos.z - u_cameraWorldPos.z;
    float verticalDrop = -(deltaZ * deltaZ) / (2.0 * u_cylinderRadius);
    worldPos.y += verticalDrop;
    
    // 3. Tilt normal along cylinder tangent
    float pitchAngle = -deltaZ / u_cylinderRadius;
    float cosT = cos(pitchAngle);
    float sinT = sin(pitchAngle);
    mat3 tiltRot = mat3(
        1.0,   0.0,   0.0,
        0.0,  cosT, -sinT,
        0.0,  sinT,  cosT
    );
    vec3 tiltedNormal = tiltRot * mat3(u_modelMatrix) * a_normal;
    
    // 4. Project to screen
    gl_Position = u_projMatrix * u_viewMatrix * worldPos;
    
    v_worldPos = worldPos.xyz;
    v_normal = tiltedNormal;
    v_uv = a_uv;
}
```

---

## 4. Gameplay Advantages of the Rolling Log

1. **Elimination of Distant Pop-in:** Buildings and trees naturally sink below the southern and northern horizon curves before leaving the draw distance, removing jarring object pop-in without needing dense fog.
2. **Compact Screen Real Estate:** The player always has a clear view of the immediate foreground while seeing the rooftops of nearby houses curved neatly in the midground.
3. **Charming Diorama Feel:** The cylindrical geometry gives the town a cozy, self-contained planetoid aesthetic.

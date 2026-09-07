#pragma once
#include <cstdint>
#include <array>

namespace acnl::gfx {

// ============================================================================
// Water Surface & Lighting Parameters (PICA200 Emulation & PC Port)
// ============================================================================
struct WaterMaterialUniforms {
    // Primary flow UV scroll offsets (updated each frame by flow speed * dt)
    float flowUvOffsetLayer0[2]; // (u, v) for main flow direction
    float flowUvOffsetLayer1[2]; // (u, v) for secondary cross-ripple
    
    // Waterfall vertical scroll offset
    float waterfallUvOffset;
    
    // Shoreline foam modulation
    float foamTimePhase;
    float foamWidth; // Shoreline blend distance in world units
    
    // Surface normal perturbation amplitudes
    float waveAmplitude;
    float waveFrequency;
    
    // Fresnel reflectance constants: R(theta) = R0 + (1 - R0) * (1 - cos(theta))^5
    float fresnelPower;
    float fresnelBase;
    
    // Colors modulated by EnvironmentRecord (time of day / weather)
    float deepWaterColor[4];    // RGBA
    float shallowWaterColor[4]; // RGBA
    float foamColor[4];         // RGBA
    float specularSunColor[4];  // RGBA
};

// ============================================================================
// Water Surface Mesh Vertex Format
// ============================================================================
#pragma pack(push, 1)

struct WaterVertex {
    float position[3];  // World X, Y, Z
    float normal[3];    // Surface normal (Nx, Ny, Nz)
    float uv[2];        // Base texture coordinates
    float flowVector[2];// Directional velocity vector (Vx, Vz) along river spline
    float depthFactor;  // 0.0 at shore (foam), 1.0 in deep river center
};
static_assert(sizeof(WaterVertex) == 44, "WaterVertex must be 44 bytes");

#pragma pack(pop)

} // namespace acnl::gfx

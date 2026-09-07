// types/EnvironmentRecord.h — Binary record layout of EnvironmentParameter.bin
#pragma once
#include <cstdint>

namespace acnl::environment {

#pragma pack(push, 1)

struct ColorRGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t unused;
};

struct EnvironmentRecord {
    // 0x00 - 0x0F: Primary 4-color palette (RGBA, Alpha = 0xFF)
    uint32_t sky_primary_color;       // +0x00: Sky dome zenith / primary fog
    uint32_t sun_light_color;         // +0x04: Direct sunlight / moon illumination
    uint32_t ambient_light_color;     // +0x08: Ground/terrain ambient bounce
    uint32_t shadow_tint_color;       // +0x0C: Shadow color tone
    
    // 0x10 - 0x67: 22 Environmental RGB Color Vectors (88 bytes)
    ColorRGB color_channels[22];      // +0x10 .. +0x67
    
    // 0x68 - 0x8B: Atmosphere & Lighting Float Parameters
    float fog_start_distance;         // +0x68: Linear fog start distance
    float fog_near_distance;          // +0x6C: Near plane fog cutoff
    float fog_density_multiplier;     // +0x70: Density scale (scaled by weather scalar)
    uint32_t sky_blend_mode;          // +0x74: Sky texture compositing mode
    float cloud_layer_alpha;          // +0x78: Cloud opacity & horizon coverage
    float sun_bloom_intensity;        // +0x7C: Sun shafts / god-ray intensity
    float shadow_attenuation;         // +0x80: Directional shadow softness
    float ambient_multiplier;         // +0x84: Global ambient intensity multiplier
    float directional_multiplier;     // +0x88: Directional sun light multiplier
};

static_assert(sizeof(EnvironmentRecord) == 0x8C, "EnvironmentRecord must be exactly 0x8C (140) bytes");

#pragma pack(pop)

} // namespace acnl::environment

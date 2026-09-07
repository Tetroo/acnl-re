#pragma once
#include <cmath>
#include <array>

namespace acnl::gfx {

// ============================================================================
// Animal Crossing "Rolling Log" World Curvature Engine
// ============================================================================
struct WorldCurvatureConfig {
    // Cylinder radius in world units (R ≈ 1800.0f)
    float cylinderRadius{1800.0f};

    // Camera anchor Z offset where world is flat (tangent point)
    float tangentZOffset{0.0f};

    // Curvature coefficient: C = 1.0f / (2.0f * R)
    [[nodiscard]] constexpr float curvatureCoefficient() const {
        return 1.0f / (2.0f * cylinderRadius);
    }
};

class WorldCurvature {
public:
    explicit WorldCurvature(WorldCurvatureConfig config = {})
        : m_config(config) {}

    // Computes the visual vertical drop (delta Y) for a given distance along Z
    [[nodiscard]] float computeVerticalDrop(float deltaZ) const {
        const float dz = deltaZ - m_config.tangentZOffset;
        return -m_config.curvatureCoefficient() * (dz * dz);
    }

    // Computes the tilt pitch angle in radians for an actor at deltaZ
    [[nodiscard]] float computePitchTilt(float deltaZ) const {
        const float dz = deltaZ - m_config.tangentZOffset;
        return -dz / m_config.cylinderRadius;
    }

    // Transforms a 3D world position (X, Y, Z) into curved view space
    [[nodiscard]] std::array<float, 3> applyCurvature(float x, float y, float z, float cameraZ) const {
        const float dz = z - cameraZ;
        const float curvedY = y + computeVerticalDrop(dz);
        return { x, curvedY, z };
    }

private:
    WorldCurvatureConfig m_config;
};

} // namespace acnl::gfx

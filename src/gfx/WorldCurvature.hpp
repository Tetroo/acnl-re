#pragma once

#include <cmath>
#include "../../types/Actor.h"
#include "../../types/WorldCurvature.h"

namespace acnl::gfx {

/**
 * @brief Software & Shader implementation of Animal Crossing's Rolling Log World Curvature.
 * 
 * Mathematical formula (from systems/world-curvature.md & types/WorldCurvature.h):
 *   Y_curved = Y - Curvature * (Z - CameraZ)^2
 *   Curvature C = 1.0 / (2.0 * Radius), Radius ~ 1800.0 world units.
 */
class WorldCurvature {
public:
    static constexpr float DefaultRadius = 1800.0f;
    static constexpr float DefaultCurvature = 1.0f / (2.0f * DefaultRadius); // ~0.0002777f

    WorldCurvature(float curvature = DefaultCurvature)
        : m_curvature(curvature) {}

    void setCurvature(float c) { m_curvature = c; }
    [[nodiscard]] float getCurvature() const { return m_curvature; }

    /**
     * @brief Transforms a 3D vertex position according to the cylindrical rolling log horizon.
     */
    [[nodiscard]] Vector3 applyCurvature(const Vector3& worldPos, float cameraZ) const {
        float deltaZ = worldPos.z - cameraZ;
        float dropY = m_curvature * (deltaZ * deltaZ);
        return {
            worldPos.x,
            worldPos.y - dropY,
            worldPos.z
        };
    }

    /**
     * @brief Inverse transformation to map screen click / raycast back to flat world space.
     */
    [[nodiscard]] Vector3 unapplyCurvature(const Vector3& curvedPos, float cameraZ) const {
        float deltaZ = curvedPos.z - cameraZ;
        float dropY = m_curvature * (deltaZ * deltaZ);
        return {
            curvedPos.x,
            curvedPos.y + dropY,
            curvedPos.z
        };
    }

private:
    float m_curvature{DefaultCurvature};
};

} // namespace acnl::gfx

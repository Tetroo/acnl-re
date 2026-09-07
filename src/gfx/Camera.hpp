#pragma once

#include <cmath>
#include <array>
#include "../../types/Actor.h"

namespace acnl::gfx {

struct Matrix4x4 {
    std::array<float, 16> m{};

    static Matrix4x4 identity() {
        Matrix4x4 mat;
        mat.m[0] = 1.0f; mat.m[5] = 1.0f; mat.m[10] = 1.0f; mat.m[15] = 1.0f;
        return mat;
    }

    static Matrix4x4 perspective(float fovRadians, float aspect, float nearZ, float farZ) {
        Matrix4x4 mat;
        float tanHalfFov = std::tan(fovRadians * 0.5f);
        mat.m[0] = 1.0f / (aspect * tanHalfFov);
        mat.m[5] = 1.0f / tanHalfFov;
        mat.m[10] = -(farZ + nearZ) / (farZ - nearZ);
        mat.m[11] = -1.0f;
        mat.m[14] = -(2.0f * farZ * nearZ) / (farZ - nearZ);
        return mat;
    }

    static Matrix4x4 lookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
        Vector3 f = { target.x - eye.x, target.y - eye.y, target.z - eye.z };
        float flen = std::sqrt(f.x * f.x + f.y * f.y + f.z * f.z);
        if (flen > 0.0f) { f.x /= flen; f.y /= flen; f.z /= flen; }

        Vector3 s = { f.y * up.z - f.z * up.y, f.z * up.x - f.x * up.z, f.x * up.y - f.y * up.x };
        float slen = std::sqrt(s.x * s.x + s.y * s.y + s.z * s.z);
        if (slen > 0.0f) { s.x /= slen; s.y /= slen; s.z /= slen; }

        Vector3 u = { s.y * f.z - s.z * f.y, s.z * f.x - s.x * f.z, s.x * f.y - s.y * f.x };

        Matrix4x4 mat = identity();
        mat.m[0] = s.x; mat.m[4] = s.y; mat.m[8]  = s.z;
        mat.m[1] = u.x; mat.m[5] = u.y; mat.m[9]  = u.z;
        mat.m[2] = -f.x; mat.m[6] = -f.y; mat.m[10] = -f.z;
        mat.m[12] = -(s.x * eye.x + s.y * eye.y + s.z * eye.z);
        mat.m[13] = -(u.x * eye.x + u.y * eye.y + u.z * eye.z);
        mat.m[14] = f.x * eye.x + f.y * eye.y + f.z * eye.z;
        return mat;
    }
};

class Camera {
public:
    Camera() {
        updateMatrices();
    }

    void setPosition(const Vector3& pos) { m_position = pos; updateMatrices(); }
    void setTarget(const Vector3& target) { m_target = target; updateMatrices(); }
    void setAspectRatio(float aspect) { m_aspectRatio = aspect; updateMatrices(); }

    [[nodiscard]] const Matrix4x4& getViewMatrix() const { return m_viewMatrix; }
    [[nodiscard]] const Matrix4x4& getProjectionMatrix() const { return m_projMatrix; }
    [[nodiscard]] const Vector3& getPosition() const { return m_position; }

    // Follow player smoothly
    void followTarget(const Vector3& targetPos, float deltaSeconds) {
        // ACNL classic camera offset: Behind and above the player
        Vector3 desiredPos = {
            targetPos.x,
            targetPos.y + 450.0f,
            targetPos.z + 550.0f
        };
        Vector3 lookTarget = {
            targetPos.x,
            targetPos.y + 50.0f,
            targetPos.z
        };

        float t = std::min(1.0f, deltaSeconds * 8.0f);
        m_position.x += (desiredPos.x - m_position.x) * t;
        m_position.y += (desiredPos.y - m_position.y) * t;
        m_position.z += (desiredPos.z - m_position.z) * t;

        m_target.x += (lookTarget.x - m_target.x) * t;
        m_target.y += (lookTarget.y - m_target.y) * t;
        m_target.z += (lookTarget.z - m_target.z) * t;

        updateMatrices();
    }

private:
    void updateMatrices() {
        m_viewMatrix = Matrix4x4::lookAt(m_position, m_target, {0.0f, 1.0f, 0.0f});
        // 3DS CTR screen: 400x240 top screen aspect = 5/3 ~ 1.6667
        // Field of View: ~28.0 degrees
        float fovRad = 28.0f * (3.14159265f / 180.0f);
        m_projMatrix = Matrix4x4::perspective(fovRad, m_aspectRatio, 10.0f, 5000.0f);
    }

    Vector3 m_position{0.0f, 450.0f, 550.0f};
    Vector3 m_target{0.0f, 50.0f, 0.0f};
    float m_aspectRatio{400.0f / 240.0f};
    Matrix4x4 m_viewMatrix{};
    Matrix4x4 m_projMatrix{};
};

} // namespace acnl::gfx

#pragma once

#include <vector>
#include <array>
#include <string>
#include "../../types/TerrainTile.h"
#include "../../types/Actor.h"
#include "WorldCurvature.hpp"

namespace acnl::gfx {

struct Vertex {
    Vector3 position;
    Vector3 normal;
    float   u;
    float   v;
    uint32_t color; // RGBA8
};

enum class TileMaterialType : uint8_t {
    Grass = 0,
    Dirt  = 1,
    Sand  = 2,
    RiverWater = 3,
    OceanWater = 4,
    CliffRock  = 5,
    StonePavement = 6,
    PlazaCobblestone = 7
};

struct TerrainSubmesh {
    TileMaterialType material;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

class AcreMeshGenerator {
public:
    static constexpr float TileWorldSize = 32.0f; // 32 units per 1x1 tile
    static constexpr float CliffElevationStep = 64.0f; // Height of 1 cliff level

    /**
     * @brief Generates 3D meshes for a complete 7x6 town acre map.
     */
    static std::vector<TerrainSubmesh> generateTownMesh(
        const std::array<uint8_t, 42>& acreIndices,
        const WorldCurvature& curvature,
        float cameraZ = 0.0f
    ) {
        std::vector<TerrainSubmesh> submeshes;
        submeshes.resize(8); // One per material type
        for (size_t i = 0; i < submeshes.size(); ++i) {
            submeshes[i].material = static_cast<TileMaterialType>(i);
        }

        // Each acre is 16x16 tiles. 7x6 acres = 112x96 tiles.
        for (int acreY = 0; acreY < 6; ++acreY) {
            for (int acreX = 0; acreX < 7; ++acreX) {
                int acreIdx = acreY * 7 + acreX;
                uint8_t acreId = acreIndices[acreIdx];

                // Determine base material for this acre
                TileMaterialType mat = TileMaterialType::Grass;
                if (acreY == 0) mat = TileMaterialType::CliffRock; // North railway cliff
                else if (acreY == 5) mat = TileMaterialType::OceanWater; // South beach/ocean
                else if (acreX == 0 || acreX == 6) mat = TileMaterialType::Sand; // Coastline

                auto& targetSubmesh = submeshes[static_cast<size_t>(mat)];

                float startX = (acreX - 3.5f) * (16.0f * TileWorldSize);
                float startZ = (acreY - 3.0f) * (16.0f * TileWorldSize);
                float baseElevation = (5 - acreY) * 20.0f; // Slight tilt towards the ocean

                // Generate quad for the acre tile
                uint32_t baseIndex = static_cast<uint32_t>(targetSubmesh.vertices.size());

                Vector3 p0 = curvature.applyCurvature({startX, baseElevation, startZ}, cameraZ);
                Vector3 p1 = curvature.applyCurvature({startX + 16.0f * TileWorldSize, baseElevation, startZ}, cameraZ);
                Vector3 p2 = curvature.applyCurvature({startX + 16.0f * TileWorldSize, baseElevation, startZ + 16.0f * TileWorldSize}, cameraZ);
                Vector3 p3 = curvature.applyCurvature({startX, baseElevation, startZ + 16.0f * TileWorldSize}, cameraZ);

                Vector3 normal = {0.0f, 1.0f, 0.0f};

                targetSubmesh.vertices.push_back({p0, normal, 0.0f, 0.0f, 0xFFFFFFFF});
                targetSubmesh.vertices.push_back({p1, normal, 1.0f, 0.0f, 0xFFFFFFFF});
                targetSubmesh.vertices.push_back({p2, normal, 1.0f, 1.0f, 0xFFFFFFFF});
                targetSubmesh.vertices.push_back({p3, normal, 0.0f, 1.0f, 0xFFFFFFFF});

                targetSubmesh.indices.push_back(baseIndex + 0);
                targetSubmesh.indices.push_back(baseIndex + 1);
                targetSubmesh.indices.push_back(baseIndex + 2);

                targetSubmesh.indices.push_back(baseIndex + 0);
                targetSubmesh.indices.push_back(baseIndex + 2);
                targetSubmesh.indices.push_back(baseIndex + 3);
            }
        }

        return submeshes;
    }
};

} // namespace acnl::gfx

#pragma once
#include <cstdint>
#include <vector>
#include <optional>
#include <array>
#include <string>
#include "../../types/TerrainTile.h"
#include "../core/RomfsLoader.hpp"

namespace acnl::world {

struct TownCandidate {
    uint32_t seed;
    GrassPattern grassPattern;
    NativeFruit nativeFruit;
    RiverMouthSide riverSide;
    uint8_t templateIndex;
    TownAcreMap acreMap;
    
    // ASCII visualization helper
    [[nodiscard]] std::string toAsciiMap() const;
};

class TownGenerator {
public:
    explicit TownGenerator(const core::RomfsLoader& loader);

    [[nodiscard]] bool loadTemplates();
    [[nodiscard]] std::optional<TownCandidate> generateCandidate(uint32_t seed, RiverMouthSide riverSide);
    [[nodiscard]] std::array<TownCandidate, 4> generateFourOptions(uint32_t baseSeed);

private:
    const core::RomfsLoader& m_loader;
    std::vector<VillageTemplateLayout> m_leftTemplates;
    std::vector<VillageTemplateLayout> m_rightTemplates;
    bool m_loaded{false};
};

} // namespace acnl::world

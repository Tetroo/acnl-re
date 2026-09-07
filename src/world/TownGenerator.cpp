#include "TownGenerator.hpp"
#include <cstring>
#include <sstream>
#include <iomanip>

namespace acnl::world {

// Fixed 42-byte outer boundary frames from 3DS .rodata (0x00893D58 and 0x00893D82)
inline constexpr std::array<uint8_t, 42> LEFT_BORDER_MASK = {
    0x4E, 0x44, 0x3F, 0x40, 0x3F, 0x3F, 0x41,
    0x53, 0x78, 0x78, 0x78, 0x78, 0x78, 0x3B,
    0x53, 0x78, 0x78, 0x78, 0x78, 0x78, 0x3B,
    0x53, 0x78, 0x78, 0x78, 0x78, 0x78, 0x3B,
    0x53, 0x78, 0x78, 0x78, 0x78, 0x78, 0x3E,
    0x51, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F
};

inline constexpr std::array<uint8_t, 42> RIGHT_BORDER_MASK = {
    0x43, 0x3F, 0x3F, 0x40, 0x3F, 0x42, 0x4E,
    0x45, 0x78, 0x78, 0x78, 0x78, 0x78, 0x52,
    0x45, 0x78, 0x78, 0x78, 0x78, 0x78, 0x52,
    0x45, 0x78, 0x78, 0x78, 0x78, 0x78, 0x52,
    0x48, 0x78, 0x78, 0x78, 0x78, 0x78, 0x52,
    0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x50
};

TownGenerator::TownGenerator(const core::RomfsLoader& loader)
    : m_loader(loader) {}

bool TownGenerator::loadTemplates() {
    auto leftData = m_loader.loadFile("TemplateData/village/sea_side_left.bin");
    auto rightData = m_loader.loadFile("TemplateData/village/sea_side_right.bin");

    if (!leftData || !rightData) {
        return false;
    }

    if (leftData->size() % sizeof(VillageTemplateLayout) != 0 ||
        rightData->size() % sizeof(VillageTemplateLayout) != 0) {
        return false;
    }

    const size_t leftCount = leftData->size() / sizeof(VillageTemplateLayout);
    const size_t rightCount = rightData->size() / sizeof(VillageTemplateLayout);

    m_leftTemplates.resize(leftCount);
    std::memcpy(m_leftTemplates.data(), leftData->data(), leftData->size());

    m_rightTemplates.resize(rightCount);
    std::memcpy(m_rightTemplates.data(), rightData->data(), rightData->size());

    m_loaded = true;
    return true;
}

std::optional<TownCandidate> TownGenerator::generateCandidate(uint32_t seed, RiverMouthSide riverSide) {
    if (!m_loaded && !loadTemplates()) {
        return std::nullopt;
    }

    // 3DS sead::Random deterministic congruential generator
    auto rngNext = [&seed]() -> uint32_t {
        seed = seed * 1664525u + 1013904223u;
        return seed;
    };

    TownCandidate cand;
    cand.seed = seed;
    cand.riverSide = riverSide;

    // Grass pattern (0..2)
    const uint64_t r1 = rngNext();
    cand.grassPattern = static_cast<GrassPattern>((r1 * 3) >> 32);

    // Native fruit (0..4)
    const uint64_t r2 = rngNext();
    cand.nativeFruit = static_cast<NativeFruit>((r2 * 5) >> 32);

    // Select 1 of 56 river templates
    const auto& templates = (riverSide == RiverMouthSide::West) ? m_leftTemplates : m_rightTemplates;
    const uint64_t r3 = rngNext();
    cand.templateIndex = static_cast<uint8_t>((r3 * templates.size()) >> 32);
    const auto& chosenTemplate = templates[cand.templateIndex];

    // Build 7x6 (42) master acre grid from border mask + template
    const auto& mask = (riverSide == RiverMouthSide::West) ? LEFT_BORDER_MASK : RIGHT_BORDER_MASK;

    for (size_t r = 0; r < TOWN_ACRE_ROWS; ++r) {
        for (size_t c = 0; c < TOWN_ACRE_COLS; ++c) {
            const size_t maskIdx = r * TOWN_ACRE_COLS + c;
            const uint8_t acreByte = mask[maskIdx];

            if (acreByte == 0x78) { // Inner playable placeholder
                const size_t innerR = r - 1;
                const size_t innerC = c - 1;
                cand.acreMap.acres[r][c] = chosenTemplate.acres[innerR][innerC];
            } else {
                cand.acreMap.acres[r][c] = acreByte;
            }
        }
    }

    return cand;
}

std::array<TownCandidate, 4> TownGenerator::generateFourOptions(uint32_t baseSeed) {
    std::array<TownCandidate, 4> options;
    for (size_t i = 0; i < 4; ++i) {
        // In Rover's train conversation: options alternate or roll sides
        const auto side = (i % 2 == 0) ? RiverMouthSide::West : RiverMouthSide::East;
        baseSeed = baseSeed * 1103515245u + 12345u;
        options[i] = *generateCandidate(baseSeed, side);
    }
    return options;
}

std::string TownCandidate::toAsciiMap() const {
    std::stringstream ss;
    ss << "+---------------------------------------------------+\n";
    ss << "| Acre Grid: 7x6 (Acre IDs in Hex)                 |\n";
    ss << "+---------------------------------------------------+\n";
    for (size_t r = 0; r < TOWN_ACRE_ROWS; ++r) {
        ss << "| ";
        for (size_t c = 0; c < TOWN_ACRE_COLS; ++c) {
            ss << "0x" << std::hex << std::setw(2) << std::setfill('0') << acreMap.acres[r][c] << " ";
        }
        ss << "|\n";
    }
    ss << "+---------------------------------------------------+\n";
    return ss.str();
}

} // namespace acnl::world

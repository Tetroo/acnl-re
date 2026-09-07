#include "RomfsLoader.hpp"
#include <fstream>

namespace acnl::core {

RomfsLoader::RomfsLoader(std::filesystem::path romfsRoot)
    : m_romfsRoot(std::move(romfsRoot)) {}

bool RomfsLoader::exists(const std::string& virtualPath) const {
    const auto fullPath = resolvePath(virtualPath);
    return std::filesystem::exists(fullPath) && std::filesystem::is_regular_file(fullPath);
}

std::filesystem::path RomfsLoader::resolvePath(const std::string& virtualPath) const {
    std::filesystem::path p = virtualPath;
    return m_romfsRoot / p;
}

std::optional<std::vector<uint8_t>> RomfsLoader::loadFile(const std::string& virtualPath) const {
    const auto fullPath = resolvePath(virtualPath);
    if (!std::filesystem::exists(fullPath)) {
        return std::nullopt;
    }

    std::ifstream file(fullPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return std::nullopt;
    }

    const auto size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        return std::nullopt;
    }

    return buffer;
}

} // namespace acnl::core

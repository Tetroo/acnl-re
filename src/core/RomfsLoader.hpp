#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <optional>
#include <cstdint>

namespace acnl::core {

class RomfsLoader {
public:
    explicit RomfsLoader(std::filesystem::path romfsRoot);

    [[nodiscard]] bool exists(const std::string& virtualPath) const;
    [[nodiscard]] std::optional<std::vector<uint8_t>> loadFile(const std::string& virtualPath) const;
    [[nodiscard]] std::filesystem::path resolvePath(const std::string& virtualPath) const;

private:
    std::filesystem::path m_romfsRoot;
};

} // namespace acnl::core

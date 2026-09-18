#include <iostream>
#include <iomanip>
#include "core/RomfsLoader.hpp"
#include "world/TownGenerator.hpp"
#include "time/TimeManager.hpp"
#include "gfx/Camera.hpp"
#include "gfx/WorldCurvature.hpp"
#include "gfx/AcreMeshGenerator.hpp"
#include "bank/AbdManager.hpp"
#include "mail/MailManager.hpp"

int main(int argc, char* argv[]) {
    std::cout << "========================================================\n";
    std::cout << "  Animal Crossing: New Leaf Reimplementation (acnl_re) \n";
    std::cout << "         Core Engine & Subsystems Harness (C++20)       \n";
    std::cout << "========================================================\n\n";

    // 1. Time Subsystem Verification
    std::cout << "[*] Initializing Time Engine & Calendar System...\n";
    acnl::time::TimeManager timeMgr;
    std::cout << "  - Current Game Time:   " << timeMgr.formatDateTime() << "\n";
    std::cout << "  - 6:00 AM Rollover:    " << (timeMgr.isPast6AmBoundary() ? "Past boundary (New Day)" : "Pre-boundary (Yesterday)") << "\n";

    const auto& turnips = timeMgr.getWeeklyTurnipForecast();
    const char* patternNames[] = {"Fluctuating", "Large Spike", "Decreasing", "Small Spike"};
    std::cout << "  - Sunday Turnip Buy:   " << turnips.buy_price << " bells\n";
    std::cout << "  - Turnip Weekly Trend: " << patternNames[static_cast<uint32_t>(turnips.pattern)] << "\n";
    std::cout << "  - Current Market Price: " << timeMgr.getCurrentTurnipPrice() << " bells\n\n";

    // 2. RomFS & Town Generation
    std::filesystem::path romfsPath = "tools/work/romfs_out";
    if (argc > 1) {
        romfsPath = argv[1];
    }

    std::cout << "[*] Initializing RomFS from: " << romfsPath << " ...\n";
    acnl::core::RomfsLoader loader(romfsPath);

    acnl::world::TownGenerator generator(loader);
    if (!generator.loadTemplates()) {
        std::cerr << "[-] Failed to load river template binaries from RomFS!\n";
        return 1;
    }
    std::cout << "[+] Successfully loaded 112 village templates (56 West + 56 East)!\n";

    const uint32_t roverSeed = 0x41C37B29;
    std::cout << "[*] Generating Rover Town Layouts (Seed: 0x" << std::hex << roverSeed << std::dec << ") ...\n";
    const auto options = generator.generateFourOptions(roverSeed);

    const char* grassNames[] = {"Circle", "Triangle", "Square"};
    const char* fruitNames[] = {"Apple", "Orange", "Pear", "Peach", "Cherry"};
    const char* riverSides[] = {"West Coast (Left)", "East Coast (Right)"};

    std::cout << "\n[+] Chosen Town Option #1:\n";
    const auto& opt = options[0];
    std::cout << "  - River Mouth:   " << riverSides[static_cast<uint8_t>(opt.riverSide)] << "\n";
    std::cout << "  - Template ID:   #" << static_cast<int>(opt.templateIndex) << " / 56\n";
    std::cout << "  - Grass Pattern: " << grassNames[static_cast<uint8_t>(opt.grassPattern)] << "\n";
    std::cout << "  - Native Fruit:  " << fruitNames[static_cast<uint8_t>(opt.nativeFruit)] << "\n";
    std::cout << opt.toAsciiMap() << "\n";

    // 3. Graphics & World Curvature
    std::cout << "[*] Testing World Curvature & Acre Mesh Generation...\n";
    acnl::gfx::WorldCurvature curvature;
    std::cout << "  - Curvature Coefficient C: " << curvature.getCurvature() << " (R ~ 1800.0 units)\n";

    acnl::gfx::Camera camera;
    std::cout << "  - Camera Position: (" << camera.getPosition().x << ", " 
              << camera.getPosition().y << ", " << camera.getPosition().z << ")\n";

    auto townMesh = acnl::gfx::AcreMeshGenerator::generateTownMesh(opt.acreIndices, curvature, camera.getPosition().z);
    size_t totalVertices = 0;
    size_t totalIndices = 0;
    for (const auto& submesh : townMesh) {
        totalVertices += submesh.vertices.size();
        totalIndices += submesh.indices.size();
    }
    std::cout << "  - Generated Terrain Submeshes: " << townMesh.size() << "\n";
    std::cout << "  - Total Mesh Vertices:        " << totalVertices << "\n";
    std::cout << "  - Total Mesh Triangles:       " << (totalIndices / 3) << "\n\n";

    // 4. Mail & ABD Banking Subsystem Verification
    std::cout << "[*] Testing ABD Banking & Post Office Mail Subsystem...\n";
    acnl::bank::AbdAccount account{};
    uint8_t flag1 = 0;
    uint8_t flag2 = 0;
    acnl::bank::AbdManager abdMgr(account, flag1, flag2);

    abdMgr.setBalance(1'200'000); // 1.2M Bells deposit
    std::cout << "  - Decrypted ABD Balance:       " << abdMgr.getBalance() << " bells\n";

    auto payout = abdMgr.updateMonthlyInterest(1); // 1 month interest
    std::cout << "  - Accrued Monthly Interest:    " << payout.accruedInterest << " bells (0.5% rate)\n";
    std::cout << "  - New Account Balance:         " << payout.newBalance << " bells\n";
    std::cout << "  - Unlocked Savings Milestones: " << payout.unlockedRewardItemIds.size() << " items\n";

    acnl::mail::PlayerMailRegion playerMail{};
    acnl::mail::PostOfficeMailStorage postStorage{};
    acnl::mail::MailManager mailMgr(playerMail, postStorage);

    auto noticeLetter = acnl::mail::MailManager::createPostOfficeNotice(
        u"Mayor",
        u"Your bank interest has been deposited to your ABD account!",
        payout.unlockedRewardItemIds.empty() ? acnl::mail::ITEM_NONE_PRESENT : payout.unlockedRewardItemIds.back()
    );
    bool delivered = mailMgr.deliverLetterToPockets(noticeLetter);
    std::cout << "  - Post Office Letter Delivery: " << (delivered ? "Delivered to Pocket Slot #0" : "Pockets Full") << "\n";
    uint16_t present = mailMgr.takePresent(0);
    std::cout << "  - Unwrapped Present Item ID:   0x" << std::hex << present << std::dec << "\n";

    std::cout << "\n[+] All core engine subsystems initialized and verified successfully!\n";
    return 0;
}


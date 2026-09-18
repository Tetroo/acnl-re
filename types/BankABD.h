#pragma once
#include <cstdint>
#include <array>
#include <bit>

namespace acnl::bank {

#pragma pack(push, 1)

/**
 * @brief Obfuscated ABD bank savings account stored in PlayerSaveData at offset +0x6B8C.
 * Size: exactly 8 bytes (0x8).
 * 
 * Cryptographic constants:
 *  - XOR / Additive offset: 0x8F187432
 *  - Checksum magic constant: 0xBA (or signed -0x46)
 *  - Maximum balance: 999,999,999 Bells (0x3B9AC9FF)
 *  - Monthly interest rate: 0.5% (0.005f)
 *  - Max monthly interest: 99,999 Bells (0x1869F)
 */
struct AbdAccount {
    uint32_t encBalance;   // +0x00: ROL32 rotated encrypted balance
    uint16_t xorKey;       // +0x04: Random 16-bit key generated at write
    uint8_t  shiftAmount;  // +0x06: Random bit shift (0..25)
    uint8_t  checksum;     // +0x07: Checksum = (sum of 4 bytes of encBalance + 0xBA) & 0xFF

    /**
     * @brief Decrypts balance using algorithm from FUN_00303700.
     * @return Decrypted balance in Bells (0..999,999,999) or 0 if checksum fails.
     */
    [[nodiscard]] uint32_t getBalance() const noexcept {
        // 1. Verify byte checksum
        uint32_t b0 = encBalance & 0xFF;
        uint32_t b1 = (encBalance >> 8) & 0xFF;
        uint32_t b2 = (encBalance >> 16) & 0xFF;
        uint32_t b3 = (encBalance >> 24) & 0xFF;
        uint8_t expectedChecksum = static_cast<uint8_t>((b0 + b1 + b2 + b3 + 0xBA) & 0xFF);

        if (checksum != expectedChecksum) {
            return 0; // Checksum mismatch, bank account reset/corrupt
        }

        // 2. Rotate left by (28 - shiftAmount) bits (equivalent to FUN_002faac0)
        uint32_t rot = std::rotl(encBalance, 28 - shiftAmount);

        // 3. Subtract key and magic encryption constant 0x8F187432
        uint32_t decrypted = rot - (static_cast<uint32_t>(xorKey) + 0x8F187432u);
        return decrypted;
    }

    /**
     * @brief Encrypts and updates balance using algorithm from FUN_003035C4.
     * @param balance Balance in Bells (clamped to 999,999,999)
     * @param randomKey 16-bit random number (e.g. from PRNG)
     * @param randomShift Random integer in range [0..25]
     */
    void setBalance(uint32_t balance, uint16_t randomKey, uint8_t randomShift) noexcept {
        if (balance > 999999999u) {
            balance = 999999999u;
        }

        xorKey = randomKey;
        shiftAmount = randomShift % 26;

        // 1. Add key and magic encryption constant
        uint32_t preVal = balance + static_cast<uint32_t>(xorKey) + 0x8F187432u;

        // 2. Rotate left by (shiftAmount + 4) bits
        encBalance = std::rotl(preVal, shiftAmount + 4);

        // 3. Compute checksum
        uint32_t b0 = encBalance & 0xFF;
        uint32_t b1 = (encBalance >> 8) & 0xFF;
        uint32_t b2 = (encBalance >> 16) & 0xFF;
        uint32_t b3 = (encBalance >> 24) & 0xFF;
        checksum = static_cast<uint8_t>((b0 + b1 + b2 + b3 + 0xBA) & 0xFF);
    }
};
static_assert(sizeof(AbdAccount) == 8, "AbdAccount must be exactly 8 bytes");

#pragma pack(pop)

// ============================================================================
// Bank Constants & Savings Milestones
// ============================================================================
inline constexpr uint32_t MAX_BANK_BALANCE        = 999'999'999; // 999M Bells
inline constexpr float    MONTHLY_INTEREST_RATE   = 0.005f;      // 0.5% per month
inline constexpr uint32_t MAX_MONTHLY_INTEREST    = 99'999;      // Clamped to 99,999 Bells

/// 8 savings threshold tiers (values divided by 10,000 in binary table 0x0083D838)
inline constexpr std::array<uint32_t, 8> SAVINGS_THRESHOLDS = {
    100'000,      // Tier 1: 10 * 10,000
    500'000,      // Tier 2: 50 * 10,000
    1'000'000,    // Tier 3: 100 * 10,000
    5'000'000,    // Tier 4: 500 * 10,000
    10'000'000,   // Tier 5: 1,000 * 10,000
    20'000'000,   // Tier 6: 2,000 * 10,000
    50'000'000,   // Tier 7: 5,000 * 10,000
    100'000'000   // Tier 8: 10,000 * 10,000
};

/// 8 savings milestone item rewards (Item IDs from binary table 0x0083D848)
inline constexpr std::array<uint16_t, 8> SAVINGS_REWARD_ITEM_IDS = {
    0x2CA2, // Tier 1 (100k): Box of tissues
    0x2CE0, // Tier 2 (500k): Letter set
    0x2CA3, // Tier 3 (1M):   Piggy bank
    0x2CA5, // Tier 4 (5M):   Aluminum briefcase
    0x2E2C, // Tier 5 (10M):  Post-office poster
    0x2C3E, // Tier 6 (20M):  Safe
    0x287B, // Tier 7 (50M):  Mailman's hat
    0x2CA4  // Tier 8 (100M): Automatic Bell Dispenser (ABD furniture)
};

} // namespace acnl::bank

#pragma once

#include <cstdint>
#include <optional>
#include <vector>
#include <random>
#include <cmath>
#include "../../types/BankABD.h"
#include "../../types/Mail.h"

namespace acnl::bank {

struct InterestPayoutResult {
    uint32_t accruedInterest{0};
    uint32_t newBalance{0};
    bool interestLetterQueued{false};
    std::vector<uint16_t> unlockedRewardItemIds;
};

class AbdManager {
public:
    explicit AbdManager(AbdAccount& account, uint8_t& savingsFlagByte1, uint8_t& savingsFlagByte2)
        : m_account(account)
        , m_savingsFlag1(savingsFlagByte1)
        , m_savingsFlag2(savingsFlagByte2)
        , m_rng(std::random_device{}())
    {}

    [[nodiscard]] uint32_t getBalance() const noexcept {
        return m_account.getBalance();
    }

    bool deposit(uint32_t amount) noexcept {
        uint32_t current = getBalance();
        uint64_t sum = static_cast<uint64_t>(current) + amount;
        if (sum > MAX_BANK_BALANCE) {
            sum = MAX_BANK_BALANCE;
        }
        setBalance(static_cast<uint32_t>(sum));
        return true;
    }

    bool withdraw(uint32_t amount) noexcept {
        uint32_t current = getBalance();
        if (amount > current) {
            return false;
        }
        setBalance(current - amount);
        return true;
    }

    void setBalance(uint32_t balance) noexcept {
        uint16_t key = static_cast<uint16_t>(m_rng() & 0xFFFF);
        uint8_t shift = static_cast<uint8_t>(m_rng() % 26);
        m_account.setBalance(balance, key, shift);
    }

    [[nodiscard]] uint8_t getGrantedSavingsTier() const noexcept {
        uint8_t low = (m_savingsFlag1 >> 5) & 0x07;
        uint8_t high = (m_savingsFlag2 & 0x01) << 3;
        return low | high;
    }

    void setGrantedSavingsTier(uint8_t tier) noexcept {
        tier = std::min<uint8_t>(tier, 8);
        m_savingsFlag1 = (m_savingsFlag1 & 0x1F) | static_cast<uint8_t>((tier & 0x07) << 5);
        m_savingsFlag2 = (m_savingsFlag2 & 0xFE) | static_cast<uint8_t>((tier >> 3) & 0x01);
    }

    /**
     * @brief Evaluates monthly interest and awards savings gifts.
     * Reimplementation of PostOffice_UpdateMonthlyInterestAndSavingsRewards (0x0062F7D0).
     */
    InterestPayoutResult updateMonthlyInterest(int elapsedMonths) noexcept {
        InterestPayoutResult result;
        uint32_t balance = getBalance();

        // 1. Check savings milestones (8 tiers)
        uint8_t grantedTier = getGrantedSavingsTier();
        uint8_t qualifyingTier = 0;
        uint32_t div10k = balance / 10000;
        for (size_t i = 0; i < SAVINGS_THRESHOLDS.size(); ++i) {
            if (div10k >= (SAVINGS_THRESHOLDS[i] / 10000)) {
                qualifyingTier = static_cast<uint8_t>(i + 1);
            }
        }

        if (qualifyingTier > grantedTier) {
            for (uint8_t t = grantedTier; t < qualifyingTier; ++t) {
                result.unlockedRewardItemIds.push_back(SAVINGS_REWARD_ITEM_IDS[t]);
            }
            setGrantedSavingsTier(qualifyingTier);
        }

        // 2. Accrue monthly interest if at least 1 month has elapsed
        if (elapsedMonths >= 1 && balance > 0) {
            uint32_t totalInterest = 0;
            for (int m = 0; m < elapsedMonths; ++m) {
                float monthInterestF = static_cast<float>(balance) * MONTHLY_INTEREST_RATE;
                auto monthInterest = static_cast<uint32_t>(monthInterestF);
                totalInterest += monthInterest;
                if (totalInterest > MAX_MONTHLY_INTEREST) {
                    totalInterest = MAX_MONTHLY_INTEREST;
                    break;
                }
            }

            if (totalInterest > 0) {
                deposit(totalInterest);
                result.accruedInterest = totalInterest;
                result.interestLetterQueued = true;
            }
        }

        result.newBalance = getBalance();
        return result;
    }

private:
    AbdAccount& m_account;
    uint8_t& m_savingsFlag1;
    uint8_t& m_savingsFlag2;
    std::mt19937 m_rng;
};

} // namespace acnl::bank

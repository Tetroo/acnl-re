#pragma once

#include <cstdint>
#include <array>
#include <string_view>
#include <optional>
#include <cstring>
#include "../../types/Mail.h"

namespace acnl::mail {

class MailManager {
public:
    explicit MailManager(PlayerMailRegion& playerMail, PostOfficeMailStorage& postStorage)
        : m_playerMail(playerMail)
        , m_postStorage(postStorage)
    {}

    /**
     * @brief Finds first free pocket mail slot (out of 10).
     */
    [[nodiscard]] std::optional<size_t> findFreePocketSlot() const noexcept {
        for (size_t i = 0; i < m_playerMail.pockets.size(); ++i) {
            if (m_playerMail.pockets[i].recipientName[0] == u'\0' &&
                m_playerMail.pockets[i].messageBody[0] == u'\0') {
                return i;
            }
        }
        return std::nullopt;
    }

    /**
     * @brief Delivers letter to player pocket or returns false if pockets full.
     */
    bool deliverLetterToPockets(const MailData& letter) noexcept {
        auto slot = findFreePocketSlot();
        if (!slot) {
            return false;
        }
        m_playerMail.pockets[*slot] = letter;
        return true;
    }

    /**
     * @brief Deposits letter into Post Office persistent storage (80 slots).
     */
    bool storeInPostOffice(size_t pocketIndex) noexcept {
        if (pocketIndex >= m_playerMail.pockets.size()) {
            return false;
        }
        // Find free slot in post office storage
        for (auto& stored : m_postStorage.storedLetters) {
            if (stored.recipientName[0] == u'\0' && stored.messageBody[0] == u'\0') {
                stored = m_playerMail.pockets[pocketIndex];
                // Clear from pocket
                std::memset(&m_playerMail.pockets[pocketIndex], 0, sizeof(MailData));
                m_playerMail.pockets[pocketIndex].presentItemId = ITEM_NONE_PRESENT;
                return true;
            }
        }
        return false; // Storage full (80 letters)
    }

    /**
     * @brief Takes attached present from letter.
     * @return Item ID of present or ITEM_NONE_PRESENT if none.
     */
    uint16_t takePresent(size_t pocketIndex) noexcept {
        if (pocketIndex >= m_playerMail.pockets.size()) {
            return ITEM_NONE_PRESENT;
        }
        auto& letter = m_playerMail.pockets[pocketIndex];
        uint16_t item = letter.presentItemId;
        letter.presentItemId = ITEM_NONE_PRESENT;
        letter.flags |= 0x02; // Mark gift as opened
        return item;
    }

    /**
     * @brief Creates a system notice letter from post office (e.g. Mail_SP_Postoffice).
     */
    static MailData createPostOfficeNotice(
        std::u16string_view recipient,
        std::u16string_view body,
        uint16_t giftItemId = ITEM_NONE_PRESENT
    ) noexcept {
        MailData mail{};
        std::memset(&mail, 0, sizeof(MailData));

        // Copy recipient
        size_t rLen = std::min(recipient.size(), size_t{32});
        std::memcpy(mail.recipientName, recipient.data(), rLen * sizeof(char16_t));
        mail.recipientName[rLen] = u'\0';

        // Copy body
        size_t bLen = std::min(body.size(), size_t{192});
        std::memcpy(mail.messageBody, body.data(), bLen * sizeof(char16_t));
        mail.messageBody[bLen] = u'\0';

        // Sender: Post Office
        std::u16string_view sender = u"Post Office";
        std::memcpy(mail.senderName, sender.data(), sender.size() * sizeof(char16_t));
        mail.senderName[sender.size()] = u'\0';

        mail.category = 2; // Special system
        mail.stationeryId = 0x01; // Post Office stationery
        mail.presentItemId = giftItemId;
        return mail;
    }

private:
    PlayerMailRegion& m_playerMail;
    PostOfficeMailStorage& m_postStorage;
};

} // namespace acnl::mail

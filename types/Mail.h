#pragma once
#include <cstdint>
#include <array>
#include "Item.h"

namespace acnl::mail {

#pragma pack(push, 1)

/**
 * @brief Binary structure of an individual letter (dSvMail).
 * Exact binary size: 640 bytes (0x280).
 * Initialized by Save_Mail_ConstructLetter640B (0x002FF304).
 */
struct MailData {
    uint8_t  headerFlags[104];     // +0x000: Internal header flags & message ID
    char16_t recipientName[33];    // +0x068: Recipient name (UTF-16 string, 66 bytes)
    char16_t messageBody[193];     // +0x0AA: Letter message content (UTF-16 string, 386 bytes)
    char16_t senderName[33];       // +0x22C: Sender name & town (UTF-16 string, 66 bytes)
    uint8_t  stationeryId;         // +0x26E: Paper / stationery item ID
    uint8_t  flags;                // +0x26F: Status flags ([0]=Read, [1]=Gift present opened)
    uint8_t  category;             // +0x270: Category (Villager, Special NPC, Friend, Bank)
    uint8_t  moodSubtype;          // +0x271: Dialogue mood / tone subtype
    uint8_t  deliveryFlags;        // +0x272: Delivery state
    uint8_t  padding;              // +0x273: Alignment byte
    uint16_t presentItemId;        // +0x274: Attached present item ID (0x7FFE = None)
    uint16_t presentFlags;         // +0x276: Present modifier / wrapping flags
    uint64_t timestamp;            // +0x278: Delivery timestamp or unique message ID
};
static_assert(sizeof(MailData) == 0x280, "MailData must be exactly 640 bytes (0x280)");

/**
 * @brief Mail structure block for a single player in SaveFile.
 * Offset within player region: +0x000.
 * Total size: 0x1B88 bytes (7,048 bytes).
 * Initialized by Save_Player_InitMailPockets (0x006F4234).
 */
struct PlayerMailRegion {
    std::array<MailData, 10> pockets;      // +0x0000: 10 letters in player inventory (0x1900 bytes)
    MailData                 workingMail;  // +0x1900: 1 incoming delivery / draft buffer (0x280 bytes)
    uint32_t                 tailFlag1;    // +0x1B80: Initialized to 0xFFFFFFFF
    uint32_t                 tailFlag2;    // +0x1B84: Initialized to 0x7FFFFFFF
};
static_assert(sizeof(PlayerMailRegion) == 0x1B88, "PlayerMailRegion must be exactly 0x1B88 bytes (7,048 bytes)");

/**
 * @brief Global Post Office mail storage (saving letters at the post office counter).
 * Total size: 80 letters * 640 bytes = 51,200 bytes (0xC800).
 * Initialized in Save_AllocateAndConstructSaveBuffer (0x005C9DD4).
 */
struct PostOfficeMailStorage {
    std::array<MailData, 80> storedLetters; // 80 persistent letters preserved at post office
};
static_assert(sizeof(PostOfficeMailStorage) == 0xC800, "PostOfficeMailStorage must be exactly 51,200 bytes (0xC800)");

#pragma pack(pop)

// ============================================================================
// Mail Constants
// ============================================================================
inline constexpr size_t POCKET_MAIL_COUNT       = 10;
inline constexpr size_t POST_OFFICE_MAIL_COUNT  = 80;
inline constexpr uint16_t ITEM_NONE_PRESENT     = 0x7FFE;

} // namespace acnl::mail

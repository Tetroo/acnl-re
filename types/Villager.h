#pragma once
#include <cstdint>
#include <array>
#include <string_view>

namespace acnl::npc {

// ============================================================================
// Villager Personalities (Internal Nintendo Codes)
// ============================================================================
enum class Personality : uint8_t {
    Lazy     = 0, // BO (ぼんやり - Bonyari)
    Jock     = 1, // HA (ハキハキ - Hakihaki)
    Cranky   = 2, // KO (コワイ - Kowai)
    Smug     = 3, // ZK (キザ - Kiza)
    Normal   = 4, // FU (ふつう - Futsuu / Sweet)
    Peppy    = 5, // GE (げんき - Genki)
    Snooty   = 6, // OT (オトナ - Otona)
    Sisterly = 7  // AN (アネキ - Aneki / Uchi)
};

// ============================================================================
// Villager Activity & AI States (from NNPC_Ai.msbf)
// ============================================================================
enum class VillagerAiState : uint8_t {
    Sleeping         = 0,
    InsideOwnHouse   = 1,
    WanderingOutdoor = 2,
    VisitingShop     = 3, // Visiting Main Street / Museum / Re-Tail
    VisitingNeighbor = 4, // Host or Guest in another villager's house
    PingingPlayer    = 5, // Exclamation mark (!) running to player (NNPC_Approach)
    HoldingTool      = 6, // Fishing at river, bug net, watering can
    HoldingUmbrella  = 7, // Triggered when WeatherType == Rain
    SittingSpot      = 8, // Sitting on PWP bench, tree stump, or fountain
    SickInBed        = 9  // Sickness quest (NNPC_Q11_Sick)
};

// ============================================================================
// Villager Daily Schedule Table (Standard Town Ordinance)
// ============================================================================
struct VillagerSchedule {
    uint8_t wakeHour;
    uint8_t wakeMinute;
    uint8_t sleepHour;
    uint8_t sleepMinute;
};

inline constexpr std::array<VillagerSchedule, 8> PERSONALITY_SCHEDULES = {{
    { 7, 30, 23,  0 }, // Lazy     (BO): 7:30 AM - 11:00 PM
    { 6, 30,  0, 30 }, // Jock     (HA): 6:30 AM - 12:30 AM
    { 9,  0,  3, 30 }, // Cranky   (KO): 9:00 AM - 3:30 AM
    { 7,  0,  1, 30 }, // Smug     (ZK): 7:00 AM - 1:30 AM
    { 6,  0,  0,  0 }, // Normal   (FU): 6:00 AM - 12:00 AM
    { 7,  0,  1, 30 }, // Peppy    (GE): 7:00 AM - 1:30 AM
    { 8, 30,  2,  0 }, // Snooty   (OT): 8:30 AM - 2:00 AM
    { 9, 30,  3,  0 }  // Sisterly (AN): 9:30 AM - 3:00 AM
}};

// ============================================================================
// Quest Classification (NNPC_Q01..Q12)
// ============================================================================
enum class QuestType : uint8_t {
    None             = 0,
    DeliverPresent   = 1,  // Q01
    CatchBug         = 2,  // Q02
    CatchFish        = 3,  // Q03
    VisitPlayerHouse = 4,  // Q04
    FetchFruit       = 5,  // Q05
    DeliverTimeCapsule=6,  // Q06
    TradeFurniture   = 7,  // Q07 / Q09
    InviteToOwnHouse = 8,  // Q08
    FindLostItem     = 10, // Q10
    CureSickness     = 11, // Q11
    PetitionSignatures=12  // Q12
};

#pragma pack(push, 1)

// ============================================================================
// Friendship & Moving Status (Bits of byte at offset +0x270)
// ============================================================================
enum class VillagerRelationTier : uint8_t {
    EmptySlot       = 0, // Unassigned / no villager
    MovingInBoxes   = 1, // Moving into town, unpacking boxes
    NormalResident  = 2, // Standard neighbor resident
    BestFriend      = 3, // High friendship tier (picture unlocked)
    MovingOutBoxes  = 4  // In boxes preparing to leave
};

// Per-player villager relationship structure (640 bytes / 0x280)
// Initialized by Save_ConstructPlayerSubStruct640B (0x002FF304)
struct PlayerVillagerRelation {
    char     catchphrase[0x31];           // +0x000: Custom catchphrase string
    uint8_t  pad_31[3];                   // +0x031
    char     custom_greeting[0x31];       // +0x034: Custom secret greeting string
    uint8_t  pad_65[3];                   // +0x065
    uint8_t  mail_header_1[0x42];         // +0x068: Inbound letter header
    uint8_t  letter_body[0x182];          // +0x0AA: Letter message UTF-16 content (386 bytes)
    uint8_t  mail_header_2[0x42];         // +0x22C: Outbound reply letter
    uint8_t  flag_26e;                    // +0x26E
    uint8_t  flag_26f;                    // +0x26F
    uint8_t  relation_state;              // +0x270: [2:0] tier, [4:3] gift exchange, [5] spoken today
    uint8_t  friendship_points;           // +0x271: Numerical friendship score (0..255)
    uint8_t  quest_state;                 // +0x272: Active quest progress tracker
    uint8_t  pad_273;                     // +0x273
    uint16_t requested_item_id;           // +0x274: Item ID requested/involved in quest
    uint16_t pad_276;                     // +0x276
    uint64_t last_interaction_ticks;      // +0x278: Wall-clock tick timestamp of last conversation
};
static_assert(sizeof(PlayerVillagerRelation) == 0x280, "PlayerVillagerRelation size mismatch");

// Individual Villager Save Record (9,496 bytes / 0x2518)
// Initialized by Save_InitSingleVillagerRecord (0x00308D08)
struct VillagerSaveRecord {
    uint8_t  unknown_header[0x30];        // +0x0000
    uint8_t  interior_furniture[0x870];   // +0x0030: Placed house furniture slots
    uint8_t  clothing_and_pattern[0x16];  // +0x08A0: Worn shirt, umbrella, pattern data
    uint8_t  personality_code;            // +0x08B6: Personality (0..7)
    uint8_t  species_id;                  // +0x08B7: Animal species ID
    uint8_t  pad_8b8[6];                  // +0x08B8
    uint8_t  speech_quirks[8];            // +0x08BE
    uint32_t catchphrase_flags[4];        // +0x08C6
    uint32_t dialogue_history[4];         // +0x08D6
    uint32_t pending_item_1;              // +0x08E6
    uint32_t pending_item_2;              // +0x08EA
    uint8_t  letter_archives[16 * 0xF2];  // +0x08EE..0x180F: 16 saved resident letters
    PlayerVillagerRelation relations[4];  // +0x1810..0x220F: 4 Player relation records (0x280 each)
    PlayerVillagerRelation newcomer_slot; // +0x2210..0x248F: Pending move-in relation record
    uint16_t move_in_year;                // +0x2490: Move-in calendar year (e.g. 2011/2012)
    uint16_t player_first_met_year[4];    // +0x2492..0x2499
    uint8_t  pad_249a[0x6E];              // +0x249A
    uint8_t  house_exterior_style;        // +0x2508: Roof/wall/door style
    uint8_t  pad_2509;                    // +0x2509
    uint8_t  house_acre_x;                // +0x250A: Acre X grid coordinate
    uint8_t  house_acre_z;                // +0x250B: Acre Z grid coordinate
    uint8_t  house_tile_x;                // +0x250C: Tile X within acre
    uint8_t  house_tile_z;                // +0x250D: Tile Z within acre
    uint8_t  moving_state_flags;          // +0x250E: Moving state (2 = stable resident)
    uint8_t  sleep_schedule_override;     // +0x250F: Ordinance shift flags
    uint32_t villager_crc32;              // +0x2510: Individual record checksum
    uint8_t  pad_2514[4];                 // +0x2514
};
static_assert(sizeof(VillagerSaveRecord) == 0x2518, "VillagerSaveRecord size mismatch");

#pragma pack(pop)

} // namespace acnl::npc

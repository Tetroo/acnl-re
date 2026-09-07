#pragma once
#include <cstdint>
#include <cstddef>

#pragma pack(push, 1)

namespace ACNL {

/**
 * @brief Total counts of collectible specimens in the Museum by category.
 */
inline constexpr uint32_t kMuseumFossilsCount        = 67;
inline constexpr uint32_t kMuseumFishCount           = 72;
inline constexpr uint32_t kMuseumDivingCount         = 30;
inline constexpr uint32_t kMuseumInsectsCount        = 72;
inline constexpr uint32_t kMuseumArtPaintingsCount   = 25;
inline constexpr uint32_t kMuseumArtSculpturesCount  = 8;

inline constexpr uint32_t kMuseumArtTotalCount       = kMuseumArtPaintingsCount + kMuseumArtSculpturesCount; // 33
inline constexpr uint32_t kMuseumAquariumTotalCount  = kMuseumFishCount + kMuseumDivingCount;               // 102
inline constexpr uint32_t kMuseumTotalSpecimens      = kMuseumFossilsCount + kMuseumFishCount + 
                                                       kMuseumDivingCount + kMuseumInsectsCount + 
                                                       kMuseumArtPaintingsCount + kMuseumArtSculpturesCount; // 274

/**
 * @brief Default/Uninitialized specimen donation date (2012-01-01).
 */
inline constexpr uint16_t kMuseumDefaultYear  = 0x07DC; // 2012
inline constexpr uint8_t  kMuseumDefaultMonth = 0x01;   // January
inline constexpr uint8_t  kMuseumDefaultDay   = 0x01;   // 1st

/**
 * @brief Donation flag byte indicating who donated the specimen.
 */
enum class MuseumDonatorStatus : uint8_t {
    Uncollected = 0,
    Player1     = 1,
    Player2     = 2,
    Player3     = 3,
    Player4     = 4,
    SpecialTown = 5 // Town / Pre-donated / Special event
};

/**
 * @brief Specimen categories for indexing and validation (0..5).
 */
enum class MuseumCategory : uint8_t {
    Fossils     = 0,
    Fish        = 1,
    Diving      = 2,
    Insects     = 3,
    Paintings   = 4,
    Sculptures  = 5
};

/**
 * @brief Museum 3D Exhibit Scene / Room IDs.
 */
enum class MuseumRoomId : uint8_t {
    AquariumEntrance        = 0x4A, // Fresh water / small river specimens
    AquariumMainTank        = 0x4B, // Large ocean fish / pelagic tank
    AquariumDeepSea         = 0x4C, // Deep-sea diving creatures & trench tank
    InsectariumTropical     = 0x4D, // Room 1: Tropical / Palm tree beetles
    InsectariumForest       = 0x4E, // Room 2: Temperate forest / Garden insects
    InsectariumSubterranean = 0x4F, // Room 3: Ground / rock / dark habitat insects
    PaleontologySmall       = 0x50, // Room 1: Small fossils, amber, trilobite, ammonite
    PaleontologyMainHall    = 0x51, // Room 2: Mesozoic dinosaurs (T-Rex, Triceratops, Diplodocus)
    PaleontologyCenozoic    = 0x52, // Room 3: Cenozoic mammals (Mammoth, Smilodon)
    ArtGalleryPaintings     = 0x53, // Paintings gallery
    ArtGallerySculptures    = 0x54  // Sculptures & statues gallery
};

/**
 * @brief Packed 4-byte donation timestamp recorded upon specimen handover.
 */
struct MuseumDonationDate {
    uint16_t year;  // Little-endian year (e.g. 2012 = 0x07DC)
    uint8_t  month; // Month (1..12)
    uint8_t  day;   // Day of month (1..31)

    [[nodiscard]] constexpr bool IsDefault() const noexcept {
        return year == kMuseumDefaultYear && month == kMuseumDefaultMonth && day == kMuseumDefaultDay;
    }
};
static_assert(sizeof(MuseumDonationDate) == 4, "MuseumDonationDate must be 4 bytes");

/**
 * @brief Monolithic Museum Donations Save Block (0x55C = 1,372 bytes).
 * Located at save buffer offset +0x6AE38 in Main Street data.
 * Contains 274 specimen donation date records followed by 274 donation flag bytes.
 */
struct MuseumDonationsBlock {
    // 274 Specimen Dates (274 * 4 = 1,096 bytes: +0x000 .. +0x447)
    MuseumDonationDate fossil_dates[kMuseumFossilsCount];               // +0x000 .. +0x10B (67 * 4 = 268 B)
    MuseumDonationDate fish_dates[kMuseumFishCount];                   // +0x10C .. +0x22B (72 * 4 = 288 B)
    MuseumDonationDate diving_dates[kMuseumDivingCount];               // +0x22C .. +0x2A3 (30 * 4 = 120 B)
    MuseumDonationDate insect_dates[kMuseumInsectsCount];              // +0x2A4 .. +0x3C3 (72 * 4 = 288 B)
    MuseumDonationDate painting_dates[kMuseumArtPaintingsCount];       // +0x3C4 .. +0x427 (25 * 4 = 100 B)
    MuseumDonationDate sculpture_dates[kMuseumArtSculpturesCount];     // +0x428 .. +0x447 (8 * 4 = 32 B)

    // 274 Specimen Donation Status Flags (274 * 1 = 274 bytes: +0x448 .. +0x559)
    uint8_t fossil_flags[kMuseumFossilsCount];           // +0x448 (67 bytes)
    uint8_t fish_flags[kMuseumFishCount];               // +0x48B (72 bytes)
    uint8_t diving_flags[kMuseumDivingCount];           // +0x4D3 (30 bytes)
    uint8_t insect_flags[kMuseumInsectsCount];          // +0x4F1 (72 bytes)
    uint8_t painting_flags[kMuseumArtPaintingsCount];   // +0x539 (25 bytes)
    uint8_t sculpture_flags[kMuseumArtSculpturesCount]; // +0x552 (8 bytes)

    uint16_t padding; // +0x55A (2 bytes alignment padding)
};
static_assert(sizeof(MuseumDonationsBlock) == 0x55C, "MuseumDonationsBlock must be exactly 0x55C (1372) bytes");

/**
 * @brief Single 2F Custom Exhibition Room data block (approx 2,574 bytes).
 */
struct MuseumCustomExhibitionRoom {
    uint16_t furniture_layout[1260]; // 2,520 bytes: 16x16 grid furniture item IDs
    char16_t room_title[27];         // 54 bytes: Custom UTF-16 exhibition room name
};

/**
 * @brief 2F Custom Exhibition Save Block at save offset 0x502BC (10,484 bytes).
 */
struct MuseumCustomExhibitionsBlock {
    uint32_t crc32;                             // +0x00: CRC32 checksum
    uint8_t  padding_04[14];                    // +0x04: Header / flags
    MuseumCustomExhibitionRoom rooms[4];        // +0x12: 4 Custom rooms
    uint8_t  trailing_padding[174];             // Alignment padding to 10,484 bytes
};
static_assert(sizeof(MuseumCustomExhibitionsBlock) == 10484, "MuseumCustomExhibitionsBlock must be 10484 bytes");

} // namespace ACNL

#pragma pack(pop)

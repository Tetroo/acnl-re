#pragma once
#include <cstdint>
#include <array>

namespace acnl::critters {

#pragma pack(push, 1)

// Shadow size categories for fish
enum class FishShadowSize : uint32_t {
    Tiny       = 1, // Crucian carp, Goldfish, Tadpole
    Small      = 2, // Pale chub, Bitterling, Yellow perch
    Medium     = 3, // Black bass, Red snapper, Squid
    Large      = 4, // Sea bass, Barbel steed, Salmon
    VeryLarge  = 5, // Stringfish, Coelacanth, Tuna
    HugeFin    = 6  // Ocean Sunfish, Sharks (Fin on top)
};

// Water environment types for fish spawning
enum class WaterEnvironment : uint32_t {
    River       = 1,
    Ocean       = 2,
    Pond        = 3,
    RiverMouth  = 4,
    Waterfall   = 5,
    IslandOcean = 6
};

// Master Fish Specification Record (76 bytes / 0x4C)
// Stride 0x4C in master table starting at 0x00869160
struct FishSpecification {
    const char*       model_path;         // +0x00: RomFS model path (e.g. "Fish/fsh_tanago.bcres")
    uint32_t          pad_04;             // +0x04
    uint32_t          sell_price;         // +0x08: Bell price at Re-Tail / Nookling
    uint32_t          pad_0c;             // +0x0C
    FishShadowSize    shadow_size;        // +0x10: 1..6
    WaterEnvironment  environment;        // +0x14: 1..6
    uint32_t          spawn_month_mask;   // +0x18: Bitmask of active months (bits 0..11)
    uint32_t          spawn_hour_mask;    // +0x1C: Bitmask of active time periods
    uint32_t          pad_20;             // +0x20
    float             swim_speed_scale;   // +0x24: Movement speed scalar (e.g. 1.0f)
    uint32_t          rarity_tier;        // +0x28: Relative spawn weight
    uint32_t          pad_2c[4];          // +0x2C..0x3B
    uint16_t          item_id;            // +0x3C: Inventory pocket item ID (0x1500..0x15FF)
    uint16_t          pad_3e;             // +0x3E
    uint32_t          unk_flag;           // +0x40
    const char*       internal_name;      // +0x44: Japanese species name (e.g. "Tanago", "Oikawa")
    const char*       shadow_asset;       // +0x48: Shadow model asset path
};
static_assert(sizeof(FishSpecification) == 0x4C, "FishSpecification size mismatch");

// Master Insect Specification Record (80 bytes / 0x50)
// Stride 0x50 in master table starting at 0x00867724
struct InsectSpecification {
    const char*       model_path;         // +0x00: RomFS model path (e.g. "Insect/ins_monsiro.bcres")
    float             net_catch_radius;   // +0x04: Hitbox radius for bug net swing
    float             player_detect_dist; // +0x08: Proximity threshold triggering flee state
    float             flight_altitude;    // +0x0C: Hover height above terrain elevation
    float             flight_speed;       // +0x10: Locomotion speed (units/frame)
    float             turn_rate;          // +0x14: Angular steering velocity
    uint32_t          sell_price;         // +0x18: Bell price at Re-Tail / Nookling
    uint32_t          spawn_month_mask;   // +0x1C: Bitmask of active months (bits 0..11)
    uint32_t          spawn_hour_mask;    // +0x20: Bitmask of active hours
    uint32_t          spawn_weather_flags;// +0x24: Rain/snow tolerance flag
    uint32_t          rarity_tier;        // +0x28: Relative spawn weight
    uint32_t          pad_2c[3];          // +0x2C..0x37
    uint16_t          item_id;            // +0x38: Inventory pocket item ID (0x1600..0x16FF)
    uint16_t          pad_3a;             // +0x3A
    const char*       internal_name;      // +0x3C: Japanese species name
    uint32_t          pad_40[4];          // +0x40..0x4F
};
static_assert(sizeof(InsectSpecification) == 0x50, "InsectSpecification size mismatch");

// Global Fish Manager (BsFishFieldMgr)
// Vtable: 0x008EDA24
struct BsFishFieldMgr {
    void*     vtable;                     // +0x00: 0x008EDA24
    uint8_t   pad_04[0x0F];               // +0x04..0x12
    uint8_t   active_count;               // +0x13: Number of swimming fish shadows (max 8)
    uint8_t   pad_14[0x10];               // +0x14..0x23
    struct {
        uint32_t actor_ptr;               // +0x00
        float    pos_x;                   // +0x04
        float    pos_y;                   // +0x08
        float    pos_z;                   // +0x0C
        uint16_t species_id;              // +0x10
        uint8_t  slot_flags;              // +0x12
        uint8_t  active_flag;             // +0x13
        uint32_t lifetime_ticks;          // +0x14
    } fish_slots[8];                      // +0x24..0xE3: 8 Fish shadow slots (0x18 each)
};

// Global Insect Manager (BsInsectFieldMgr)
// Vtable: 0x008F0188
struct BsInsectFieldMgr {
    void*     vtable;                     // +0x00: 0x008F0188
    uint8_t   pad_04[0x0F];               // +0x04..0x12
    uint8_t   active_count;               // +0x13: Number of active insects (max 12)
    uint8_t   pad_14[0x14];               // +0x14..0x27
    uint8_t   insect_slots[12 * 0x144];   // +0x28..0xF57: 12 Insect actor slots (0x144 each)
    uint8_t   special_slots[2 * 0x144];   // +0xF58..0x11DF: 2 Special event slots (bees/ants)
};

#pragma pack(pop)

} // namespace acnl::critters

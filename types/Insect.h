#pragma once
#include <cstdint>
#include "Actor.h"

namespace acnl::insect {

#pragma pack(push, 1)

/**
 * @brief 80-byte (0x50) parameter record for each insect species.
 * Stored at binary table 0x0086773C (72 entries).
 */
struct InsectSpeciesParams {
    float    collision_radius;     // +0x00: Net hit detection radius
    float    collision_radius_copy;// +0x04
    float    detection_radius;     // +0x08: Player approach detection radius
    float    flying_altitude;      // +0x0C: Altitude above ground
    float    flight_speed;         // +0x10: Movement speed units/frame
    float    turn_rate;            // +0x14: Angular heading change
    uint8_t  unknown_flags[56];    // +0x18..0x4F
};
static_assert(sizeof(InsectSpeciesParams) == 0x50, "InsectSpeciesParams must be 0x50 bytes");

/**
 * @brief Insect surface category.
 */
enum class InsectCategory : uint8_t {
    Flying = 'A', // Butterflies, Dragonflies (despawn in rain/snow)
    Tree   = 'F', // Beetles, Cicadas on trunks
    Ground = 'G'  // Mantis, Grasshopper, Cricket on flowers/grass
};

/**
 * @brief Base insect entity in town (AcInsectFieldBase).
 * Inherits from AcObjectBase.
 */
struct AcInsectFieldBase : public AcObjectBase {
    uint8_t  species_id;            // +0xC9: Species index (0..71)
    uint8_t  category_char;         // 'A', 'F', 'G'
    float    current_coll_radius;   // +0x8C
    float    detection_radius_sq;   // +0x114: (detection_radius * detection_radius)
    float    target_altitude;       // +0x118
    uint8_t  flee_state_flag;       // +0x1CB: Flagged when running player detected
};

#pragma pack(pop)

} // namespace acnl::insect

#pragma once
#include <cstdint>
#include "Actor.h"

namespace acnl::world {

#pragma pack(push, 1)

/**
 * @brief Public Works Projects (PWP) and Town Structure IDs.
 */
enum class StructureType : uint16_t {
    TownHallOffice     = 0x01, // AcStrcOffice (Mayor & Isabelle desk)
    TownPlazaTree      = 0x02, // AcStrcSymboltree (grows with player time)
    RecycleShop        = 0x03, // AcStrcRecycleShop (Re-Tail)
    PlayerHouse        = 0x04, // AcStrcPlayerHouse (Levels 1..8)
    VillagerHouse      = 0x05, // AcStrcNpcHouse (10 villager plots)
    RailwayStation     = 0x06, // Porter's train station
    RailwayCrossing    = 0x07, // AcStrcCrossing (flashing signal gate)
    PierDock           = 0x08, // AcStrcPier (Kapp'n's boat dock)
    ResetCenter        = 0x09, // AcStrcReset (Resetti manhole)
    CobblestoneBridge  = 0x10, // AcStrcBridge variations
    WoodenBridge       = 0x11,
    SuspensionBridge   = 0x12,
    BrickBridge        = 0x13,
    ModernBridge       = 0x14,
    FairyTaleBridge    = 0x15,
    Lighthouse         = 0x20, // AcStrcLightHouse
    Geyser             = 0x21, // AcStrcGeyser
    CampgroundSite     = 0x22,
    PoliceStation      = 0x23, // Copper / Booker
    CafeRoost          = 0x24  // Brewster
};

/**
 * @brief Plaza Symbol Tree growth stages based on total gameplay time.
 */
enum class SymbolTreeStage : uint8_t {
    PlantedSeedling = 0, // Day 1 (Town founding ceremony)
    Stage1_5Days    = 1, // 5 days / 5 hours: Small tree with 1 leaf cluster
    Stage2_20Days   = 2, // 20 days / 20 hours: 2 leaf clusters
    Stage3_50Days   = 3, // 50 days / 50 hours: Trunk thickens
    Stage4_100Days  = 4, // 100 days / 100 hours: Can sit on tree plaza rim
    Stage5_180Days  = 5, // 180 days / 180 hours: Credits roll interaction
    Stage6_300Days  = 6, // 300 days / 300 hours: Full canopy
    Stage7_500Days  = 7  // 500 days / 500 hours: Massive ancient tree
};

/**
 * @brief Base structure entity layout (AcStrc).
 * Inherits from AcObjectBase.
 */
struct AcStrc : public AcObjectBase {
    StructureType structure_type;   // +0xD0
    uint8_t       door_state;       // 0=Closed, 1=Opening, 2=Open, 3=Closing
    uint8_t       light_state;      // 0=Off, 1=On (night window light)
    uint16_t      acre_grid_pos_x;  // World tile X (0..111)
    uint16_t      acre_grid_pos_z;  // World tile Z (0..95)
    uint8_t       footprint_w;      // Width in tiles (e.g. 3 for house)
    uint8_t       footprint_h;      // Height in tiles (e.g. 3 for house)
};

#pragma pack(pop)

} // namespace acnl::world

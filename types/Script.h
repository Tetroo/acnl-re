#pragma once
#include <cstdint>
#include <cstddef>
#include <span>
#include <string_view>

/**
 * @file Script.h
 * @brief Dialogue, Flowchart & Text Engine (MSBT, MSBF, MSBP) for ACNL: Welcome amiibo
 * 
 * Nintendo Message Studio Binary specification and runtime classes:
 * - MSBT (MsgStdBn): Localized dialogue text with embedded formatting tags
 * - MSBF (MsgFlwBn): Event flowcharts and dialogue trees (FLW3, FEN1, REF1)
 * - MSBP (MsgPrjBn): Project palette (CLR1), color labels (CLB1), and flow metadata
 * - UMSBT container: Multi-language container indexing localized MSBT binaries
 */

namespace acnl::script {

#pragma pack(push, 1)

// ============================================================================
// 1. Common Message Studio Binary (MSB) Header
// ============================================================================

struct MsbHeader {
    char magic[8];           ///< "MsgStdBn", "MsgFlwBn", or "MsgPrjBn"
    uint16_t byte_order_mark;///< 0xFEFF = Big-Endian, 0xFFFE = Little-Endian (CTR uses 0xFEFF / Little-Endian)
    uint16_t version;        ///< Typically 0x0000 or 0x0001
    uint16_t section_count;  ///< Number of sections in file
    uint16_t reserved;       ///< Alignment padding
    uint32_t file_size;      ///< Total file size in bytes
    uint8_t  padding[10];    ///< Pad header to 0x20 (32 bytes)
};
static_assert(sizeof(MsbHeader) == 0x20, "MsbHeader must be 32 bytes");

struct MsbSectionHeader {
    char tag[4];             ///< FourCC: "LBL1", "TXT2", "ATR1", "FLW3", "FEN1", etc.
    uint32_t size;           ///< Section data size (excluding 16-byte header)
    uint8_t  reserved[8];    ///< Padding / reserved
};
static_assert(sizeof(MsbSectionHeader) == 0x10, "MsbSectionHeader must be 16 bytes");

// ============================================================================
// 2. MSBT Text & Label Structures
// ============================================================================

struct MsbtLabelBucket {
    uint32_t count;          ///< Number of labels in this hash bucket
    uint32_t offset;         ///< Offset relative to start of label data table
};

struct MsbtLabelEntry {
    uint8_t length;          ///< Length of label name string
    // char name[length];    ///< ASCII/UTF-8 label name (variable length)
    // uint32_t string_index;///< Index into TXT2 table
};

struct MsbtTxtHeader {
    uint32_t string_count;   ///< Number of strings in TXT2
    // uint32_t offsets[string_count]; ///< Offsets to UTF-16 string data relative to &string_count
};

// ============================================================================
// 3. Embedded Control Tags (Escape 0x000E in UTF-16)
// ============================================================================

enum class TagGroup : uint16_t {
    System      = 0x0000,    ///< System commands (rubi/furigana)
    Color       = 0x0007,    ///< Text color switch (0..7)
    SoundEffect = 0x0008,    ///< Sound effects / voice pitch modification
    Animation   = 0x0003,    ///< NPC emotion animation (e.g. 0=smile, 21=shock, 37=thinking)
    Variable    = 0x000C,    ///< Variable insertion: Player (17:1), Town, Villager, Catchphrase
    Choice      = 0x0009     ///< Choice prompt / interactive menu
};

struct TextControlTag {
    uint16_t escape_code;    ///< Always 0x000E
    TagGroup group;          ///< Tag group ID
    uint16_t tag_id;         ///< Tag ID within group
    uint16_t param_length;   ///< Length of parameter payload in bytes
    // uint8_t params[param_length];
};
static_assert(sizeof(TextControlTag) == 8, "TextControlTag header must be 8 bytes");

// ============================================================================
// 4. MSBF Flowchart Structures (FLW3 & FEN1)
// ============================================================================

enum class FlowNodeType : uint8_t {
    Message     = 0x01,      ///< Displays a dialogue box (MSBT string reference)
    Action      = 0x02,      ///< Executes a gameplay command/event (give item, play SFX, set flag)
    Branch      = 0x03,      ///< Condition branch (queries boolean condition, forks flow)
    EntryPoint  = 0x04       ///< Flowchart starting node (points to initial execution node)
};

struct FlwNode {
    FlowNodeType type;       ///< 0x01=Message, 0x02=Action, 0x03=Branch, 0x04=Entry
    uint8_t sub_type;        ///< Node sub-command or flag
    uint16_t next_node;      ///< Index of destination node on sequential step
    uint16_t param1;         ///< Command ID / Condition ID / Message ID
    uint16_t param2;         ///< Additional parameter
    uint16_t param3;         ///< Additional parameter / branch table offset
    uint8_t  extra[6];       ///< Node-specific payload
};
static_assert(sizeof(FlwNode) == 16, "FlwNode must be 16 bytes");

struct FlwHeader {
    uint16_t node_count;     ///< Number of FlwNode records in FLW3
    uint16_t branch_count;   ///< Number of entries in branch jump table
    uint8_t  reserved[12];   ///< Section padding
};
static_assert(sizeof(FlwHeader) == 16, "FlwHeader must be 16 bytes");

// ============================================================================
// 5. Palette & Text Colors (CLR1)
// ============================================================================

struct ColorRGBA {
    uint8_t r, g, b, a;
};

/// 8 Standard Dialogue Colors from CTR_GardenPlus.msbp
inline constexpr ColorRGBA kDialoguePalette[8] = {
    {   7,   0,   0,   0 },  ///< 0: Default (Dark black)
    { 255, 255, 255, 255 },  ///< 1: Player (Pure White)
    { 245,  90, 230, 255 },  ///< 2: Important (Magenta / Key terms)
    {   0, 190, 220, 255 },  ///< 3: Town (Cyan / Town name, rivers)
    {   0, 195,   0, 255 },  ///< 4: NPC (Green / Villager names)
    { 240, 140,   0, 255 },  ///< 5: Tweet (Orange / Dialogue bubbles)
    { 165, 190, 220, 255 },  ///< 6: ForEscapeIsland (Pastel blue)
    { 255,  32,  32, 255 }   ///< 7: Alert / Red
};

// ============================================================================
// 6. Villager Personalities & Dialogue Script IDs
// ============================================================================

enum class VillagerPersonality : uint8_t {
    Uchi_BigSister = 0,      ///< "An" (Aneki)
    Lazy           = 1,      ///< "Bo" (Boku)
    Normal         = 2,      ///< "Fu" (Futsuu)
    Smug           = 3,      ///< "Ge" (Gekijo)
    Jock           = 4,      ///< "Ha" (Harikiri)
    Cranky         = 5,      ///< "Ko" (Kowai)
    Snooty         = 6,      ///< "Ot" (Otona)
    Peppy          = 7       ///< "Zk" (Genki/Zokugo)
};

// ============================================================================
// 7. Multi-Language UMSBT Container
// ============================================================================

struct UmsbtLanguageEntry {
    uint32_t offset;         ///< Absolute offset to localized MsbHeader in UMSBT file
    uint32_t size;           ///< Size of localized MSBT binary
};

#pragma pack(pop)

// ============================================================================
// 8. Hash Calculation (CTR Message Studio Standard)
// ============================================================================

/**
 * @brief Computes hash for label lookup in LBL1 table
 * Formula: hash = hash * 1170 (0x492) + c; bucket = hash % bucket_count
 */
inline uint32_t CalculateLabelHash(std::string_view label, uint32_t bucket_count) {
    if (bucket_count == 0) return 0;
    uint32_t hash = 0;
    for (char c : label) {
        hash = hash * 0x492 + static_cast<uint8_t>(c);
    }
    return hash % bucket_count;
}

} // namespace acnl::script

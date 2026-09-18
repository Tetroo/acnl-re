// Auto-generated symbols.h for ACNL Reimplementation (PC Port)
#pragma once
#include <cstdint>

namespace acnl::symbols {

    // Initializes heap for weather sub-system
    inline constexpr uintptr_t Weather_InitWeatherHeap = 0x001003bc;

    // Per-frame update tick for a state manager object. Flushes pending state transitions across multiple subsystems, then che
    inline constexpr uintptr_t Time_StateManagerUpdateTick = 0x00100684;

    // Cleans up and destructs weather heap objects
    inline constexpr uintptr_t Weather_CleanupWeatherHeap = 0x001009e4;

    // First sub-ctor called in FUN005ca4f4's chain, at param1 = iVar1 + 0x29200 (where iVar1 is the cursor after the initial D
    inline constexpr uintptr_t Town_GridSubObjectConstructor = 0x00112584;

    // AUDIT 2026-09-07: does NOT iterate all 20 acres -- picks ONE random eligible acre via bounded RNG and processes it via Town_ProcessFloraTileGrid, then returns early. See systems/flora-growth.md.
    inline constexpr uintptr_t Town_AdvanceFloraGrowthForPlayableAcres = 0x00114e10;

    // Main daily flora tick: checks rain/snow weather to auto-water all flowers and triggers acre flora update.
    inline constexpr uintptr_t Town_BsGrowUp_UpdateDailyFloraGrowth = 0x001175bc;

    // Monitors GameDateTime day-of-month and executes acre scan when date advances.
    inline constexpr uintptr_t Town_BsGrowUp_CheckDayChange = 0x001176c4;

    // Application entry point. Initializes core services, waits for applet readiness, then enters the main game loop via FUN00
    inline constexpr uintptr_t Core_Main_EntryPoint = 0x0011d538;

    // Initializes core 3DS OS services required before the game loop starts. Contains IPC calls via SVC 0x24 (WaitSynchronizat
    inline constexpr uintptr_t Core_AppServiceInitializer = 0x0011e3d8;

    // The main game loop / application lifecycle function. Manages three initialization phases (tracked via bytes in appstate)
    inline constexpr uintptr_t Core_MainGameLoop = 0x0011edfc;

    // Converts a date object to a 64-bit tick representation. Called as Phase 1 of FUN00308F5C before tick-space arithmetic.
    inline constexpr uintptr_t Time_ConvertDateToTicks = 0x00124120;

    // Computes current wall-clock time by combining: 1. Elapsed ticks since startup (from FUN0012994C) 2. Base wall-clock time
    inline constexpr uintptr_t Time_ConvertTicksToWallClock = 0x00124154;

    // Converts a 64-bit tick value back to a date struct. Inverse of FUN00124120. Core utility for the entire date arithmetic
    inline constexpr uintptr_t Time_ConvertTicksToDate = 0x00126514;

    // Converts a broken-down date/time into a single 64-bit integer timestamp. Used as the foundation of all date arithmetic i
    inline constexpr uintptr_t Time_CalculateJulianDay = 0x001296bc;

    // Returns the current time as a 64-bit tick count derived from the 3DS system tick counter. Critically: this is not a dire
    inline constexpr uintptr_t Time_ReadCalibratedSystemTicks = 0x0012994c;

    // Waits for a service handle to become ready (SVC 0x24 = WaitSynchronization1), then proceeds to open the service port via
    inline constexpr uintptr_t Core_ServiceHandleWaitIpcInit = 0x0012a53c;

    // Destructor / cleanup function for the gardenplus.dat save buffer. Checks if the buffer is loaded, calls C++ destructors
    inline constexpr uintptr_t Save_CleanupSaveDataBuffer = 0x0012cc9c;

    // Converts a calendar date to a Julian Day Number (or equivalent day-count integer), using an epoch anchored at year 2000.
    inline constexpr uintptr_t Time_JulianDayNumberCalculator = 0x0012f8a0;

    // Computes a tick/Julian Day value from (year, month, dayfield). Variant of FUN0012F8A0 used specifically for sub-field no
    inline constexpr uintptr_t Time_JulianDaySubFieldCalculator = 0x0012f998;

    // Dispatches 2D lyt layout rendering for 16 pocket item slots and active item cursor.
    inline constexpr uintptr_t UI_BsMenuItem_DrawPocketsUI = 0x0019f2f4;

    // Initializes 16 pocket item icon sprites, equips hand slot layout, and binds touch callbacks.
    inline constexpr uintptr_t UI_BsMenuItem_InitPocketsMenu = 0x0019f37c;

    // AUDIT 2026-09-07: real code is a generic focus/dirty-flag dispatcher (toggles state bits, calls a vtable callback + two redraw calls at +0xE0/+0xD44). No drag-and-drop, fruit stacking, or context-menu logic found. See systems/inventory.md.
    inline constexpr uintptr_t UI_BsMenuItem_UpdatePocketsTouchAndButtons = 0x0019f768;

    // Closes pockets menu, saves inventory ordering to active player buffer, and resumes gameplay.
    inline constexpr uintptr_t UI_BsMenuItem_ClosePocketsMenu = 0x0019fa5c;

    // Per-frame wrapper around the save state machine. Prepares context, calls the dispatcher, then executes a virtual callbac
    inline constexpr uintptr_t Save_SaveSystemPerFrameTick = 0x001af1d8;

    // Generic low-level file reader. Implements the full open → seek → read → verify → close cycle against the nn::fs API. Ret
    inline constexpr uintptr_t Save_ReadSaveFileRaw = 0x001b8b9c;

    // Mounts the save data archive via nn::fs using the "data:" virtual drive. Sets a global busy flag before mounting. Return
    inline constexpr uintptr_t Save_MountSaveArchive = 0x001b927c;

    // Pre-flight check before reading gardenplus.dat. Verifies the save archive is accessible and mounts "data:" if needed. Re
    inline constexpr uintptr_t Save_CheckSaveMount = 0x001b9428;

    // Reads the entire gardenplus.dat save file into the runtime save buffer.
    inline constexpr uintptr_t Save_LoadGardenPlusFile = 0x001b96c8;

    // This function implements a state machine for resource loading. It manages the transition from raw data loading to struct
    inline constexpr uintptr_t Save_DispatchSaveLifecycle = 0x001d3748;

    // Per-tick dispatcher for the save system state machine. Extracts state bits [1:5] from param3 (uVar4 = param3 & 0x3e) and
    inline constexpr uintptr_t Save_DispatchSaveLoadStateMachine = 0x001d3d3c;

    // Minimal constructor - zeroes 3 uint32 fields and 1 uint16 field at offset +0x22, then calls FUN0027a490.
    inline constexpr uintptr_t Core_ConstructSmallMapStateObject = 0x001e0340;

    // Initializes the entire weather system: loads all four shaders, then populates a set of weather data tables from an exter
    inline constexpr uintptr_t Weather_InitializeWeatherSystem = 0x001e5bc8;

    // Queries a 4-byte property from the 23-period season parameter table (DAT_00951138, 0x28 bytes per period).
    inline constexpr uintptr_t Weather_GetSeasonParameter = 0x001e5eb0;

    // Per-frame tick that throttles weather parameter updates to every 300 frames (~5 seconds at 60fps). When the counter expi
    inline constexpr uintptr_t Weather_PeriodicWeatherUpdate = 0x001e5ef4;

    // Per-frame weather update. Reads current and next weather state indices from a time/date object, interpolates all weather
    inline constexpr uintptr_t Weather_InterpolateWeatherEnvironment = 0x001e63dc;

    // Blends up to 10 weighted sky color layers and pushes the results as GPU uniforms to PICA200. This is the per-frame sky r
    inline constexpr uintptr_t Weather_CompositeSkyColors = 0x001e6fe0;

    // Maps (weathertype, periodindex) → groupindex into EnvironmentParameter.bin.
    inline constexpr uintptr_t Weather_ResolveWeatherGroupIndex = 0x001e82e0;

    // Called on weather state transitions (not periodically). Resets weather object fields, sets cloud parameters, then calls
    inline constexpr uintptr_t Weather_ImmediateWeatherStateUpdater = 0x001e8518;

    // Ticks reel-in reaction timer. If player fails to press A before timer expires, fish transitions to Escape state.
    inline constexpr uintptr_t Fish_AcFsFdShadow_StateHit_Update = 0x001e9e68;

    // Evaluates 20%-per-nibble bite formula: P(Bite) = nibble_count * 20%. Nudges bobber on fail, bites on success.
    inline constexpr uintptr_t Fish_AcFsFdShadow_StatePick_Update = 0x001ea010;

    // Submerges bobber underwater, triggers splash SFX, and computes frame reel-in reaction window (seconds * 30.0).
    inline constexpr uintptr_t Fish_AcFsFdShadow_StateHit_Enter = 0x001eb5e4;

    // Enters Pick state: orients fish shadow towards detected player fishing bobber.
    inline constexpr uintptr_t Fish_AcFsFdShadow_StatePick_Enter = 0x001eb920;

    // Increases swimming speed by 3x and plots escape trajectory away from player before entity despawn.
    inline constexpr uintptr_t Fish_AcFsFdShadow_StateEscape_Enter = 0x001ebc08;

    // 12-state fish AI state machine dispatch and Euclidean player-distance check for running footstep spooking.
    inline constexpr uintptr_t Fish_AcFsFdShadow_UpdateSwimmingAI = 0x001ec774;

    // Draw dispatch: invokes model component draw (+0x18) and renders dynamic ground shadow ellipse via FUN_006EE3CC.
    inline constexpr uintptr_t Actor_AcObjectBase_Draw = 0x001f4b0c;

    // Velocity integration (pos += vel), ground elevation clamping via Town_SampleFlatGroundHeight, and affine model transform calculation.
    inline constexpr uintptr_t Actor_AcObjectBase_UpdatePhysics = 0x001f4b94;

    // Two-phase initialization of world entity: calculates acre coordinates (X/Z / 256), samples terrain elevation, registers in spatial acre grid.
    inline constexpr uintptr_t Actor_AcObjectBase_Init = 0x001f4cc4;

    // Per-frame entity tick: resets visual offset, executes behavior state machine (+0x60), updates physics/movement (+0x64), and calls post-update hook (+0x50).
    inline constexpr uintptr_t Actor_AcObjectBase_Update = 0x001f4de8;

    // Constructs 4x3 affine transform matrix from position Vector3 and applies XYZ Euler rotation angles via trigonometric LUT.
    inline constexpr uintptr_t Actor_AcObjectBase_BuildMatrix = 0x001f4ebc;

    // Deleting destructor for AcObjectBase. Invokes Actor_Actor_Dtor and deallocates instance memory via operator delete.
    inline constexpr uintptr_t Actor_AcObjectBase_Dtor = 0x001f50c0;

    // Master factory dispatcher (dFieldFactory.cpp) that instantiates the appropriate FieldBuilder (VillagePrologueBuilder, MuseumBuilder, CampNpcBuilder, etc.) based on game state flags.
    inline constexpr uintptr_t Town_FieldFactory_CreateBuilderForState = 0x00200d34;

    // Updates rain particle simulation state
    inline constexpr uintptr_t Weather_UpdateRainParticles = 0x00229684;

    // Updates snow particle simulation state
    inline constexpr uintptr_t Weather_UpdateSnowParticles = 0x0022a9c8;

    // Address does not resolve in Ghidra (re-audit 2026-09-07) - was: Timer/tick-based check function. Uses SVC 0x28 (GetSystemTick) and divides by 0x1a/0x5a constants. Calls FUN00300fdc onl
    inline constexpr uintptr_t Time_PeriodicTimerCheck = 0x0022eba4;

    // Constructor for the SetDateTimeBg UI screen - the in-game date/time settings screen. Sets up UI components: input fields
    inline constexpr uintptr_t Time_SetdatetimebgConstructor = 0x00244884;

    // Loads 80-byte (0x50) physics and behavior record from table 0x0086773C for insect species (0..71).
    inline constexpr uintptr_t Insect_AcInsectFieldBase_LoadSpeciesParams = 0x0024f314;

    // Queries cumulative probability table for river/ocean based on current time/date and spawns fish species.
    inline constexpr uintptr_t Fish_BsFishFieldMgr_SelectAndSpawnFish = 0x002526a0;

    // Enforces 8-fish maximum concurrency in town (river + ocean < 8) and ticks spawn timer.
    inline constexpr uintptr_t Fish_BsFishFieldMgr_TickSpawns = 0x00253a08;

    // Packs 6-byte state into one of 8 active fish slots ('T'..'['), packing 10-bit world X/Z coordinates.
    inline constexpr uintptr_t Fish_BsFishFieldMgr_AllocateFishSlot = 0x00253f54;

    // Main update tick for fish manager: processes active fish pool and checks player bobber proximity.
    inline constexpr uintptr_t Fish_BsFishFieldMgr_UpdateFishLoop = 0x002554a4;

    // Updates confetti/paper particle simulation state
    inline constexpr uintptr_t Weather_UpdatePaperParticles = 0x002663f0;

    // Calculates monthly interest on ABD savings (0.5%, max 99,999 Bells), deposits interest to +0x6B8C, sends Mail_SP_Postoffice letter, and awards 8 tiers of savings milestone items (100k to 100M Bells).
    inline constexpr uintptr_t PostOffice_UpdateMonthlyInterestAndSavingsRewards = 0x0062F7D0;

    // Decrypts obfuscated ABD savings balance from 8-byte player struct (+0x6B8C). Validates checksum with 0xBA, applies ROL32(28 - shift), and subtracts (key + 0x8F187432).
    inline constexpr uintptr_t Player_Abd_GetBalance = 0x00303700;

    // Encrypts ABD bank balance into 8-byte player struct (+0x6B8C). Generates random 16-bit key, random shift (0..25), applies ROL32(shift + 4), and writes checksum with constant 0xBA.
    inline constexpr uintptr_t Player_Abd_SetBalance = 0x003035C4;

    // Adds deposit amount to ABD savings balance with overflow check, clamping to max limit (999,999,999 Bells).
    inline constexpr uintptr_t Player_Abd_DepositClamped = 0x00612CD4;

    // Calls Player_Abd_DepositClamped on player account (+0x6B8C) with hardcoded max limit of 999,999,999 Bells.
    inline constexpr uintptr_t Player_Abd_AddInterest = 0x00305AD8;

    // Constructs 640-byte (0x280) dSvMail structure: Recipient (66B @ +0x68), Message (386B @ +0xAA), Sender (66B @ +0x22C), Stationery (+0x26E), Flags (+0x26F), Present item (4B @ +0x274), Timestamp (+0x278).
    inline constexpr uintptr_t Save_Mail_ConstructLetter640B = 0x002FF304;

    // Initializes player mail data: 10 pocket letter slots (10x0x280 = 0x1900 bytes), 1 incoming/work buffer letter (0x280 bytes @ +0x1900), and tail struct (+0x1B80). Total 0x1B88 bytes per player.
    inline constexpr uintptr_t Save_Player_InitMailPockets = 0x006F4234;

    // Creates special system letter from template (e.g. Mail_SP_Postoffice) with recipient, stationery, item attachment, and queue flags.
    inline constexpr uintptr_t Mail_CreateSpecialLetter = 0x005CB34C;

} // namespace acnl::symbols

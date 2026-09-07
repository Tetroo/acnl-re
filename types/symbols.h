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

    // Iterates across all 20 playable town acres (5x4 grid, X:1..5, Y:1..4), advancing tree stages and flora.
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

    // Processes CTR bottom touch screen drag-and-drop, fruit stacking, and context menus for 16 pocket slots.
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

    // Timer/tick-based check function. Uses SVC 0x28 (GetSystemTick) and divides by 0x1a/0x5a constants. Calls FUN00300fdc onl
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

    // Master town generation driver. Loads sea_side templates from RomFS and loops Rover generator until 4 valid layouts are found.
    inline constexpr uintptr_t Town_GenerateTownOptions_Main = 0x0027c2cc;

    // Async resource loader: loads Bg/Outdoor/Textures/GrassBaldness.bcres and then kicks off async loads of TemplateData/vill
    inline constexpr uintptr_t Town_LoadVillageTemplateResources = 0x0027c56c;

    // Generates candidate town seed (sead::GlobalRandom). Validates exactly 5 villagers and 1 plaza/hall, stores 4 options.
    inline constexpr uintptr_t Town_GenerateTownLayout_4Options = 0x0027c8fc;

    // State machine dispatcher with a save write path. At case 1, calls FUN00300fdc to access the save buffer - likely the wri
    inline constexpr uintptr_t Save_DispatchSaveWriteStateMachine = 0x0028de1c;

    // Updates cherry blossom sakura particle simulation
    inline constexpr uintptr_t Weather_UpdateSakuraParticles = 0x002960d0;

    // Checks rain and snow weather flags; automatically despawns flying ('A') and ground ('F'/'G') insects.
    inline constexpr uintptr_t Insect_BsInsectFieldMgr_CheckWeatherDespawn = 0x002abfe0;

    // Main update tick for insect manager: manages 12 concurrent slots ('H'..'S') and updates active bug AI.
    inline constexpr uintptr_t Insect_BsInsectFieldMgr_UpdateBugLoop = 0x002acb7c;

    // State machine (states 0-10) that loads and initialises the Town Map menu screen (Nook's Homes / Town Hall bulletin board
    inline constexpr uintptr_t Town_InitBsMenuMapVillage = 0x002b32f4;

    // Looks up the single named layout pane "Pmp00" within the loaded mapvillage.arc layout (state 8 of FUN002b32f4's init sta
    inline constexpr uintptr_t Town_LookupTownMapPane = 0x002b385c;

    // Evaluates player bug net swing collision sphere against insect collision radius.
    inline constexpr uintptr_t Insect_AcInsectFieldFly_CheckNetCatchOverlap = 0x002c28b4;

    // Adjusts flying bug flight altitude relative to sampled terrain elevation and flower attachment.
    inline constexpr uintptr_t Insect_AcInsectFieldFly_UpdateAltitude = 0x002c2d94;

    // Larger sibling of FUN0027c56c: same grass-baldness/village-template setup, then loops 18 times (uVar15 <= 0x11, i.e. ind
    inline constexpr uintptr_t Town_VillageIslandTemplateResourceLoader = 0x002ca040;

    // Thin wrapper. Reads /exhibition.dat into the buffer pointed to by param1.
    inline constexpr uintptr_t Save_ReadExhibitionFile = 0x002cd508;

    // C++ placement constructor for a POD (no vtable) sub-object inside the FUN005ca4f4 save-buffer construction chain. Called
    inline constexpr uintptr_t Town_ConstructFieldTownObject = 0x002d457c;

    // Handles state transitions for a game object. Checks active state via vtable calls, updates state tracking fields at offs
    inline constexpr uintptr_t Time_HandleStateTransition = 0x002f72bc;

    // Flushes a pending state transition in a state slot object. If a transition is pending (+4 != 0), processes it and resets
    inline constexpr uintptr_t Core_FlushPendingStateTransition = 0x002f744c;

    // Releases an object stored in a slot by calling its destructor via vtable +0x10, then nulls the pointer. Identical patter
    inline constexpr uintptr_t Core_ReleaseVtableObject = 0x002f7510;

    // Checks and clears dirty flag pair in state machine
    inline constexpr uintptr_t Core_DirtyFlagCheckAndClear = 0x002f753c;

    // Returns the current state index of a global game object. This is a simple getter - no logic, no RNG. The byte at offset
    inline constexpr uintptr_t Core_GetStateIndex = 0x002f755c;

    // Checks whether specific bits (defined by mask) are set in a static flag table, at position index. Returns 1 if the flags
    inline constexpr uintptr_t Core_CheckStateFlag = 0x002f7574;

    // Generic backward iterator that calls a function pointer on each element from last to first. Used as the destructor patte
    inline constexpr uintptr_t Core_DestructorBackwardIterator = 0x002f7740;

    // c int FUN002faec0(void) { FUN00606c24(); FUN006064bc(); return DAT002faedc + 0x80; }
    inline constexpr uintptr_t Core_ErrorAbortHandler = 0x002faec0;

    // c undefined4 FUN002fb34c(void) { return DAT002FB358; }
    inline constexpr uintptr_t Save_GetSaveBufferPointer = 0x002fb34c;

    // Returns the global time object pointer stored at DAT002FB3A4. This is a singleton getter - the actual object is initiali
    inline constexpr uintptr_t Time_GetTimeSingletonPrimary = 0x002fb39c;

    // c int FUN002fb860(void) { FUN00606c24(); FUN006064bc(); // pre-check, same as FUN00300fdc pattern return DAT002fb880 + 0
    inline constexpr uintptr_t Core_GetSmallMapStateObject = 0x002fb860;

    // Core heap allocator operator new(size, heap, align) delegating to sead::Heap::alloc.
    inline constexpr uintptr_t Core_HeapAllocate = 0x002fd108;

    // Converts two time objects to 64-bit millisecond timestamps via FUN001296BC, subtracts them, and divides by a period leng
    inline constexpr uintptr_t Time_CalculateSignedPeriodDelta = 0x002fd4a0;

    // Zero-initialises and constructs a 640-byte (0x280) save sub-struct. Called 10+1 times per player region by FUN006f4234.
    inline constexpr uintptr_t Save_ConstructPlayerSubStruct640B = 0x002ff304;

    // Scans 16x16 tile grid of an acre: advances sapling stages 0..3, wilts unwatered flowers, breeds hybrids.
    inline constexpr uintptr_t Town_ProcessFloraTileGrid = 0x002ffd1c;

    // Second getter for the gardenplus.dat runtime buffer. Calls FUN006064bc as a pre-flight check before returning the buffer
    inline constexpr uintptr_t Save_GetSaveBufferWithPreCheck = 0x00300fdc;

    // Called from FUN005c9a24 (villager slot reader) as the per-slot gate that decides whether a villager slot is occupied/val
    inline constexpr uintptr_t Town_CheckVillagerSlotValidity = 0x00301ebc;

    // Initialises a different-typed object at offset +0x1b80 within the player region. Called once per player by FUN006f4234 a
    inline constexpr uintptr_t Player_ConstructPlayerTailObject = 0x00303d80;

    // Thunk redirecting to Save_AllocateAndConstructSaveBuffer (0x005c9dd4)
    inline constexpr uintptr_t Save_Thunk_AllocateAndConstructSaveBuffer = 0x00304f70;

    // Reads a single byte from the main game state object at a fixed offset. Used as a condition gate in FUN0062eee8 - if resu
    inline constexpr uintptr_t Core_ReadGameStateFlag = 0x0030601c;

    // Normalizes a date object in-place. Called by FUN0062EF1C on both the input date and the current time before comparison.
    inline constexpr uintptr_t Time_NormalizeDate = 0x00308f5c;

    // Thunk redirecting to weather type calculation
    inline constexpr uintptr_t Weather_Thunk_ResolveWeatherType_Alt = 0x0030c2c8;

    // Adds signed offsets (year/month/day/hour) to a date struct in-place. Uses 64-bit arithmetic internally for overflow hand
    inline constexpr uintptr_t Time_DateOffsetCalculator = 0x00312a5c;

    // The "normal path" period calculator. Copies the input date, shifts it back by 6 months via FUN00312a5c, then falls throu
    inline constexpr uintptr_t Time_ShiftDateMinus6Months = 0x0031872c;

    // Takes (buffer_ptr, size, initial_crc=0xffffffff). Used across all save-buffer integrity checks.
    inline constexpr uintptr_t Save_CalculateCrc32_SaveBuffer = 0x00319340;

    // Destructor for wasp / bee swarm pursuer entity spawned from shaken trees.
    inline constexpr uintptr_t Insect_AcInsectFieldFlyPursue_Dtor = 0x0032c96c;

    // Processes a buffer using a 256-entry XOR lookup table. Applied to gardenplus.dat after raw read, before parsing. Returns
    inline constexpr uintptr_t Save_DecryptSaveXor = 0x0047dac0;

    // Periodically updates the global time singleton object at DAT0056a928 → 0x00AD46B0. This is the function that keeps the i
    inline constexpr uintptr_t Time_PeriodicTimeUpdater = 0x0056a74c;

    // Reads 3DS CTR hardware RTC ticks, combines with user time-travel offset, and converts to 9-byte GameDateTime calendar format.
    inline constexpr uintptr_t Time_ConvertHardwareRtcToGameDate = 0x0056aa90;

    // Sets the game epoch timestamp to January 1st, 2012 (0x7DC, 1, 1) using Julian day subfield calculator.
    inline constexpr uintptr_t Time_GetEpochDate2012 = 0x0056abe0;

    // Returns a period index (0-22) representing where the current date falls within the annual season calendar. Used by FUN00
    inline constexpr uintptr_t Time_GetSeasonPeriodIndex = 0x0056ac80;

    // This function is the direct source of the 2050 date limit.
    inline constexpr uintptr_t Time_NormalizeYearRange_2050Limit = 0x0056bbd4;

    // Evaluates calendar day progression by checking hour - 6. If hour < 6 AM, day belongs to previous date; triggers daily reset at 06:00:00.
    inline constexpr uintptr_t Time_Check6AmDailyRolloverBoundary = 0x0056bfd8;

    // Applies manual clock adjustment from title screen settings, updating user offset variable and clamping calendar range between 2012 and 2050.
    inline constexpr uintptr_t Time_SetPlayerTimeTravelOffset = 0x0056c288;

    // Reads 9-byte timestamp record at offset 0x55A8 in garden_plus.dat and checks bit 0 of 0x55BA for Daylight Saving / Summer Time flag.
    inline constexpr uintptr_t Save_ReadClockHeaderRecord = 0x0056c344;

    // Converts a raw state index (from FUN002F755C) into a weather type integer used by the record index formula in FUN001E63D
    inline constexpr uintptr_t Weather_MapStateIndexToWeatherType = 0x005b2fe0;

    // Increments a 64-bit counter (DAT005c96dc as {uint32 lo; uint32 hi;}) by 1, with special-case logic to skip the value 0 o
    inline constexpr uintptr_t Core_IncrementNonZero64BitCounter = 0x005c96a0;

    // c void FUN005c9760(void) { puVar1 = DAT005c97b4; // struct ptr if (DAT005c97b4[1] == 0) { // lazy-init check: not yet lo
    inline constexpr uintptr_t Save_LoadLazySaveResource = 0x005c9760;

    // Iterates over all 10 villager slots in the save buffer. For each slot, reads a uint16 field at offset +0x2c and writes i
    inline constexpr uintptr_t Town_ReadVillagerSlots = 0x005c9a24;

    // Trivial getter. Returns the current runtime pointer to the gardenplus.dat in-memory buffer.
    inline constexpr uintptr_t Save_GetGardenPlusBufferPointer = 0x005c9d64;

    // Allocates the runtime save buffer, constructs C++ objects in-place, then initializes their fields. Paired with FUN0012cc
    inline constexpr uintptr_t Save_AllocateAndConstructSaveBuffer = 0x005c9dd4;

    // Resets TownAcres, playable grids, villager array, and player slots to default values.
    inline constexpr uintptr_t Town_ResetSaveBuffer_AllSubObjects = 0x005ca2c4;

    // The primary C++ object constructor for the gardenplus.dat buffer. Called from FUN005c9dd4 with param1 = buffer + 0x80. C
    inline constexpr uintptr_t Save_SaveObjectMasterConstructor = 0x005ca4f4;

    // Initialises the C++ object region immediately after the 167,936-byte raw data block in the save buffer. Called from FUN0
    inline constexpr uintptr_t Save_SaveBufferSubRegionConstructor = 0x005ccdc8;

    // Called by FUN00300fdc immediately before returning the save buffer pointer. Purpose unknown - likely an assertion or val
    inline constexpr uintptr_t Save_PreCheckSaveBuffer = 0x006064bc;

    // Performs rendering or UI operations using a sub-object at param1 + 0x18. Calls FUN00300fdc but discards the return value
    inline constexpr uintptr_t Core_UiRenderingFunction = 0x006079b8;

    // Sets uint16 acre ID to 0x109. Called for all 42 entries in the 7x6 TownAcres grid.
    inline constexpr uintptr_t Town_SetDefaultAcreId_109 = 0x00611d34;

    // SOLVED TOWN ACRES RUNTIME HANDLER. Initializes 7x6 TownAcres (0x53484) and 5x4 playable acres.
    inline constexpr uintptr_t Town_InitTownGridAndAcres = 0x00612820;

    // Returns a pointer to the global weather data source object. The object contains weather pattern records of size 0x8C (14
    inline constexpr uintptr_t Weather_GetWeatherPatternData = 0x0061494c;

    // Four-instruction thunk. Fetches the time singleton and returns. The actual weather type resolver is FUN0062E094 - identi
    inline constexpr uintptr_t Weather_Thunk_ResolveWeatherType = 0x0062e084;

    // Resolves weather type byte from a 52x24 lookup table indexed by period index (0-51) and normalised hour (0-23). Called t
    inline constexpr uintptr_t Weather_ResolveWeatherType_52x24 = 0x0062e094;

    // Returns base precipitation particle density float from 7-entry LUT for weather type (0-6).
    inline constexpr uintptr_t Weather_GetPrecipitationRateForType = 0x0062ebc4;

    // Returns base wind speed float from 7-entry LUT for weather type (0-6).
    inline constexpr uintptr_t Weather_GetWindSpeedForType = 0x0062ebe0;

    // Computes the current weather period index from a date object and stores it in the singleton DAT002FB358. Always terminat
    inline constexpr uintptr_t Weather_ResolvePeriodIndexFromDate = 0x0062eee8;

    // Given a date object (param1), resolves and returns a period index used by the weather table lookup system (FUN0062E094).
    inline constexpr uintptr_t Weather_ResolveWeatherPeriodIndex = 0x0062ef1c;

    // Resolves weather row for Tortimer Island using 3-day cyclic table [4, 21, 33] indexed by JulianDay % 3.
    inline constexpr uintptr_t Weather_ResolveIslandWeatherWeekIndex = 0x0062efd4;

    // Renders player character model, applies texture swaps for facial expressions, and projects dynamic circular shadow with radius scaled by 13.0f.
    inline constexpr uintptr_t Player_AcPlayer_Draw = 0x0068b9bc;

    // Snaps player vertical elevation to town heightmap resolves collision against obstacles and updates physics
    inline constexpr uintptr_t Player_UpdateMovementPhysics = 0x0068C0EC;

    // Manager tick iterating over town's 10 villager slots: updates wander pathfinding, home/outdoor presence, and interaction state.
    inline constexpr uintptr_t Npc_BsNpcMgr_UpdateVillagers = 0x006a0c58;

    // Dispatches .cro dynamic modules based on current game state flags. Called during scene transitions. Reads state via FUN0
    inline constexpr uintptr_t Core_DispatchCroModule = 0x006a3578;

    // Initializes the active field/world map for current state transition, invoking Town_FieldFactory_CreateBuilderForState and setting active field pointers.
    inline constexpr uintptr_t Town_InitializeCurrentStateField = 0x006a506c;

    // Queries terrain tile collision type against soil bitmask DAT_0095cd1a to test if trees/flowers can grow.
    inline constexpr uintptr_t Town_CheckTileAllowsFloraGrowth = 0x006c478c;

    // Simple one-liner getter. Returns a context object used to gate state machine transitions.
    inline constexpr uintptr_t Core_GetStateTransitionContext = 0x006d0eac;

    // Reads fields from a large object at param1 (offsets 0x2768-0x27c6) and interacts with network-layer functions. Calls FUN
    inline constexpr uintptr_t Core_NetworkSessionObjectHandler = 0x006e1d70;

    // Initializes 16x16 tile grid (0x400 bytes) for a playable acre. Stride row*0x40 + col*4 (4B/tile)
    inline constexpr uintptr_t Town_InitPlayableAcreTileGrid = 0x006eed70;

    // Initialises the array of 10+1 sub-structs within a player's save region. Called once per player in FUN005c9dd4's 4-playe
    inline constexpr uintptr_t Save_PlayerRegionSubObjectInitializer = 0x006f4234;

    // Returns a state-specific sub-object if bit 0x4000 is set for the given state index. Used in FUN005B2FE0 to handle weathe
    inline constexpr uintptr_t Core_GetStateSubObjectFlag4000 = 0x006f7c54;

    // > [!WARNING] Discarded > "Player/Town Name String Reader" (Session 22c) - the 0x105-byte buffers are real, but they're U
    inline constexpr uintptr_t Town_UiStringBuilderForSaveFileLabels = 0x0070d894;

    // Calls FUN00300fdc (result unused) then immediately calls the non-returning FUN002faec0. This is an error/abort path, not
    inline constexpr uintptr_t Save_SaveBufferErrorAbort = 0x0071ee44;

    // Reads a single byte from a struct array: field +0x36 of element param2. Used to check "level" values for state sub-objec
    inline constexpr uintptr_t Core_ReadStructArrayLevelField = 0x0071f3ec;

    // Calls CalculateSubObjectChecksums across all save regions, then updates small map state.
    inline constexpr uintptr_t Save_SerializeSaveBuffer_CalculateChecksums = 0x00756c34;

    // Calls ValidateSubObjectChecksums to verify CRC32 of all save sub-objects on load.
    inline constexpr uintptr_t Save_DeserializeSaveBuffer_ValidateChecksums = 0x00756c54;

    // Calculates CRC32 over 0x1E4D8 bytes of Field/Town data and writes it to buffer + 0x53424.
    inline constexpr uintptr_t Town_CalculateSubObjectChecksums = 0x00756f20;

    // Verifies that stored CRC32 at buffer + 0x53424 matches recalculated CRC32 over 0x1E4D8 bytes.
    inline constexpr uintptr_t Save_ValidateSubObjectChecksums = 0x00756f90;

    // Verifies the CRC32 checksum of a save-file buffer. Used after loading design/mail/friend files (states 0x06-0x0C in FUN0
    inline constexpr uintptr_t Save_VerifySaveFileCrc32 = 0x0076dfac;

    // Fetches APT service handle for App module
    inline constexpr uintptr_t Core_GetAppServicePointer = 0x007b0ba0;

    // Resolves hardware region (Japan=0 USA=1 Europe=2) and language to return game language index (0..9) for MSBT localization
    inline constexpr uintptr_t System_GetLanguageIndex = 0x005D3734;

    // Initializes MSBT (Message Studio Binary Text) parser validating MsgStdBn magic BOM and mapping LBL1 TXT2 ATR1 ATO1 TSY1 sections
    inline constexpr uintptr_t Msg_MsbtFile_Init = 0x00159A30;

    // Initializes MSBF (Message Studio Binary Flow) parser validating MsgFlwBn magic BOM and mapping FLW3 FEN1 REF1 sections
    inline constexpr uintptr_t Msg_MsbfFile_Init = 0x001599A8;

    // Initializes MSBP (Message Studio Binary Project) parser validating MsgPrjBn magic and resolving CLR1 CLB1 CTI1 sections
    inline constexpr uintptr_t Msg_MsbpFile_Init = 0x00159AF0;

    // Parses 32-byte MSB header validates BOM 0xFEFF and loads 16-byte aligned section descriptors
    inline constexpr uintptr_t Msg_Header_ParseSections = 0x00159C74;

    // Searches section table by 4-byte FourCC tag (LBL1 TXT2 FLW3 FEN1) returning section index or 0xFFFFFFFF
    inline constexpr uintptr_t Msg_FindSectionByTag = 0x00159F08;

    // Returns bounds-checked pointer to raw data within an MSB section given relative byte offset
    inline constexpr uintptr_t Msg_GetSectionDataPtr = 0x00159718;

    // Computes multiplicative hash (accumulator * 1170 + char) modulo bucket count for LBL1 label lookup
    inline constexpr uintptr_t Msbt_CalculateLabelHash = 0x00159DE4;

    // Searches LBL1 label hash bucket for matching key string and returns 32-bit string index or 0xFFFFFFFF
    inline constexpr uintptr_t Msbt_FindStringIndexByLabel = 0x001598AC;

    // Retrieves pointer to UTF-16LE text string from TXT2 section by 0-based string index
    inline constexpr uintptr_t Msbt_GetStringByIndex = 0x00159804;

    // Convenience lookup combining label hash search and TXT2 pointer extraction returning UTF-16 string
    inline constexpr uintptr_t Msbt_GetStringByLabel = 0x00159848;

    // Performs reverse lookup in LBL1 table finding label key string corresponding to given string index
    inline constexpr uintptr_t Msbt_GetLabelNameByIndex = 0x00159750;

    // Allocates wrapper descriptor and binds MSBT binary buffer pointer
    inline constexpr uintptr_t Msg_MsbtWrapper_Load = 0x005B07B8;

    // Allocates wrapper descriptor and binds MSBF binary buffer pointer
    inline constexpr uintptr_t Msg_MsbfWrapper_Load = 0x005B0854;

    // Releases MSBF wrapper descriptor and clears buffer references
    inline constexpr uintptr_t Msg_MsbfWrapper_Destroy = 0x005B0870;

    // Zeroes out internal wrapper buffer and state fields
    inline constexpr uintptr_t Msg_MsbfWrapper_Clear = 0x005B0898;

    // Loads software keyboard localized text swkbd.msbt and style RI.mstl based on active language index
    inline constexpr uintptr_t Msg_LoadSwkbdMsbt = 0x00523378;

    // Looks up entry point name in FEN1 hash table and returns root node index for flowchart execution
    inline constexpr uintptr_t Msbf_FindEntryPointNode = 0x00159634;

    // Executes Type 2 action node in MSBF flowchart invoking receiver hook and advancing next node
    inline constexpr uintptr_t FlowSeq_ExecuteActionNode = 0x005F305C;

    // Evaluates boolean condition in MSBF branch node and transitions to appropriate destination node
    inline constexpr uintptr_t FlowSeq_StepBranchNode = 0x005F2A4C;

    // Evaluates multi-way choice table in MSBF flow resolving player dialogue choice index to target node
    inline constexpr uintptr_t FlowSeq_StepMultiBranchNode = 0x005F2AFC;

    // Advances sequential MSBF flowchart node index or flags flow sequence completion
    inline constexpr uintptr_t FlowSeq_StepToNextNode = 0x005F29A4;

    // Unpacks Type 1 message node binds dialogue string and opens villager text box with emotion animation
    inline constexpr uintptr_t FlowSeq_ExecuteMessageNode = 0x005F2614;

    // Resets active dialogue state machine clears string buffers and closes speech window
    inline constexpr uintptr_t TalkEngine_ResetDialogueBuffer = 0x00600EE4;

    // Loads localized UMSBT file for active villager personality selects language stream and binds to talk state
    inline constexpr uintptr_t TalkEngine_LoadActiveDialogue = 0x00601664;

    // Deducts purchase amount from remaining upgrade quota and evaluates spending and days-open thresholds from 0x0088E5E4 and 0x0088E5F8 to advance store tier
    inline constexpr uintptr_t Shop_AddSpendingAndCheckUpgrade = 0x001E5218;

    // Records store spending transaction updates save buffer and triggers town persistence
    inline constexpr uintptr_t Shop_RecordPurchase = 0x001E5494;

    // Applies store upgrade sets new tier index resets renovation timer and reallocates daily inventory slots
    inline constexpr uintptr_t Shop_ApplyTierUpgrade = 0x001E4EC8;

    // Formats and posts store renovation and renewal announcement on town bulletin board using BBS_Kodanuki strings
    inline constexpr uintptr_t Shop_PostRenewalBbsNotice = 0x001E5628;

    // Calculates target timestamp for 9:00 AM morning and 12:00 PM afternoon turnip price shifts
    inline constexpr uintptr_t Shop_Turnip_CalculateNextPriceTime = 0x001E42BC;

    // Checks if current in-game time has crossed turnip price period boundary and triggers price re-evaluation
    inline constexpr uintptr_t Shop_Turnip_CheckPriceExpiry = 0x001E423C;

    // Copies 640-byte (0x280) catalog record including 386-byte unlocked item bitmask and recent orders
    inline constexpr uintptr_t Shop_Catalog_CopyPlayerRecord = 0x001E4370;

    // Scans and compacts active player catalog records across up to 80 storage slots in save file
    inline constexpr uintptr_t Shop_Catalog_CompactPlayerRecords = 0x001E4454;

    // Initializes daily floor display items and marks purchased items with sold out sign 0x2083
    inline constexpr uintptr_t Shop_InitDailyStock = 0x001E4718;

    // Processes floor item purchase decrements daily stock count and places 0x2083 or 0x2086 sold out marker
    inline constexpr uintptr_t Shop_BuyDisplayItem = 0x001E4CF4;

    // Selects daily premium double-price item for Re-Tail and binds STR_Recycle_High to exterior chalkboard
    inline constexpr uintptr_t RecycleShop_InitDailyPremiumItem = 0x0056C548;

    // Initializes the 8 interactive flea market selling tables and prices inside Re-Tail
    inline constexpr uintptr_t RecycleShop_InitFleaMarketStalls = 0x0056C634;

    // Initializes StageOpusMgr singleton binds audio dispatch channels and sets initial volume state
    inline constexpr uintptr_t Audio_Bgm_StageOpusMgr_Init = 0x005831A8;

    // Crossfades and switches active stage BGM stream based on target location and stage ID
    inline constexpr uintptr_t Audio_Bgm_SwitchStageBgm = 0x005832F8;

    // Maps game location flags to specific StageOpus BGM classes (Town, Tour, Island, Office, Club LOL)
    inline constexpr uintptr_t Audio_Bgm_ResolveStageId = 0x0058392C;

    // Computes minute 59 countdown frames for smooth crossfade transition into top-of-hour theme
    inline constexpr uintptr_t Audio_Bgm_CalculateCrossfadeFrames = 0x00588BEC;

    // Sets target volume and fade rate on active BGM voice channel
    inline constexpr uintptr_t Audio_Bgm_SetTrackVolumeAndFade = 0x00589F40;

    // Initializes river positional sound emitter and allocates audio stream
    inline constexpr uintptr_t SoundObjRiver_Init = 0x00246938;

    // Culls river audio beyond 300 units and tracks nearest orthogonal point on river spline for 3D panning
    inline constexpr uintptr_t SoundObjRiver_UpdatePositionAndPanning = 0x002469C8;

    // Halts river audio stream and releases voice handle
    inline constexpr uintptr_t SoundObjRiver_StopAndRelease = 0x00246B94;

    // Initializes waterfall sound emitter with SFX ID 0x01000786
    inline constexpr uintptr_t SoundObjFall_Init = 0x0020AFE0;

    // Updates 3D positional panning and distance attenuation for waterfall roar 0x01000786
    inline constexpr uintptr_t SoundObjFall_Update = 0x0020B0FC;

    // Binds NPC voice emitter to actor transform and initializes phonetic playback state
    inline constexpr uintptr_t SoundObjNpc_Init = 0x001D9CC4;

    // Synthesizes Animalese speech modulating sound effect 0x01000565 across 13 chromatic pitch semitones
    inline constexpr uintptr_t SoundObjNpc_UpdateVoiceSynthesis = 0x001D9E6C;

    // Initializes AcPlayer actor instance transform model data and locomotion state machine
    inline constexpr uintptr_t Player_Init = 0x006E3724;

    // Caches previous frame 3D coordinates and facing rotation for interpolation and movement delta calculations
    inline constexpr uintptr_t Player_UpdatePreviousCoords = 0x0064BC6C;

    // Evaluates locomotion state machine transitioning between walk run net sneak swim and dive states
    inline constexpr uintptr_t Player_UpdateStateMachine = 0x006540B8;

    // Rotates velocity vector by player facing angle and applies 3D displacement to position
    inline constexpr uintptr_t Player_ApplyVelocityAndRotation = 0x0068C1FC;

    // Tests equipped tool target flags against interaction candidate resolving shovel dig fish water or bug net catch action
    inline constexpr uintptr_t Player_ResolveToolActionTarget = 0x0064BE70;

    // Dispatches resolved tool action animation and sound effect trigger
    inline constexpr uintptr_t Player_ToolFunctor_ExecuteAction = 0x00690800;

    // Computes player world transform matrix and enqueues matrix to the active scene draw list
    inline constexpr uintptr_t Player_BuildWorldMatrix = 0x0064F26C;

    // Calculates active rain particle count (32 for light, 128 for storm) with 30s linear crossfade
    inline constexpr uintptr_t Weather_BsWeatherRain_UpdateParticles = 0x00229D10;

    // Calculates active snow particle count (100 for light, 400 for blizzard) with 30s linear crossfade
    inline constexpr uintptr_t Weather_BsWeatherSnow_UpdateParticles = 0x0022AFA0;

    // Initializes BsThunderMgr actor (size 0x64, vtable 0x008EC120)
    inline constexpr uintptr_t Weather_BsThunderMgr_Init = 0x001FE9F0;

    // Computes 60-second deterministic lightning strike bitmap using calendar PRNG
    inline constexpr uintptr_t Weather_BsThunderMgr_GenerateMinuteStrikes = 0x001FE740;

    // Evaluates current second against bitmap, flashes screen, and plays indoor/outdoor thunder SE
    inline constexpr uintptr_t Weather_BsThunderMgr_TickLightning = 0x001FE8DC;

    // Checks if current weather is clear and time is between 19:00 and 04:00 for shooting stars
    inline constexpr uintptr_t Weather_CheckMeteorShowerActiveHours = 0x0062F4F4;

    // Constructs BsShootingStarMgr actor (size 0x3A4, vtable 0x008F1410)
    inline constexpr uintptr_t Weather_BsShootingStarMgr_ctor = 0x002C9E74;

    // Initializes 4 star streak slots and positional audio emitters
    inline constexpr uintptr_t Weather_BsShootingStarMgr_Init = 0x002C9964;

    // Updates active star streaks, audio chime 0x01000570, and lifetime timers
    inline constexpr uintptr_t Weather_BsShootingStarMgr_Update = 0x002C9AAC;

    // Updates directional transform matrix for shooting star particle emitters
    inline constexpr uintptr_t Weather_BsShootingStarMgr_UpdateStarMatrix = 0x002C965C;

    // Checks visible star arc during player wish, emits sparkles, and plays 0x01000571
    inline constexpr uintptr_t Weather_BsShootingStarMgr_TryWish = 0x002C9850;

    // Triggers star wish attempt when player presses A without tools looking at sky
    inline constexpr uintptr_t Player_PerformStarWish = 0x00680E00;

    // Central actor engine registration initializing 669 actors, meta array 0x00952FB0, factory array 0x00954DA4
    inline constexpr uintptr_t Actor_RegisterAllActors = 0x001D3594;

    // Fetches 12-byte metadata entry for given actor index (bounds-checked against 669)
    inline constexpr uintptr_t Actor_GetActorMetaByIndex = 0x0051FD40;

    // Invokes 8-byte entry factory/constructor method from 0x00954DA4 for given actor index
    inline constexpr uintptr_t Actor_InvokeFactoryByIndex = 0x0051FD74;

    // Initializes the 142,284-byte villager save region (10 resident slots + newcomer moving queue)
    inline constexpr uintptr_t Save_InitVillagersBlock = 0x0011253C;

    // Initializes an individual 9,496-byte villager save record (furniture, letters, 4 player relation records)
    inline constexpr uintptr_t Save_InitSingleVillagerRecord = 0x00308D08;

    // Finds first unoccupied town resident slot (0..9) and migrates newcomer data from queue
    inline constexpr uintptr_t Villager_MoveIn_AssignPlayerSlot = 0x006F3F78;

    // Checks if relationship state bits [2:0] equal 3 (Best Friend tier, picture reward eligible)
    inline constexpr uintptr_t Villager_CheckIsBestFriend = 0x007570D0;

    // Checks bit 5 of relationship state byte to enforce once-per-day friendship conversation reward
    inline constexpr uintptr_t Villager_CheckSpokenToday = 0x00757108;

    // Checks if relationship state bits [2:0] equal 0 (unassigned / empty resident slot)
    inline constexpr uintptr_t Villager_CheckSlotEmpty = 0x0075711C;

    // Sets bits [4:3] of relationship state byte during item/present trades and records item ID
    inline constexpr uintptr_t Villager_SetGiftExchangeStatus = 0x00767150;

    // Constructs AcNpcNml animal villager actor, binds vtable 0x008F7304 and initializes animation components
    inline constexpr uintptr_t Npc_AcNpcNml_ctor = 0x0064B410;

    // Constructs AcNpc base actor, initializes AI state machine and audio receivers
    inline constexpr uintptr_t Npc_AcNpc_ctor = 0x0057A4F0;

    // Chebyshev metric candidate tile distance validator for villager wandering pathfinding
    inline constexpr uintptr_t Npc_AcNpc_WanderSearchCandCB_Execute = 0x005781D0;

    // Constructs central BsNpcMgr manager (max 10 villagers, up to 14 on Main Street, 4x4 acre boundary clamping)
    inline constexpr uintptr_t Npc_BsNpcMgr_ctor = 0x006A0D60;

    // Initializes 18,508-byte PWP save region (4 PWPSaveBlock instances)
    inline constexpr uintptr_t Save_InitPublicWorksBlock = 0x002EBD84;

    // Initializes 6 PWP construction slots, zeroes built count, and sets max project limit (0x1E = 30)
    inline constexpr uintptr_t PWP_InitSaveBlock = 0x0024DBC4;

    // Initializes single 770-byte project slot with 10x10 collision grid and 8x8 placement grid
    inline constexpr uintptr_t PWP_InitProjectSlot = 0x002D4F24;

    // Adds newly suggested project ID to player 40-byte unlocked project buffer if not duplicate
    inline constexpr uintptr_t PWP_UnlockProject = 0x0024DD34;

    // Marks PWP active, stores 0x2E placement metadata, registers world collision, and updates state
    inline constexpr uintptr_t PWP_ValidateAndPlaceProject = 0x0024DF60;

    // Evaluates Lloid gyroid Bell donations against project total cost and triggers completion
    inline constexpr uintptr_t PWP_CheckDonationComplete = 0x0024E0D8;

    // Constructs Isabelle PWP celebration ceremony actor with Mayor and 3 villager attendee slots
    inline constexpr uintptr_t AcNpcSpSecretaryCeremony_ctor = 0x00576484;

    // Constructs BsInsectFieldMgr with 12 normal slots (0x144 each) and 2 special event slots (wasps/ants)
    inline constexpr uintptr_t Item_BsInsectFieldMgr_ctor = 0x002AD780;

    // Ticks 12 standard insect slots and 2 special event slots, dynamically binding 3D .bcres models
    inline constexpr uintptr_t Item_BsInsectFieldMgr_Update = 0x002ADCC8;

    // Constructs BsFishFieldMgr with 8 active swimming fish slots (0x18 each) and 15 sub-manager slots
    inline constexpr uintptr_t Item_BsFishFieldMgr_ctor = 0x0025597C;

    // Ticks active swimming fish shadow slots in river and ocean, updating spatial lifetime ticks
    inline constexpr uintptr_t Item_BsFishFieldMgr_Update = 0x00255500;

    // Validates specimen category (0..5) and category-relative index bounds (fossil:67, fish:72, diving:30, insect:72, art_p:25, art_s:8)
    inline constexpr uintptr_t Museum_ValidateSpecimenIndex = 0x0076C72C;

    // Returns donation flag byte (0=uncollected, 1..4=donated by player 0..3, 5=special/town donation)
    inline constexpr uintptr_t Museum_GetSpecimenDonationStatus = 0x0076C680;

    // Returns boolean indicating if specimen has been donated (donation flag != 0)
    inline constexpr uintptr_t Museum_IsSpecimenDonated = 0x0076B820;

    // Returns packed 4-byte donation date (uint16_t year, uint8_t month, uint8_t day) for specimen
    inline constexpr uintptr_t Museum_GetSpecimenDonationDate = 0x0076BD80;

    // Resolves donator player UTF-16 name from player save +0x55A6 based on specimen donation flag
    inline constexpr uintptr_t Museum_GetSpecimenDonatorName = 0x0076B970;

    // Checks if at least one specimen is donated across all 274 museum slots
    inline constexpr uintptr_t Museum_HasAnyDonations = 0x0076B9E8;

    // Iterates all 274 donation flag bytes and returns total number of donated specimens (0..274)
    inline constexpr uintptr_t Museum_GetTotalDonationCount = 0x0076BB88;

    // Returns true if all 67 fossil specimens have been donated
    inline constexpr uintptr_t Museum_CheckFossilsComplete = 0x0076BD24;

    // Returns true if all 72 insect specimens have been donated
    inline constexpr uintptr_t Museum_CheckInsectsComplete = 0x0076BE80;

    // Returns true if aquarium is complete (all 72 fish + all 30 deep-sea diving creatures = 102)
    inline constexpr uintptr_t Museum_CheckAquariumComplete = 0x0076B8D0;

    // Returns true if art gallery is complete (all 25 paintings + all 8 sculptures = 33)
    inline constexpr uintptr_t Museum_CheckArtGalleryComplete = 0x0076C14C;

    // Returns total count of fully completed museum wings (0..4: Paleontology, Insectarium, Aquarium, Art Gallery)
    inline constexpr uintptr_t Museum_GetCompletedWingCount = 0x0076C2E4;

    // Computes total museum completion percentage: (total_donations * 100) / 266
    inline constexpr uintptr_t Museum_CalculateOverallCompletionPercentage = 0x0076C1EC;

    // Computes aquarium completion percentage: (aquarium_donations * 100) / 102
    inline constexpr uintptr_t Museum_CalculateAquariumCompletionPercentage = 0x0076C22C;

    // Computes paleontology completion percentage: (fossil_donations * 100) / 67
    inline constexpr uintptr_t Museum_CalculateFossilCompletionPercentage = 0x0076C448;

    // Computes insectarium completion percentage: (insect_donations * 100) / 72
    inline constexpr uintptr_t Museum_CalculateInsectCompletionPercentage = 0x0076C4C4;

    // Computes art gallery completion percentage: (art_donations * 100) / 33
    inline constexpr uintptr_t Museum_CalculateArtCompletionPercentage = 0x0076C5C8;

    // Checks if all fossil parts belonging to a multi-part dinosaur skeleton group ID (<= 30) are donated
    inline constexpr uintptr_t Museum_CheckDinosaurSkeletonComplete = 0x0076C540;

    // Checks if specific museum exhibition room (IDs 0x4A..0x54) contains any donated specimens
    inline constexpr uintptr_t Museum_RoomHasAnyDonatedSpecimens = 0x0076BEDC;

    // Counts donated art pieces across 25 paintings and 8 sculptures (0..33)
    inline constexpr uintptr_t Museum_GetDonatedArtCount = 0x0076C3BC;

    // Maps specimen category and index to specific 3D exhibit room scene ID (0x4A..0x54)
    inline constexpr uintptr_t Item_GetMuseumExhibitRoomId = 0x0076AC9C;

    // Reads multi-part dinosaur skeleton group ID from item metadata offset +0x12
    inline constexpr uintptr_t Item_GetFossilDinosaurGroupId = 0x00767B44;

    // Converts museum category and relative index to global 16-bit ACNL item ID
    inline constexpr uintptr_t Item_GetItemIdFromCategoryIndex = 0x006BAD9C;

    // Clears 1372-byte museum save block at +0x6AE38: sets 274 dates to 2012-01-01 and zeroes 274 flags
    inline constexpr uintptr_t Museum_InitDonationsSaveBlock = 0x006BB3FC;

    // Marks specimen as donated with special town flag (5) and sets current in-game date
    inline constexpr uintptr_t Museum_ForceDonateTownSpecimen = 0x006BB544;

    // Records donation by active player (1..4), writes packed date, checks wing completion, and updates catalog
    inline constexpr uintptr_t Museum_DonateSpecimen = 0x006BB660;

    // Initializes 10484-byte 2F custom exhibition block at +0x502BC (4 rooms x 2520B furniture + 54B titles)
    inline constexpr uintptr_t Museum_InitCustomExhibitions = 0x002BF46C;

} // namespace acnl::symbols

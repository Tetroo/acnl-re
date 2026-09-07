#ifndef ACNL_WEATHER_STATE_H
#define ACNL_WEATHER_STATE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WEATHER_PERIODS_COUNT   52
#define WEATHER_HOURS_PER_DAY   24
#define WEATHER_ENV_RECORD_SIZE 0x8C    // 140 bytes per EnvironmentParameter.bin record
#define WEATHER_ENV_GROUPS      9

// Vtable addresses for precipitation C++ classes
#define VTABLE_WEATHER_RAIN     0x008ECFE4
#define VTABLE_WEATHER_SNOW     0x008ED044
#define VTABLE_WEATHER_PAPER    0x008EE040
#define VTABLE_WEATHER_SAKURA   0x008EF688

typedef enum WeatherPrecipitationType {
    WEATHER_TYPE_NONE   = 0,
    WEATHER_TYPE_RAIN   = 1,
    WEATHER_TYPE_SNOW   = 2,
    WEATHER_TYPE_PAPER  = 3,
    WEATHER_TYPE_SAKURA = 4
} WeatherPrecipitationType;

/**
 * @brief 52x24 Weather resolution matrix pointer array at DAT_0062E1AC.
 */
typedef struct WeatherTypeTable {
    const uint8_t* rows[WEATHER_PERIODS_COUNT]; // 52 row pointers to 24-byte entries
} WeatherTypeTable;

#ifdef __cplusplus
}
#endif

#endif // ACNL_WEATHER_STATE_H

#ifndef ACNL_DATE_OBJECT_H
#define ACNL_DATE_OBJECT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Runtime Date & Time Structure used throughout ACNL.
 * Total size: 24 bytes (0x18).
 */
typedef struct DateObject {
    int32_t year;    /**< Absolute year, valid range [2012, 2050] */
    int32_t month;   /**< Month of year (1 - 12) */
    int32_t day;     /**< Day of month (1 - 31) */
    int32_t hour;    /**< Hour of day (0 - 23) */
    int32_t minute;  /**< Minute of hour (0 - 59) */
    int32_t second;  /**< Second of minute (0 - 59) */
} DateObject;

#ifdef __cplusplus
}
#endif

#endif // ACNL_DATE_OBJECT_H

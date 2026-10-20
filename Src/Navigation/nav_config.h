/*
 * nav_config.h - every tuning value in one place.
 *
 */
#ifndef NAV_CONFIG_H
#define NAV_CONFIG_H

// Kobuki Buttons
#define BUTTON_0_START      0x01U
#define BUTTON_1_STOP       0x02U

// Start Bollards
#define A1_RANGE_MM         595
#define B1_RANGE_MM         445
#define PATH_TOL_MM         50

// Deposit Markers
#define D1_RANGE_MM         0 // Need to measure
#define D2_RANGE_MM         0 // Need to measure
#define DEPOSIT_TOL_MM      150 // Acceptable to be wide considering there are few potential errors

// Turns after the detection of starting bollards
#define TURN_AFTER_A1_DEG   (-90)
#define TURN_AFTER_B1_DEG   (90)

// Search Controllers
#define SCAN_SPEED          50      // mm/s
#define DEBOUNCE_COUNT      10
#define TRUST_RUN           6 
#define FULL_TURN_CDEG      36000
#define MAX_SEARCH_RETRIES  3
#define SCAN_LAG_CDEG       125
#define RANGE_SAMPLES       16
#define MIN_TARGET_SAMPLES  3

// Deposit Controllers
#define DEPOSIT_TARGET_MM   100
#define SENSOR_OFFSET_MM    0
#define DEPOSIT_SAMPLES     25
#define SENSOR_MIN_MM       300 // Min value allowed to be detected by sensor

#endif // NAV_CONFIG_H
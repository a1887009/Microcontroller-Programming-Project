#ifndef NAV_CONFIG_H
#define NAV_CONFIG_H

// Kobuki buttons
#define BUTTON_0_START      0x01U
#define BUTTON_1_STOP       0x02U

// Kobuki bumper / cliff bits (same layout for both bytes)
#define SENSOR_RIGHT        0x01U
#define SENSOR_CENTRE       0x02U
#define SENSOR_LEFT         0x04U

// Global offset defined for kobuki testing
#define SENSOR_GLOBAL_OFFSET 100

// Search variables
#define SCAN_SPEED          50      // mm/s
#define SEARCH_HITS         10      // how many times the search must accept a reading
#define SEARCH_CENTRE_DEG   12      // distance from first edge to the centre (degrees) 
                                    // mainly used as an extimation

// Bollard variables
#define A1_RANGE_MM         (550 + SENSOR_GLOBAL_OFFSET)    // expected reading for A1
#define B1_RANGE_MM         (400 + SENSOR_GLOBAL_OFFSET)    // expected reading for B1
#define PATH_MIN_MM         (330 + SENSOR_GLOBAL_OFFSET)    // search window
#define PATH_MAX_MM         (650 + SENSOR_GLOBAL_OFFSET)
#define TURN_AFTER_A1_DEG   (-90)   // A: right onto the bottom corridor
#define TURN_AFTER_B1_DEG   (90)    // B: left onto the left corridor

// Following variables
#define LEG_SPEED           100     // mm/s
#define SIDE_CORRECT_DEG    5       // angle to turn when the side detects a collision/drop
#define FRONT_CORRECT_DEG   15      // incase front sensor triggers when not expected (very rare)
#define CLIFF_CONFIRM_LOOPS 2       // how many times the front cliff must detect 
#define LEG_BACKUP_MM       50      // distance it backs up to prepare for turn
#define LEG_TIMEOUT_LOOPS   1500    // ~30 s how long before it gives up finding the ledge

// Turns degrees
#define TURN_CORNER_A_DEG   (90)
#define TURN_CORNER_B_DEG   (-90)
#define TURN_TO_STORAGE_A_DEG (130)
#define TURN_TO_STORAGE_B_DEG (-130)
#define STORAGE_DRIVE_MM    950     // corner -> middle of storage

// Deposit window definition so outside markers are detected
#define DEP_MIN_MM          250
#define DEP_MAX_MM          (520 + SENSOR_GLOBAL_OFFSET)
#define DEP_BAND_MIN_DEG    45
#define DEP_BAND_MAX_DEG    130
#define DEP_RIGHT_IS_D1     1       // marker found on the right = D1

// Final approach to drop off
#define DEPOSIT_TARGET_MM   100     // distance between robot front and the deposit
#define SENSOR_TO_FRONT_MM  (177 + SENSOR_GLOBAL_OFFSET)    // distance from robot front to sensor + offset
#define DEPOSIT_SAMPLES     25      // how many sample readings to determine distance
#define SENSOR_MIN_MM       300     // defined by the ultrasonic
#define DEPOSIT_BACKOFF_MM  80      // too close for accurate reading

#endif // NAV_CONFIG_H

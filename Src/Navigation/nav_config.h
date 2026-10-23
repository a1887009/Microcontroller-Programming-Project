/*
 * nav_config.h - every tuning value in one place.
 *
 * Angles: +ve = left (CCW). Distances in mm.
 */
#ifndef NAV_CONFIG_H
#define NAV_CONFIG_H

// Kobuki buttons
#define BUTTON_0_START      0x01U
#define BUTTON_1_STOP       0x02U

// Kobuki bumper / cliff bits (same layout for both bytes)
#define SENSOR_RIGHT        0x01U
#define SENSOR_CENTRE       0x02U
#define SENSOR_LEFT         0x04U

// ---- Search (used for both the path bollard and the deposit marker) ----
// Spin until SEARCH_HITS readings in a row are in range, then add
// SEARCH_CENTRE_DEG to get from the target's first edge to its centre.
#define SCAN_SPEED          50      // mm/s
#define SEARCH_HITS         10      // loops in a row (~5 loops per sensor reading)
#define SEARCH_CENTRE_DEG   12      // first edge -> centre. Tune: spin slowly past a
                                    // bollard, note over how many degrees it reads in
                                    // range, and set this to about half of that.

// ---- Path bollard (seen from the start circle) -------------------------
#define A1_RANGE_MM         650     // expected reading for A1
#define B1_RANGE_MM         450     // expected reading for B1
#define PATH_MIN_MM         300     // search window
#define PATH_MAX_MM         750
#define TURN_AFTER_A1_DEG   (-90)   // A: right onto the bottom corridor
#define TURN_AFTER_B1_DEG   (90)    // B: left onto the left corridor

// ---- Following walls / ledges -----------------------------------------
#define LEG_SPEED           100     // mm/s
#define SIDE_CORRECT_DEG    5       // turn away from a side bumper / side cliff
#define FRONT_CORRECT_DEG   15      // turn away from the wall on a head-on bump
#define CLIFF_CONFIRM_LOOPS 2       // front cliff seen this many loops in a row = ledge
#define LEG_BACKUP_MM       50      // reverse after the front cliff, before turning
#define LEG_TIMEOUT_LOOPS   1500    // ~30 s without finding a ledge -> give up

// ---- Corner and into storage ------------------------------------------
#define TURN_CORNER_A_DEG   (90)
#define TURN_CORNER_B_DEG   (-90)
#define TURN_TO_STORAGE_A_DEG (130)    // 135 on paper; side corrections leave the robot
#define TURN_TO_STORAGE_B_DEG (-130)   // ~5 deg towards the wall, 130 cancels that
#define STORAGE_DRIVE_MM    950     // corner -> middle of storage (sim best: 950; 1000 too far)

// ---- Deposit marker (seen from the middle of storage) -----------------
// The path bollard outside (A2 / B2) is also visible from in there, so a
// target only counts if it is in the range window AND in the bearing band
// (degrees either side of the heading the robot arrives with).
#define DEP_MIN_MM          325
#define DEP_MAX_MM          595
#define DEP_BAND_MIN_DEG    45
#define DEP_BAND_MAX_DEG    130
#define DEP_RIGHT_IS_D1     1       // marker found on the right = D1

// ---- Final approach -----------------------------------------------------
#define DEPOSIT_TARGET_MM   100     // gap between the robot's front and the marker
#define SENSOR_TO_FRONT_MM  252     // sensor face -> robot front (177 = sensor at the centre)
#define DEPOSIT_SAMPLES     25      // readings (median) taken before the final drive
#define SENSOR_MIN_MM       300     // closest reading the sensor guarantees
#define DEPOSIT_BACKOFF_MM  80      // too close to measure -> reverse this far and re-measure

#endif // NAV_CONFIG_H

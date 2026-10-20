/*
 * nav_common.h - shared types and angle helpers
 *
 * Angles: kobuki.angle is in 0.01 deg ("cdeg"), range -18000..18000.
 *         +ve rotation = CCW (left).
 */
#ifndef NAV_COMMON_H
#define NAV_COMMON_H

#include <stdint.h>

// Every sub-task's Run() returns one of these
typedef enum { TASK_RUNNING, TASK_DONE, TASK_FAILED } Task_Status;

typedef enum { PATH_UNKNOWN, PATH_A, PATH_B } Path;
typedef enum { DEPOSIT_UNKNOWN, DEPOSIT_1, DEPOSIT_2 } Deposit;

// Change between two kobuki.angle readings, handling the +-180 deg jump
int32_t  Angle_Delta_cdeg(int16_t now, int16_t before);

// Any cdeg value -> -18000..18000
int16_t  Wrap_cdeg(int32_t a);

// Any degree value -> -180..180 (shortest turn)
int16_t  Wrap_Deg(int32_t deg);

// Degrees (rounded) to turn from the current heading to face a heading
int16_t  Turn_To_Heading_Deg(int16_t current_cdeg, int16_t target_cdeg);

// Median of n readings (sorts the array in place)
uint16_t Median_mm(uint16_t *v, uint8_t n);

#endif // NAV_COMMON_H

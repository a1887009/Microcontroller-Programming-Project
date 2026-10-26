#ifndef NAV_COMMON_H
#define NAV_COMMON_H

#include <stdint.h>

// Every sub-task's Run() returns one of these
typedef enum { TASK_RUNNING, TASK_DONE, TASK_FAILED } Task_Status;

typedef enum { PATH_UNKNOWN, PATH_A, PATH_B } Path;

// Change between two kobuki.angle readings, handling the +-180 deg jump
int32_t Angle_Delta_cdeg(int16_t now, int16_t before);

// Any cdeg value -> -18000..18000
int16_t Wrap_cdeg(int32_t a);

// Degrees (rounded) to turn from the current heading to face a heading
int16_t Turn_To_Heading_Deg(int16_t current_cdeg, int16_t target_cdeg);

#endif // NAV_COMMON_H

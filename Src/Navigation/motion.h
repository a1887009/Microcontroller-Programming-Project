#ifndef MOTION_H
#define MOTION_H

#include <stdint.h>
#include "kobuki.h"
#include "nav_common.h"

typedef enum { MOVE_ROTATE, MOVE_DRIVE, MOVE_FOLLOW, MOVE_END } Move_Type;

#define WALL_LEFT   (1)
#define WALL_RIGHT  (-1)

typedef struct {
    Move_Type type;
    int16_t   value;        // degrees for ROTATE (+ve = CCW), mm for DRIVE, wall side for FOLLOW
} Move;

void        Orient_Start(const Kobuki_Typedef *k, int16_t turn_deg);
Task_Status Orient_Run(const Kobuki_Typedef *k);

void        Moves_Start(const Kobuki_Typedef *k, const Move *list);  // list must end with MOVE_END
Task_Status Moves_Run(const Kobuki_Typedef *k);   // FAILED if a FOLLOW leg times out

#endif // MOTION_H

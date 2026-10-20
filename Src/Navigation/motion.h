/*
 * motion.h - precise turns and drives using the P-controller wrappers.
 *
 *   Orient_*  one rotation by a number of degrees
 *   Moves_*   a list of rotate / drive steps run one after another
 *
 * Start() functions set the target only (no motor command). Run() sends
 * at most one motor command and must be called every loop.
 */
#ifndef MOTION_H
#define MOTION_H

#include <stdint.h>
#include "kobuki.h"
#include "nav_common.h"

typedef enum { MOVE_ROTATE, MOVE_DRIVE, MOVE_END } Move_Type;

typedef struct {
    Move_Type type;
    int16_t   value;        // degrees for ROTATE (+ve = CCW), mm for DRIVE
} Move;

void        Orient_Start(const Kobuki_Typedef *k, int16_t turn_deg);
Task_Status Orient_Run(const Kobuki_Typedef *k);

void        Moves_Start(const Kobuki_Typedef *k, const Move *list);  // list must end with MOVE_END
Task_Status Moves_Run(const Kobuki_Typedef *k);

#endif // MOTION_H

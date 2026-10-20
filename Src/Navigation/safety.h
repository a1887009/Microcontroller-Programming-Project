/*
 * safety.h - hardware safety checks, run every loop before the FSM.
 *
 * Bumpers and cliff sensors are no longer faults - they are used to
 * navigate (see MOVE_FOLLOW in motion.c). Only a wheel drop stops the run.
 */
#ifndef SAFETY_H
#define SAFETY_H

#include "kobuki.h"

typedef enum {
    SAFETY_OK,
    SAFETY_WHEEL_DROP       // Robot lifted off the ground
} Safety_Fault;

// Check
Safety_Fault Safety_Check(const Kobuki_Typedef *k);

#endif // SAFETY_H

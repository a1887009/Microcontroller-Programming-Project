/*
 * safety.h - hardware safety checks, run every loop before the FSM.
 */
#ifndef SAFETY_H
#define SAFETY_H

#include "kobuki.h"

typedef enum {
    SAFETY_OK,
    SAFETY_WHEEL_DROP,      // Robot lifted off the ground
    SAFETY_BUMPER,          // Hit something
    SAFETY_CLIFF            // About to go over the ledge
} Safety_Fault;

// Check
Safety_Fault Safety_Check(const Kobuki_Typedef *k);

#endif // SAFETY_H

// a1887009, a1921636, a1645500
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

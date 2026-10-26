#ifndef SEARCH_H
#define SEARCH_H

#include <stdint.h>
#include "kobuki.h"
#include "ultrasonic.h"
#include "nav_common.h"

typedef struct {
    uint16_t range_mm;      // average range over the hits
    int16_t  heading_cdeg;  // kobuki.angle that faces the target's centre
} Search_Result;

void        Search_Start(const Kobuki_Typedef *k, uint16_t min_mm, uint16_t max_mm);
Task_Status Search_Run(const Kobuki_Typedef *k, const US_Typedef *obj, Search_Result *result);   // FAILED after a full turn

#endif // SEARCH_H

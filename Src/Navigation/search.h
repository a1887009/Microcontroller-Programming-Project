/*
 * search.h - rotate on the spot and find one target by its range
 *
 * Used by both PATH_SEARCH and DEPOSIT_SEARCH with different windows
 *   Search_Start(k, min_mm, max_mm)   once, when entering the state
 *   Search_Run(k, obj, &result)       every loop until DONE / FAILED
 */
#ifndef SEARCH_H
#define SEARCH_H

#include <stdint.h>
#include "kobuki.h"
#include "ultrasonic.h"
#include "nav_common.h"

// Search findings
typedef struct {
    uint16_t range_mm;      // Median range to the target
    int16_t  heading_cdeg;  // Absolute kobuki.angle of the target midpoint
} Search_Result;

void        Search_Start(const Kobuki_Typedef *k, uint16_t min_mm, uint16_t max_mm);
Task_Status Search_Run(const Kobuki_Typedef *k, const US_Typedef *obj,
                       Search_Result *result);

#endif // SEARCH_H

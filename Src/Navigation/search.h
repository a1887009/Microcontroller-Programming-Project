/*
 * search.h - spin on the spot until a target is found.
 *
 *   Search_Start(k, min_mm, max_mm, band_min_deg, band_max_deg)  once
 *   Search_Run(k, obj, &result)                                   every loop
 *
 * A reading counts if it is in [min_mm, max_mm] AND the robot is pointing
 * between band_min_deg and band_max_deg either side of where it was facing
 * when the search started ((0, 180) = any direction). SEARCH_HITS counting
 * readings in a row = found.
 */
#ifndef SEARCH_H
#define SEARCH_H

#include <stdint.h>
#include "kobuki.h"
#include "ultrasonic.h"
#include "nav_common.h"

typedef struct {
    uint16_t range_mm;      // average range over the hits
    int16_t  heading_cdeg;  // kobuki.angle that faces the target's centre
    int16_t  bearing_deg;   // target relative to the start heading (+ve = left)
} Search_Result;

void        Search_Start(const Kobuki_Typedef *k, uint16_t min_mm, uint16_t max_mm,
                         int16_t band_min_deg, int16_t band_max_deg);
Task_Status Search_Run(const Kobuki_Typedef *k, const US_Typedef *obj,
                       Search_Result *result);   // FAILED after a full turn

#endif // SEARCH_H

#include <stdlib.h>
#include "search.h"
#include "nav_config.h"

static struct {
    uint16_t min_mm, max_mm;          // range window
    int16_t  band_min, band_max;      // bearing band (deg)
    int16_t  last_angle;
    int32_t  turned_cdeg;             // total turned since Search_Start
    int32_t  first_cdeg;              // turned angle at the first hit
    uint8_t  hits;                    // in-range readings in a row
    uint32_t sum_mm;                  // for the average range
} search;

void Search_Start(const Kobuki_Typedef *k, uint16_t min_mm, uint16_t max_mm,
                  int16_t band_min_deg, int16_t band_max_deg)
{
    search.min_mm      = min_mm;
    search.max_mm      = max_mm;
    search.band_min    = band_min_deg;
    search.band_max    = band_max_deg;
    search.last_angle  = k->angle;
    search.turned_cdeg = 0;
    search.hits        = 0;
}

Task_Status Search_Run(const Kobuki_Typedef *k, const US_Typedef *obj,
                       Search_Result *result)
{
    // How far have we turned?
    search.turned_cdeg += Angle_Delta_cdeg(k->angle, search.last_angle);
    search.last_angle   = k->angle;

    // Does this reading count?
    int16_t bearing = (int16_t)abs(Wrap_cdeg(search.turned_cdeg) / 100);
    uint8_t in_band = (bearing >= search.band_min) && (bearing <= search.band_max);
    uint8_t in_range = (obj->range_mm >= search.min_mm) && (obj->range_mm <= search.max_mm);

    if (in_band && in_range) {
        if (search.hits == 0U) {             // first edge
            search.first_cdeg = search.turned_cdeg;
            search.sum_mm     = 0;
        }
        search.hits++;
        search.sum_mm += obj->range_mm;

        if (search.hits >= SEARCH_HITS) {    // found it
            int32_t centre = search.first_cdeg + SEARCH_CENTRE_DEG * 100;
            int32_t back   = search.turned_cdeg - centre;   // centre is this far behind us

            result->range_mm     = (uint16_t)(search.sum_mm / search.hits);
            result->heading_cdeg = Wrap_cdeg(k->angle - back);
            result->bearing_deg  = (int16_t)(Wrap_cdeg(centre) / 100);
            Kobuki_Drive(STOP);
            return TASK_DONE;
        }
    } else {
        search.hits = 0;
    }

    if (labs(search.turned_cdeg) > 36000) {  // full turn, nothing found
        Kobuki_Drive(STOP);
        return TASK_FAILED; // fail if noting found in a full turn
    }

    Kobuki_Rotate(SCAN_SPEED);               // spin left
    return TASK_RUNNING;
}

/*
 * search.c - rotate on the spot and find one target by its range.
 *
 *  CLEARING: if a target is already in the beam, rotate until it leaves
 *            (otherwise we'd miss its rising edge and get a bad midpoint).
 *  LOOKING:  rotate until something in [min_mm, max_mm] enters the beam.
 *  TRACKING: keep rotating until it leaves, then midpoint = centre.
 *
 *  Angles are a running total of turned angle, not raw kobuki.angle, so
 *  the +/-180 jump can't corrupt the midpoint. Edges need DEBOUNCE_COUNT
 *  samples to be confirmed, but the angles stored are the first and last
 *  in-window samples, so the midpoint stays centred.
 */
#include <stdlib.h>
#include "search.h"
#include "nav_config.h"

typedef enum { SEARCH_CLEARING, SEARCH_LOOKING, SEARCH_TRACKING } Search_Phase;

static struct {
    uint16_t     min_mm;
    uint16_t     max_mm;
    Search_Phase phase;
    int16_t      last_angle;
    int32_t      turned_cdeg;       // total turned since Search_Start
    int32_t      phase_start_cdeg;  // for the full-turn timeout
    int32_t      look_start_cdeg;   // LOOKING start, kept if a target is rejected as noise
    int32_t      rise_cdeg;         // angle at FIRST in-window sample
    int32_t      last_hit_cdeg;     // angle at LAST in-window sample
    uint8_t      count;             // debounce counter
    uint8_t      run;               // consecutive in-window loops
    uint8_t      miss;              // consecutive out-of-window loops
    uint16_t     ranges[RANGE_SAMPLES];  // trusted in-window readings
    uint8_t      n_ranges;
} search;

void Search_Start(const Kobuki_Typedef *k, uint16_t min_mm, uint16_t max_mm)
{
    search.min_mm           = min_mm;
    search.max_mm           = max_mm;
    search.phase            = SEARCH_CLEARING;
    search.last_angle       = k->angle;
    search.turned_cdeg      = 0;
    search.phase_start_cdeg = 0;
    search.look_start_cdeg  = 0;
    search.rise_cdeg        = 0;
    search.last_hit_cdeg    = 0;
    search.count            = 0;
    search.run              = 0;
    search.miss             = 0;
    search.n_ranges         = 0;
}

Task_Status Search_Run(const Kobuki_Typedef *k, const US_Typedef *obj,
                       Search_Result *result)
{
    // Track how far turned
    search.turned_cdeg += Angle_Delta_cdeg(k->angle, search.last_angle);
    search.last_angle   = k->angle;

    uint8_t in_window = (obj->range_mm >= search.min_mm) &&
                        (obj->range_mm <= search.max_mm);

    switch (search.phase) {
        case SEARCH_CLEARING:
            search.count = in_window ? 0U : (uint8_t)(search.count + 1U);
            if (search.count >= DEBOUNCE_COUNT) {
                search.phase            = SEARCH_LOOKING;
                search.phase_start_cdeg = search.turned_cdeg;
                search.look_start_cdeg  = search.turned_cdeg;
                search.count            = 0;
                search.miss             = TRUST_RUN;   // no candidate edge yet
            }
            break;

        case SEARCH_LOOKING:
            /* Confirming needs DEBOUNCE_COUNT in-window loops IN A ROW.
             * But a short dropout (one bad reading, ~5 loops) doesn't move
             * the candidate edge: if hits resume within TRUST_RUN loops the
             * original rise angle is kept.                                 */
            if (in_window) {
                if ((search.count == 0U) && (search.miss >= TRUST_RUN)) {
                    search.rise_cdeg = search.turned_cdeg; // new candidate edge
                }
                search.count++;
                search.miss = 0;
            } else {
                search.count = 0;
                if (search.miss < 255U) search.miss++;
            }
            if (search.count >= DEBOUNCE_COUNT) { // edge confirmed
                search.phase            = SEARCH_TRACKING;
                search.last_hit_cdeg    = search.turned_cdeg;
                search.phase_start_cdeg = search.turned_cdeg; // fresh timeout
                search.count            = 0;
                search.run              = DEBOUNCE_COUNT;
            }
            break;

        case SEARCH_TRACKING:
            if (in_window) {
                if (search.run < 255U) search.run++;
                /* only trust readings that last longer than one sensor
                 * update, so a single stray reading after the target has
                 * left can't drag the edge (and the midpoint) along       */
                if (search.run >= TRUST_RUN) {
                    search.last_hit_cdeg = search.turned_cdeg;
                    // keep one sample per sensor update (~every 5 loops)
                    if (((search.run % 5U) == 0U) && (search.n_ranges < RANGE_SAMPLES)) {
                        search.ranges[search.n_ranges++] = obj->range_mm;
                    }
                }
            } else {
                search.run = 0;
            }
            search.count = in_window ? 0U : (uint8_t)(search.count + 1U);
            if (search.count >= DEBOUNCE_COUNT) { // it really has left
                if (search.n_ranges < MIN_TARGET_SAMPLES) {
                    // too brief to be a real target (noise) - keep looking
                    search.phase            = SEARCH_LOOKING;
                    search.phase_start_cdeg = search.look_start_cdeg;
                    search.count            = 0;
                    search.miss             = TRUST_RUN;
                    search.n_ranges         = 0;
                    break;
                }
                int32_t mid_cdeg = (search.rise_cdeg + search.last_hit_cdeg) / 2;
                // undo the sensor's lag, in the direction we were turning
                mid_cdeg += (search.turned_cdeg >= 0) ? -SCAN_LAG_CDEG : SCAN_LAG_CDEG;
                // how far back from where we are now the midpoint lies
                int32_t back = search.turned_cdeg - mid_cdeg;

                result->range_mm     = Median_mm(search.ranges, search.n_ranges);
                result->heading_cdeg = Wrap_cdeg((int32_t)k->angle - back);

                Kobuki_Drive(STOP);
                return TASK_DONE;
            }
            break;
    }

    // Full turn in one phase without success -> give up
    if (labs(search.turned_cdeg - search.phase_start_cdeg) > FULL_TURN_CDEG) {
        Kobuki_Drive(STOP);
        return TASK_FAILED;
    }

    Kobuki_Rotate(SCAN_SPEED); // +ve = CCW
    return TASK_RUNNING;
}

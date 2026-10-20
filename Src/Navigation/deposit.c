/*
 * deposit.c - drive towards the deposit marker
 *
 * Stand still, take the median of DEPOSIT_SAMPLES readings, then drive the
 * rest of the way with wheel odometry so the robot's front stops
 * DEPOSIT_TARGET_MM from the marker. (The final reading would be below the
 * sensor's guaranteed 300 mm, so the last part can't be done by sensor.)
 */
#include "deposit.h"
#include "nav_config.h"

typedef enum { DEPOSIT_MEASURE, DEPOSIT_BACKOFF, DEPOSIT_DRIVE } Deposit_Phase;

static struct {
    Deposit_Phase phase;
    uint16_t      max_range_mm;
    uint8_t       samples;
    uint16_t      buf_mm[DEPOSIT_SAMPLES];
} dep;

void Deposit_Start(uint16_t max_range_mm)
{
    dep.phase        = DEPOSIT_MEASURE;
    dep.max_range_mm = max_range_mm;
    dep.samples      = 0;
}

Task_Status Deposit_Run(const Kobuki_Typedef *k, const US_Typedef *obj)
{
    switch (dep.phase) {
        case DEPOSIT_MEASURE:
            Kobuki_Drive(STOP);
            dep.buf_mm[dep.samples++] = obj->range_mm;
            if (dep.samples >= DEPOSIT_SAMPLES) { // Run for n number of samples to collect an accurate sensor reading 
                int32_t range = (int32_t)Median_mm(dep.buf_mm, dep.samples); // Find the median of the sensor reading

                // Too close for the sensor to measure: reverse, then measure again
                if (range <= SENSOR_MIN_MM) {
                    Kobuki_Drive_Distance_Setpoint(k->distance, -DEPOSIT_BACKOFF_MM);
                    dep.phase = DEPOSIT_BACKOFF;
                    return TASK_RUNNING;
                }

                // If it is significantly above an expected range then compute an error
                if (range > (int32_t)dep.max_range_mm) {
                    return TASK_FAILED;
                }

                int32_t gap = range - DEPOSIT_TARGET_MM - SENSOR_TO_FRONT_MM; // Set the gap needed to be driven
                if (gap < 0) gap = 0;
                Kobuki_Drive_Distance_Setpoint(k->distance, (int16_t)gap); // Set our setpoint at target distance
                dep.phase = DEPOSIT_DRIVE;
            }
            return TASK_RUNNING;

        case DEPOSIT_BACKOFF:
            if (k->distance_complete) {
                Kobuki_Drive(STOP);
                dep.phase   = DEPOSIT_MEASURE;
                dep.samples = 0;
                return TASK_RUNNING;
            }
            Kobuki_Drive_Distance(k->distance);
            return TASK_RUNNING;

        case DEPOSIT_DRIVE:
            if (k->distance_complete) {
                Kobuki_Drive(STOP); // Sends stop when the distance is covered
                return TASK_DONE;
            }
            Kobuki_Drive_Distance(k->distance); // Drive when distance not covered
            return TASK_RUNNING;
    }
    return TASK_FAILED; // Default return case
}

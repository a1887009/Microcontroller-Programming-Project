/*
 * deposit.h - drive towards the deposit marker
 *
 *   Deposit_Start(max_range_mm)   once, when entering the state
 *   Deposit_Run(k, obj)           every loop until DONE / FAILED
 *
 * FAILED (robot stays stopped) if the measured range is above max_range_mm
 * (nothing in front) or too close to trust.
 */
#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <stdint.h>
#include "kobuki.h"
#include "ultrasonic.h"
#include "nav_common.h"

void        Deposit_Start(uint16_t max_range_mm);
Task_Status Deposit_Run(const Kobuki_Typedef *k, const US_Typedef *obj);

#endif // DEPOSIT_H 

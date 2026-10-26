#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <stdint.h>
#include "kobuki.h"
#include "ultrasonic.h"
#include "nav_common.h"

void        Deposit_Start(uint16_t max_range_mm);
Task_Status Deposit_Run(const Kobuki_Typedef *k, const US_Typedef *obj);

#endif // DEPOSIT_H 

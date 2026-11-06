// a1887009, a1921636, a1645500
#ifndef ADC_H
#define ADC_H

#include <stdint.h>

#define ADC_CHANNEL_1   ((uint8_t)1U)

void ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);

#endif // ADC_H
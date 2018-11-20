#ifndef _ADC_H_
#define _ADC_H_

#include "stm32f4xx.h"
#include <stdlib.h>

#include "../UART/UART.h"
#include "../DISPLAY/Display.h"

void ADCinit(ADC_TypeDef *ADCx);
uint16_t ADC_convert(ADC_TypeDef *ADCx);
void ADC_print(ADC_TypeDef *ADCx);
uint16_t map(int x, int in_min, int in_max, int out_min, int out_max);




#endif

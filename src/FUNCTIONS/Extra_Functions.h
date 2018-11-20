/*
 * Extra_Functions.h
 *
 *  Created on: Jul 16, 2017
 *      Author: babu_wanyeki
 */

#ifndef FUNCTIONS_EXTRA_FUNCTIONS_H_
#define FUNCTIONS_EXTRA_FUNCTIONS_H_
#include <stdio.h>
#include "stm32f4xx.h"

//f405 clock is 21000000
//f411 clock is 24000000
#define SystemClock 21000000//This SystemClock frequency is based off the APBs clock frequencies. APB1 = APB2
#define TIMER_CLOCK (SystemClock*2)//When the TIMPRE bit is 0 if APBx prescaler !=1, then the Timer is 2x the APBx frequency
#define UARTCLOCK   (SystemClock)//USART2 is APB1, UART4 is APB1, USART6 is APB2



void Start_Clock_HSI_f405(void);
void Start_Clock_HSE_f405(void);
void Start_Clock_HSI_f411(void);


#endif /* FUNCTIONS_EXTRA_FUNCTIONS_H_ */

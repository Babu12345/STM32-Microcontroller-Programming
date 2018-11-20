#ifndef _Functions_H_
#define _Functions_H_
#include "stm32f4xx.h"


void GPIO_enable(void);
void delay1(double milliseconds);
void led_on_blue(GPIO_TypeDef *GPIO1);
void led_off_blue(GPIO_TypeDef *GPIO1);
void led_toggle_blue(GPIO_TypeDef *GPIO1);



#endif

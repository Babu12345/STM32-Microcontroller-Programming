#ifndef _DAC_H_
#define _DAC_H_

#include "../I2C/I2C.h"
#define add 0x60
//The pin for the DAC is A4
void DAC_init(I2C_TypeDef *I2Cx);

void setVoltage(I2C_TypeDef *I2Cx,uint16_t frac, uint8_t x);


#endif

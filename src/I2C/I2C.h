#ifndef _I2C_H_
#define _I2C_H_

#include "stm32f4xx.h"

void i2c_enable(I2C_TypeDef *I2Cx);
void i2c_command(I2C_TypeDef *I2Cx, uint32_t address, uint32_t type,uint32_t data);
void i2c_cmd(I2C_TypeDef *I2Cx, uint8_t address,uint8_t type,uint8_t MSB,uint8_t LSB);
void i2c_off(I2C_TypeDef *I2Cx);
void i2c_on(I2C_TypeDef *I2Cx);
#endif

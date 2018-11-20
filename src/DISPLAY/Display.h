#ifndef _Display_H_
#define _Display_H_

#include "stm32f4xx.h"
#include "../I2C/I2C.h"


#define Data  0x40
#define Cmd 0x00

uint32_t buffer2[1025];

void Display_draw(I2C_TypeDef *I2Cx, uint32_t address, uint32_t *buffer);
void Display_oled_init(I2C_TypeDef *I2Cx, uint32_t ssd1306_slave_address);
void Display_char(uint32_t x, int page,char char_1, uint32_t *buffer);
void Display_string(uint32_t x, int page, char *string, uint32_t *buffer);
void Display(uint32_t x , int page, char *string, I2C_TypeDef *I2C);
void clear(uint32_t *buffer);


#endif

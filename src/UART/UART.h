#ifndef _UART_H_
#define _UART_H_

#include "stm32f4xx.h"
#include "../src/FUNCTIONS/Extra_Functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void Usart_enable(USART_TypeDef *USARTx, uint32_t Baudrate);
void send_data(uint8_t data,USART_TypeDef *USARTx );
void send_string_ln(char *s,USART_TypeDef *USARTx);
void send_string(char *s,USART_TypeDef *USARTx);
uint8_t recieve_data(USART_TypeDef *USARTx);
void send(int s[],int y, USART_TypeDef *USARTx);







#endif

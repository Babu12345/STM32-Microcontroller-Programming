#ifndef _SPI_H_
#define _SPI_H_

#include "stm32f4xx.h"

void SPI_init(SPI_TypeDef* SPIx, unsigned int prescaler);
uint8_t SPI_send_single(SPI_TypeDef* SPIx, unsigned char data);
unsigned char SPI_receive_single(SPI_TypeDef* SPIx);
void SPI_send(SPI_TypeDef* SPIx, unsigned char* data, unsigned int length);
void SPI_receive(SPI_TypeDef* SPIx, unsigned char* data, unsigned int length);
void SPI_transmit(SPI_TypeDef* SPIx, unsigned char* txbuf, unsigned char* rxbuf, unsigned int len);
uint8_t* SPI_sendRead(SPI_TypeDef* SPIx, uint8_t *buf, unsigned char* data, unsigned int length);

#endif // _SPI_H_

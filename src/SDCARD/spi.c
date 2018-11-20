#include "../SDCARD/spi.h"
void SPI_init(SPI_TypeDef* SPIx, unsigned int prescaler)
{ 

/* configure pins used by SPI1
 * PA5 = SCK
 * PA6 = MISO
 * PA7 = MOSI
 */
// connect SPI1 pins to SPI alternate function
 RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
 GPIOA->AFR[0]|=0x55500000;
 GPIOA->MODER|=GPIO_MODER_MODER5_1;
 GPIOA->MODER|=GPIO_MODER_MODER6_1;
 GPIOA->MODER|=GPIO_MODER_MODER7_1;
 GPIOA->OTYPER&=~GPIO_OTYPER_OT_5;
 GPIOA->OTYPER&=~GPIO_OTYPER_OT_6;
 GPIOA->OTYPER&=~GPIO_OTYPER_OT_7;
 GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR5_1;
 GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR6_1;
 GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR7_1;
 GPIOA->PUPDR&=~GPIO_PUPDR_PUPDR5;
 GPIOA->PUPDR&=~GPIO_PUPDR_PUPDR6;
 GPIOA->PUPDR&=~GPIO_PUPDR_PUPDR7;
// enable peripheral clock
 RCC->APB2ENR|=RCC_APB2ENR_SPI1EN;
 SPIx->CR1&=~SPI_CR1_BIDIMODE;
 SPIx->CR1|=SPI_CR1_MSTR;
 SPIx->CR1&=~SPI_CR1_DFF;
 SPIx->CR1&=~SPI_CR1_CPOL;
 SPIx->CR1&=~SPI_CR1_CPHA;
 SPIx->CR1|=SPI_CR1_SSM;
 SPIx->CR1|=SPI_CR1_SSI;
 SPIx->CR1&=~SPI_BAUDRATEPRESCALER_256;
 SPIx->CR1|= prescaler;
 SPIx->CR1&=~SPI_CR1_LSBFIRST;
 SPIx->CR1|=SPI_CR1_SPE;
/* configure SPI1 in Mode 0
 * CPOL = 0 --> clock is low when idle
 * CPHA = 0 --> data is sampled at the first edge
 */
}
//I changed the order of reading the status register then transmitting
//so if anything is wrong this is probably the reason
uint8_t SPI_send_single(SPI_TypeDef* SPIx, unsigned char data)
{


	while(!(SPIx->SR & SPI_SR_TXE));
	SPIx->DR = data; // write data to be transmitted to the SPI data register
	while(!(SPIx->SR & SPI_SR_TXE));
	while(!(SPIx->SR & SPI_SR_RXNE));
while( SPIx->SR & SPI_SR_BSY ){}; // wait until SPI is not busy anymore
//Data

 return SPIx->DR;// return received data from SPI data register

} 
unsigned char SPI_receive_single(SPI_TypeDef* SPIx)
{

	while(!(SPIx->SR & SPI_SR_TXE));
	SPIx->DR = 0xFF; // write data to be transmitted to the SPI data register
	while(!(SPIx->SR & SPI_SR_RXNE));
while( SPIx->SR & SPI_SR_BSY ); // wait until SPI is not busy anymore
return SPIx->DR; // return received data from SPI data register
}
void SPI_send(SPI_TypeDef* SPIx, unsigned char* data, unsigned int length)
{
while (length--)
 {
 (void)SPI_send_single(SPIx, *data);
 data++;
 }
}
uint8_t* SPI_sendRead(SPI_TypeDef* SPIx, uint8_t *buf, unsigned char* data, unsigned int length)
{
while (length--)
 {
 *buf=SPI_send_single(SPIx, *data);
 data++;
 buf++;
 }
return buf;
}
void SPI_receive(SPI_TypeDef* SPIx, unsigned char* data, unsigned int length)
{
while (length--)
 {
 *data = SPI_receive_single(SPIx);
 data++;
 }
}
void SPI_transmit(SPI_TypeDef* SPIx, unsigned char* txbuf, unsigned char*
rxbuf, unsigned int len)
{
while (len--)
 {
 SPIx->DR = *txbuf; // write data to be transmitted to the SPIdata register
 while( !(SPIx->SR & SPI_SR_TXE) ); // wait until transmitcomplete
 while( !(SPIx->SR & SPI_SR_RXNE) ); // wait until receivecomplete
 while( SPIx->SR & SPI_SR_BSY ); // wait until SPI is not busyanymore
 *rxbuf = SPIx->DR; // return received data from SPI data register
 txbuf++;
 rxbuf++;
 }
} 

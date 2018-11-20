#include "ADC.h"

uint16_t map(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void ADCinit(ADC_TypeDef *ADCx){

//Pin C4: Hall effect 1
RCC->APB2ENR|=RCC_APB2ENR_ADC1EN;
RCC ->AHB1ENR |=RCC_AHB1ENR_GPIOCEN;

//Pin C5: Hall effect 2
RCC ->AHB1ENR |=RCC_AHB1ENR_GPIOCEN;

//Pin A4: Hall effect 3
RCC ->AHB1ENR |=RCC_AHB1ENR_GPIOAEN;

//Pin B0: Hall effect 4
RCC ->AHB1ENR |=RCC_AHB1ENR_GPIOBEN;

GPIOC->MODER|= GPIO_MODER_MODER4;
GPIOC->OSPEEDR|= GPIO_OSPEEDER_OSPEEDR4;

GPIOC->MODER|= GPIO_MODER_MODER5;
GPIOC->OSPEEDR|= GPIO_OSPEEDER_OSPEEDR5;

GPIOA->MODER|= GPIO_MODER_MODER4;
GPIOA->OSPEEDR|= GPIO_OSPEEDER_OSPEEDR4;

GPIOB->MODER|= GPIO_MODER_MODER0;
GPIOB->OSPEEDR|= GPIO_OSPEEDER_OSPEEDR0;

ADCx->CR1 &=~(ADC_CR1_RES);//There is also the choice of lowing the resolution to increase the speed
ADCx->CR1 |=(ADC_CR1_SCAN);

ADCx->CR2 &=~(ADC_CR2_EXTEN);

//Can Switch from Continuous to single conversion depending on preferences
//ADCx->CR2 &=~(ADC_CR2_CONT);
ADCx->CR2 |=(ADC_CR2_CONT);

ADCx->SQR1|=(0<<20);//4 conversions

//Channel 14 is the first conversion in the sequence

ADCx->SQR3|=14<<0;

//Channel 15 is the second conversion in the sequence
ADCx->SQR3|=15<<5;
//Channel 4 is the third conversion in the sequence
ADCx->SQR3|=4<<10;
//Channel 8 is the fourth conversion in the sequence
ADCx->SQR3|=8<<15;


//ADCx->SMPR2|=0x30000000;//Conversion speed


ADCx->CR2|=ADC_CR2_ADON;
ADCx->CR2|=ADC_CR2_SWSTART;



}

uint16_t ADC_convert(ADC_TypeDef *ADCx){

while(!(ADCx->SR && ADC_SR_EOC));

return ADCx->DR;
}


void ADC_print(ADC_TypeDef *ADCx){

	char a[100];

	send_string("Reading 1 data values: ",USART2);
	//Display(0,1,"Reading 1 data values: ",I2C2);


	itoa(ADC_convert(ADC1),a,10);
	send_string_ln(a,USART2);
	//Display(0,2,a,I2C2);


}

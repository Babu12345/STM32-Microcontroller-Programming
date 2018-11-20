#include "UART.h"



void Usart_enable(USART_TypeDef *USARTx, uint32_t Baudrate){

	int x,y,z;
	double frac,integer;
	RCC->APB1ENR|=RCC_APB1ENR_USART2EN;
	RCC->APB1ENR|=RCC_APB1ENR_UART4EN;
	RCC->APB2ENR|=RCC_APB2ENR_USART6EN;

	RCC->AHB1ENR |=RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |=RCC_AHB1ENR_GPIOCEN;



	//Pins C6 and C7
	//USART6
	GPIOC->AFR[0]|=0x88000000;
	GPIOC->MODER|=GPIO_MODER_MODER6_1|GPIO_MODER_MODER7_1;
	GPIOC->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR6_1;
	GPIOC->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR7_1;

	GPIOC->OTYPER&=~GPIO_OTYPER_OT_6;
	GPIOC->OTYPER&=~GPIO_OTYPER_OT_7;

	GPIOC->PUPDR  |=(GPIO_PUPDR_PUPDR6_0);
	GPIOC->PUPDR  |=(GPIO_PUPDR_PUPDR7_0);



	//Pins A2 and A3
	//*((unsigned int*)0x40020020)=0x00007700;
    GPIOA->AFR[0]|=0x00007700;
	GPIOA->MODER  |=GPIO_MODER_MODER2_1|GPIO_MODER_MODER3_1;

    GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR2_1;
    GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR3_1;


	GPIOA->OTYPER&=~GPIO_OTYPER_OT_2;
	GPIOA->OTYPER&=~GPIO_OTYPER_OT_3;


    GPIOA->PUPDR  |=(GPIO_PUPDR_PUPDR2_0);
    GPIOA->PUPDR  |=(GPIO_PUPDR_PUPDR3_0);



    //Pins A0 and A1
    GPIOA->AFR[0]&=~(0x000000FF);
    GPIOA->AFR[0]|=0x00000088;

    GPIOA->MODER  |=GPIO_MODER_MODER0_1|GPIO_MODER_MODER1_1;

    GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR0;
    GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR1;

    GPIOA->OTYPER&=~GPIO_OTYPER_OT_0;
    GPIOA->OTYPER&=~GPIO_OTYPER_OT_1;

    GPIOA->PUPDR  |=(GPIO_PUPDR_PUPDR0_0);
    GPIOA->PUPDR  |=(GPIO_PUPDR_PUPDR1_0);


    Baudrate= ((UARTCLOCK)/(Baudrate*16));
    frac=modf(Baudrate,&integer);
    x=((int)integer)<<4;
    y=round(frac*16);
    z=x|y;

    //USARTx ->BRR =0;
    USARTx ->BRR = (z);
    USARTx ->CR1&=~USART_CR1_OVER8;
    USARTx ->CR1&=~USART_CR1_M;
    USARTx ->CR1&=~USART_CR1_PCE;
    USARTx ->CR1&=~USART_CR1_RWU;
    USARTx ->CR1|=USART_CR1_TE;
    USARTx ->CR1|=USART_CR1_RE;
    //USARTx ->CR1|=USART_CR1_WAKE;
    USARTx->CR1&=~USART_CR1_PCE;
    USARTx ->CR2&=~USART_CR2_STOP;
    USARTx ->CR3&=~USART_CR3_CTSE;
    USARTx ->CR3&=~USART_CR3_RTSE;

    //This statement is used to read the camera data via interrupts
    UART4->CR1|=USART_CR1_RXNEIE;


    USARTx ->CR1|=USART_CR1_UE;

}


void send_data(uint8_t data,USART_TypeDef *USARTx ){
	while(1){
		if(USARTx->SR & USART_SR_TXE){
			break;
		}
	}
	USARTx->DR=(data);

	while(1){
			if(USARTx->SR & USART_SR_TC){
				break;
			}
		}
   //while(!(USARTx->SR & USART_SR_TC));

}

uint8_t recieve_data(USART_TypeDef *USARTx){

	while(1){

	if((USARTx->SR & USART_SR_RXNE)){
	return (USARTx->DR) ;
	break;
	}
	}
}


void send_string_ln(char *s,USART_TypeDef *USARTx){

	while(*s){
		send_data(*s++,USARTx);

	}
	send_data('\n',USARTx);
}
void send_string(char *s,USART_TypeDef *USARTx){

	while(*s){
		send_data(*s++,USARTx);

	}

}

void send(int s[],int y, USART_TypeDef *USARTx){

	for(int x=0;x<y;x++){

		send_data(s[x],USARTx);


	}
}





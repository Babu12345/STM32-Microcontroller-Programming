#include "Functions.h"
#include "Extra_Functions.h"


void GPIO_enable(void){
	// GPIOB and A pins 4 13 15 10 11

	RCC ->AHB1ENR |= RCC_AHB1ENR_GPIOAEN|RCC_AHB1ENR_GPIOBEN|RCC_AHB1ENR_GPIOCEN;

	GPIOB ->MODER |= GPIO_MODER_MODER4_0;
	GPIOA ->MODER |= GPIO_MODER_MODER13_0;
	GPIOA ->MODER |= GPIO_MODER_MODER15_0;

	RCC->APB1RSTR|=RCC_APB1RSTR_I2C2RST;
	RCC->APB1RSTR&=~(RCC_APB1RSTR_I2C2RST);


}

//Note that the SystemClock defined variable is stored on the Extra_Functions.h header file
//The delay1 function is in milliseconds
//Also note that the Start_Clock function is on the Extra_Functions files
void delay1(double milliseconds){

	//System clock oscillator

	uint32_t PSC = 30000;
	milliseconds= milliseconds/1000.0;
	//The System Clock is x2 the speed of the the clock if the prescaler is not 1
	uint32_t ARR= ((TIMER_CLOCK*milliseconds)/(PSC));

	if(ARR<1){
		ARR=1;
	}


	RCC->APB2ENR|=RCC_APB2ENR_TIM1EN;

	TIM1->CR1|=TIM_CR1_ARPE;

	TIM1->PSC =PSC-1;
	TIM1->ARR =ARR-1;

	TIM1->DIER|=TIM_DIER_UIE;
	TIM1->CR1|=TIM_CR1_CEN;

	TIM1->EGR|=TIM_EGR_UG;
	TIM1->SR;
	TIM1->SR &= ~TIM_SR_UIF;

	while(!(TIM1->SR & TIM_SR_UIF));
	TIM1->SR &= ~TIM_SR_UIF;


}

void led_on_blue(GPIO_TypeDef *GPIO1){
	GPIO1 ->MODER |= GPIO_MODER_MODER8_0;
	GPIO1->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8;
	GPIO1->OTYPER &=~(GPIO_OTYPER_OT_8);
	GPIO1->PUPDR  &=~(GPIO_PUPDR_PUPDR8);
	GPIO1->ODR |= GPIO_ODR_ODR_8;
}

void led_off_blue(GPIO_TypeDef *GPIO1){
	GPIO1 ->MODER |= GPIO_MODER_MODER8_0;
	GPIO1->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8;
	GPIO1->OTYPER &=~(GPIO_OTYPER_OT_8);
	GPIO1->PUPDR  &=~(GPIO_PUPDR_PUPDR8);

	GPIO1->ODR &= ~(GPIO_ODR_ODR_8);
}
void led_toggle_blue(GPIO_TypeDef *GPIO1){
	GPIO1 ->MODER |= GPIO_MODER_MODER8_0;
	GPIO1->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8;
	GPIO1->OTYPER &=~(GPIO_OTYPER_OT_8);
	GPIO1->PUPDR  &=~(GPIO_PUPDR_PUPDR8);

	GPIO1->ODR ^=(GPIO_ODR_ODR_8);
}

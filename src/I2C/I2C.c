#include "I2C.h"

void i2c_enable(I2C_TypeDef *I2Cx){
	RCC ->AHB1ENR |=RCC_AHB1ENR_GPIOBEN;
	RCC ->AHB1ENR |=RCC_AHB1ENR_GPIOAEN;
	//I2C 2 on GPIO B pins 10 and 11
	RCC ->APB1ENR|=RCC_APB1ENR_I2C2EN;
	//I2C1 on GPIO B pins 6 and 7
	RCC->APB1ENR|=RCC_APB1ENR_I2C1EN;


	//GPIOB

	GPIOB->AFR[0]|=0x00000000;
	GPIOB->AFR[1]|=0x00004400;


	GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR6_1;
	GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR7_1;

	GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR10_1;
	GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR11_1;

	GPIOB->OTYPER|=GPIO_OTYPER_OT_6;
	GPIOB->OTYPER |=GPIO_OTYPER_OT_7;
	GPIOB->OTYPER|=GPIO_OTYPER_OT_10;
	GPIOB->OTYPER |=GPIO_OTYPER_OT_11;

	GPIOB->PUPDR  |=(GPIO_PUPDR_PUPDR6_0);
	GPIOB->PUPDR  |=(GPIO_PUPDR_PUPDR7_0);

	GPIOB->PUPDR  |=(GPIO_PUPDR_PUPDR10_0);
	GPIOB->PUPDR  |=(GPIO_PUPDR_PUPDR11_0);


	GPIOB->MODER  |=GPIO_MODER_MODER6_1|GPIO_MODER_MODER7_1;
	GPIOB->MODER  |=GPIO_MODER_MODER10_1|GPIO_MODER_MODER11_1;



	I2Cx->CR1    &=~(I2C_CR1_SMBUS);
	I2Cx->OAR1   |=I2C_ADDRESSINGMODE_7BIT;
	I2Cx ->OAR1  |=0x000;
	I2Cx->CR2    |= 0x0008;
	I2Cx->CCR    |= 0x0028;
	I2Cx->TRISE  |= 0x0009;
	I2Cx->CR1    |=I2C_CR1_PE;

}

void i2c_command(I2C_TypeDef *I2Cx, uint32_t address, uint32_t type,uint32_t data){

	while(I2Cx->SR2&I2C_SR2_BUSY){}

	I2Cx->CR1 |=I2C_CR1_START;

	while(!(I2Cx->SR1&I2C_SR1_SB)){}
	I2Cx->DR = address;
	while(!(I2Cx->SR1&I2C_SR1_ADDR)){}
	while(!(I2Cx->SR2&I2C_SR2_MSL)){}

	while(!(I2Cx->SR1&I2C_SR1_TXE)){}
	I2Cx->DR  = type;
	while(!(I2Cx->SR1&I2C_SR1_BTF)){}

	while(!(I2Cx->SR1&I2C_SR1_TXE)){}
	I2Cx->DR  = data;
	while(!(I2Cx->SR1&I2C_SR1_BTF)){}


	I2Cx->CR1 |=I2C_CR1_STOP;
}
void i2c_cmd(I2C_TypeDef *I2Cx, uint8_t address,uint8_t type,uint8_t MSB,uint8_t LSB){


	if(type){
		type=0x60;
	}
	else{
		type=0x40;
	}
	while(I2Cx->SR2&I2C_SR2_BUSY){}

	I2Cx->CR1 |=I2C_CR1_START;

	while(!(I2Cx->SR1&I2C_SR1_SB)){}
	I2Cx->DR = address;
	while(!(I2Cx->SR1&I2C_SR1_ADDR)){}
	while(!(I2Cx->SR2&I2C_SR2_MSL)){}

	while(!(I2Cx->SR1&I2C_SR1_TXE)){}
	I2Cx->DR  = type;
	while(!(I2Cx->SR1&I2C_SR1_BTF)){}

	while(!(I2Cx->SR1&I2C_SR1_TXE)){}
	I2Cx->DR  = MSB;
	while(!(I2Cx->SR1&I2C_SR1_BTF)){}

	while(!(I2Cx->SR1&I2C_SR1_TXE)){}
	I2Cx->DR  = LSB;
	while(!(I2Cx->SR1&I2C_SR1_BTF)){}


	I2Cx->CR1 |=I2C_CR1_STOP;
}



void i2c_off(I2C_TypeDef *I2Cx){
I2Cx->CR1&=~(I2C_CR1_PE);
}
//i2c_on needs to be tested to see if the settings are still saved after disable
void i2c_on(I2C_TypeDef *I2Cx){
I2Cx->CR1|=(I2C_CR1_PE);
}

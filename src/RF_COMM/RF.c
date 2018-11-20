#include "RF.h"

//Note that the delay time might need to be modified
//and I need to make sure that its able to go as fast
//as a few microseconds
//B0 was changed to B8 for the breadboard
#define SIZE 16


#if Board

void RF_setup(void){

 RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
 RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;

//CSN: This is the card select line PB8
 GPIOB->MODER|=GPIO_MODER_MODER8_0;
 GPIOB->OTYPER&=~GPIO_OTYPER_OT_8;
 GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR8_1;
 GPIOB->PUPDR|=GPIO_PUPDR_PUPDR8_0;
 GPIOB->ODR |= GPIO_ODR_ODR_8;//High inactive

 // CE: Chip enable is PB1
 GPIOB->MODER&=~GPIO_MODER_MODER1_0;
 GPIOB->OTYPER&=~GPIO_OTYPER_OT_1;
 GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR1_1;
 GPIOB->PUPDR|=GPIO_PUPDR_PUPDR1_0;
 GPIOB->ODR &=~ GPIO_ODR_ODR_1;//Low inactive


 SPI_init(SPI1, SPI_BAUDRATEPRESCALER_2);

}
void RF_init(void){
	RF_setup();
	RF_writeReg(RF24_RF_SETUP, RF24_RF_SETUP_RF_PWR_0|RF24_RF_SETUP_RF_DR_250);
	RF_writeReg(RF24_RX_PW_P0, SIZE);
	RF_writeReg(RF24_RF_CH, 2);
	RF_WriteRegisterData(RF24_RX_ADDR_P0, (uint8_t*)TADDR, sizeof(TADDR));
	RF_WriteRegisterData(RF24_TX_ADDR, (uint8_t*)TADDR, sizeof(TADDR));
	RF_writeReg(RF24_EN_RXADDR, RF24_EN_RXADDR_ERX_P0);
	RF_ResetStatus(RF24_STATUS_RX_DR|RF24_STATUS_TX_DS|RF24_STATUS_MAX_RT);

}
void RF_writeReg(uint8_t reg, uint8_t val){
RF_CSNlow();
(void)SPI_send_single(SPI1, RF24_W_REGISTER|reg);
(void)SPI_send_single(SPI1, val);
RF_CSNhigh();
delay1(10);
}
void RF_write(uint8_t val){
GPIOB->ODR &=~ GPIO_ODR_ODR_8;
(void)SPI_send_single(SPI1, val);
GPIOB->ODR |= GPIO_ODR_ODR_8;
delay1(10);
}
uint8_t RF_Status(void){
	RF_CSNlow();
	uint8_t temp=SPI_send_single(SPI1, RF24_NOP);
	RF_CSNhigh();
	return temp;
}
void RF_ResetStatus(uint8_t flag){
	delay1(10);
	RF_CSNlow();
	delay1(10);
	RF_writeReg(RF24_STATUS, flag);
	delay1(10);
	RF_CSNhigh();
	delay1(10);
}
void RF_WriteRegisterData(uint8_t reg, uint8_t *buf, uint8_t bufSize){
	RF_CSNlow();
	(void)SPI_send_single(SPI1, RF24_W_REGISTER|reg);
	SPI_send(SPI1, buf, bufSize);
	RF_CSNhigh();
}
void RF_TXPayload(uint8_t *payload, uint8_t size){
	RF_write(RF24_FLUSH_TX);
	RF_WriteRegisterData(RF24_W_TX_PAYLOAD,payload,size);
	RF_CEhigh();
	delay1(10);
	RF_CElow();

}


uint8_t* RF_ReadRegisterData(uint8_t reg, uint8_t * buf, uint8_t size){
	//uint8_t * buf1;
	GPIOB->ODR &=~ GPIO_ODR_ODR_8;
	(void)SPI_send_single(SPI1,RF24_R_REGISTER|reg );
	(void)SPI_sendRead(SPI1, buf, buf, size);
	GPIOB->ODR |= GPIO_ODR_ODR_8;
	delay1(10);
	return buf;
}

void RF_RxPayload(uint8_t *payload, uint8_t size){

	GPIOB->ODR &=~ GPIO_ODR_ODR_1;
	RF_ReadRegisterData(RF24_R_RX_PAYLOAD,payload,size);
	GPIOB->ODR |= GPIO_ODR_ODR_1;
}
void RF_CElow(void){
	 GPIOB->ODR &=~ GPIO_ODR_ODR_1;
}
void RF_CEhigh(void){
	 GPIOB->ODR |= GPIO_ODR_ODR_1;
}
void RF_CSNlow(void){
	 GPIOB->ODR &=~ GPIO_ODR_ODR_8;
}
void RF_CSNhigh(void){
	 GPIOB->ODR |= GPIO_ODR_ODR_8;
}

#else
void RF_setup(void){

 RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
 RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;

//CSN: This is the card select line PB0
 GPIOB->MODER|=GPIO_MODER_MODER0_0;
 GPIOB->OTYPER&=~GPIO_OTYPER_OT_0;
 GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR0_1;
 GPIOB->PUPDR|=GPIO_PUPDR_PUPDR0_0;
 GPIOB->ODR |= GPIO_ODR_ODR_0;//High inactive

 // CE: Chip enable is PB1
 GPIOB->MODER&=~GPIO_MODER_MODER1_0;
 GPIOB->OTYPER&=~GPIO_OTYPER_OT_1;
 GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR1_1;
 GPIOB->PUPDR|=GPIO_PUPDR_PUPDR1_0;
 GPIOB->ODR &=~ GPIO_ODR_ODR_1;//Low inactive


 SPI_init(SPI1, SPI_BAUDRATEPRESCALER_2);

}
void RF_init(void){
	RF_setup();
	RF_writeReg(RF24_RF_SETUP, RF24_RF_SETUP_RF_PWR_0|RF24_RF_SETUP_RF_DR_250);
	RF_writeReg(RF24_RX_PW_P0, SIZE);
	RF_writeReg(RF24_RF_CH, 2);
	RF_WriteRegisterData(RF24_RX_ADDR_P0, (uint8_t*)TADDR, sizeof(TADDR));
	RF_WriteRegisterData(RF24_TX_ADDR, (uint8_t*)TADDR, sizeof(TADDR));
	RF_writeReg(RF24_EN_RXADDR, RF24_EN_RXADDR_ERX_P0);
	RF_ResetStatus(RF24_STATUS_RX_DR|RF24_STATUS_TX_DS|RF24_STATUS_MAX_RT);

}
void RF_writeReg(uint8_t reg, uint8_t val){
RF_CSNlow();
(void)SPI_send_single(SPI1, RF24_W_REGISTER|reg);
(void)SPI_send_single(SPI1, val);
RF_CSNhigh();
delay1(10);
}
void RF_write(uint8_t val){
GPIOB->ODR &=~ GPIO_ODR_ODR_0;
(void)SPI_send_single(SPI1, val);
GPIOB->ODR |= GPIO_ODR_ODR_0;
delay1(10);
}
uint8_t RF_Status(void){
	RF_CSNlow();
	uint8_t temp=SPI_send_single(SPI1, RF24_NOP);
	RF_CSNhigh();
	return temp;
}
void RF_ResetStatus(uint8_t flag){
	delay1(10);
	RF_CSNlow();
	delay1(10);
	RF_writeReg(RF24_STATUS, flag);
	delay1(10);
	RF_CSNhigh();
	delay1(10);
}
void RF_WriteRegisterData(uint8_t reg, uint8_t *buf, uint8_t bufSize){
	RF_CSNlow();
	(void)SPI_send_single(SPI1, RF24_W_REGISTER|reg);
	SPI_send(SPI1, buf, bufSize);
	RF_CSNhigh();
}
void RF_TXPayload(uint8_t *payload, uint8_t size){
	RF_write(RF24_FLUSH_TX);
	RF_WriteRegisterData(RF24_W_TX_PAYLOAD,payload,size);
	RF_CEhigh();
	delay1(10);
	RF_CElow();

}


uint8_t* RF_ReadRegisterData(uint8_t reg, uint8_t * buf, uint8_t size){
	//uint8_t * buf1;
	GPIOB->ODR &=~ GPIO_ODR_ODR_0;
	(void)SPI_send_single(SPI1,RF24_R_REGISTER|reg );
	(void)SPI_sendRead(SPI1, buf, buf, size);
	GPIOB->ODR |= GPIO_ODR_ODR_0;
	delay1(10);
	return buf;
}

void RF_RxPayload(uint8_t *payload, uint8_t size){

	GPIOB->ODR &=~ GPIO_ODR_ODR_1;
	RF_ReadRegisterData(RF24_R_RX_PAYLOAD,payload,size);
	GPIOB->ODR |= GPIO_ODR_ODR_1;
}
void RF_CElow(void){
	 GPIOB->ODR &=~ GPIO_ODR_ODR_1;
}
void RF_CEhigh(void){
	 GPIOB->ODR |= GPIO_ODR_ODR_1;
}
void RF_CSNlow(void){
	 GPIOB->ODR &=~ GPIO_ODR_ODR_0;
}
void RF_CSNhigh(void){
	 GPIOB->ODR |= GPIO_ODR_ODR_0;
}

#endif


void RF_RXEN(void){

	int y=0;
	uint8_t payload1[SIZE];

	RF_writeReg(RF24_CONFIG, RF24_EN_CRC|RF24_CRCO|RF24_PWR_UP|RF24_PRIM_RX);
	RF_CEhigh();

	while(1){
	y=RF_Status();

	if(y &RF24_STATUS_TX_DS){
		RF_ResetStatus(RF24_STATUS_TX_DS);
		//break;
	}
	if(y&RF24_STATUS_MAX_RT){
	    RF_ResetStatus(RF24_STATUS_MAX_RT);
		//break;
	}
	if(y&RF24_STATUS_RX_DR){

		RF_RxPayload(payload1, sizeof(payload1));

		Display(0,3,(char*)payload1,I2C1);
		RF_ResetStatus(RF24_STATUS_RX_DR);
		led_on_green(GPIOB);//Data received
		break;
	}

	}

}

void RF_TXEN(void){
	uint8_t payload[SIZE]="Hello sir";
	int y=0;
	RF_writeReg(RF24_EN_AA, RF24_EN_AA_ENAA_P0);
	RF_writeReg(RF24_SETUP_RETR, RF24_SETUP_RETR_ARD_750|RF24_SETUP_RETR_ARC_15);
	RF_writeReg(RF24_CONFIG, RF24_EN_CRC|RF24_CRCO|RF24_PWR_UP|RF24_PRIM_TX);
	RF_CElow();

	RF_TXPayload(payload, sizeof(payload));


	while(1){
	y=RF_Status();

	if(y &RF24_STATUS_TX_DS){
		RF_ResetStatus(RF24_STATUS_TX_DS);

		led_toggle_red(GPIOB);//Data sent
		break;
	}
	if(y&RF24_STATUS_MAX_RT){
	    RF_ResetStatus(RF24_STATUS_MAX_RT);
	    //led_toggle_red(GPIOB);//Data sent
	    //RF_TXPayload(payload, SIZE);
		//break;
	}
	if(y&RF24_STATUS_RX_DR){
		RF_ResetStatus(RF24_STATUS_RX_DR);

		//break;
	}

	}
}

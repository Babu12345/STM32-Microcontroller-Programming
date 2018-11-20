#include "Extra_Functions.h"

void Start_Clock_HSI_f405(void){
	//The final frequency PLL General CLock is 168 MHZ
	//PLLQ=7
	RCC->PLLCFGR&=~RCC_PLLCFGR_PLLQ;
	RCC->PLLCFGR|=0x07000000;


	RCC->PLLCFGR&=~RCC_PLLCFGR_PLLSRC;//HSI CLOCK SOURCE

	//PLLP=2
	RCC->PLLCFGR&=~RCC_PLLCFGR_PLLP;

	//PLLN=336
	RCC->PLLCFGR&=~RCC_PLLCFGR_PLLN;
	RCC->PLLCFGR|=0x5400;

    //PLLM=16
	RCC->PLLCFGR&=~RCC_PLLCFGR_PLLM;
	RCC->PLLCFGR|=0x10;


	//Prescaler for the AHB clock which is fed into the APBs
	//AHB clock(168MHZ/4)= 42 MHZ
	RCC->CFGR&=~RCC_CFGR_HPRE_DIV512;
	RCC->CFGR|=RCC_CFGR_HPRE_DIV4;

	//(42MHZ/2)= 21 MHZ for APB2; The prescaler for APB2 is x2 for the timers
	RCC->CFGR&=~RCC_CFGR_PPRE2_DIV16;
	RCC->CFGR|=RCC_CFGR_PPRE2_DIV2;

	//(42MHZ/2)= 21 MHZ for APB1; The clock seems to work well. I changed the frequency from 10.5MHZ
	//to be able to access a UART frequency of 115200
	RCC->CFGR&=~RCC_CFGR_PPRE1_DIV16;
	RCC->CFGR|=RCC_CFGR_PPRE1_DIV2;

	RCC->CFGR&=~0x00000003;
	RCC->CFGR|=RCC_CFGR_SW_PLL;//PLL used as a system clock

	RCC->CFGR|=RCC_CFGR_SWS_PLL;


	RCC->CR|=RCC_CR_HSION;

	RCC->CR|=RCC_CR_PLLON;
	SystemCoreClockUpdate();
}

void Start_Clock_HSI_f411(void){

		//The final frequency PLL General CLock is 168 MHZ
		//PLLQ=7
		RCC->PLLCFGR&=~RCC_PLLCFGR_PLLQ;
		RCC->PLLCFGR|=0x07000000;


		RCC->PLLCFGR&=~RCC_PLLCFGR_PLLSRC;//HSI CLOCK SOURCE

		//PLLP=2
		RCC->PLLCFGR&=~RCC_PLLCFGR_PLLP;

		//PLLN=336
		RCC->PLLCFGR&=~RCC_PLLCFGR_PLLN;
		RCC->PLLCFGR|=0x5400;

	    //PLLM=16
		RCC->PLLCFGR&=~RCC_PLLCFGR_PLLM;
		RCC->PLLCFGR|=0x10;


		//Prescaler for the AHB clock which is fed into the APBs
		//AHB clock(168MHZ/4)= 42 MHZ
		RCC->CFGR&=~RCC_CFGR_HPRE_DIV512;
		RCC->CFGR|=RCC_CFGR_HPRE_DIV4;

		//(42MHZ/4)= 21 MHZ for APB2; The prescaler for APB2 is x2 for the timers
		RCC->CFGR&=~RCC_CFGR_PPRE2_DIV16;
		RCC->CFGR|=RCC_CFGR_PPRE2_DIV2;

		//(42MHZ/2)= 21 MHZ for APB1; The clock seems to work well. I changed the frequency from 10.5MHZ
		//to be able to access a UART frequency of 115200
		RCC->CFGR&=~RCC_CFGR_PPRE1_DIV16;
		RCC->CFGR|=RCC_CFGR_PPRE1_DIV2;

		RCC->CFGR&=~0x00000003;
		RCC->CFGR|=RCC_CFGR_SW_PLL;//PLL used as a system clock

		RCC->CFGR|=RCC_CFGR_SWS_PLL;


		RCC->CR|=RCC_CR_HSION;

		RCC->CR|=RCC_CR_PLLON;
		SystemCoreClockUpdate();


}
void Start_Clock_HSE_f405(void){

	//The final frequency PLL General CLock is 168 MHZ

	RCC->CR|=RCC_CR_HSION;//HSI ON
	RCC->CFGR&=~0x00000003;//HSI USED AS THE SYSTEM CLOCK


	RCC->CR&=~RCC_CR_HSEON;//HSE ON
	RCC->CR&=~RCC_CR_HSEBYP;//HSE BYPASS

	RCC->CR&=~RCC_CR_PLLON;//PLL OFF
	RCC->CR&=~RCC_CR_PLLI2SON;//PLLI2S OFF




	RCC->PLLCFGR|=RCC_PLLCFGR_PLLSRC_HSE;//HSE USED AS PLL CLOCK SOURCE

		//PLLQ=7
		RCC->PLLCFGR&=~RCC_PLLCFGR_PLLQ;
		RCC->PLLCFGR|=0x07000000;

		//PLLP=2
		RCC->PLLCFGR&=~RCC_PLLCFGR_PLLP;

		//PLLN=336
		RCC->PLLCFGR&=~RCC_PLLCFGR_PLLN;
		RCC->PLLCFGR|=0x5400;

	    //PLLM=8
		RCC->PLLCFGR&=~RCC_PLLCFGR_PLLM;
		RCC->PLLCFGR|=0x0C;


		//Prescaler for the AHB clock which is fed into the APBs
		//AHB clock(168MHZ/4)= 42 MHZ
		RCC->CFGR&=~RCC_CFGR_HPRE_DIV512;
		RCC->CFGR|=RCC_CFGR_HPRE_DIV4;

		//(42MHZ/4)= 21 MHZ for APB2; The prescaler for APB2 is x2 for the timers
		RCC->CFGR&=~RCC_CFGR_PPRE2_DIV16;
		RCC->CFGR|=RCC_CFGR_PPRE2_DIV2;

		//(42MHZ/2)= 21 MHZ for APB1; The clock seems to work well. I changed the frequency from 10.5MHZ
		//to be able to access a UART frequency of 115200
		RCC->CFGR&=~RCC_CFGR_PPRE1_DIV16;
		RCC->CFGR|=RCC_CFGR_PPRE1_DIV2;


		RCC->CFGR&=~0x00000003;
		RCC->CFGR|=RCC_CFGR_SW_PLL;//PLL USED AS THE SYSTEM CLOCK

		RCC->CR|=RCC_CR_HSEON;//HSI ON
		RCC->CR|=RCC_CR_HSEBYP;//HSI BYPASS



		while(1){
		if(RCC->CR & RCC_CR_HSERDY) break;//HSI CLOCK READY
		}

		RCC->CR|=RCC_CR_PLLON;


		SystemCoreClockUpdate();




}

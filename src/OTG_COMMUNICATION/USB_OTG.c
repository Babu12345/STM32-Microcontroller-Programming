

//This file is under development but may not be implemented because of the huge amount of code needed.
/*
 * USB_OTG.c
 *
 *  Created on: Jul 17, 2017
 *      Author: babu_wanyeki
 */

//The clock configurations for the OTG interface are already in place on the Function.c file
#include "USB_OTG.h"

void OTG_init(void){
//This is the hardware initialization portion of the code



	RCC ->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;



	GPIOA->AFR[1]&=~(0x000FFF00);
	GPIOA->AFR[1]|=(0x000AAA00);


	GPIOA->MODER|=GPIO_MODER_MODER9|
			GPIO_MODER_MODER11_1|GPIO_MODER_MODER12_1|GPIO_MODER_MODER10_1;

	GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR11;
	GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR12;

	GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR10;
	GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR9;



	GPIOA->OTYPER|=GPIO_OTYPER_OT9;
	GPIOA->OTYPER|=GPIO_OTYPER_OT_10;

	GPIOA->OTYPER&=~GPIO_OTYPER_OT_11;
	GPIOA->OTYPER&=~GPIO_OTYPER_OT_12;



	GPIOA->PUPDR  &=~(GPIO_PUPDR_PUPDR9);
	//Also note that the ID pin already has a pull-up
	GPIOA->PUPDR  |=(GPIO_PUPDR_PUPDR10_0);//Pull-up resistor at _0.

	GPIOA->PUPDR  &=~(GPIO_PUPDR_PUPDR11);
	GPIOA->PUPDR  &=~(GPIO_PUPDR_PUPDR12);

	RCC->AHB2ENR|=RCC_AHB2ENR_OTGFSEN;

}

int OTG_DEVICE(void){
	if((USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_CMOD)){
		//This is the A-device mode
		return 0;
	}else{
		//This is the B-Device mode
		return 1;
	}
}
/*void OTG_Enable_Interrupts(void){
	uint32_t prioritygroup = NVIC_GetPriorityGrouping();
	uint32_t priority = NVIC_EncodePriority(prioritygroup, 0,0);
	NVIC_SetPriority(OTG_FS_IRQn, priority);
	NVIC_EnableIRQ(OTG_FS_IRQn);


}*/
int OTG_CORE_Start(void){


	//Restarts multiple control systems and interrupts for the USB
	   USB_OTG_FS->GINTSTS=0x04000020;//Clears the GINTSTS register
	   USB_OTG_FS->GINTMSK=0x00000000;//Clears the GINTMSK register
	   USB_OTG_FS->GOTGINT=0x00000000;//Clears the GOTINT register

	   *((volatile uint32_t*)(0x50000000+0xB00))&=~0x80000000;//Turns off ENP0 OUT

USB_OTG_FS->GRSTCTL|=USB_OTG_GRSTCTL_CSRST;
while(1){
	if(!(USB_OTG_FS->GRSTCTL & USB_OTG_GRSTCTL_CSRST))break;
}
USB_OTG_FS->GRSTCTL|=USB_OTG_GRSTCTL_HSRST;
while(1){
	if(!(USB_OTG_FS->GRSTCTL & USB_OTG_GRSTCTL_HSRST))break;
}
USB_OTG_FS->GRSTCTL|=USB_OTG_GRSTCTL_RXFFLSH;
while(1){
	if(!(USB_OTG_FS->GRSTCTL & USB_OTG_GRSTCTL_RXFFLSH))break;
}
USB_OTG_FS->GRSTCTL|=0x00000400;
USB_OTG_FS->GRSTCTL|=USB_OTG_GRSTCTL_TXFFLSH;
while(1){
	if(!(USB_OTG_FS->GRSTCTL & USB_OTG_GRSTCTL_TXFFLSH))break;
}
USB_OTG_FS->GRSTCTL|=USB_OTG_GRSTCTL_FCRST;
while(1){
	if(!(USB_OTG_FS->GRSTCTL & USB_OTG_GRSTCTL_FCRST))break;
}




		USB_OTG_FS->GAHBCFG|=USB_OTG_GAHBCFG_GINT;//Un-mask the global interrupt
		USB_OTG_FS->GINTMSK|=USB_OTG_GINTMSK_RXFLVLM;
		USB_OTG_FS->GAHBCFG|=USB_OTG_GAHBCFG_PTXFELVL;


		USB_OTG_FS->GUSBCFG|=USB_OTG_GUSBCFG_HNPCAP;//Host negotiation protocol capable
		USB_OTG_FS->GUSBCFG|=USB_OTG_GUSBCFG_SRPCAP;//Session request protocol capable

		//THE TOCAL REGISTER MIGHT NEED TO BE CHANGED TO FIT THE ENUMERATION SPEED
		USB_OTG_FS->GUSBCFG|=0x5;//FS timeout calibration. This number might need to be altered. I just picked the MAX value.
		USB_OTG_FS->GUSBCFG|=0x00003C00;//USB turn around time AHB clock >32MHZ


		USB_OTG_FS->GINTMSK|=USB_OTG_GINTMSK_OTGINT;//OTG interrupt mask
		USB_OTG_FS->GINTMSK|=USB_OTG_GINTMSK_MMISM;//Mode mismatch mask



		if(OTG_DEVICE()){
            //Indicates a B-device(peripheral)
			return 1;

		}else{
		//This indicates a A-device(host)
			//led_on(GPIOB);
		return 0;
		}
}


void OTG_MODULE_START(void){

    //Starts the B-device initiation protocol
	if(OTG_CORE_Start()){



		*((volatile uint32_t*)(0x50000000+0x800))|=USB_OTG_DCFG_DSPD;//Prelimanary Speed of USB enumeration(48 MHZ)
		*((volatile uint32_t*)(0x50000000+0x800))|=USB_OTG_DCFG_NZLSOHSK;//Sends a STALL handshack for a non-zero OUT status length


		USB_OTG_FS->GINTMSK|=USB_OTG_GINTMSK_USBRST;//USB reset un-mask
		USB_OTG_FS->GINTMSK|=USB_OTG_GINTMSK_ENUMDNEM;//Enumeration done un-mask
		USB_OTG_FS->GINTMSK|=USB_OTG_GINTMSK_ESUSPM;//Early suspend un-mask
		USB_OTG_FS->GINTMSK|=USB_OTG_GINTMSK_USBSUSPM;//USB suspend un-mask
	    USB_OTG_FS->GINTMSK|=USB_OTG_GINTMSK_SOFM;//Start of frame un-mask. The host sends this every ms so it may need to be masked so we're not swamped.
		USB_OTG_FS->GINTMSK|=USB_OTG_GINTMSK_RXFLVLM;//Receive buffer empty un-mask register



		USB_OTG_FS->GCCFG|=USB_OTG_GCCFG_VBUSBSEN;//V-BUS hardware sensing for a B-device
		USB_OTG_FS->GCCFG|=USB_OTG_GCCFG_PWRDWN;//The power down procedure is deactivated.



		while(1){
		//Session request sent to the Device
	   if(USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_SRQINT){//Session request test
		   USB_OTG_FS->GINTSTS|=USB_OTG_GINTSTS_SRQINT;//Clears the session request status interrupt

	    	break;
	    }
		}
		//B-device connection confirmed
		while(1){
			   if(USB_OTG_FS->GOTGCTL &USB_OTG_GOTGCTL_BSVLD)break;

				}
while(1){
	if(USB_OTG_FS->GOTGCTL &USB_OTG_GOTGCTL_CIDSTS)break;//B-device connector ID status
}


	}

	else{//Programming if this was a host


	}
}


void USB_RESET(void){
//One potential problem is interrupt masks and also any clock that I may need to initialize.
//I may also need to disable any of the other clocks connected to this peripheral.
	if((USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_USBRST)& (USB_OTG_FS->GINTMSK & USB_OTG_GINTMSK_USBRST)){



		Display(0,2,"USB Reset",I2C2);
		USB_OTG_FS->GINTSTS|= USB_OTG_GINTSTS_USBRST;
		*((unsigned int*)(0x50000000+0xB00))&=~0x80000000;//Turns off ENP0 OUT



		*((volatile uint32_t*)(0x50000000+0xB00))|=0x08000000;//SNAK for ENP0 OUT
		*((volatile uint32_t*)(0x50000000+0x900))|=0x08000000;//SNAK for ENP0 IN
		*((volatile uint32_t*)(0x50000000+0xB00+(1*0x20)))|=0x08000000;//SNAK for ENP1 OUT
		*((volatile uint32_t*)(0x50000000+0x900+(1*0x20)))|=0x08000000;//SNAK for ENP1 IN
		*((volatile uint32_t*)(0x50000000+0xB00+(2*0x20)))|=0x08000000;//SNAK for ENP2 OUT
		*((volatile uint32_t*)(0x50000000+0x900+(2*0x20)))|=0x08000000;//SNAK for ENP2 IN
		*((volatile uint32_t*)(0x50000000+0xB00+(3*0x20)))|=0x08000000;//SNAK for ENP3 OUT
		*((volatile uint32_t*)(0x50000000+0x900+(3*0x20)))|=0x08000000;//SNAK for ENP3 IN

		//These three register operations un-mask 6 interrupts
		*((volatile uint32_t*)(0x50000000+0x81C))|=0x00010001;
		*((volatile uint32_t*)(0x50000000+0x814))|=0x00000009;
		*((volatile uint32_t*)(0x50000000+0x810))|=0x00000009;

//The endpoint needs to be disabled before this point




		USB_OTG_FS->GRXFSIZ&=~0xFFFF;
		USB_OTG_FS->GRXFSIZ|=28;//ENP0 OUT(RxFIFO) can receive 28(32-bit) words. The minimum is 28 words or 64 bytes+8 bytes+40 bytes.

		USB_OTG_FS->DIEPTXF0_HNPTXFSIZ&=~0xFFFF;
		USB_OTG_FS->DIEPTXF0_HNPTXFSIZ|=0x001000000;//ENP0 IN can transmit 16 (32-bit) words. 16 words, or 64 bytes, is the least possible .
         //The ENP0 starting RAM address is with the offset 0.



		*((volatile uint32_t*)(0x50000000+0xB10))|=(0x3<<29) | (0x1<<19);//ENP0 can receive 3 back-back setup packets. 1 packet per transfer size

		*((volatile uint32_t*)(0x50000000+0xB10))&=~0x7f;
		*((volatile uint32_t*)(0x50000000+0xB10))|=0x8;//ENP0 transfer size is 8 bytes

while(1){
	if((USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_ENUMDNE)){

		break;
	}
}


		}



}

void USB_ENUM(void){
	if((USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_ENUMDNE)& (USB_OTG_FS->GINTMSK & USB_OTG_GINTMSK_ENUMDNEM)){

		Display(0,3,"USB ENUM",I2C2);


			USB_OTG_FS->GINTSTS|=USB_OTG_GINTSTS_ENUMDNE;




            while(1){
			if(*((volatile uint32_t*)(0x50000000+0x808)) & 0x00000005)break;//ENUM speed is operating with FS at 48 MHZ
			}


				*((volatile uint32_t*)(0x50000000+0x900))&=~(0x00000003);//Max ENP0 IN size: 64 bytes
				*((volatile uint32_t*)(0x50000000+0xB00))&=~(0x00000003);//ENP0 Max OUT size: 64 bytes

				*((volatile uint32_t*)(0x50000000+0xB00))|= 0x80000000;//ENP0 OUT enable
				//*((uint32_t*)(0x50000000+0x900))|= 0x80000000;//ENP0 IN enable

				//*((uint32_t*)(0x50000000+0xB00+(1*0x20)))|= 0x80000000;//ENP1 OUT enable
				//*((uint32_t*)(0x50000000+0xB00+(2*0x20)))|= 0x80000000;//ENP2 OUT enable
				//*((uint32_t*)(0x50000000+0xB00+(3*0x20)))|= 0x80000000;//ENP3 OUT enable
				//*((uint32_t*)(0x50000000+0x900+(1*0x20)))|= 0x80000000;//ENP1 IN enable
				//*((uint32_t*)(0x50000000+0x900+(2*0x20)))|= 0x80000000;//ENP2 IN enable
				//*((uint32_t*)(0x50000000+0x900+(3*0x20)))|= 0x80000000;//ENP3 IN enable


				//*((uint32_t*)(0x50000000+0xB00))|= 0x00008000;//ENP0 OUT USB active
				//*((uint32_t*)(0x50000000+0x900))|= 0x00008000;//ENP0 IN USB active

				//*((uint32_t*)(0x50000000+0xB00+(1*0x20)))|= 0x00008000;//ENP1 OUT USB active
				//*((uint32_t*)(0x50000000+0x900+(1*0x20)))|= 0x00008000;//ENP1 IN USB active
				//*((uint32_t*)(0x50000000+0xB00+(2*0x20)))|= 0x00008000;//ENP2 OUT USB active
				//*((uint32_t*)(0x50000000+0x900+(2*0x20)))|= 0x00008000;//ENP2 IN USB active
				//*((uint32_t*)(0x50000000+0xB00+(3*0x20)))|= 0x00008000;//ENP3 OUT USB active
				//*((uint32_t*)(0x50000000+0x900+(4*0x20)))|= 0x00008000;//ENP3 IN USB active



while(1){
	if((USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_RXFLVL)){

		break;
	}

}


		}
}


void GET_DESCRIPTOR(void){
	//Sees if the TxFIFO for ENP0 is empty and then writes data into it.



	                *((volatile uint32_t*)(0x50000000+0x910))|=0x12;//Send 18 bytes
	                *((volatile uint32_t*)(0x50000000+0x910))|=0x00080000;//Packet=1

		         	*((volatile uint32_t*)(0x50000000+0x900))|= 0x04000000;//ENP0 clear NAK bit
		            *((volatile uint32_t*)(0x50000000+0x900))|= 0x80000000;//ENP0 IN enable

	                while(1){
	                	 if(*((volatile uint32_t*)(0x50000000+0x908)) & 0x80)break;//This is to see if the TxFIFO is open
	                }


	                *((volatile uint32_t*)(0x50000000+0x1000))|=0x12011001;
	                *((volatile uint32_t*)(0x50000000+0x1004))|=0x00000040;
	                *((volatile uint32_t*)(0x50000000+0x1008))|=0x620F0110;
	                *((volatile uint32_t*)(0x50000000+0x100C))|=0x01000102;
	                *((volatile uint32_t*)(0x50000000+0x1010))|=0x0001;


	                CONTROL_SEND_TEST();

//led_on(GPIOB);




}

//The data setup command seems to be the SET_ADDRESS. All that needs to be done is to read the correct address.
//ENP 0 has the address offset 0x1000 - 0x1FFC for reading and writing to the TxFifo or the RxFifo.
//Each register will be able to hold 4 bytes.
void USB_RECIEVE_NEMPTY(void){
	if((USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_RXFLVL)  & (USB_OTG_FS->GINTMSK & USB_OTG_GINTMSK_RXFLVLM)){


		    uint32_t t1[4];
			uint32_t t = USB_OTG_FS->GRXSTSP;
			USB_OTG_FS->GINTMSK&=~ USB_OTG_GINTMSK_RXFLVLM;




if(((t>>4)&0x7FF)==8){
// 8 bytes data size

	if(((t>>17)& 0xF)==6){

		//led_on(GPIOB);
		t1[0]= *((volatile uint32_t*)(0x50001000));
		t1[1]= *((volatile uint32_t*)(0x50001004));
		t1[2]= *((volatile uint32_t*)(0x50001008));
		t1[3]= *((volatile uint32_t*)(0x5000100C));

		if((((t1[0]>>8)&0xff)==5)){

		}

	}



}



if(((*((volatile uint32_t*)(0x50000000+0xB10))>>29)& 0x3)==1){
	led_on_blue(GPIOB);
}




//Setup stage done
if((*((volatile uint32_t*)(0x50000000+0xB08)) & 0x8) ){
	//led_on(GPIOB);




	          //Tests if the last command was a SetAddress packet
			 if(((t1[0]>>8)&0xff)==5){
				 *((volatile uint32_t*)(0x50000000+0x808))|=((t1[0]>>16)&0xffff)<<4;



}

			 }


USB_OTG_FS->GINTMSK|=USB_OTG_GINTMSK_RXFLVLM;
}
	 }




void USB_SOF(void){

	if(USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_SOF){

		USB_OTG_FS->GINTSTS|=USB_OTG_GINTSTS_SOF;

		led_on_blue(GPIOB);



	}
}


void SESSION_END(void){

	if(USB_OTG_FS->GOTGINT & USB_OTG_GOTGINT_SEDET){
		Display(0,4,"SESSION END!!",I2C2);
	}



}

void SETUP_DONE(void){

	if(*((volatile uint32_t*)(0x50000000+0x818))){

		//led_on(GPIOB);

	}
}

void CONTROL_SEND_TEST(void){
	//This is a send test only for control ENP0
    while(1){
   	 if(USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_IEPINT)break;
    }

    while(1){
   	 if(*((volatile uint32_t*)(0x50000000+0x818)) & 0x1)break;
    }
    while(1){
     if(*((volatile uint32_t*)(0x50000000+0x908)) & 0x1)break;
    }
}



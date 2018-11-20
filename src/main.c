/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/DISPLAY/Display.h"
#include "../src/CAMERA/Camera.h"
#include "../src/CAMERA/JPEG_CONVERTER.h"
#include "../src/ADC/ADC.h"
#include "../src/FUNCTIONS/Functions.h"
#include "../src/FUNCTIONS/Extra_Functions.h"
#include "../src/UART/UART.h"
#include "../src/SDCARD/ff.h"
#include "../src/OTG_COMMUNICATION/USB_OTG.h"
#include "../src/RF_COMM/RF.h"
#include "../src/PWM/PWM.h"
#include "../src/WIFI/ESP8266.h"
//Code that interacts and with the SD card and displays messages

int main(void)
{



	FATFS FatFs;
	FIL Fil;
	Start_Clock_HSI_f411();
	GPIO_enable();
	Display_oled_init(I2C2,0x78);
	clear(buffer2);
	Display_draw(I2C2,0x78,buffer2);
	//Usart_enable(USART2,115200);
    //send_string_ln("Accessing the SD card",USART2);

	Display(0,0,"Starting...!!",I2C2);


		if(f_mount(&FatFs,"", 1)!=FR_OK)		// Give a work area to the default drive
			{

				Display(0,1,"CARD NOT MOUNTED!!",I2C2);

			}else{

				Display(0,1,"CONNECTED TO SD CARD",I2C2);

				if (f_open(&Fil, "NEW_FILE.txt", FA_WRITE|FA_CREATE_ALWAYS) == FR_OK) {
					Display(0,2,"FILE Created",I2C2);
					led_on_blue(GPIOB);


				}else{
					Display(0,2,"FILE failed to open",I2C2);
				}
				Display(0,5,"||CLOSING FILE||",I2C2);
				f_close(&Fil);
				f_mount(NULL, "", 1);
			}





	while(1);


}

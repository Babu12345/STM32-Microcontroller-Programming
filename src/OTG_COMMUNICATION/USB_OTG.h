/*
 * USB_OTG.h
 *
 *  Created on: Jul 17, 2017
 *      Author: babu_wanyeki
 */

#ifndef OTG_COMMUNICATION_USB_OTG_H_
#define OTG_COMMUNICATION_USB_OTG_H_
#include "stm32f405xx.h"
#include "../src/FUNCTIONS/Functions.h"
#include "../src/DISPLAY/Display.h"

void OTG_init(void);
int OTG_test(void);
int OTG_DEVICE(void);
void OTG_Enable_Interrupts(void);
int OTG_CORE_Start(void);
void OTG_MODULE_START(void);
void USB_RESET(void);
void USB_ENUM(void);
void USB_RECIEVE_NEMPTY(void);
void USB_SOF(void);
void SESSION_END(void);
void SETUP_DONE(void);
void GET_DESCRIPTOR(void);
void CONTROL_SEND_TEST(void);

#endif
/* OTG_COMMUNICATION_USB_OTG_H_ */

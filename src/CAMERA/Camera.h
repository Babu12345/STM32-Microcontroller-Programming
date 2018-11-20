#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../SDCARD/ff.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../UART/UART.h"
#include "../DISPLAY/Display.h"

FATFS FatFs;
FIL Fil;
UINT bw;

void Camera_init(uint32_t baudrate);
void Camera_speed(void);
int reply(char *buffer, int b);
int Camera_reset(void);
void version(void);
int Camera_read(uint16_t size);
int capture(void);
uint16_t length(void);
void Camera_stop(void);
int image_size(int pixel_number_register);
void compress(void);
int min(int x, int y);
void getimagesize(void);
void POWER_save_disable(void);
void Stop_motion();
void delete_range(char *array, int a, int b, int c);
#endif

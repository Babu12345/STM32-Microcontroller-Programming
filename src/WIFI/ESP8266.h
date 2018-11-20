
#ifndef _ESP8288_H_
#define _ESP8266_H_
#include "../UART/UART.h"
#include "../src/FUNCTIONS/Functions.h"
#include <string.h>

void init(void);
void restart();

void set(const char* arg, int *data, int size);
void read_data(void);
#endif

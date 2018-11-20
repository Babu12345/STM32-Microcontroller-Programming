#include "Display.h"
#include "Font.h"
#include <string.h>
void Display_draw(I2C_TypeDef *I2Cx, uint32_t address, uint32_t *buffer){
	int x,y=0;
	i2c_command(I2Cx,address,Cmd, 0x21);
	i2c_command(I2Cx,address,Cmd, 0x00);
	i2c_command(I2Cx,address,Cmd, 127);

	i2c_command(I2Cx,address,Cmd, 0x22);
	i2c_command(I2Cx,address,Cmd, 0x00);
	i2c_command(I2Cx,address,Cmd, 0x07);



	while(I2Cx->SR2&I2C_SR2_BUSY){}

	I2Cx->CR1 |=I2C_CR1_START;

	while(!(I2Cx->SR1&I2C_SR1_SB)){}
	I2Cx->DR = address;
	while(!(I2Cx->SR1&I2C_SR1_ADDR)){}
	while(!(I2Cx->SR2&I2C_SR2_MSL)){}

	while(!(I2Cx->SR1&I2C_SR1_TXE)){}
	I2Cx->DR = 0x40;
	while(!(I2Cx->SR1&I2C_SR1_BTF)){}

	for(x=0;x<8;x++){

		for(y=0;y<128;y++){
			while(!(I2Cx->SR1&I2C_SR1_TXE)){}
			I2Cx->DR  = buffer[(128*x)+y];
			while(!(I2Cx->SR1&I2C_SR1_BTF)){}



		}


	}

	I2Cx->CR1 |=I2C_CR1_STOP;

}

void Display_oled_init(I2C_TypeDef *I2Cx, uint32_t ssd1306_slave_address){
		i2c_enable(I2Cx);
	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0xAE);

	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0x00 | 0x0);      // low col = 0

	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0x10 | 0x0);      // hi col = 0
	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0x40 | 0x0);      // line #0

	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0x81);            // Set Contrast 0x81
	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0xCF);
	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0xA1);            // Segremap - 0xA1
	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0xC8);            // COMSCAN DEC 0xC8 C0
	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0xA6);            // Normal Display 0xA6 (Invert A7)

	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0xA4);            // DISPLAY ALL ON RESUME - 0xA4
	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0xA8);            // Set Multiplex 0xA8
	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0x3F);            // 1/64 Duty Cycle

	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0xD3);            // Set Display Offset 0xD3
	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0x0);             // no offset

	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0xD5);            // Set Display Clk Div 0xD5
	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0x80);            // Recommneded resistor ratio 0x80

	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0xD9);            // Set Precharge 0xd9
	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0xF1);

	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0xDA);            // Set COM Pins0xDA
	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0x12);

	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0xDB);            // Set VCOM Detect - 0xDB
	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0x40);

	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0x20);            // Set Memory Addressing Mode
	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0x00);            // 0x00 - Horizontal

	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0x40 | 0x0);      // Set start line at line 0 - 0x40

	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0x8D);            // Charge Pump -0x8D
	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0x14);


	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0xA4);            //--turn on all pixels - A5. Regular mode A4
	    i2c_command(I2Cx, ssd1306_slave_address,Cmd, 0xAF);           //--turn on oled panel - AF


}

void Display_char(uint32_t x, int page,char char_1, uint32_t *buffer){
	int x1;
	uint32_t y;

	y=x+((page)*128);

	for(x1=0;x1<5;x1++){
		buffer[y+x1]= Ascii_8x5_font[char_1-32][x1];
	}

}

void Display_string(uint32_t x, int page, char *string, uint32_t *buffer){
	int x1=0;

	while(string[x1]!=0){

		Display_char(x+(5*x1),page, string[x1],buffer);
		x1++;

	}
}

void Display(uint32_t x , int page, char *string, I2C_TypeDef *I2Cx){
	Display_string(x,page,string,buffer2);
	Display_draw(I2Cx,0x78,buffer2);


}
void clear(uint32_t *buffer){
// Clear all bytes of the 128x64 display buffer given by buffer_pointer
    uint8_t x, page_index = 0;

    for(page_index=0; page_index < 8; page_index++){
        for(x=0; x<128; x++){
            buffer[(128*page_index)+x] = 0x00;
        }
    }
    //memset(buffer,0,((128*64)/8));
}

#include "Camera.h"
#include "../src/FUNCTIONS/Functions.h"
#include "../src/SDCARD/ff.h"

//The data here will all be read by interrupts
void Camera_init(uint32_t baudrate){

	Usart_enable(UART4, baudrate);

}

int min(int x,int y){

	if(x==y){
		return x;
	}

	if(x<y){
		return x;
	}
	else{
		return y;
	}

}


void delete_range(char *array, int a, int b,int c){

    c++;
    for(int x=a;x<c;x++){
        array[x]=array[b];
        b++;
        if(b==c){
        	break;
        }
    }


}
int reply(char *buffer, int b){

	if(((int)buffer[0]==0x76)&&((int)buffer[1]==0x00)&&(
			(int)buffer[2]==b)&&((int)buffer[3]==0x00)){
        return 1;
	}
	else{
		return 0;
	}


}

int Camera_reset(){



	char b[58];
	int c[4];
	c[0]=0x56;
	c[1]=0x00;
	c[2]=0x26;
	c[3]=0x00;

	delay1(10);
send(c,4,UART4);
//This function returns 58 values

for(int y=0;y<58;y++){

	while(1){

		if((UART4->SR & USART_SR_RXNE)){
			b[y]=(char)(UART4->DR);
			break;
		}

	}


			}


if(reply(b,0x26)){
	return 1;
}
return 0;

}

int capture(void){




    char b[5];
    int c[5];
    c[0]=0x56;
    c[1]=0x00;
    c[2]=0x36;
    c[3]=0x01;
    c[4]=0x00;

	delay1(10);
    send(c,5,UART4);

	for(int y=0;y<5;y++){
		while(1){

			if((UART4->SR & USART_SR_RXNE)){
				b[y]=(char)(UART4->DR);
				break;
			}

		}


			}


	if(reply(b,0x36)){

		return 1;
	}
	return 0;

}
uint16_t length(void){


	uint16_t size=0;

	char b[9];

	int c[5];
	c[0]=0x56;
	c[1]=0x00;
	c[2]=0x34;
	c[3]=0x01;
	c[4]=0x00;
	delay1(10);
    send(c,5,UART4);
		for(int y=0;y<9;y++){

			while(1){

				if((UART4->SR & USART_SR_RXNE)){
					b[y]=(char)(UART4->DR);
					break;
				}

			}



		}
if(!reply(b,0x34)){
	return 0;
}

    size=(int)b[5];
    size<<=8;
    size|=(int)b[6];
    size<<=8;
    size|=(int)b[7];
    size<<=8;
    size|=(int)b[8];





return size;
}

int image_size(int pixel_number_register){
	int c[9];
	char b[5];

	c[0]=0x56;
	c[1]=0x00;
	c[2]=0x31;
	c[3]=0x05;
	c[4]=0x04;
	c[5]=0x01;
	c[6]=0x00;
	c[7]=0x19;
	c[8]=pixel_number_register;//0x11-(320*240);0x00-(640*480)=;0x22-(160*120)
	delay1(10);
	send(c,9,UART4);
	//This function returns 5 values

	for(int y=0;y<5;y++){

				while(1){

					if((UART4->SR & USART_SR_RXNE)){
						b[y]=(char)(UART4->DR);
						break;
					}

				}



			}
if(reply(b,0x31)){
	return 1;
}
return 0;

}


void getimagesize(void){
	int c[8];
	c[0]=0x56;
	c[1]=0x00;
	c[2]=0x30;

	c[3]=0x4;
	c[4]=0x4;
	c[5]=0x1;
	c[6]=0x00;
	c[7]=0x19;

	send(c,8,UART4);

	//This function returns 6 values



}
void Camera_speed(void){
	//Set the Baudrate
	//9600  AE C8
	//19200 56 E4
	//38400 2A F2
	//57600 1C 4C
	//115200 0D A6


	int c[7];


	c[0]=0x56;
	c[1]=0x00;
	c[2]=0x31;
	c[3]=0x06;
	c[4]=0x04;
	c[5]=0x02;
	c[6]=0x00;
	c[7]=0x08;
	c[8]=0x0D;
	c[9]=0xA6;


	send(c,10,UART4);



}



void version(void){


	int c[4];


	c[0]=0x56;
	c[1]=0x00;
	c[2]=0x11;
	c[3]=0x00;



	send(c,4,UART4);

//This function returns 16 values

}



void Camera_stop(void){
	int c[5];


	c[0]=0x56;
	c[1]=0x00;
	c[2]=0x36;
	c[3]=0x01;
	//0x02 is to resume
	//0x03 is to step frame
	//0x01 is to stop next frame
	c[4]=0x02;

	send(c,5,UART4);
//This function returns 5 values





}


void compress(void){

//The value of y should be between 00 and FF
//The default value of y is 36
int c[9];


c[0]=0x56;
c[1]=0x00;
c[2]=0x31;
c[3]=0x05;
c[4]=0x01;
c[5]=0x01;
c[6]=0x12;
c[7]=0x04;
c[8]=36;

send(c,9,UART4);

//This function returns 5 values



}

int Camera_read(uint16_t size){

   FATFS FatFs;
   FIL Fil;
   UINT bw;

   uint16_t add=0;
   int inc=1000;
   uint16_t temp=0;
   uint16_t x=0;
   uint16_t z=0;
   uint16_t y=0;

   int c[16];
//If the camera size is larger than 320*240 then the image buffer won't be able to store all of the data in one variable for the Heap

   char *b=(char*)malloc(min(size, inc)*sizeof(char*));
   char *buffer=(char*)malloc(min(size, inc)*sizeof(char*));
   f_mount(&FatFs,"", 1);
   if (f_open(&Fil,"Test1.jpg", FA_WRITE|FA_CREATE_ALWAYS) == FR_OK) {

   while(add<size){
   temp=min(size-add, inc);

   c[0]=0x56;
   c[1]=0x00;
   c[2]=0x32;
   c[3]=0x0c;
   c[4]=0x00;
   c[5]=0x0a;

   c[6]=(add>>24)&0xff;
   c[7]=(add>>16)&0xff;
   c[8]=(add>>8)&0xff;
   c[9]=(add&0xff);


   c[10]=(temp>>24)&0xff;
   c[11]=(temp>>16)&0xff;
   c[12]=(temp>>8)&0xff;
   c[13]=(temp&0xff);



   c[14]=0x00;
   c[15]=0x01;

   delay1(10);
   send(c,16,UART4);


   for(y=0;y<(temp+10);y++){
	   while(1){

	   		if((UART4->SR & USART_SR_RXNE)){
	   			b[y]=(char)(UART4->DR);
	   			break;
	   		}

	   }



   }

   if(add==0){
	   if(!reply(b,0x32)){
		   free(b);
		   free(buffer);
		   f_mount(NULL, "", 1);
		   return 0;
	   }


	   delete_range(b, 0, 5, temp+10);
	   delete_range(b, temp+5, temp+10, temp+10);

	   f_write(&Fil, b, temp, &bw);

	   for(x=0;x<(temp);x++,z++){
	   	   buffer[z]=b[x];
	      }


   }else{
	   delete_range(b, 0, 5, temp+10);
	   delete_range(b, temp+5, temp+10, temp+10);
	   f_write(&Fil, b, temp, &bw);





   }



   add+=temp;
   memset(b,'\0',(temp+10));


   }


   f_close(&Fil);
   f_mount(NULL,"",1);

   if(((int)buffer[0]==0xff)&((int)buffer[1]==0xd8)
		   &((int)buffer[2]==0xff)&((int)buffer[3]==0xfe)){
   Display(0,5,"Image reading done1!",I2C2);

   }




      free(b);
      free(buffer);
      f_mount(NULL,"",1);
      return 1;



   }









   f_mount(NULL, "", 1);
   free(b);
   free(buffer);
   return 0;

}


void POWER_save_disable(void){


	int c[7];
	c[0]=0x56;
	c[1]=0x00;
	c[2]=0x3E;
	c[3]=0x03;
	c[4]=0x00;
	c[5]=0x01;
	c[6]=0x00;
	send(c,7,UART4);


}

void Resume_motion(){

	    int c[7];
		c[0]=0x56;
		c[1]=0x00;
		c[2]=0x37;
		c[3]=0x01;
		c[4]=0x00;

		send(c,7,UART4);




}


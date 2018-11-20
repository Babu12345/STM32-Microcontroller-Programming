#include "ESP8266.h"



void init(void){
	Usart_enable(USART6,115200);
	Usart_enable(USART2,115200);
	send_string_ln("Start Communication:\n",USART2);
    int a[40];



    set("AT",a,2);
    send(a,4,USART6);
    read_data();

    set("AT+RST",a,6);
    send(a,8,USART6);
    read_data();


    set("AT+RST",a,6);
    send(a,8,USART6);
    read_data();

    set("AT",a,2);
    send(a,4,USART6);
    read_data();

    //set("AT+CIPMODE=0",a,12);
    //send(a,14,USART6);
    //read_data();
    //delay1(1000);

    //set("AT+CWMODE=1",a,11);
    //send(a,13,USART6);
    //read_data();
    //delay1(1000);

    //send(b,19,USART6);
    //read_data();
    //delay1(1000);

    set("AT+CIPMUX=1",a,11);
    send(a,13,USART6);

    set("AT+CIPMUX=1",a,11);
    send(a,13,USART6);

    set("AT+CIPSERVER=1,80",a,17);
    send(a,19,USART6);
    delay1(1000);

    //send(d,8,USART6);
    //delay1(1000);


/*


    set("AT+CIPSERVER=1,80",a,16);
    send(a,18,USART6);
    read_data();


    set("AT+CIPSEND=0,21",a,15);
    send(a,17,USART6);


    set("<TITLE>HELLO<TITLE>",a,19);
    send(a,21,USART6);

*/
    while(1){
    	send_data(recieve_data(USART6),USART2);
    }



}

void set(const char* arg, int *data, int size){

	memset(data,0,40*sizeof(int));
	for(int x=0;x<size;x++){
		data[x]=(int)arg[x];

	}
	data[size]=0x0d;
	data[size+1]=0x0a;

}

void read_data(void){
	char b;
	char info[4];
	while(1){
		b=recieve_data(USART6);
		if(b==(char)0x4f) info[0]='O';
		if(b==(char)0x4b) info[1]='K';

	    send_data(b,USART2);

	    if(strcmp(info,"OK")==0){
	    	break;
	    }

	 }
}
void restart(void){




}

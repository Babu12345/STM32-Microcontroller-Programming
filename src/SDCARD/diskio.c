/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs (C)ChaN, 2013 */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be */
/* attached to the FatFs via a glue function rather than modifying it. */
/* This is an example of glue functions to attach various existing */
/* storage control module to the FatFs module with a defined API. */
/*-----------------------------------------------------------------------*/
#include "../SDCARD/diskio.h" /* FatFs lower layer API */
/* MMC/SD command (SPI mode) */
#define CMD0 (0) /* GO_IDLE_STATE */
#define CMD1 (1) /* SEND_OP_COND */
#define ACMD41 (41) /* SEND_OP_COND (SDC) */
#define CMD8 (8) /* SEND_IF_COND */
#define CMD9 (9) /* SEND_CSD */
#define CMD10 (10) /* SEND_CID */
#define CMD12 (12) /* STOP_TRANSMISSION */
#define CMD13 (13) /* SEND_STATUS */
#define ACMD13 (13) /* SD_STATUS (SDC) */
#define CMD16 (16) /* SET_BLOCKLEN */
#define CMD17 (17) /* READ_SINGLE_BLOCK */
#define CMD18 (18) /* READ_MULTIPLE_BLOCK */
#define CMD23 (23) /* SET_BLOCK_COUNT */
#define ACMD23 (23) /* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24 (24) /* WRITE_BLOCK */
#define CMD25 (25) /* WRITE_MULTIPLE_BLOCK */
#define CMD32 (32) /* ERASE_ER_BLK_START */
#define CMD33 (33) /* ERASE_ER_BLK_END */
#define CMD38 (38) /* ERASE */
#define CMD55 (55) /* APP_CMD */
#define CMD58 (58) /* READ_OCR */
void sdc_assert(void)
{
 GPIOA->ODR &= ~GPIO_ODR_ODR_4;
 SPI_send_single(SPI1, 0xFF);
}
void sdc_deassert(void)
{
 GPIOA->ODR |= GPIO_ODR_ODR_4;
 SPI_send_single(SPI1, 0xFF);
} 
uint8_t sdc_isConn(void)
{
if (GPIOB->IDR & GPIO_IDR_IDR_0)
 {
 return 1;
 }
else
 {
 return 0;
 }
}
uint8_t sdc_getResponse(uint8_t response)
{
//delay1(100);
	uint8_t b;
	//char a[100];
for(uint8_t n = 0; n < 8; n++)
 {
	b=SPI_receive_single(SPI1);
    //sprintf(a,"%02x",b);//Used for reading the response data
 //send_string_ln(a,USART2);
 if (b == response)
 {
 return 0;
 
 }
 }
return 1;
}
void sdc_sendCommand(uint8_t command, uint32_t part1)
{
BYTE n;
int i;
//sdc_deassert();
 sdc_assert();
 SPI_send_single(SPI1,(0x40 | command));
 SPI_send_single(SPI1,(BYTE)(part1>>24));
 SPI_send_single(SPI1,(BYTE)(part1>>16));
 SPI_send_single(SPI1,(BYTE)(part1>>8));
 SPI_send_single(SPI1,(BYTE)(part1));
 n=0x01;
if ((command == 0))
 {
 n=(0x95);// precalculated CRC
 }
if((command==8)){
 n=(0x87);// precalculated CRC
 }
 SPI_send_single(SPI1,n); 
if(command==12){
 for(i=0;i<8;i++){
 SPI_send_single(SPI1,0xFF);
 }
 }
}
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
BYTE pdrv /* Physical drive number (0..) */
)
{
	//B0,A7,A6,A5,A4
 RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
 RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;
//This is the card select line
 GPIOA->MODER|=GPIO_MODER_MODER4_0;
 GPIOA->OTYPER&=~GPIO_OTYPER_OT_4;
 GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR4_1;
 GPIOA->PUPDR|=GPIO_PUPDR_PUPDR4_0;
 GPIOA->ODR |= GPIO_ODR_ODR_4;
 // initialize card detect line
 // card detect is PB0
 GPIOB->MODER&=~GPIO_MODER_MODER0;
 GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR0_1;
 GPIOB->PUPDR|=GPIO_PUPDR_PUPDR0_0;
 
 //The SPI clock frequency should be around 100-400 kHZ during initilization
 //The APB clock frequency here is 10.5 MHZ
 //So to get a SPI clock speed below 400KHZ the prescaler must be 32
 //SPI clock = 328 kHZ
 SPI_init(SPI1, SPI_BAUDRATEPRESCALER_2);
 // check whether SDC is inserted into socket
 if (!sdc_isConn())
 {
 return STA_NODISK;
 }
 // assert SDC
 sdc_deassert();
 sdc_assert();
 // send 10 dummy bytes to wake up SDC
 for (uint8_t i = 0; i < 10; i++)
 { 
 SPI_send_single(SPI1, 0xFF);
 }
//IDLE mode CMD0
 sdc_sendCommand(CMD0,0);
 if (sdc_getResponse(0x01))
 {
 return STA_NOINIT;
 }
 //CMD8
 sdc_sendCommand(CMD8,0x1AA);
 if (sdc_getResponse(0x01))
 {
 return STA_NOINIT;
 }
 for(int x=0;;x++){
 sdc_sendCommand(CMD55,0);
 SPI_send_single(SPI1, 0xFF);
 //sdc_deassert();
 SPI_send_single(SPI1, 0xFF);
 sdc_sendCommand(ACMD41,1UL<<30);
 if (!sdc_getResponse(0x00))
 {
 break;
 }
 }
 SPI_send_single(SPI1, 0xFF);

 //The SPI clock frequency should be around 0-25 MHZ during the normal operating mode
 //So in order to increase the clock speed the APB2 frequency needs to increase. This means that the SystemClock variable must also
 //be modified in order to match the new frequency.
 //The frequency should be at around 10.5 MHZ at normal operating mode
 //The maximum SPI clock frequency can be changed to 0-50 MHZ by switching to high speed mode
 //This switch can be done by using the CMD6 command found on http://users.ece.utexas.edu/~valvano/EE345M/SD_Physical_Layer_Spec.pdf

 SPI_init(SPI1, SPI_BAUDRATEPRESCALER_32);
 //sdc_deassert();
 return 0;
} 
/*-----------------------------------------------------------------------*/
/* Get Disk Status */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
BYTE pdrv /* Physical drive number (0..) */
)
{
return !sdc_isConn();
}
/*-----------------------------------------------------------------------*/
/* Read Sector(s) */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
BYTE pdrv, /* Physical drive nmuber (0..) */
BYTE *buff, /* Data buffer to store read data */
DWORD sector, /* Sector address (LBA) */
UINT count /* Number of sectors to read (1..255) */
)
{
if (sdc_isConn()==1){
unsigned char buf[2];
// check if parameters are in valid range
if(count>255){
 }
//sdc_assert(); // assert SDC
// if multiple sectors are to be read
if(count==1){
 sdc_sendCommand(CMD17,sector);
 while(sdc_getResponse(0x00)); // wait for command acknowledgement
 while(sdc_getResponse(0xFE)); // wait for data token 0xFE
 SPI_receive(SPI1, buff, 512); // receive data
 SPI_receive(SPI1, buf, 2); // receive two byte CRC
 }
else{
 send_string_ln("TEST CODE!!!!...",USART2);
 sdc_sendCommand(CMD18,sector);
 while(sdc_getResponse(0x00)); // wait for command acknowledgement
 while(count){ 
 while(sdc_getResponse(0xFE)); // wait for data token 0xFE
 SPI_receive(SPI1, buff, 512); // receive data
 SPI_receive(SPI1, buf, 2); // receive two byte CRC
 --count;
 buff+=512;
 }
 sdc_sendCommand(CMD12,0);
 }
while(!SPI_receive_single(SPI1)){}; // wait until card is not busyanymore
 SPI_send_single(SPI1,0xFF);
//sdc_deassert(); // deassert SDC
//send_string_ln("Success on reading",USART2);
return RES_OK;
}
else{
return STA_NOINIT;
}
}
/*-----------------------------------------------------------------------*/
/* Write Sector(s) */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT disk_write (
BYTE pdrv, /* Physical drive nmuber (0..) */
const BYTE* buff, /* Data to be written */
DWORD sector, /* Sector address (LBA) */
UINT count /* Number of sectors to write (1..128) */
)
{
unsigned char result;
unsigned char buf[2];
 buf[0] = 0xFF;
 buf[1] = 0xFF;
// check if parameters are in valid range
if( count > 255 )
 {
 }
//sdc_assert(); // assert SDC
// if multiple sectors are to be written
if(count==1){
 while(1){ 
 sdc_sendCommand(CMD24,(sector));
 if(!sdc_getResponse(0x00)){
 break;
 }
 }
 //while(sdc_getResponse(0x00)); // wait for R1 response
 SPI_send_single(SPI1, 0xFE); // send data token 0xFE
 SPI_send(SPI1, (unsigned char*)buff, 512); // send data
 SPI_send(SPI1, buf, 2); // send two byte CRC
 // check if card has accepted data
 result = SPI_receive_single(SPI1);
 if( (result & 0x1F) != 0x05)
 {
 return RES_ERROR;
 }
}
else{
 send_string_ln("TEST CODE!!!!...",USART2);
while(1){
 sdc_sendCommand(55,0);
 SPI_send_single(SPI1, 0xFF);
 //sdc_deassert();
 SPI_send_single(SPI1, 0xFF);
 //sdc_assert();
 sdc_sendCommand(ACMD23,count);
 if (!sdc_getResponse(0x00))
 {
 break;
 }
 }
 sdc_sendCommand(0x25,(sector));
while(count){
 SPI_send_single(SPI1, 0xFC); // send data token 0xFE
 SPI_send(SPI1, (unsigned char*)buff, 512); // send data
 SPI_send(SPI1, buf, 2); // send two byte CRC
 --count;
 buff+=512;
 }
 SPI_send_single(SPI1, 0xFD); // send data token 0xFE
 count =1;
}
while(!SPI_receive_single(SPI1)){}; // wait until card is not busyanymore
 (void)SPI_send_single(SPI1,0xFF);
//sdc_deassert(); // deassert SDC
//send_string_ln("Success on Writing",USART2);
return RES_OK;
}
#endif
/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions */
/*-----------------------------------------------------------------------*/
#if _USE_IOCTL
DRESULT disk_ioctl (
BYTE pdrv, /* Physical drive nmuber (0..) */
BYTE cmd, /* Control code */
void *buff /* Buffer to send/receive control data */
)
{
return RES_OK;
}
#endif
DWORD get_fattime (void)
{
return 0;
} 

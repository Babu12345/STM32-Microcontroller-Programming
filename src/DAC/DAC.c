#include "DAC.h"

//This circuit utilizes the DAC chip instead of the onboard module
void DAC_init(I2C_TypeDef *I2Cx){
	i2c_enable(I2C1);

}
void setVoltage(I2C_TypeDef *I2Cx,uint16_t frac, uint8_t x){
//When x=0, only the DAC is set
//When x=1, the DAC and the EPPROM are set

		i2c_cmd(I2Cx,0xC0,x,(frac>>4),((frac&15)<<4));




}

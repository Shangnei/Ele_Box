#include "Ina226.h"

float Current_Now ;
void INA226_Init(void)
{
    I2C_DRV_MasterSetSlaveAddr(1,I2C_MasterConfig0.slaveAddress,I2C_MasterConfig0.is10bitAddr);
    INA226_Write2Byte(Config_Reg, 0x4527);//0100_010_100_100_111 //16次平均,1.1ms,1.1ms,连续测量分流电压和总线电压
	INA226_Write2Byte(Calib_Reg, 0x0A00);
}

void INA226_Read2Byte(unsigned char reg_addr)
{
    unsigned char dat[2]={reg_addr,READ_ADDR};
    I2C_DRV_MasterSetSlaveAddr(1,I2C_MasterConfig0.slaveAddress,I2C_MasterConfig0.is10bitAddr);
    I2C_DRV_MasterSendDataBlocking(1,dat,2,false,1000);
    I2C_DRV_MasterReceiveDataBlocking(1, dat, 2, true, 1000);
    Current_Now = ((dat[0]<<8)|dat[1])*0.002;
}

void INA226_Write2Byte(unsigned char  reg_addr,unsigned short reg_data)
{
    unsigned char dat[3] ={ reg_addr,(reg_data>>8),(reg_data&0x00ff)};
    I2C_DRV_MasterSendDataBlocking(1,dat,3,false,1000);
}
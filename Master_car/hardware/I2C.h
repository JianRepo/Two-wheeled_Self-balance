#ifndef _I2C_H
#define _I2C_H
#include "sys.h"

//如果移植程序时只要改一下三个地方就行了
#define I2C_SCL GPIO_Pin_3	
#define I2C_SDA GPIO_Pin_2
#define GPIO_I2C GPIOC

#define I2C_SCL_H GPIO_SetBits(GPIO_I2C,I2C_SCL)
#define I2C_SCL_L GPIO_ResetBits(GPIO_I2C,I2C_SCL)

#define I2C_SDA_H GPIO_SetBits(GPIO_I2C,I2C_SDA)
#define I2C_SDA_L GPIO_ResetBits(GPIO_I2C,I2C_SDA)

void I2C_Simulation_Init(void);
void I2C_SDA_OUT(void);
void I2C_SDA_IN(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NAck(void);
u8   I2C_Wait_Ack(void);
void I2C_Send_Byte(u8 txd);
u8   I2C_Read_Byte(u8 ack);

#endif

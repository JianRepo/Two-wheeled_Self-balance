#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "moto.h"
#include "time.h"
#include "usart2.h"
#include "usart1.h"
#include "mpu6050.h"
#include "imu.h"

extern u8 TIM4_IMU;
extern u8 KEY_ON ;
extern u8 USART2_BZ;
extern u8 USART2_RX_BUF[11]; 

int MOTO_SPEED_L = 0 ;
int MOTO_SPEED_R = 0 ;

int main (void)
{
	float IMU[2];
	u16 D[5];
	/***********************************************************************/
	delay_init(72);
	delay_ms(20);
	KEY_Init();
	LED_Init();
	MOTO_Init();
	TIM3_Pwm_Init();
	USART1_Init(9600);
	USART2_Init(9600);
	MPU6050_Init();
	TIM4_Exit_Init();
	TIM1_Count_Init();
	TIM2_Count_Init();
	KEY_Exit_Init();
	delay_ms(20);
	/***********************************************************************/
	while(1)
	{
		if(TIM4_IMU)
			{
				IMU_GetAngle_Now_(IMU);
				IMU_Control_Gyro4(IMU[0],IMU[1]);
 				TIM4_IMU=0;
			}
		else if(KEY_ON)
			{
				delay_ms(10);
				if(KEY_2 == 0)
					NC = !NC , LED1 = !NC ;
				KEY_ON = 0;
			}
		if(USART2_BZ)
		{
			USART2_BZ = 0 ;
			BC04_1(USART2_RX_BUF,D);
			MOTO_SPEED_L = D[1] - 30000 ;
			MOTO_SPEED_R = D[2] - 30000 ;
			u1_printf("-1-:%d\t-2-:%d\t-3-:%d\r\n",D[0],MOTO_SPEED_L,MOTO_SPEED_R);
		}
	}
	
}






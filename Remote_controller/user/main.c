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

int main (void)
{
// 	float IMU[2];
	/***********************************************************************/
	SystemInit();
	delay_init(72);

	KEY_Init();
	LED_Init();

	USART1_Init(9600);
	USART2_Init(9600);
	delay_ms(20);
	/***********************************************************************/
	while(1)
	{
		if(KEY_UP==0)
		{
			delay_ms(30);
			if(KEY_UP==0)
			{
				LED1 = 1 ;
				while(KEY_UP==0)
				{
					hc04(1,500,500);
					delay_ms(200);
				}
			}
		}
		
		
		if(KEY_DO==0)
		{
			delay_ms(30);
			if(KEY_DO==0)
			{
				LED1 = 0 ;
			}
			while(KEY_DO==0)
				{
					hc04(1,-500,-500);
					delay_ms(200);
				}
		}
		
		
		if(KEY_LE==0)
		{
			delay_ms(30);
			if(KEY_LE==0)
			{
				LED2 = 1 ;
			}
			while(KEY_LE==0)
				{
					hc04(1,200,-200);
					delay_ms(200);
				}
		}
		
		
		if(KEY_RI==0)
		{
			delay_ms(30);
			if(KEY_RI==0)
			{
				LED2 = 0 ;
			}
			while(KEY_RI==0)
				{
					hc04(1,-200,200);
					delay_ms(200);
				}
		}
		
		delay_ms(200);
		hc04(1,0,0);

	}
	
}






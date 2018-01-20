#include "mpu6050.h"
#include "imu.h"
#include <math.h>
#include "usart2.h"
#include "usart1.h"
#include "moto.h"
#include "time.h"
#include "led.h"

float Gyro=0;

int abs(int abc)
{
	if(abc<0)
		return -abc;
	else 
		return abc;
}


void IMU_GetAngle_Now_(float *Ang)
{
	float Angle_1;
	float Angle_2;
	short acc[3];
	short gyro[3];
	
	MPU6050ReadAcc(acc);
	Angle_1 = atan((acc[0]*1.0f) / (acc[2]*1.0f));
	Angle_1 = (Angle_1 * 180 ) / 3.14 ;
	MPU6050ReadGyro(gyro);

	Gyro += ( -gyro[1] / 16.4f )*0.02 ;


	Angle_2 = 0.98 *  Gyro + 0.02 * Angle_1;

	Gyro = Angle_2 ;

	Ang[0] = Angle_2-0.75;
	Ang[1] = (-gyro[1] / 16.4f) * 0.01;
}

void IMU_Control_Gyro4(float Angle ,float Gyro)
{
	static int speed[9] ;
	int Left , Right ;
	
	if(Angle > -45 && Angle < 45)
	{
		MOTO_Get_Speed2(speed);	

		Left = Angle * 75 + Gyro * 75 ;		
		Right = Angle * 75 + Gyro * 75 ;	

		speed[7] = speed[4] * 2.0 + speed[0] * 28 ;
		speed[8] = speed[5] * 2.0 + speed[1] * 28 ;

		speed[7] *= 0.09 ;
		speed[8] *= 0.09 ;

		Left += speed[7] ;
		Right += speed[8] ;

		TIM3_Pwm_Set(Left,Right);
	}
	else 
	{
		NC = 0 ;
		LED1 = !NC ;
	}
}
















































#ifndef _MOTO_H
#define _MOTO_H

#include "sys.h"

#define AIN_1	PBout(14)
#define AIN_2	PBout(15)
#define NC		PAout(8)
#define BIN_1	PCout(10)
#define BIN_2 PCout(11)

#define PWM_A	PCout(6)
#define PWM_B	PCout(7)

#define GOGO 	NC=1,AIN_1=1,AIN_2=0,BIN_1=1,BIN_2=0	
#define BACK 	NC=1,AIN_1=0,AIN_2=1,BIN_1=0,BIN_2=1	
#define STOP 	NC=0	

#define MOTO_SPEED_L 0
#define MOTO_SPEED_R 0

void MOTO_Init(void);
void MOTO_Get_Speed2 (int *V);

#endif



















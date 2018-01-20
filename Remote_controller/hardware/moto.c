#include "stm32f10x.h"
#include "moto.h"

/**********************************************************/

/**********************************************************/


void MOTO_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	/*****************************************************************/
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 GPIO_SetBits(GPIOA,GPIO_Pin_8);	
	/*****************************************************************/
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;			
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 GPIO_SetBits(GPIOB,GPIO_Pin_14|GPIO_Pin_15);	
	/*****************************************************************/
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;			
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	 GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_11);	
	 

	 NC=0;
	 AIN_1=1;
	 BIN_1=1;
	 
}


void MOTO_Get_Speed2 (int *V)
{
	int I_ = 1000 ;

	V[0] =  TIM_GetCounter(TIM2);
	V[1] =  TIM_GetCounter(TIM1);

	TIM_SetCounter( TIM1,0);
	TIM_SetCounter( TIM2,0);

	if(AIN_1==1)
		V[0] = -V[0] ;
	if(BIN_1==1)
		V[1] = -V[1] ;	

	V[0] = V[2] * 0.7 + V[0] * 0.3 ;
	V[1] = V[3] * 0.7 + V[1] * 0.3 ;
	
	V[2] = V[0] ;
	V[3] = V[1] ;

	V[0] = ( V[0] + V[1] ) / 2 ;
	V[1] = V[0] ;

	V[4] += V[0] ;	
	V[5] += V[1] ;

	if(	V[4] < -I_)	V[4] = -I_ ;
	if( V[4] > I_ )	V[4] =  I_ ;
	if( V[5] < -I_) V[5] = -I_ ;
	if( V[5] > I_ ) V[5] =  I_ ;
	
	V[4] = V[4] + MOTO_SPEED_L ;
	V[5] = V[5] + MOTO_SPEED_R ;
}









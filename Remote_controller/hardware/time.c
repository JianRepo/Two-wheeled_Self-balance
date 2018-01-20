#include "stm32f10x.h"
#include "time.h"
#include "led.h"
#include "imu.h"
#include "moto.h"

/*********************************************************************/
//
//
/*********************************************************************/

void TIM3_Pwm_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7|GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 1000; //自动重装载数值
	TIM_TimeBaseStructure.TIM_Prescaler =0;//时钟预分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);//初始化TIM3
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM mode
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);	
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
	
}

void TIM3_Pwm_Set(int LL,int RR)
{
	
	if(RR<0)
		AIN_1=1,AIN_2=0;
	else
		AIN_1=0,AIN_2=1;
	
	if(LL<0)
		BIN_1=1,BIN_2=0;
	else
		BIN_1=0,BIN_2=1;
	
	LL = abs(LL)+25;
	RR = abs(RR)+25;
	
	if(LL>=999) LL=999;
	if(RR>=999) RR =999;
	
	TIM_SetCompare2(TIM3,LL);
	TIM_SetCompare1(TIM3,RR);
}
/*********************************************************************/
//
//
/*********************************************************************/
void TIM4_Exit_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = 200; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_Cmd(TIM4, ENABLE);
}

u8 TIM4_IMU=0;

void TIM4_IRQHandler(void)   
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
			TIM4_IMU=1;
			LED2 = !LED2 ;
		}
}

/*********************************************************************/
//
//
/*********************************************************************/
void TIM2_Count_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);		//PA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//时钟

	TIM_TimeBaseStructure.TIM_Period = 0XFFFF;	 //最大计数数值
	TIM_TimeBaseStructure.TIM_Prescaler = 0;			//预分频
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 

	
  TIM_ETRClockMode2Config (TIM2,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0);  //设定为模式2
	TIM_SetCounter( TIM2,0)	;   //从0开始
	TIM_Cmd(TIM2, ENABLE);			//开计数器
}

void TIM1_Count_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);		//PA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);		//时钟

	TIM_TimeBaseStructure.TIM_Period = 0XFFFF;	 //最大计数数值
	TIM_TimeBaseStructure.TIM_Prescaler = 0;			//预分频
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 

	
  TIM_ETRClockMode2Config (TIM1,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0);  //设定为模式2
	TIM_SetCounter( TIM1,0)	;   //从0开始
	TIM_Cmd(TIM1, ENABLE);			//开计数器
}















































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
	
	TIM_TimeBaseStructure.TIM_Period = 1000; //�Զ���װ����ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =0;//ʱ��Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);//��ʼ��TIM3
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM mode
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//�������
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);	
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
	
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
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = 200; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	TIM_Cmd(TIM4, ENABLE);
}

u8 TIM4_IMU=0;

void TIM4_IRQHandler(void)   
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//ʱ��

	TIM_TimeBaseStructure.TIM_Period = 0XFFFF;	 //��������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 0;			//Ԥ��Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //����
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 

	
  TIM_ETRClockMode2Config (TIM2,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0);  //�趨Ϊģʽ2
	TIM_SetCounter( TIM2,0)	;   //��0��ʼ
	TIM_Cmd(TIM2, ENABLE);			//��������
}

void TIM1_Count_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);		//PA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);		//ʱ��

	TIM_TimeBaseStructure.TIM_Period = 0XFFFF;	 //��������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 0;			//Ԥ��Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //����
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 

	
  TIM_ETRClockMode2Config (TIM1,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0);  //�趨Ϊģʽ2
	TIM_SetCounter( TIM1,0)	;   //��0��ʼ
	TIM_Cmd(TIM1, ENABLE);			//��������
}















































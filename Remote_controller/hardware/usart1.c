#include "stm32f10x.h"
#include "usart1.h"
#include "stdio.h"
#include "stdarg.h"	 
#include "string.h"	 

__align(8) u8 USART1_TX_BUF[100];
u8 USART1_RX_BUF[100]; 


void USART1_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_DeInit(USART2);
	 //USART2_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	//USART2_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	
	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1
	
	//USART ��ʼ������
   
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
   
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
}


void u1_printf(char* fmt,...)  
{  
	int cnt=0; 
   int len=0; 
    va_list ap; 
    va_start(ap,fmt); 
    vsprintf((char*)USART1_TX_BUF,fmt,ap); 
    va_end(ap); 
    len = strlen((const char*)USART1_TX_BUF); 
   while(len--)
			{	 
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=1);
				USART_SendData(USART1,USART1_TX_BUF[cnt++]);
			}
}

void USART1_IRQHandler(void)
{
	u8 res;	    
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//���յ�����
	{	 
	res =USART_ReceiveData(USART1);
	USART_SendData(USART2	,res);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
	}  											 
} 




































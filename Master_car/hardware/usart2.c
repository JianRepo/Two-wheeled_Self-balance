#include "stm32f10x.h"
#include "usart2.h"
#include "stdio.h"
#include "stdarg.h"	 
#include "string.h"	 

__align(8) u8 USART2_TX_BUF[200];


void USART2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_DeInit(USART2);
	 //USART2_TX   PA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	//USART2_RX	  PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	
	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
	
	//USART 初始化设置
   
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
   
  USART_Cmd(USART2, ENABLE);                    //使能串口 
}



void USART2_Send_Byte(u8 byte)
{
	USART_SendData(USART2,byte);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET){}								
}


u8 USART2_RX_BUF[11]; 
u8 USART2_RX_STA ;
u8 z ;
u8 USART2_BZ = 0 ;

void USART2_IRQHandler(void)
{
	u8 res;	    
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		res =USART_ReceiveData(USART2);
		if(res == 0x99)
		{
			USART2_RX_STA =28 ;
			z = 0;
		}
		if(USART2_RX_STA == 28)
		{
			USART2_RX_BUF[z] = res ;			
			z++;
			if(z > 10)
			{
				z = 0 ;
				USART2_RX_STA = 9 ;		//
				USART2_BZ = 1 ;
			}
		}
	}  											 
} 

void u2_printf(char* fmt,...)  
{  
	int cnt=0; 
   int len=0; 
    va_list ap; 
    va_start(ap,fmt); 
    vsprintf((char*)USART2_TX_BUF,fmt,ap); 
    va_end(ap); 
    len = strlen((const char*)USART2_TX_BUF); 
   while(len--)
			{	 
				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
				USART_SendData(USART2,USART2_TX_BUF[cnt++]);
			}
}

void niming (int data1,int data2,int data3)
{
		u32 num1 = data1;			
	  u32 num2 = data2;
		u32 num3 = data3;
		u8 xiaoyan;
	
	xiaoyan=0xaa+0xaa+0xf1+(num1>>24)+((num1-((num1>>24)<<24))>>16)+((num1-((num1>>16)<<16))>>8)+((num1<<24)>>24)+
												 (num2>>24)+((num2-((num2>>24)<<24))>>16)+((num2-((num2>>16)<<16))>>8)+((num2<<24)>>24)+
												 (num3>>24)+((num3-((num3>>24)<<24))>>16)+((num3-((num3>>16)<<16))>>8)+((num3<<24)>>24)+0x08;
	
	
	AnBT_Uart1_Send_Char(0xAA);
	AnBT_Uart1_Send_Char(0xAA);
	AnBT_Uart1_Send_Char(0xF1);
	AnBT_Uart1_Send_Char(0x0c);				
			
	AnBT_Uart1_Send_Char(num1>>24);			
	AnBT_Uart1_Send_Char((num1-((num1>>24)<<24))>>16);
	AnBT_Uart1_Send_Char((num1-((num1>>16)<<16))>>8);
	AnBT_Uart1_Send_Char((num1<<24)>>24);
	
	AnBT_Uart1_Send_Char(num2>>24);
	AnBT_Uart1_Send_Char((num2-((num2>>24)<<24))>>16);
	AnBT_Uart1_Send_Char((num2-((num2>>16)<<16))>>8);
	AnBT_Uart1_Send_Char((num2<<24)>>24);
	
	AnBT_Uart1_Send_Char(num3>>24);
	AnBT_Uart1_Send_Char((num3-((num3>>24)<<24))>>16);
	AnBT_Uart1_Send_Char((num3-((num3>>16)<<16))>>8);
	AnBT_Uart1_Send_Char((num3<<24)>>24);
	
	AnBT_Uart1_Send_Char(xiaoyan);
	
}

void AnBT_Uart1_Send_Char(unsigned char ascii_code) 		
{
	USART_SendData(USART1,ascii_code);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}								
}


void BC04_1(u8 *DI,u16 *DO)
{

		DO[0] = DI[1] ;
	
		DO[1]  = DI[2]<<24 ;
		DO[1] += DI[3]<<16 ;
		DO[1] += DI[4]<<8 ;
		DO[1] += DI[5];
	
		DO[2]  = DI[6]<<24 ;
		DO[2] += DI[7]<<16 ;
		DO[2] += DI[8]<<8 ;
		DO[2] += DI[9] ;

		DO[4] = 1 ;
	
	DI[0] = 0 ;
	
	DI[1] = 0 ;
	
	DI[2] = 0 ;
	DI[3] = 0 ;
	DI[4] = 0 ;
	DI[5] = 0 ;
	
	DI[6] = 0 ;
	DI[7] = 0 ;
	DI[8] = 0 ;
	DI[9] = 0 ;
	
	DI[10] = 0 ;
}

















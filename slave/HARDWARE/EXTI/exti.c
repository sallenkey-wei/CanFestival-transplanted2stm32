/**
  *	@version			test0.1
  *	@depend_on		stm32f10x.h
  *	@depend_on		exti.c/exti.h
  *	@brief				The functions in this file are use to initialize external interruption line11.
  *	@attention		
  *	@author				SallenKey
  *	@E-mail				weiyuyinn@gmail.com
  */
  
#include "exti.h"
#include "usart.h"

/**
  * @brief  Initialize the EXTI_line11 as falling trigger mode and EXTI_line11 correspond to external pin GPIOB11 .
  * @param  None.
  * @retval None
  */
void EXTIX_Init(void)
{
	
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
    //使能复用功能时钟
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);	


	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);


  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource2);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
	

  	EXTI_InitStructure.EXTI_Line=EXTI_Line0 | EXTI_Line1 | EXTI_Line2 | EXTI_Line3 | EXTI_Line4 | EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;		 //中断线10到15共用一个中断
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		 //子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;		 
  	NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;		 
  	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;		 
  	NVIC_Init(&NVIC_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;		 
  	NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;		 
  	NVIC_Init(&NVIC_InitStructure);
}



void EXTI0_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line0) !=RESET){
		printf("EXTI_Line0\r\n");
		EXTI_ClearITPendingBit(EXTI_Line0); 
	}
}

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line1) !=RESET){
		printf("EXTI_Line1\r\n");
		EXTI_ClearITPendingBit(EXTI_Line1); 
	}
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line2) !=RESET){
		printf("EXTI_Line2\r\n");
		EXTI_ClearITPendingBit(EXTI_Line2); 
	}
}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line3) !=RESET){
		printf("EXTI_Line3\r\n");
		EXTI_ClearITPendingBit(EXTI_Line3); 
	}
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line4) !=RESET){
		printf("EXTI_Line4\r\n");
		EXTI_ClearITPendingBit(EXTI_Line4); 
	}
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line5) !=RESET){
		printf("EXTI_Line5\r\n");
		EXTI_ClearITPendingBit(EXTI_Line5); 
	}
}

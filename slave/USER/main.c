#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "oled.h"
#include "usart.h"
#include "beep.h"
#include "can_hw.h"
#include "adc.h"
#include <math.h>
#include "ws2811_rgb.h"
#include "exti.h"
#include "timer3.h"

#include "data.h"

double Temperature(u16 adc_value);
unsigned char nodeID = 0x07;
extern CO_Data TestSlave_Data;
extern UNS8 TEST[8];
 int main(void)
 {
//CanFestival测试
#if 1
	 	 UNS32 value = 1;
	 UNS32 value_size = 4;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200   
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps
	 printf("break1\r\n");
	setNodeId(&TestSlave_Data, nodeID);
	 	 printf("break2\r\n");
	setState(&TestSlave_Data, Initialisation);
	 printf("break3\r\n");

	 setState(&TestSlave_Data, Operational);
	 printf("break4\r\n");
	 TIM3_Init();
	 while(1)
	{
		value++;
		setODentry(&TestSlave_Data,
					0x2000,
					0,
					(void *)&value,
					(unsigned long *)&value_size,
					RW
		);
					delay_ms(1000);
	}
#endif
//oled测试
#if 0

	u8 t;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	 uart_init(115200);
 	//LED_Init();			     //LED端口初始化
	OLED_Init();
	OLED_Clear();	 //初始化OLED      
    OLED_ShowString(0,0,"HELLO",24, 1);  
	OLED_ShowString(0,24, "HAPPYNE",18, 0);  
 	OLED_ShowString(0,42,"ATOM 201",16, 1);  
 	OLED_ShowString(0,58,"ASCII:",12, 1);  
 	OLED_ShowString(0,70,"CODE:",12, 1);

	OLED_Refresh_Gram();		//更新显示到OLED 
//	t=' ';  
	while(1) 
	{
//		printf("hello\r\n");
//		OLED_ShowChar(40,58,t,12,1);//显示ASCII字符	   
//		OLED_Refresh_Gram();
//		t++;
//		if(t>'~')t=' ';
//		OLED_ShowNum(40,70,t,3,12);//显示ASCII字符的码值 
//		delay_ms(500);
//		LED0=!LED0;
	}	  
#endif
//	u8 t;
//	delay_init();	    	 //延时函数初始化	  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
//// 	LED_Init();			     //LED端口初始化
//	Beep_init();
//	OLED_Init();			//初始化OLED    
//	OLED_Clear();	 //初始化OLED      
//	
//	OLED_ShowString(0,0,"ALIENTEK",24);  
//	OLED_ShowString(0,24, "0.96' OLED TEST",16);  
// 	OLED_ShowString(0,40,"ATOM 2015/1/14",12);  
// 	OLED_ShowString(0,52,"ASCII:",12);  
// 	OLED_ShowString(64,52,"CODE:",12);  
//  
//	OLED_Refresh_Gram();		//更新显示到OLED 
//	t=' ';  
//	while(1) 
//	{
//		OLED_Refresh_Gram();
//		//OLED_ShowChar(48,52,t,12,1);//显示ASCII字符	   
//		OLED_Refresh_Gram();
//		t++;
//		if(t>'~')t=' ';
//		//OLED_ShowNum(103,52,t,3,12);//显示ASCII字符的码值 
//		delay_ms(500);
//		//LED0=!LED0;
//	}	  
//#endif

	
//can测试  测试之前关闭can接收中断
#if 0
#define SEND 0 
#if SEND
	u8 i=0,t=0;

	u8 canbuf[8];
	u8 ret = 0;


	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
		 	
   
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps   
	while(1){
	for (t=0; t<8; t++){
		canbuf[t] = t+i;
	}
printf("hi\r\n");
	ret = Can_Send_Msg(canbuf, 8);
	printf(ret==0?"success\r\n":"failed\r\n");
	i += 8;
	if(i>100)
		i=0;
	delay_ms(1000);
	
	}
#else

	u8 canbuf[8];
	u8 ret = 0;


	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
		 	
   
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps   
	while(1){

	ret = Can_Receive_Msg(canbuf);
	if(ret > 0)
		printf("\r\nreceive %u datapacks\r\n", ret);

	while(ret>0){
		ret--;
		printf("%u  ", canbuf[ret]);
	}

	
	}
	
#endif
#endif
//adc 测试
#if 0
	u16 adc_data[2] = {0};
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	Adc_Init();
	while(1){
		Get_Adc_Average(adc_data, 20);
		//printf("channel_0 = %u, channel_1 = %u\r\n", adc_data[0], adc_data[1]);
		printf("temp_1 = %lf, temp_2 = %lf\r\n", Temperature(adc_data[0]), Temperature(adc_data[1]));
	}
}
double Temperature(u16 adc_value)
{
	double K=273.15;
	double T2=25;
	double R=10000;
	double B=3950;
	double Rt;	
	double temp;
	

	Rt = 4096*10000/adc_value-10000; //?????? 10K Rt = 4096*??????/Get_Adc_Average(1,50)-??????;
	//temp=exp(3590*(1/(K+temp_value)+1/(K+T2)));
	
	temp = Rt/R;
	temp = log(temp); //ln(Rt/R)
	temp /= B;        //ln(Rt/R)/B
	temp += (1/(K+T2));
	temp = 1/temp;
	temp -= K;

	return temp*10;
#endif
//RGB测试
#if 0
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	WS_Init();
	//WS_Set1(Green);
			//ColorToColor(Red, Green);

	while(1)
	{
//		ColorToColor(Red, Green);
//		delay_ms(1000);

//		ColorToColor(Green, Blue);
//		delay_ms(1000);

//		ColorToColor(Blue, Red);
//		delay_ms(1000);
		WsDat[0] = Blue;
		WsDat[1] = Red;
		WS_Set(WsDat);

		delay_ms(1000);
		//		WS_Set1(Red);


		//printf("RGB\r\n");
		//ColorToColor(Red, Green);
			//WS_Set1(Green);

	}
#endif
//EXTI测试
#if 0
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	EXTIX_Init();
	while(1)
	{
//		if(PBin(3)==1)
//		{
//			printf("EXTI_Line\r\n");
//		}
//		else	
//		{
//			printf("EXTI\r\n");
//		}
//		PBout(0)=0;PBout(1)=0;PBout(2)=0;PBout(5)=0;PBout(3)=0;PBout(4)=0;
//		delay_ms(5);
//		PBout(0)=1;PBout(1)=1;PBout(2)=1;PBout(5)=1;PBout(3)=1;PBout(4)=1;
//		delay_ms(5);
//		printf("EXTI\r\n");
		
	}
#endif	
}

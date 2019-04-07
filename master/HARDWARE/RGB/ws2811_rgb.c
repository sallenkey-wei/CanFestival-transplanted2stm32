#include "sys.h"
#include <STM32f10x.h>
#include "ws2811_rgb.h"
#include "delay.h"
        

unsigned long WsDat[nWs];

void WS_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;        

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//PA15 复用

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;                              
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                    
}


void delay2us()
{
        unsigned char i;
        for(i=0; i<12; i++);
}
void delay05us()
{
        unsigned char i;
        for(i=0; i<1; i++);
}


void TX0()          { PAout(15) = 1; delay05us(); PAout(15) = 0; delay2us(); } // ??0
void TX1()          { PAout(15) = 1; delay2us();  PAout(15) = 0; delay05us(); } // ??1
void WS_Reset() { PAout(15) = 0; delay_us(60);  PAout(15) = 1; PAout(15) = 0; }


void WS_Set1(unsigned long dat)
{
        unsigned char i;
        
        for(i=0; i<24; i++)
        {
			if(0x800000 == (dat & 0x800000) )        
				TX1();
			else
				TX0();
			dat<<=1;                                                        //????
        }
}


void WS_SetAll()
{
        unsigned char j;
        
        for(j=0; j<nWs; j++)
        {
              WS_Set1(WsDat[0]);
        }
        WS_Reset();
}
//传入一个usigned long 类型的数组 长度为2
void WS_Set(unsigned long * pdata)
{
	WS_Set1(WsDat[0]);
	WS_Set1(WsDat[1]);
}


unsigned char abs0(int num)
{
        if(num>0) return num;
        
        num = -num;
        return (unsigned char) num;
}


u32 ColorToColor(unsigned long color0, unsigned long color1)
{
        unsigned char Red0, Green0, Blue0;  
        unsigned char Red1, Green1, Blue1;  
        int                          RedMinus, GreenMinus, BlueMinus;        
        unsigned char NStep;                                                       
        float                  RedStep, GreenStep, BlueStep;               
        unsigned long color;                                                       
        unsigned char i;
        
        Red0   = color0>>8;
        Green0 = color0>>16;
        Blue0  = color0;
        
        Red1   = color1>>8;
        Green1 = color1>>16;
        Blue1  = color1;
        
        RedMinus   = Red1 - Red0; 
        GreenMinus = Green1 - Green0; 
        BlueMinus  = Blue1 - Blue0;
        
        NStep = ( abs0(RedMinus) > abs0(GreenMinus) ) ? abs0(RedMinus):abs0(GreenMinus);
        NStep = ( NStep > abs0(BlueMinus) ) ? NStep:abs0(BlueMinus);
        
        RedStep   = (float)RedMinus   / NStep;
        GreenStep = (float)GreenMinus / NStep;
        BlueStep  = (float)BlueMinus  / NStep;
        
        for(i=0; i<NStep; i++)
        {
                Red1   = Red0   + (int)(RedStep   * i);
                Green1 = Green0 + (int)(GreenStep * i);
                Blue1  = Blue0  + (int)(BlueStep  * i);
                
                color  = Green1<<16 | Red1<<8 | Blue1;      
                WsDat[0] = color;
                WS_SetAll(); 
                delay_ms(1);  
        }
        return color;
}

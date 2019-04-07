#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
#include <stdio.h>
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//SSD1306 OLED 驱动IC驱动代码
//驱动方式:8080并口/4线串口
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/14
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 



u8 OLED_GRAM[16][64];	

//更新显存到LCD	
void OLED_Refresh_Gram(void)
{
	u8 i;
	s8 n;	
	for(i=0;i<16;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~16）

		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   

		for(n=0;n<64;n++){
			OLED_WR_Byte(OLED_GRAM[i][n],OLED_DATA); 
		}

		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   

	}   
}


#if OLED_MODE==1	//8080并口 
//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	DATAOUT(dat);	    
 	OLED_RS=cmd;
	OLED_CS=0;	   
	OLED_WR=0;	 
	OLED_WR=1;
	OLED_CS=1;	  
	OLED_RS=1;	 
} 	    	    
#else
//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	OLED_RS=cmd; //写命令 
	OLED_CS=0;		  
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK=0;
		if(dat&0x80)OLED_SDIN=1;
		else OLED_SDIN=0;
		OLED_SCLK=1;
		dat<<=1;   
	}				 
	OLED_CS=1;		  
	OLED_RS=1;   	  
} 
#endif
	  	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
			OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//更新显示
}
//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空	

void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,by,temp=0;

	if(x>63||y>127)
		return;//超出范围了.
#if ROTATE
	x = 63 - x;
	y = 127 - y;
#endif
	pos=y/8;
	by=y%8;
	temp=1<<by;
	if(t)
		OLED_GRAM[pos][x]|=temp;
	else 
		OLED_GRAM[pos][x]&=~temp;	    
}

//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';//得到偏移后的值		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
		else if(size==18)temp = asc2_1809[chr][t];
		else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
		else return;								//没有的字库
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//显示字符串
//x,y:起点坐标  
//size:字体大小 
//*p:字符串起始地址 
//自动换行
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size, u8 mode)
{	
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {
		
        if(x>(64-(size/2))){x=0;y+=size;}
        if(y>(128-size)){y=x=0;OLED_Clear();}
		

        OLED_ShowChar(x,y,*p,size,mode);	 
        x+=size/2;
        p++;
    }  
	
}
//初始化SSD1306					    
void OLED_Init(void)
{ 	
 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;	 //PD3,PD6推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOD3,6
 	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15);	//PD3,PD6 输出高

 #if OLED_MODE==1
 
 	GPIO_InitStructure.GPIO_Pin =0xFF; //PC0~7 OUT推挽输出
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOC,0xFF); //PC0~7输出高

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //PG13,14,15 OUT推挽输出
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOG,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);						 //PG13,14,15 OUT  输出高



 #endif
  							  
	OLED_CS=0;
	 
	OLED_RST = 1;
	delay_ms(10);
	OLED_RST=0;
	delay_ms(10);
	OLED_RST=1; 
					  
	OLED_WR_Byte(0xAE,OLED_CMD);/*display off*/
	
	OLED_WR_Byte(0x00,OLED_CMD);/*set lower column address*/
	OLED_WR_Byte(0x10,OLED_CMD);/*set higher column address*/
	
	OLED_WR_Byte(0xB0,OLED_CMD);/*set page address*/
	
	OLED_WR_Byte(0xDC,OLED_CMD);/*set display start line*/
	OLED_WR_Byte(0x00,OLED_CMD);
	
	OLED_WR_Byte(0x81,OLED_CMD);/*contract control*/
	OLED_WR_Byte(0xfF,OLED_CMD);
	
	OLED_WR_Byte(0x20,OLED_CMD);/* Set Memory addressing mode (0x20/0x21) */

	OLED_WR_Byte(0xA0,OLED_CMD);/*set segment remap*/
	
	OLED_WR_Byte(0xC0,OLED_CMD);/*Com scan direction*/

	
	OLED_WR_Byte(0xA4,OLED_CMD);/*Disable Entire Display On (0xA4/0xA5)*/
	OLED_WR_Byte(0xA6,OLED_CMD); /*normal / reverse*/
	
	OLED_WR_Byte(0xA8,OLED_CMD);/*multiplex ratio*/
	OLED_WR_Byte(0x3F,OLED_CMD);/*duty = 1/64*/
	
	OLED_WR_Byte(0xD3,OLED_CMD);/*set display offset*/
	OLED_WR_Byte(0x60,OLED_CMD);
	
	OLED_WR_Byte(0xD5,OLED_CMD);/*set osc division*/
	OLED_WR_Byte(0x51,OLED_CMD);
	OLED_WR_Byte(0xD9,OLED_CMD);/*set pre-charge period*/
	OLED_WR_Byte(0x22,OLED_CMD);
	
	OLED_WR_Byte(0xDB,OLED_CMD);/*set vcomh*/
	OLED_WR_Byte(0x35,OLED_CMD);
	
	OLED_WR_Byte(0xAD,OLED_CMD);/*set charge pump enable*/
	OLED_WR_Byte(0x80,OLED_CMD);/*Set DC-DC enable (a=0:disable; a=1:enable) */
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
}  






























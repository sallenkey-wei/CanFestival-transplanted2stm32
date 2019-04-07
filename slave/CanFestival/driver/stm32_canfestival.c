#include "canfestival.h"
#include "timer3.h"
#include "stm32f10x_tim.h"

unsigned int TimeCNT=0;//时间计数
unsigned int NextTime=0;//下一次触发时间计数
unsigned int TIMER_MAX_COUNT=70000;//最大时间计数
static TIMEVAL last_time_set = TIMEVAL_MAX;//上一次的时间计数

void setTimer(TIMEVAL value)
{
    NextTime=(TimeCNT+value)%TIMER_MAX_COUNT;
}
TIMEVAL getElapsedTime(void)
{ 
	int ret=0;
	ret = TimeCNT> last_time_set ? TimeCNT - last_time_set : TimeCNT + TIMER_MAX_COUNT - last_time_set;
	last_time_set = TimeCNT;
	return ret;
}


#if 0
void timerForCan(void)
{
	TimeCNT++;
	if (TimeCNT>=TIMER_MAX_COUNT)
	{
		TimeCNT=0;
	}
	if (TimeCNT==NextTime)
	{
		TimeDispatch();
	}
}
#endif

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) 
	{
//printf("in tim3 TI\r\n");
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

		//TimerForCan();
		TimeCNT++;
		if (TimeCNT>=TIMER_MAX_COUNT)
		{
			TimeCNT=0;
		}
		if (TimeCNT==NextTime)
		{
			printf("in TimeDispath\r\n");
			TimeDispatch();
			printf("out TimeDispatch\r\n");
		}
	}
}

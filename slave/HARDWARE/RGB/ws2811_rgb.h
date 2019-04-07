#ifndef __WS2811_H
#define __WS2811_H         
#include "sys.h"


#define White       0xFFFFFF  // ??
#define Black       0x000000  // ??
#define Red         0x002200  // ??
#define Green       0x220000  // ??
#define Blue        0x000022  // ??


#define nWs 2                // ????WS2811??

extern unsigned long WsDat[];

extern void WS_Init(void);
extern void WS_SetAll(void);
extern void WS_Set1(unsigned long dat);
extern u32 ColorToColor(unsigned long color0, unsigned long color1);
//传入一个usigned long 类型的数组 长度为2
void WS_Set(unsigned long * pdata);


                                                     
#endif

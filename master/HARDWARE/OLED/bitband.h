/**
  * @version		test0.1
  * @depend_on		stm32f10x.h
  * @brief		The definition in this file can be used to simplify the GPIO operation by
			the way of the BITBAND_ADDR operation
  * @attention		
  * @Author		Sallenkey
  * @E-mail		weiyuyinn@gmail.com
  */

#ifndef _BITBAND_H
#define _BITBAND_H	
#include "stm32f10x.h"

/**
  * @brief  Convert the bit_address to bitband_address
  * @param  addr:	the base address of GPIOx_ODR or GPIO_IDR
	* @param	bitnum: pin num(offset address) of GPIOx
  */
#define BITBAND_ADDR(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 

/**
  * @brief  Get the variable of the given address, so that we can operate it
  * @param  addr:	the address of the variable we want to operation and this variable can determine
  *					the value of GPIOx's pin
  */
#define ADDR_TO_VAR(addr)  *((volatile unsigned long  *)(addr))

/**
  * @brief  The combination of the BITBAND_ADDR and the ADDR_TO_VAR
  * @param  addr:	the base address of GPIOx_ODR or GPIO_IDR
	* @param	bitnum: pin num(offset address) of GPIOx
  */
#define BIT_ADDR(addr, bitnum)   ADDR_TO_VAR(BITBAND_ADDR(addr, bitnum)) 

#define GPIOA_ODR_ADDR    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_ADDR    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_ADDR    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_ADDR    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_ADDR    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_ADDR    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_ADDR    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_ADDR    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_ADDR    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_ADDR    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_ADDR    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_ADDR    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_ADDR    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_ADDR    (GPIOG_BASE+8) //0x40011E08 
 

#define GPIOAout(n)   BIT_ADDR(GPIOA_ODR_ADDR,n)
#define GPIOAin(n)    BIT_ADDR(GPIOA_IDR_ADDR,n) 

#define GPIOBout(n)   BIT_ADDR(GPIOB_ODR_ADDR,n)
#define GPIOBin(n)    BIT_ADDR(GPIOB_IDR_ADDR,n)

#define GPIOCout(n)   BIT_ADDR(GPIOC_ODR_ADDR,n)
#define GPIOCin(n)    BIT_ADDR(GPIOC_IDR_ADDR,n)

#define GPIODout(n)   BIT_ADDR(GPIOD_ODR_ADDR,n)
#define GPIODin(n)    BIT_ADDR(GPIOD_IDR_ADDR,n)

#define GPIOEout(n)   BIT_ADDR(GPIOE_ODR_ADDR,n)
#define GPIOEin(n)    BIT_ADDR(GPIOE_IDR_ADDR,n)

#define GPIOFout(n)   BIT_ADDR(GPIOF_ODR_ADDR,n)
#define GPIOFin(n)    BIT_ADDR(GPIOF_IDR_ADDR,n)

#define GPIOGout(n)   BIT_ADDR(GPIOG_ODR_ADDR,n)
#define GPIOGin(n)    BIT_ADDR(GPIOG_IDR_ADDR,n)

#endif

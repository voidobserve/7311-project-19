/******************************************************************************
;  *   	@型号				  : MC32P7311
;  *   	@创建日期             : 2021.08.04
;  *   	@公司/作者			  : SINOMCU-FAE
;  *   	@晟矽微技术支持       : 2048615934
;  *   	@晟矽微官网           : http://www.sinomcu.com/
;  *   	@版权                 : 2021 SINOMCU公司版权所有.
;  *---------------------- 建议 ---------------------------------
;  *   				变量定义时使用全局变量	               	
******************************************************************************/
#ifndef __USER_H
#define __USER_H
#include "mc32-common.h"
#include "MC32P7311.h"

/*****************************************************************
;       Function : Define variables
;*****************************************************************/

#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long int
#define uint8_t  unsigned char
#define uint16_t unsigned int
#define uint32_t unsigned long int

// #define // 

// 定义充电时驱动升压（充电）电路的PWM占空比
// 电池没有电时，测得充电样板上最大的占空比为43.8%
// 电池几乎满电时，测得充电样板上最大的占空比为
const u8 charge_buf[] = {43, 0};

// 定义电池不同电量下，在adc检测到的电压值（不是adc值）
const u8 battery_refer_vol_buf[] = {0};

// demo提供的中断服务程序使用到的两个变量：
u8 abuf;
u8 statusbuf;

void Sys_Init(void);
void CLR_RAM(void);
void IO_Init(void);
// void TIMER1_PWM_Init(void);

//============Define  Flag=================
typedef union {
   	unsigned char byte;
   	struct
   	{
   	   	u8 bit0 : 1;
   	   	u8 bit1 : 1;
   	   	u8 bit2 : 1;
   	   	u8 bit3 : 1;
   	   	u8 bit4 : 1;
   	   	u8 bit5 : 1;
   	   	u8 bit6 : 1;
   	   	u8 bit7 : 1;
   	} bits;
}bit_flag;
volatile bit_flag flag1;

//#define    	FLAG_TIMER0_5000ms  	flag1.bits.bit0	 


#endif // end __USER_H


/**************************** end of file *********************************************/





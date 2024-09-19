/******************************************************************************
;  *   	@�ͺ�				  : MC32P7311
;  *   	@��������             : 2021.08.04
;  *   	@��˾/����			  : SINOMCU-FAE
;  *   	@����΢����֧��       : 2048615934
;  *   	@����΢����           : http://www.sinomcu.com/
;  *   	@��Ȩ                 : 2021 SINOMCU��˾��Ȩ����.
;  *---------------------- ���� ---------------------------------
;  *   				��������ʱʹ��ȫ�ֱ���	               	
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

// ������ʱ������ѹ����磩��·��PWMռ�ձ�
// ���û�е�ʱ����ó������������ռ�ձ�Ϊ43.8%
// ��ؼ�������ʱ����ó������������ռ�ձ�Ϊ
const u8 charge_buf[] = {43, 0};

// �����ز�ͬ�����£���adc��⵽�ĵ�ѹֵ������adcֵ��
const u8 battery_refer_vol_buf[] = {0};

// demo�ṩ���жϷ������ʹ�õ�������������
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





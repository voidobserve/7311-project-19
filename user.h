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
#define uint8_t unsigned char
#define uint16_t unsigned int
#define uint32_t unsigned long int

// #define //
#define UNUSED_PIN P10D // P10��14��оƬ��û�е�����

// ����ָʾ�Ƶ����Ŷ���
// #define LED_WORKING_PIN // ����ָʾ��
// #define LED_CHARGING_PIN // ���ָʾ��
// #define LED_FULL_CHARGE_PIN // ����ָʾ��

// ��ⰴ��״̬�����Ŷ��壬��⵽�͵�ƽΪ��Ч
// #define KEY_HEAT_PIN P11D // �����Ƿ���ȵ�����
// #define KEY_CHANGE_PIN P11D // ����ģʽ������
// #define KEY_POWER_PIN P11D // �����Ƿ���������

// #define CONTROL_HEAT_PIN // �������Ƽ��ȵ�����

#ifndef CONTROL_HEAT_PIN
#define CONTROL_HEAT_PIN UNUSED_PIN // �������Ƽ��ȵ�����
#endif
#ifndef KEY_HEAT_PIN
#define KEY_HEAT_PIN UNUSED_PIN // �����Ƿ���ȵ�����
#endif
#ifndef KEY_CHANGE_PIN
#define KEY_CHANGE_PIN UNUSED_PIN // ����ģʽ������
#endif
#ifndef KEY_POWER_PIN
#define KEY_POWER_PIN UNUSED_PIN // �����Ƿ���������
#endif

#ifndef LED_WORKING_PIN
#define LED_WORKING_PIN UNUSED_PIN // ����ָʾ��
#endif
#ifndef LED_CHARGING_PIN
#define LED_CHARGING_PIN UNUSED_PIN // ���ָʾ��
#endif
#ifndef LED_FULL_CHARGE_PIN
#define LED_FULL_CHARGE_PIN UNUSED_PIN // ����ָʾ��
#endif

#define LED_WORKING_ON()         \
	{                            \
		do                       \
		{                        \
			LED_WORKING_PIN = 0; \
		} while (0);             \
	}
#define LED_WORKING_OFF()        \
	{                            \
		do                       \
		{                        \
			LED_WORKING_PIN = 1; \
		} while (0);             \
	}
#define LED_CHARGING_ON()         \
	{                             \
		do                        \
		{                         \
			LED_CHARGING_PIN = 0; \
		} while (0);              \
	}
#define LED_CHARGING_OFF()        \
	{                             \
		do                        \
		{                         \
			LED_CHARGING_PIN = 1; \
		} while (0);              \
	}
#define LED_FULL_CHARGE_ON()         \
	{                                \
		do                           \
		{                            \
			LED_FULL_CHARGE_PIN = 0; \
		} while (0);                 \
	}
#define LED_FULL_CHARGE_OFF()        \
	{                                \
		do                           \
		{                            \
			LED_FULL_CHARGE_PIN = 1; \
		} while (0);                 \
	}
/*ʵ�ʲ�ø��ߵ�ƽΪ����*/
#define HEATING_ON()              \
	{                             \
		do                        \
		{                         \
			CONTROL_HEAT_PIN = 1; \
		} while (0);              \
	}
#define HEATING_OFF()             \
	{                             \
		do                        \
		{                         \
			CONTROL_HEAT_PIN = 0; \
		} while (0);              \
	}

enum
{
	KEY_NONE = 0,
	KEY_HEAT_PRESS,	  // ���Ƽ��ȵİ�������
	KEY_CHANGE_PRESS, // ����ģʽ�İ�������
	KEY_POWER_PRESS,  // ��Դ��������
};
volatile u8 key_press_flag; // ��Ű���״̬�ı�־λ

volatile u8 i; // ѭ������ֵ

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
typedef union
{
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
} bit_flag;
volatile bit_flag flag1;
#define FLAG_IS_DEVICE_OPEN flag1.bits.bit0 // �豸�Ƿ񿪻��ı�־λ��0--δ������1--����
// #define    	FLAG_TIMER0_5000ms  	flag1.bits.bit0

#endif // end __USER_H

/**************************** end of file *********************************************/

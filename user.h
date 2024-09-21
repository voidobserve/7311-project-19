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

#define USE_MY_DEBUG 1

#define ADCDETECT_CHARING_THRESHOLD 4095 // ����Ƿ����adcֵ

/*
	������Ƿ�������adcֵ
	�����Ǽ���ó���
	ԭ����õ�ص�ѹ 8.04V���ڼ�����������ñ�����2.59V��ADCת�����ֵ�� 2753 -- 2.01V
	��ô���㣬����ʱ��ص�ѹΪ 8.40V,�����ñ���Ӧ����2.70V,ADCת�����ֵ�� 2852
*/
#define ADCDETECT_BAT_THRESHOLD 2852 //

#define ONE_CYCLE_TIME_MS 80 // һ����ѭ���ĺ�ʱ����λ��ms

// #define //
#define UNUSED_PIN P10D // P10��14��оƬ��û�е�����

// ����ָʾ�Ƶ����Ŷ���
#define LED_WORKING_PIN P14D	 // ����ָʾ��
#define LED_CHARGING_PIN P04D	 // ���ָʾ��
#define LED_FULL_CHARGE_PIN P11D // ����ָʾ��

// ��ⰴ��״̬�����Ŷ��壬��⵽�͵�ƽΪ��Ч
#define KEY_HEAT_PIN P00D	// �����Ƿ���ȵ�����
#define KEY_CHANGE_PIN P01D // ����ģʽ������
#if USE_MY_DEBUG
#define KEY_POWER_PIN P05D // �����Ƿ���������(����ʹ��P05������)
#else
#define KEY_POWER_PIN P13D // �����Ƿ���������
#endif

#define CONTROL_HEAT_PIN P12D // �������Ƽ��ȵ�����

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

// ����adc�������ţ�����adc�л��������
enum
{
	ADC_PIN_P03_AN2 = 1, // ����ؽ�ѹ��ĵ�ѹ������
	ADC_PIN_P02_AN1,	 // ����Ƿ��г��ĵ�ѹ
};

// ����ģʽ�����ֲ�ͬƵ�ʵ�ģʽ
enum
{
	MODE_1 = 0, // һ�ϵ磬���µ�Դ������ʹ�õ�ģʽ
	MODE_2,
	MODE_3,
};
volatile u8 mode_flag; // ���ģʽ�ı�־λ

// ���尴����״̬
enum
{
	KEY_NONE = 0,
	KEY_HEAT_PRESS,	  // ���Ƽ��ȵİ�������
	KEY_CHANGE_PRESS, // ����ģʽ�İ�������
	KEY_POWER_PRESS,  // ��Դ��������
};
volatile u8 key_press_flag; // ��Ű���״̬�ı�־λ

volatile u8 i; // ѭ������ֵ

volatile u16 adc_val; // ���adc��⵽����ֵ

// u16 mode_pwm_duty; // ��Ų�ͬģʽ�£���Ӧ��pwmռ�ձȣ�ʵ��ռ�ձ� == TxDATA / TxLOAD

volatile u32 turn_dir_ms_cnt; // ����������ʱÿ2min�л�һ��ת��ļ�ʱ����
volatile u32 shut_down_ms_cnt;	  // �������(��������15min���Զ��ػ�)

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
#define FLAG_IS_HEATING flag1.bits.bit1		// �����Ƿ����ı�־λ
#define FLAG_IS_IN_CHARGING flag1.bits.bit2 // �Ƿ��ڳ��ı�־λ
#define FLAG_DIR flag1.bits.bit3			// ��ת����ת�ı�־λ�� 0--��ת��Ĭ����0Ϊ��ת����1--��ת

#define FLAG_BAT_IS_NEED_CHARGE flag1.bits.bit4 // ����Ƿ���Ҫ���ı�־λ, 0--����Ҫ��磬1--��Ҫ���

#endif // end __USER_H

/**************************** end of file *********************************************/

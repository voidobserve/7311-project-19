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

// #define ADCDETECT_CHARING_THRESHOLD 4095 // ����Ƿ����adcֵ
#define ADCDETECT_CHARING_THRESHOLD 2048 // ����Ƿ����adcֵ

/*
	���µİ壬����ؽ�ѹ��ĵ�ѹ���ⲿ����������Ϊ1M���ⲿ����������Ϊ220K��
	��ô��ѹϵ�� == 220K / (1M + 220K) == 0.180��ʵ�ʲ�úͼ���� 0.174
	��ؼ�����ż���adֵ���ʵ�ʵĻ�ҪС300mV���ڼ�����ŵĵ�ѹ����
	���ҵ����0.95A��1.2A���ʱ����ص�ѹ������0.04V���ڼ�����ŵĵ�ѹ������Ҫ��ȥ��Щ����

	ͨ��һ����Դ����Ϊ��أ�����Դ�趨��ѹ��8.39V��ADC���õĲο���ѹΪ�ڲ���3V��12λ���ȣ�0~4095��
	��AD��õ����ݣ�1596��1595��1596��1595
	�ڵ�ؽ�ѹ��ļ��IO��õ����ݣ�1.46V

	����Դ�趨��ѹ��8.00V��
	��AD��õ����ݣ�1502��1501��1502��1501
	�ڵ�ؽ�ѹ��ļ��IO��õ����ݣ�1.39V

	��ʵ�ʵĵ�������룬��ص�ѹ��7.98~7.99V��
	��AD��õ����ݣ�1488��1487��1488��1487��
	�ڵ�ؽ�ѹ��ļ��IO��õ����ݣ�1.39V
*/
// #define ADCDETECT_BAT_FULL 1595		 // ������Ƿ�������adcֵ--��Ӧ��ص�ѹ 8.40V
// #define ADCDETECT_BAT_WILL_FULL 1501 // ��ؽ�Ҫ�����adcֵ--��Ӧ��ص�ѹ 8.00V
// #define ADCDETECT_BAT_EMPTY 1147	 // ���Ϊ�գ���ô���ʱֻ�г��ĵ�ѹ�����û�����PWM������ѹ�Ļ���ʵ�ʲ��ֻ�� 0.84V
#define ADCDETECT_BAT_FULL 1640		 // ������Ƿ�������adcֵ--��Ӧ��ص�ѹ 8.40V
#define ADCDETECT_BAT_WILL_FULL 1545 // ��ؽ�Ҫ�����adcֵ--��Ӧ��ص�ѹ 8.00V
#define ADCDETECT_BAT_EMPTY 1147	 // ���Ϊ�գ���ô���ʱֻ�г��ĵ�ѹ�����û�����PWM������ѹ�Ļ���ʵ�ʲ��ֻ�� 0.84V
// #define ADCDETECT_BAT_EMPTY 833	 //�����������ݣ��ڳ��ʱ��ε�أ�ָʾ�ƻ�������˸�� ���Ϊ�գ���ô���ʱֻ�г��ĵ�ѹ�����û�����PWM������ѹ�Ļ���ʵ�ʲ��ֻ�� 0.84V

#define ONE_CYCLE_TIME_MS 80 // һ����ѭ���ĺ�ʱ����λ��ms

#define UNUSED_PIN P10D // P10��14��оƬ��û�е�����

// ����ָʾ�Ƶ����Ŷ���
#define LED_WORKING_PIN P14D	 // ����ָʾ��
#define LED_CHARGING_PIN P04D	 // ���ָʾ��
#define LED_FULL_CHARGE_PIN P03D // ����ָʾ��

// ��ⰴ��״̬�����Ŷ��壬��⵽�͵�ƽΪ��Ч
#if USE_MY_DEBUG
#define KEY_HEAT_PIN P05D // �����Ƿ���ȵ�����(����ʹ��P05������)
#else
#define KEY_HEAT_PIN P13D // �����Ƿ���ȵ�����
#endif
#define KEY_POWER_PIN P11D	// �����Ƿ���������
#define KEY_CHANGE_PIN P01D // ����ģʽ������

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
	ADC_PIN_P00_AN0 = 1, // ����Ƿ��г��ĵ�ѹ
	ADC_PIN_P02_AN1,	 // ����ؽ�ѹ��ĵ�ѹ������
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

volatile u32 turn_dir_ms_cnt;  // ����������ʱÿ2min�л�һ��ת��ļ�ʱ����
volatile u32 shut_down_ms_cnt; // �������(��������15min���Զ��ػ�)

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
#define FLAG_BAT_IS_FULL flag1.bits.bit5 // ����Ƿ�����ı�־λ��0--δ���磬1--����

#endif // end __USER_H

/**************************** end of file *********************************************/

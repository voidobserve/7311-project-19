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
#define uint8_t unsigned char
#define uint16_t unsigned int
#define uint32_t unsigned long int

#define USE_MY_DEBUG 1

// #define ADCDETECT_CHARING_THRESHOLD 4095 // 检测是否充电的adc值
#define ADCDETECT_CHARING_THRESHOLD 2048 // 检测是否充电的adc值

/*
	最新的板，检测电池降压后的电压，外部的上拉电阻为1M，外部的下拉电阻为220K，
	那么分压系数 == 220K / (1M + 220K) == 0.180，实际测得和计算得 0.174
	电池检测引脚检测的ad值会比实际的还要小300mV（在检测引脚的电压），
	而且电池以0.95A或1.2A充电时，电池电压会提升0.04V（在检测引脚的电压），需要减去这些数据

	通过一个电源来作为电池，给电源设定电压：8.39V，ADC配置的参考电压为内部的3V，12位精度（0~4095）
	在AD测得的数据：1596、1595、1596、1595
	在电池降压后的检测IO测得的数据：1.46V

	给电源设定电压：8.00V，
	在AD测得的数据：1502、1501、1502、1501
	在电池降压后的检测IO测得的数据：1.39V

	用实际的电池来接入，电池电压：7.98~7.99V，
	在AD测得的数据：1488、1487、1488、1487，
	在电池降压后的检测IO测得的数据：1.39V
*/
// #define ADCDETECT_BAT_FULL 1595		 // 检测电池是否充满电的adc值--对应电池电压 8.40V
// #define ADCDETECT_BAT_WILL_FULL 1501 // 电池将要满电的adc值--对应电池电压 8.00V
// #define ADCDETECT_BAT_EMPTY 1147	 // 电池为空，那么充电时只有充电的电压，如果没有输出PWM控制升压的话，实际测得只有 0.84V
#define ADCDETECT_BAT_FULL 1640		 // 检测电池是否充满电的adc值--对应电池电压 8.40V
#define ADCDETECT_BAT_WILL_FULL 1545 // 电池将要满电的adc值--对应电池电压 8.00V
#define ADCDETECT_BAT_EMPTY 1147	 // 电池为空，那么充电时只有充电的电压，如果没有输出PWM控制升压的话，实际测得只有 0.84V
// #define ADCDETECT_BAT_EMPTY 833	 //（填这项数据，在充电时插拔电池，指示灯会来回闪烁） 电池为空，那么充电时只有充电的电压，如果没有输出PWM控制升压的话，实际测得只有 0.84V

#define ONE_CYCLE_TIME_MS 80 // 一次主循环的耗时，单位：ms

#define UNUSED_PIN P10D // P10是14脚芯片上没有的引脚

// 驱动指示灯的引脚定义
#define LED_WORKING_PIN P14D	 // 工作指示灯
#define LED_CHARGING_PIN P04D	 // 充电指示灯
#define LED_FULL_CHARGE_PIN P03D // 满电指示灯

// 检测按键状态的引脚定义，检测到低电平为有效
#if USE_MY_DEBUG
#define KEY_HEAT_PIN P05D // 控制是否加热的引脚(这里使用P05来仿真)
#else
#define KEY_HEAT_PIN P13D // 控制是否加热的引脚
#endif
#define KEY_POWER_PIN P11D	// 控制是否工作的引脚
#define KEY_CHANGE_PIN P01D // 控制模式的引脚

#define CONTROL_HEAT_PIN P12D // 驱动控制加热的引脚

#ifndef CONTROL_HEAT_PIN
#define CONTROL_HEAT_PIN UNUSED_PIN // 驱动控制加热的引脚
#endif
#ifndef KEY_HEAT_PIN
#define KEY_HEAT_PIN UNUSED_PIN // 控制是否加热的引脚
#endif
#ifndef KEY_CHANGE_PIN
#define KEY_CHANGE_PIN UNUSED_PIN // 控制模式的引脚
#endif
#ifndef KEY_POWER_PIN
#define KEY_POWER_PIN UNUSED_PIN // 控制是否工作的引脚
#endif

#ifndef LED_WORKING_PIN
#define LED_WORKING_PIN UNUSED_PIN // 工作指示灯
#endif
#ifndef LED_CHARGING_PIN
#define LED_CHARGING_PIN UNUSED_PIN // 充电指示灯
#endif
#ifndef LED_FULL_CHARGE_PIN
#define LED_FULL_CHARGE_PIN UNUSED_PIN // 满电指示灯
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
/*实际测得给高电平为加热*/
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

// 定义adc检测的引脚，用在adc切换检测引脚
enum
{
	ADC_PIN_P00_AN0 = 1, // 检测是否有充电的电压
	ADC_PIN_P02_AN1,	 // 检测电池降压后的电压的引脚
};

// 定义模式，三种不同频率的模式
enum
{
	MODE_1 = 0, // 一上电，按下电源按键，使用的模式
	MODE_2,
	MODE_3,
};
volatile u8 mode_flag; // 存放模式的标志位

// 定义按键的状态
enum
{
	KEY_NONE = 0,
	KEY_HEAT_PRESS,	  // 控制加热的按键按下
	KEY_CHANGE_PRESS, // 控制模式的按键按下
	KEY_POWER_PRESS,  // 电源按键按下
};
volatile u8 key_press_flag; // 存放按键状态的标志位

volatile u8 i; // 循环计数值

volatile u16 adc_val; // 存放adc检测到的数值

volatile u32 turn_dir_ms_cnt;  // 控制在运行时每2min切换一次转向的计时变量
volatile u32 shut_down_ms_cnt; // 毫秒计数(用于运行15min后自动关机)

// demo提供的中断服务程序使用到的两个变量：
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
#define FLAG_IS_DEVICE_OPEN flag1.bits.bit0 // 设备是否开机的标志位，0--未开机，1--开机
#define FLAG_IS_HEATING flag1.bits.bit1		// 加热是否工作的标志位
#define FLAG_IS_IN_CHARGING flag1.bits.bit2 // 是否处于充电的标志位
#define FLAG_DIR flag1.bits.bit3			// 正转，反转的标志位， 0--正转（默认是0为正转），1--反转

#define FLAG_BAT_IS_NEED_CHARGE flag1.bits.bit4 // 电池是否需要充电的标志位, 0--不需要充电，1--需要充电
#define FLAG_BAT_IS_FULL flag1.bits.bit5 // 电池是否满电的标志位，0--未满电，1--满电

#endif // end __USER_H

/**************************** end of file *********************************************/

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

#define ADCDETECT_CHARING_THRESHOLD 4095 // 检测是否充电的adc值

/*
	检测电池是否充满电的adc值
	这里是计算得出，
	原来测得电池电压 8.04V，在检测引脚用万用表测得是2.59V，ADC转换后的值是 2753 -- 2.01V
	那么计算，满电时电池电压为 8.40V,在万用表测得应该是2.70V,ADC转换后的值是 2852
*/
#define ADCDETECT_BAT_THRESHOLD 2852 //

#define ONE_CYCLE_TIME_MS 80 // 一次主循环的耗时，单位：ms

// #define //
#define UNUSED_PIN P10D // P10是14脚芯片上没有的引脚

// 驱动指示灯的引脚定义
#define LED_WORKING_PIN P14D	 // 工作指示灯
#define LED_CHARGING_PIN P04D	 // 充电指示灯
#define LED_FULL_CHARGE_PIN P11D // 满电指示灯

// 检测按键状态的引脚定义，检测到低电平为有效
#define KEY_HEAT_PIN P00D	// 控制是否加热的引脚
#define KEY_CHANGE_PIN P01D // 控制模式的引脚
#if USE_MY_DEBUG
#define KEY_POWER_PIN P05D // 控制是否工作的引脚(这里使用P05来仿真)
#else
#define KEY_POWER_PIN P13D // 控制是否工作的引脚
#endif

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
	ADC_PIN_P03_AN2 = 1, // 检测电池降压后的电压的引脚
	ADC_PIN_P02_AN1,	 // 检测是否有充电的电压
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

// u16 mode_pwm_duty; // 存放不同模式下，对应的pwm占空比，实际占空比 == TxDATA / TxLOAD

volatile u32 turn_dir_ms_cnt; // 控制在运行时每2min切换一次转向的计时变量
volatile u32 shut_down_ms_cnt;	  // 毫秒计数(用于运行15min后自动关机)

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

#endif // end __USER_H

/**************************** end of file *********************************************/

/******************************************************************************
;  *   	@型号				  : MC32P7311
;  *   	@创建日期             : 2021.08.04
;  *   	@公司/作者			  : SINOMCU-FAE
;  *   	@晟矽微技术支持       : 2048615934
;  *   	@晟矽微官网           : http://www.sinomcu.com/
;  *   	@版权                 : 2021 SINOMCU公司版权所有.
;  *----------------------摘要描述---------------------------------
******************************************************************************/

#include "user.h"

// 毫秒级延时(约有5%的误差)
// 前提条件：FCPU = FOSC / 4
void delay_ms(u32 xms)
{
	while (xms)
	{
		u32 i = 287;
		while (i--)
		{
			Nop();
		}

		__asm;
		CLRWDT; // 喂狗指令
		__endasm;
		xms--;
	}
}

#if USE_MY_DEBUG
// 发送32位数据，低位先行
void send_32bits_data_by_irsir(u32 send_data)
{
	// 先发送格式头
	// __set_input_pull_up(); // 高电平
	P10D = 1;
	delay_ms(15);
	// __set_output_open_drain(); // 低电平
	P10D = 0;
	delay_ms(7); // 延时4.5ms

	for (u8 i = 0; i < 32; i++)
	{
		if ((send_data >> i) & 0x01)
		{
			// 如果要发送逻辑1
			// __set_input_pull_up();	   // 高电平
			P10D = 1;
			delay_ms(6); //
			// __set_output_open_drain(); // 低电平
			P10D = 0;
			delay_ms(17); //
		}
		else
		{
			// 如果要发送逻辑0
			// __set_input_pull_up();	   // 高电平
			P10D = 1;
			delay_ms(6); //
			// __set_output_open_drain(); // 低电平
			P10D = 0;
			delay_ms(6); //
		}
	}

	// 最后，设置为低电平
	// __set_output_open_drain(); // 低电平
	P10D = 0;
}
#endif

/************************************************
;  *    @Function Name       : C_RAM
;  *    @Description         : 初始化RAM
;  *    @IN_Parameter      	 :
;  *    @Return parameter    :
;  ***********************************************/
void CLR_RAM(void)
{
	for (FSR0 = 0; FSR0 < 0xff; FSR0++)
	{
		INDF0 = 0x00;
	}
	FSR0 = 0xFF;
	INDF0 = 0x00;
}
/************************************************
;  *    @Function Name       : IO_Init
;  *    @Description         : io口初始化
;  *    @IN_Parameter      	 :
;  *    @Return parameter    :
;  ***********************************************/
void IO_Init(void)
{
	IOP0 = 0x00;   // io口数据位
	OEP0 = 0xFF;   // io口方向 1:out  0:in   P00、1输出为开漏
	PUP0 = 0x00;   // io口上拉电阻   1:enable  0:disable
	PDP0 = 0x00;   // io口下拉电阻   1:enable  0:disable
	ANSEL0 = 0x00; // io类型选择  1:模拟输入  0:通用io

	IOP1 = 0x00;   // io口数据位
	OEP1 = 0xFF;   // io口方向 1:out  0:in   P13输出为开漏
	PUP1 = 0x00;   // io口上拉电阻   1:enable  0:disable
	PDP1 = 0x00;   // io口下拉电阻   1:enable  0:disable
	ANSEL1 = 0x00; // io类型选择  1:模拟输入  0:通用io

	DRVCR = 0x80;
}

// 定时器0的pwm配置--输出引脚 P16
void timer0_pwm_config(void)
{
	// ====================================================
	// 接近15.645KHz版本，前提条件：FCPU = FOSC / 4
	T0CR |= 0x02; // 4分频
	T0LOAD = 128 - 1;
	T0DATA = 25; // 占空比 == T0DATA / T0LOAD
	PWM0OE = 0;	 // 禁止PWM输出
	// PWM0OE = 1;
	TC0EN = 1; // 启动定时器

	// ====================================================
	// 接近154.19KHz版本，前提条件：FCPU = FOSC / 4
	// FCPU = FOSC / 4 之后，FCPU为 8MHz，定时器即使不分频，计数一次约为0.125us
	// T0LOAD 至少要有100以上，才能更方便地调节占空比，因此定时器计数溢出一次的时间约为12.5us，约为80KHz，做不到154.19KHz
	// T0LOAD = 100 - 1;
	// T0DATA = 25; // 占空比 == T0DATA / T0LOAD
	// // PWM0OE = 0; // 禁止PWM输出
	// PWM0OE = 1; // 使能PWM输出
	// TC0EN = 1;	// 启动定时器

	// 如果一定要接近154.19KHz，使用以下配置：
	// T0LOAD = 52 - 1;
	// T0DATA = 20; // 占空比 == T1DATA / T1LOAD
	// // PWM0OE = 0;	 // 禁止PWM输出
	// PWM0OE = 1; // 使能PWM输出
	// TC0EN = 1;
}

// 定时器1pwm配置，输出引脚 P17
void timer1_pwm_config(void)
{
	// ====================================================
	// 接近15.645KHz版本，前提条件：FCPU = FOSC / 4
	T1CR |= 0x02; // 4分频
	T1LOAD = 128 - 1;
	T1DATA = 25; // 占空比 == T1DATA / T1LOAD
	PWM1OE = 0;	 // 禁止PWM1输出
	TC1EN = 1;	 // 启动定时器

	// ====================================================
	// 接近154.19KHz版本，前提条件：FCPU = FOSC / 4
	// FCPU = FOSC / 4 之后，FCPU为 8MHz，定时器即使不分频，计数一次约为0.125us
	// T1LOAD 至少要有100以上，才能更方便地调节占空比，因此定时器计数溢出一次的时间约为12.5us，约为80KHz，做不到154.19KHz
	// T1LOAD = 100 - 1;
	// T1DATA = 25; // 占空比 == T1DATA / T1LOAD
	// // PWM1OE = 0; // 禁止PWM1输出
	// // PWM1OE = 1; // 使能PWM1输出
	// TC1EN = 1;	// 启动定时器

	// 如果一定要接近154.19KHz，使用以下配置：
	// T1LOAD = 52 - 1;
	// T1DATA = 20; // 占空比 == T1DATA / T1LOAD
	// PWM1OE = 0;	 // 禁止PWM1输出
	// PWM1OE = 1;	 // 使能PWM1输出
	// TC1EN = 1;	 // 启动定时器
}

// // 开启定时器1的pwm，默认是PWM1
// void timer1_pwm_open(void)
// {
// 	PWM1OE = 1;
// }

// // 关闭定时器1的pwm
// void timer1_pwm_close(void)
// {
// 	PWM1OE = 0;
// }

// 定时器2的PWM配置，输出引脚 P15
void timer2_pwm_config(void)
{
	// ====================================================
	// 接近15.645KHz版本，前提条件：FCPU = FOSC / 4
	// T2CR |= 0x02; // 4分频
	// T2LOAD = 128 - 1;
	// T2DATA = 25; // 占空比 == T2DATA / T2LOAD
	// // PWM2OE = 0; // 禁止PWM输出
	// PWM2OE = 1;
	// TC2EN = 1; // 启动定时器

	// ====================================================
	// 接近154.19KHz版本，前提条件：FCPU = FOSC / 4
	// FCPU = FOSC / 4 之后，FCPU为 8MHz，定时器即使不分频，计数一次约为0.125us
	// T2LOAD 至少要有100以上，才能更方便地调节占空比，因此定时器计数溢出一次的时间约为12.5us，约为80KHz，做不到154.19KHz
	// T2LOAD = 100 - 1;
	// T2DATA = 25; // 占空比 == T2DATA / T2LOAD
	// // PWM2OE = 0; // 禁止PWM输出
	// PWM2OE = 1; // 使能PWM输出
	// TC2EN = 1;	// 启动定时器

	// 如果一定要接近154.19KHz，使用以下配置：
	T2LOAD = 52 - 1;
	// T2DATA = 22; // 占空比 == TxDATA / TxLOAD
	// T2DATA = 23; // 占空比 == TxDATA / TxLOAD   实际测得是43%占空比
	T2DATA = 24; // 占空比 == TxDATA / TxLOAD   实际测得是46.15%占空比
	PWM2OE = 0;	 // 禁止PWM输出
	// PWM2OE = 1;	 // 使能PWM输出
	TC2EN = 1;
}

// 按键检测引脚的配置：
void key_config(void)
{
	// 控制是否加热的按键的配置
#if USE_MY_DEBUG
	// 由于P13无法参与仿真，这里使用P05
	P05PU = 1;
	P05OE = 0;
#else
	P13PU = 1; // 上拉
	P13OE = 0; // 输入模式
#endif
	// P00PU = 1;
	// P00OE = 0;

	// 开机/关机按键的配置，一定要中断触发：
	MINT00 = 1; // MINT00、MINT01，组合配置INT0为下降沿中断
	MINT01 = 0;
	P11PU = 1;
	P11OE = 0;
	INT0IF = 0; // 清除中断标志
	INT0IE = 1; // 外部中断使能

	// 切换模式按键的配置
	P01PU = 1;
	P01OE = 0;
}

// adc配置
void adc_config(void)
{
	// 检测是否有充电的电压：
	P00PU = 0; // 关闭上下拉
	P00PD = 0;
	P00OE = 0;	// 输入模式
	P00ANS = 1; // 模拟输入

	// 检测电池降压后的电压
	P02PU = 0; // 关闭上下拉
	P02PD = 0;
	P02OE = 0;	// 输入模式
	P02ANS = 1; // 模拟输入

	ADCR0 = 0x0A; // 12位精度、不启用ADC转换，不使能ADC
	// ADCR0 |= 0x03 << 5; // AIN6--P11

	// ADCR1 &= 0x00; // 清零寄存器
	// ADCR1 |= 0xE1; // 125K采样  内部3V参考电压
	ADCR1 = 0xE1; // 125K采样（最高精度）  内部3V参考电压
	ADCR2 = 0xFF; // ADC采样时间为15个ADC时钟
	ADON = 1;	  // 使能ADC
}

// 切换adc检测的引脚
void adc_sel_pin(u8 adc_pin)
{
	// 根据传参，切换成对应的通道
	switch (adc_pin)
	{
	case ADC_PIN_P00_AN0:
		ADCR0 &= ~(0x0F << 4); // 清空寄存器的通道选择位
		// 清空后的通道就是 AIN0--P00
		break;

	case ADC_PIN_P02_AN1:
		ADCR0 &= ~(0x0F << 4); // 清空寄存器的通道选择位
		ADCR0 |= 0x01 << 4;	   // AIN1--P02;
		break;

	default:
		break;
	}

	delay_ms(1); // 切换adc检测的引脚后，要延时一段时间，防止意料之外的检测结果
}

// 获取adc单次转换后的值
u16 adc_get_val(void)
{
	u8 cnt = 0; // adc采集次数的计数
	u16 g_temp_value = 0;
	u16 g_tmpbuff = 0;
	u16 g_adcmax = 0;
	u16 g_adcmin = 0xFFFF;

	// 采集20次，去掉前两次采样，再去掉一个最大值和一个最小值，再取平均值
	for (cnt = 0; cnt < 20; cnt++)
	{
		ADEOC = 0; // 清除ADC转换完成标志位，启动AD转换
		while (!ADEOC)
			;				 // 等待转换完成
		g_temp_value = ADRH; // 取出转换后的值
		g_temp_value = g_temp_value << 4 | (ADRL & 0x0F);
		if (cnt < 2)
			continue; // 丢弃前两次采样的
		if (g_temp_value > g_adcmax)
			g_adcmax = g_temp_value; // 最大
		if (g_temp_value < g_adcmin)
			g_adcmin = g_temp_value; // 最小
		g_tmpbuff += g_temp_value;
	}
	g_tmpbuff -= g_adcmax;			 // 去掉一个最大
	g_tmpbuff -= g_adcmin;			 // 去掉一个最小
	g_temp_value = (g_tmpbuff >> 4); // 除以16，取平均值

	return g_temp_value;
}

/************************************************
;  *    @Function Name       : Sys_Init
;  *    @Description         : 系统初始化
;  *    @IN_Parameter      	 :
;  *    @Return parameter    :
;  ***********************************************/
void Sys_Init(void)
{
	GIE = 0;
	CLR_RAM();
	IO_Init();

	timer0_pwm_config();
	timer1_pwm_config();
	timer2_pwm_config();

	key_config();
	adc_config();

	GIE = 1;
}

// 按键扫描函数
void key_scan(void)
{
	u8 cnt = 0;

	if (0 == KEY_POWER_PIN)
	{
		for (i = 0; i < 20; i++)
		{
			if (0 == KEY_POWER_PIN)
			{
				cnt++;
			}
			delay_ms(1);
		}

		if (cnt >= 15)
		{
			key_press_flag = KEY_POWER_PRESS;
		}

		// 等待按键松开
		while (0 == KEY_POWER_PIN)
		{
			__asm;
			CLRWDT; // 喂狗指令
			__endasm;
		}
	}

	if (FLAG_IS_DEVICE_OPEN)
	{
		// 如果设备开启，才扫描 控制加热的按键 和 控制切换模式的按键
		if (0 == KEY_HEAT_PIN)
		{
			for (i = 0; i < 20; i++)
			{
				if (0 == KEY_HEAT_PIN)
				{
					cnt++;
				}
				delay_ms(1);
			}

			if (cnt >= 15)
			{
				key_press_flag = KEY_HEAT_PRESS;
			}

			while (0 == KEY_HEAT_PIN) // 等待按键松开
			{
				__asm;
				CLRWDT; // 喂狗指令
				__endasm;
			}
		}
		else if (0 == KEY_CHANGE_PIN)
		{
			for (i = 0; i < 20; i++)
			{
				if (0 == KEY_CHANGE_PIN)
				{
					cnt++;
				}
				delay_ms(1);
			}

			if (cnt >= 15)
			{
				key_press_flag = KEY_CHANGE_PRESS;
			}

			while (0 == KEY_CHANGE_PIN) // 等待按键松开
			{
				__asm;
				CLRWDT; // 喂狗指令
				__endasm;
			}
		}
	}
}

// 处理按键对应的功能
void key_handle(void)
{
	if (key_press_flag)
	{
		// 如果有按键按下，进一步判断是哪个按键按下
		switch (key_press_flag)
		{
		case KEY_HEAT_PRESS: // 加热按键按下
			if (FLAG_IS_DEVICE_OPEN)
			{
				// 如果设备已经处于工作状态，才可以打开加热
				if (0 == FLAG_IS_HEATING)
				{
					HEATING_ON(); // 打开加热
					FLAG_IS_HEATING = 1;
				}
				else
				{
					HEATING_OFF(); // 关闭加热
					FLAG_IS_HEATING = 0;
				}
			}
			break;

		case KEY_CHANGE_PRESS: // 控制模式的按键按下
			// 初始为 89.9%占空比，每按一次 从 89.9%->100%->80.5%->89.9%->...这样变化
			if (FLAG_IS_DEVICE_OPEN)
			{
				// 如果开机，才切换模式
				if (MODE_1 == mode_flag)
				{
					// 设置PWM的占空比
					T0DATA = 255;
					T1DATA = 255; // 100%占空比（确保大于TxLOAD的值就可以）

					mode_flag = MODE_2;
				}
				else if (MODE_2 == mode_flag)
				{
					// 设置PWM的占空比
					T0DATA = 103;
					T1DATA = 103; // 80.5%占空比

					mode_flag = MODE_3;
				}
				else if (MODE_3 == mode_flag)
				{
					// 设置PWM的占空比
					T0DATA = 115;
					T1DATA = 115; // 89.9%占空比

					mode_flag = MODE_1;
				}
			}
			break;

		case KEY_POWER_PRESS: // 电源按键按下
			if (0 == FLAG_IS_DEVICE_OPEN)
			{
				// 如果未开机，关机->开机
				LED_WORKING_ON(); // 打开电源指示灯
				HEATING_ON();	  // 打开加热
				FLAG_IS_DEVICE_OPEN = 1;
				FLAG_IS_HEATING = 1;

				// 设定正转、反转的PWM的初始占空比
				T0DATA = 115;
				T1DATA = 115;		// 约为 89.9%
				mode_flag = MODE_1; // 下一次切换模式时，会变成 MODE_2

				// 打开控制正转的PWM
				PWM0OE = 1;
			}
			else
			{
				// 开机->关机
				LED_WORKING_OFF(); // 关闭电源指示灯
				HEATING_OFF();	   // 关闭加热
				FLAG_IS_DEVICE_OPEN = 0;
				FLAG_IS_HEATING = 0;

				mode_flag = MODE_1; // 下一次切换模式时，会变成 MODE_2

				// 关闭 正转和反转的PWM
				PWM0OE = 0;
				PWM1OE = 0;
			}
			break;

		default:
			break;
		}

		key_press_flag = KEY_NONE;
	}
}

void adc_scan_handle(void)
{
	volatile u8 cnt = 0;				  // 计数值，用于检测电池是否满电，也是用于检测是否插入/拔出充电器的计数值
	volatile u8 need_charge_cnt = 0;	  // 计数值，用于检测是否要快速充电
	volatile u8 need_charge_slow_cnt = 0; // 计数值，用于检测是否要稍微降低充电速度
	volatile u8 flag_bat_is_empty = 0;	  // 标志位，用于检测是否拔出了电池

	adc_sel_pin(ADC_PIN_P02_AN1); // 切换到检测电池降压后的电压的检测引脚
	for (i = 0; i < 10; i++)
	{
		adc_val = adc_get_val();
		// send_32bits_data_by_irsir(adc_val);
		if (FLAG_IS_IN_CHARGING)
		{
			// 如果正在充电，检测是否充满电
			if (adc_val <= ADCDETECT_BAT_EMPTY) // 如果有一次电池一侧的电压小于5V，认为是拔出了电池
			{
				flag_bat_is_empty = 1;
			}
			else if (adc_val >= ADCDETECT_BAT_FULL) // 检测电池是否满电
			{
				cnt++;
			}
			else if (adc_val < ADCDETECT_BAT_WILL_FULL) // 检测是否要快速充电
			{
				need_charge_cnt++;
			}
			else if (adc_val >= ADCDETECT_BAT_WILL_FULL) // 检测是否要稍微降低充电速度
			{
				need_charge_slow_cnt++;
			}

			if (flag_bat_is_empty)
			{
				// 如果检测到拔出了电池
				LED_FULL_CHARGE_ON(); // 开启电池充满电的指示灯
				LED_CHARGING_OFF();	  // 关闭充电指示灯
				PWM2OE = 0;			  // 关闭控制升压电路的pwm
				FLAG_BAT_IS_NEED_CHARGE = 0;
				break;
			}
			else if (cnt >= 8)
			{
				// 如果在充电时，电池之前需要充电，现在检测到充满电
				LED_FULL_CHARGE_ON(); // 开启电池充满电的指示灯
				LED_CHARGING_OFF();	  // 关闭充电指示灯
				PWM2OE = 0;			  // 关闭控制升压电路的pwm
				// FLAG_IS_IN_CHARGING = 0; // 不能给这个标志位清零（交给充电扫描来清零）
				FLAG_BAT_IS_NEED_CHARGE = 0;
				FLAG_BAT_IS_FULL = 1;
				break;
			}
			else if (need_charge_cnt >= 8)
			{
				// 如果在充电，且电池需要充电（电池电量小于 ADCDETECT_BAT_WILL_FULL，电池将要满电的电压）
				T2DATA = 24;		   // 占空比 == TxDATA / TxLOAD   实际测得是46.15%占空比
				LED_FULL_CHARGE_OFF(); // 关闭充满电的指示灯
				FLAG_BAT_IS_NEED_CHARGE = 1;
				FLAG_BAT_IS_FULL = 0;

#if USE_MY_DEBUG
// send_32bits_data_by_irsir(0xA5A5);
#endif
				break;
			}
			else if (need_charge_slow_cnt >= 8 && 0 == FLAG_BAT_IS_FULL)
			{
				// 如果需要降低充电速度
				T2DATA = 23;		   // 占空比 == TxDATA / TxLOAD   实际测得是41.54%、43%或44.62 这两个占空比
				LED_FULL_CHARGE_OFF(); // 关闭充满电的指示灯
				FLAG_BAT_IS_NEED_CHARGE = 1;

#if USE_MY_DEBUG
				// P10D = ~P10D;
// send_32bits_data_by_irsir(0x5A5A);
#endif
				break;
			}
		} // if (FLAG_IS_IN_CHARGING)
		else // 如果未在充电，检测电池是否需要充电
		{
			if (adc_val <= ADCDETECT_BAT_FULL - 130)
			{
				cnt++;
			}

			if (cnt >= 8)
			{
				// 如果未在充电，且电池需要充电（电池电量小于 满电-死区电量 ）
				FLAG_BAT_IS_NEED_CHARGE = 1;
				FLAG_BAT_IS_FULL = 0;
				break;
			}
		}
	} // for (i = 0; i < 10; i++)

	cnt = 0;
	adc_sel_pin(ADC_PIN_P00_AN0); // 切换到检测充电的电压检测引脚
	for (i = 0; i < 10; i++)
	{
		adc_val = adc_get_val();
		if (FLAG_IS_IN_CHARGING)
		{
			// 如果正在充电，检测是否拔出了充电线
			if (adc_val < ADCDETECT_CHARING_THRESHOLD)
			{
				cnt++;
			}

			if (cnt >= 8)
			{
				// 如果在充电时，检测到拔出了充电线
				LED_CHARGING_OFF();	   // 关闭充电指示灯
				PWM2OE = 0;			   // 关闭控制升压电路的pwm
				LED_FULL_CHARGE_OFF(); // 关闭电池充满电的指示灯
				FLAG_IS_IN_CHARGING = 0;

				break;
			} // if (cnt >= 8)

			if ((PWM2OE == 0) && FLAG_BAT_IS_NEED_CHARGE)
			{
				// 如果正在充电时，检测到没有打开控制充电（升压）电路的PWM，并且电池需要充电
				// 有可能是充满了电，关闭了pwm，又使用了一段时间，导致耗电，这样要重新打开充电的功能
				LED_CHARGING_ON(); // 开启充电指示灯
				PWM2OE = 1;		   // 开启控制升压电路的pwm

				// P10D = ~P10D;
				// FLAG_IS_IN_CHARGING = 1; // 这里不用给这个标志位置一
			}

		} // if (FLAG_IS_IN_CHARGING)
		else
		{
			// 如果不在充电，检测是否插入了充电线
			if (adc_val >= ADCDETECT_CHARING_THRESHOLD)
			{
				cnt++;
			}

			if (cnt >= 8)
			{
				// if (FLAG_BAT_IS_NEED_CHARGE)
				{
					LED_CHARGING_ON(); // 开启充电指示灯
					PWM2OE = 1;		   // 开启控制升压电路的pwm
				}

				FLAG_IS_IN_CHARGING = 1;
				break;
			} // if (cnt >= 8)
		}
	} // for (i = 0; i < 10; i++)
}

void turn_dir_scan_handle(void)
{
	if (FLAG_IS_DEVICE_OPEN)
	{
		// 设备运行时，才开始计时并判断是否要转向
		turn_dir_ms_cnt += ONE_CYCLE_TIME_MS;
		if (turn_dir_ms_cnt >= 120000)
		{
			// 如果大于2min
			if (0 == FLAG_DIR)
			{
				// 如果当前是正转
				PWM0OE = 0; // 关闭驱动正转的pwm
				PWM1OE = 1; // 打开驱动反转的pwm

				FLAG_DIR = 1; // 表示当前为反转
			}
			else
			{
				// 如果当前是反转
				PWM1OE = 0; // 关闭驱动反转的pwm
				PWM0OE = 1; // 打开驱动正转的pwm

				FLAG_DIR = 0; // 表示当前为正转
			}

			turn_dir_ms_cnt = 0;
		}
	}
	else
	{
		turn_dir_ms_cnt = 0; // 设备未运行时，清空计数值
	}
}

// 关机检测和处理
void shutdown_scan_handle(void)
{
	if (FLAG_IS_DEVICE_OPEN)
	{
		// 如果设备开启，开始计时，15min后自动关机
		shut_down_ms_cnt += ONE_CYCLE_TIME_MS;
		if (shut_down_ms_cnt >= 900000)
		{
			// 如果超过了15min，关机：

			LED_WORKING_OFF(); // 关闭电源指示灯
			HEATING_OFF();	   // 关闭加热
			FLAG_IS_HEATING = 0;
			mode_flag = MODE_3; // 下一次切换模式时，会变成 MODE_1
			// 关闭 正转和反转的PWM
			PWM0OE = 0;
			PWM1OE = 0;

			FLAG_IS_DEVICE_OPEN = 0;
		}
	}
	else
	{
		// 如果设备未开启，清空计时
		shut_down_ms_cnt = 0;
	}
}

void low_power_scan_handle(void)
{
	if (FLAG_IS_DEVICE_OPEN)
	{
		// 如果设备已经启动，不进入低功耗
		return;
	}

	// 如果运行到这里，说明设备没有启动
	// 可能需要考虑正在充电的情况
	if (FLAG_IS_IN_CHARGING)
	{
		// 如果正在充电，不进入低功耗，因为可能还需要输出PWM来控制充电
		return;
	}

	GIE = 0;
	KBIF = 0;
	KBIE = 1;
	// 休眠前关闭外设 AD等 使能唤醒条件键盘中断
	Nop();
	Nop();
	Stop();
	Nop();
	Nop();
	KBIE = 0;
	KBIF = 0;
	GIE = 1;
	// 唤醒后使能外设关闭键盘中断，实际应用用户按需配置
}

void main(void)
{
	Sys_Init();
	// 关闭所有指示灯
	LED_WORKING_OFF();
	LED_FULL_CHARGE_OFF();
	LED_CHARGING_OFF();

	// adc_sel_pin(ADC_PIN_P02_AN1); // 测试用，测量电池降压后的电压

	while (1)
	{
#if USE_MY_DEBUG
		// P10D = 1; // 测试一次循环所需的时间
#endif

		key_scan();
		key_handle();
		adc_scan_handle(); // 充电扫描和处理函数

		turn_dir_scan_handle();
		shutdown_scan_handle(); // 自动关机检测和处理函数
		low_power_scan_handle();

		// adc_val = adc_get_val();
		// send_32bits_data_by_irsir(adc_val);
		// delay_ms(20);

#if USE_MY_DEBUG
		// P10D = 0; // 测试一次循环所需的时间
#endif

		__asm;
		CLRWDT; // 喂狗指令
		__endasm;
	}
}

void int_isr(void) __interrupt
{
	__asm;
	movra _abuf;
	swapar _PFLAG;
	movra _statusbuf;
	__endasm;
	// 用不到的中断可以删掉  节约ROM
	//=======T0========================
	// if (T0IF & T0IE)
	// {
	// 	T0IF = 0;
	// }
	//=======T1========================
	// if (T1IF & T1IE)
	// {
	// 	T1IF = 0;
	// }
	// //=======T2========================
	// if (T2IF & T2IE)
	// {
	// 	T2IF = 0;
	// }
	//=======T3========================
	// if (T3IF & T3IE)
	// {
	// 	T3IF = 0;
	// }
	//=======键盘======================
	// if (KBIF & KBIE)
	// {
	// 	KBIF = 0;
	// }
	//=======ADC=======================
	// if (ADIF & ADIE)
	// {
	// 	ADIF = 0;
	// }
	//=======外部中断0=================
	if (INT0IF & INT0IE)
	{
		INT0IF = 0;
	}
	// //=======外部中断1=================
	// if (INT1IF & INT1IE)
	// {
	// 	INT1IF = 0;
	// }
	__asm;
	swapar _statusbuf;
	movra _PFLAG;
	swapr _abuf;
	swapar _abuf;
	__endasm;
}

#if 0
while (1)
{
	// 延时测试：
	// P10D = ~P10D;
	// delay_ms(200);

	// 定时器开启PWM、关闭PWM测试
	// timer1_pwm_close();
	// delay_ms(500);
	// timer1_pwm_open();
	// delay_ms(500);

	// 宏函数+指示灯驱动引脚功能测试：
	// LED_WORKING_ON();
	// delay_ms(500);
	// LED_WORKING_OFF();
	// delay_ms(500);

	// 按键扫描和执行对应的功能测试：
	// key_scan();
	// if (key_press_flag)
	// {
	// 	// 如果有按键按下，进一步判断是哪个按键按下

	// 	switch (key_press_flag)
	// 	{
	// 	case KEY_HEAT_PRESS:
	// 		// P10D = ~P10D;
	// 		break;

	// 	case KEY_CHANGE_PRESS:
	// 		// P10D = ~P10D;
	// 		break;

	// 	case KEY_POWER_PRESS:
	// 		P10D = ~P10D;
	// 		break;

	// 	default:
	// 		break;
	// 	}

	// 	key_press_flag = KEY_NONE;
	// }

	// adc检测测试：
	adc_val = adc_get_val();
	send_32bits_data_by_irsir((u32)adc_val);
	delay_ms(200);
}

#endif

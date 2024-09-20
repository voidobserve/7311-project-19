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
		xms--;
	}
}

// 发送32位数据，低位先行
void send_32bits_data_by_irsir(u32 send_data)
{
	// 先发送格式头 9ms高电平+4.5ms低电平
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
	// T0CR |= 0x02; // 4分频
	// T0LOAD = 128 - 1;
	// T0DATA = 25; // 占空比 == T0DATA / T0LOAD
	// // PWM0OE = 0; // 禁止PWM输出
	// PWM0OE = 1;
	// TC0EN = 1;	// 启动定时器

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
	T0LOAD = 52 - 1;
	T0DATA = 20; // 占空比 == T1DATA / T1LOAD
	// PWM0OE = 0;	 // 禁止PWM输出
	PWM0OE = 1; // 使能PWM输出
	TC0EN = 1;
}

// 定时器1pwm配置，输出引脚 P17
void timer1_pwm_config(void)
{
	// ====================================================
	// 接近15.645KHz版本，前提条件：FCPU = FOSC / 4
	// T1CR |= 0x02; // 4分频
	// T1LOAD = 128 - 1;
	// T1DATA = 25; // 占空比 == T1DATA / T1LOAD
	// PWM1OE = 0; // 禁止PWM1输出
	// TC1EN = 1;	// 启动定时器

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
	T1LOAD = 52 - 1;
	T1DATA = 20; // 占空比 == T1DATA / T1LOAD
	PWM1OE = 0;	 // 禁止PWM1输出
	PWM1OE = 1;	 // 使能PWM1输出
	TC1EN = 1;	 // 启动定时器
}

// 开启定时器1的pwm，默认是PWM1
void timer1_pwm_open(void)
{
	PWM1OE = 1;
}

// 关闭定时器1的pwm
void timer1_pwm_close(void)
{
	PWM1OE = 0;
}

// 定时器2的PWM配置，输出引脚 P15
void timer2_pwm_config(void)
{
	// ====================================================
	// 接近15.645KHz版本，前提条件：FCPU = FOSC / 4
	T2CR |= 0x02; // 4分频
	T2LOAD = 128 - 1;
	T2DATA = 25; // 占空比 == T2DATA / T2LOAD
	// PWM2OE = 0; // 禁止PWM输出
	PWM2OE = 1;
	TC2EN = 1; // 启动定时器

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
	// T2LOAD = 52 - 1;
	// T2DATA = 20; // 占空比 == T1DATA / T1LOAD
	// // PWM2OE = 0;	 // 禁止PWM输出
	// PWM2OE = 1;	 // 使能PWM输出
	// TC2EN = 1;
}

// 按键检测引脚的配置：
void key_config(void)
{
	P11PU = 1; // 上拉电阻
	P11OE = 0; // 输入模式
}

// adc配置
void adc_config(void)
{
	// ANSEL0 |= 0xc8; // P04 P11  P12 3  6  7  模拟输入  1 模拟输入  0  IO口
	// ADCR0 &= 0x07;
	// ADCR0 |= 0x78; // 通道7  12位

	// 关闭上下拉
	P11PU = 0;
	P11PD = 0;
	P11OE = 0;	// 输入模式
	P11ANS = 1; // 模拟输入

	ADCR0 = 0x0A;		// 12位精度、不启用ADC转换，不使能ADC
	ADCR0 |= 0x03 << 5; // AIN6--P11
	// ADCR0 = 0x6A;

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
	case ADC_PIN_P11:
		ADCR0 |= 0x03 << 5; // AIN6--P11
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

	// timer0_pwm_config();
	// timer1_pwm_config();
	// timer2_pwm_config();

	// key_config();
	// adc_config();

	GIE = 1;
}

// 设置PWM的占空比
// void set_pwm_duty(u8 pwm_chx, u8 duty)
// {
// 	// if (PWM_CHX_PWM1 == pwm_chx)
// 	{
// 		// 如果要修改驱动电机的pwm占空比
// 	}
// }

// 按键扫描函数
void key_scan(void)
{
	u8 cnt = 0;
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

		while (0 == KEY_HEAT_PIN)
			; // 等待按键松开
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

		while (0 == KEY_CHANGE_PIN)
			; // 等待按键松开
	}
	else if (0 == KEY_POWER_PIN)
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

		while (0 == KEY_POWER_PIN)
			; // 等待按键松开
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
		case KEY_HEAT_PRESS:
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

		case KEY_CHANGE_PRESS:
			
			break;

		case KEY_POWER_PRESS:
			if (0== FLAG_IS_DEVICE_OPEN )
			{
				//如果未开机，关机->开机
				FLAG_IS_DEVICE_OPEN = 1;
			}
			else
			{
				// 开机->关机
				FLAG_IS_DEVICE_OPEN = 0;
			}
			break;

		default:
			break;
		}

		key_press_flag = KEY_NONE;
	}
}

void main(void)
{
	Sys_Init();

	while (1)
	{
		key_scan();
		key_handle();
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
	// if (INT0IF & INT0IE)
	// {
	// 	INT0IF = 0;
	// }
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

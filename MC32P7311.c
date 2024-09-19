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
		u32 i = 290;
		while (i--)
		{
			Nop();
		}
		xms--;
	}
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
/************************************************
;  *    @Function Name       : timer1_pwm_config
;  *    @Description         :
;  *    @IN_Parameter        :
;  *    @Return parameter    :
;  ***********************************************/
void timer1_pwm_config(void)
{
	// T1CR = 0x40; // PWM1输出  Fcpu 	1分频	0.25us
	// T1CNT = 100 - 1; //
	T1LOAD = 100 - 1;
	// T1LOAD = 2;
	T1DATA = 50; // 占空比 == T1DATA / T1LOAD

	// PWMCR0 = 0xA4; // FPWM输出  FPWM取反 2*TPWM
	// PWMCR1 = 0x00; // T1时钟为Fcpu，TMRCKS无效
	PWMCR1 |= 0x01 << 4; // TIMER计数时钟为高频32MHz

	// PWM1OE = 0; //  禁止PWM1输出
	TC1EN = 1; // 启动定时器
}

void timer1_pwm1_output(void)
{
	FPWMEN = 0;
	PWM1OE = 1;
}

void timer1_fpwm1_output(void)
{
	FPWMEN = 1;
	PWM1OE = 0;
}

// 开启定时器1的pwm，默认是PWM1
void timer1_pwm_open(void)
{
}

// 关闭定时器1的pwm
void timer1_pwm_close(void)
{
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

	timer1_pwm_config();
	GIE = 1;
}

// 设置PWM的占空比
void set_pwm_duty(u8 pwm_chx, u8 duty)
{
	// if (PWM_CHX_PWM1 == pwm_chx)
	{
		// 如果要修改驱动电机的pwm占空比
	}
}

void main(void)
{
	Sys_Init();

	while (1)
	{

		// P10D = ~P10D;
		// delay_ms(200);

		timer1_pwm1_output();
		delay_ms(500);
		timer1_fpwm1_output();
		delay_ms(500);
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
	if (T3IF & T3IE)
	{
		T3IF = 0;
	}
	//=======键盘======================
	// if (KBIF & KBIE)
	// {
	// 	KBIF = 0;
	// }
	//=======ADC=======================
	if (ADIF & ADIE)
	{
		ADIF = 0;
	}
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

/******************************************************************************
;  *   	@�ͺ�				  : MC32P7311
;  *   	@��������             : 2021.08.04
;  *   	@��˾/����			  : SINOMCU-FAE
;  *   	@����΢����֧��       : 2048615934
;  *   	@����΢����           : http://www.sinomcu.com/
;  *   	@��Ȩ                 : 2021 SINOMCU��˾��Ȩ����.
;  *----------------------ժҪ����---------------------------------
******************************************************************************/

#include "user.h"

// ���뼶��ʱ(Լ��5%�����)
// ǰ��������FCPU = FOSC / 4
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
;  *    @Description         : ��ʼ��RAM
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
;  *    @Description         : io�ڳ�ʼ��
;  *    @IN_Parameter      	 :
;  *    @Return parameter    :
;  ***********************************************/
void IO_Init(void)
{
	IOP0 = 0x00;   // io������λ
	OEP0 = 0xFF;   // io�ڷ��� 1:out  0:in   P00��1���Ϊ��©
	PUP0 = 0x00;   // io����������   1:enable  0:disable
	PDP0 = 0x00;   // io����������   1:enable  0:disable
	ANSEL0 = 0x00; // io����ѡ��  1:ģ������  0:ͨ��io

	IOP1 = 0x00;   // io������λ
	OEP1 = 0xFF;   // io�ڷ��� 1:out  0:in   P13���Ϊ��©
	PUP1 = 0x00;   // io����������   1:enable  0:disable
	PDP1 = 0x00;   // io����������   1:enable  0:disable
	ANSEL1 = 0x00; // io����ѡ��  1:ģ������  0:ͨ��io

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
	// T1CR = 0x40; // PWM1���  Fcpu 	1��Ƶ	0.25us
	// T1CNT = 100 - 1; //
	T1LOAD = 100 - 1;
	// T1LOAD = 2;
	T1DATA = 50; // ռ�ձ� == T1DATA / T1LOAD

	// PWMCR0 = 0xA4; // FPWM���  FPWMȡ�� 2*TPWM
	// PWMCR1 = 0x00; // T1ʱ��ΪFcpu��TMRCKS��Ч
	PWMCR1 |= 0x01 << 4; // TIMER����ʱ��Ϊ��Ƶ32MHz

	// PWM1OE = 0; //  ��ֹPWM1���
	TC1EN = 1; // ������ʱ��
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

// ������ʱ��1��pwm��Ĭ����PWM1
void timer1_pwm_open(void)
{
}

// �رն�ʱ��1��pwm
void timer1_pwm_close(void)
{
}

/************************************************
;  *    @Function Name       : Sys_Init
;  *    @Description         : ϵͳ��ʼ��
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

// ����PWM��ռ�ձ�
void set_pwm_duty(u8 pwm_chx, u8 duty)
{
	// if (PWM_CHX_PWM1 == pwm_chx)
	{
		// ���Ҫ�޸����������pwmռ�ձ�
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
	// �ò������жϿ���ɾ��  ��ԼROM
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
	//=======����======================
	// if (KBIF & KBIE)
	// {
	// 	KBIF = 0;
	// }
	//=======ADC=======================
	if (ADIF & ADIE)
	{
		ADIF = 0;
	}
	//=======�ⲿ�ж�0=================
	// if (INT0IF & INT0IE)
	// {
	// 	INT0IF = 0;
	// }
	// //=======�ⲿ�ж�1=================
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

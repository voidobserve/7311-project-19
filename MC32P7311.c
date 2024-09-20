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
		u32 i = 287;
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

// ��ʱ��0��pwm����--������� P16
void timer0_pwm_config(void)
{

	// ====================================================
	// �ӽ�15.645KHz�汾��ǰ��������FCPU = FOSC / 4
	// T0CR |= 0x02; // 4��Ƶ
	// T0LOAD = 128 - 1;
	// T0DATA = 25; // ռ�ձ� == T0DATA / T0LOAD
	// // PWM0OE = 0; // ��ֹPWM���
	// PWM0OE = 1;
	// TC0EN = 1;	// ������ʱ��

	// ====================================================
	// �ӽ�154.19KHz�汾��ǰ��������FCPU = FOSC / 4
	// FCPU = FOSC / 4 ֮��FCPUΪ 8MHz����ʱ����ʹ����Ƶ������һ��ԼΪ0.125us
	// T0LOAD ����Ҫ��100���ϣ����ܸ�����ص���ռ�ձȣ���˶�ʱ���������һ�ε�ʱ��ԼΪ12.5us��ԼΪ80KHz��������154.19KHz
	// T0LOAD = 100 - 1;
	// T0DATA = 25; // ռ�ձ� == T0DATA / T0LOAD
	// // PWM0OE = 0; // ��ֹPWM���
	// PWM0OE = 1; // ʹ��PWM���
	// TC0EN = 1;	// ������ʱ��

	// ���һ��Ҫ�ӽ�154.19KHz��ʹ���������ã�
	T0LOAD = 52 - 1;
	T0DATA = 20; // ռ�ձ� == T1DATA / T1LOAD
	// PWM0OE = 0;	 // ��ֹPWM���
	PWM0OE = 1; // ʹ��PWM���
	TC0EN = 1;
}

// ��ʱ��1pwm���ã�������� P17
void timer1_pwm_config(void)
{
	// ====================================================
	// �ӽ�15.645KHz�汾��ǰ��������FCPU = FOSC / 4
	// T1CR |= 0x02; // 4��Ƶ
	// T1LOAD = 128 - 1;
	// T1DATA = 25; // ռ�ձ� == T1DATA / T1LOAD
	// PWM1OE = 0; // ��ֹPWM1���
	// TC1EN = 1;	// ������ʱ��

	// ====================================================
	// �ӽ�154.19KHz�汾��ǰ��������FCPU = FOSC / 4
	// FCPU = FOSC / 4 ֮��FCPUΪ 8MHz����ʱ����ʹ����Ƶ������һ��ԼΪ0.125us
	// T1LOAD ����Ҫ��100���ϣ����ܸ�����ص���ռ�ձȣ���˶�ʱ���������һ�ε�ʱ��ԼΪ12.5us��ԼΪ80KHz��������154.19KHz
	// T1LOAD = 100 - 1;
	// T1DATA = 25; // ռ�ձ� == T1DATA / T1LOAD
	// // PWM1OE = 0; // ��ֹPWM1���
	// // PWM1OE = 1; // ʹ��PWM1���
	// TC1EN = 1;	// ������ʱ��

	// ���һ��Ҫ�ӽ�154.19KHz��ʹ���������ã�
	T1LOAD = 52 - 1;
	T1DATA = 20; // ռ�ձ� == T1DATA / T1LOAD
	PWM1OE = 0;	 // ��ֹPWM1���
	PWM1OE = 1;	 // ʹ��PWM1���
	TC1EN = 1;	 // ������ʱ��
}

// ������ʱ��1��pwm��Ĭ����PWM1
void timer1_pwm_open(void)
{
	PWM1OE = 1;
}

// �رն�ʱ��1��pwm
void timer1_pwm_close(void)
{
	PWM1OE = 0;
}

// ��ʱ��2��PWM���ã�������� P15
void timer2_pwm_config(void)
{
	// ====================================================
	// �ӽ�15.645KHz�汾��ǰ��������FCPU = FOSC / 4
	T2CR |= 0x02; // 4��Ƶ
	T2LOAD = 128 - 1;
	T2DATA = 25; // ռ�ձ� == T2DATA / T2LOAD
	// PWM2OE = 0; // ��ֹPWM���
	PWM2OE = 1;
	TC2EN = 1; // ������ʱ��

	// ====================================================
	// �ӽ�154.19KHz�汾��ǰ��������FCPU = FOSC / 4
	// FCPU = FOSC / 4 ֮��FCPUΪ 8MHz����ʱ����ʹ����Ƶ������һ��ԼΪ0.125us
	// T2LOAD ����Ҫ��100���ϣ����ܸ�����ص���ռ�ձȣ���˶�ʱ���������һ�ε�ʱ��ԼΪ12.5us��ԼΪ80KHz��������154.19KHz
	// T2LOAD = 100 - 1;
	// T2DATA = 25; // ռ�ձ� == T2DATA / T2LOAD
	// // PWM2OE = 0; // ��ֹPWM���
	// PWM2OE = 1; // ʹ��PWM���
	// TC2EN = 1;	// ������ʱ��

	// ���һ��Ҫ�ӽ�154.19KHz��ʹ���������ã�
	// T2LOAD = 52 - 1;
	// T2DATA = 20; // ռ�ձ� == T1DATA / T1LOAD
	// // PWM2OE = 0;	 // ��ֹPWM���
	// PWM2OE = 1;	 // ʹ��PWM���
	// TC2EN = 1;
}

// ����������ŵ����ã�
void key_config(void)
{
	P11PU = 1; // ��������
	P11OE = 0; // ����ģʽ
}

// adc����
// void adc_config(void)
// {
// 	// ANSEL0 |= 0xc8; // P04 P11  P12 3  6  7  ģ������  1 ģ������  0  IO��



// 	ADCR0 &= 0x07;
// 	ADCR0 |= 0x78; // ͨ��7  12λ
// 	ADCR1 &= 0x00;
// 	ADCR1 |= 0xe1; // 125K����  �ڲ�3V
// 	ADCR2 = 0xff;  // Ĭ�Ϲ̶�15��ʱ��
// 	ADON = 1;	   // ʹ��ADC
// }

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

	// timer0_pwm_config();
	// timer1_pwm_config();
	// timer2_pwm_config();

	key_config();

	GIE = 1;
}

// ����PWM��ռ�ձ�
// void set_pwm_duty(u8 pwm_chx, u8 duty)
// {
// 	// if (PWM_CHX_PWM1 == pwm_chx)
// 	{
// 		// ���Ҫ�޸����������pwmռ�ձ�
// 	}
// }

// ����ɨ�躯��
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
			; // �ȴ������ɿ�
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
			; // �ȴ������ɿ�
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
			; // �ȴ������ɿ�
	}
}

void main(void)
{
	Sys_Init();

	while (1)
	{
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

#if 0
while (1)
{
	// ��ʱ���ԣ�
	// P10D = ~P10D;
	// delay_ms(200);

	// ��ʱ������PWM���ر�PWM����
	// timer1_pwm_close();
	// delay_ms(500);
	// timer1_pwm_open();
	// delay_ms(500);

	// �꺯��+ָʾ���������Ź��ܲ��ԣ�
	// LED_WORKING_ON();
	// delay_ms(500);
	// LED_WORKING_OFF();
	// delay_ms(500);

	// ����ɨ���ִ�ж�Ӧ�Ĺ��ܲ��ԣ�
	// key_scan();
	// if (key_press_flag)
	// {
	// 	// ����а������£���һ���ж����ĸ���������

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
}

#endif

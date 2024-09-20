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

// ����32λ���ݣ���λ����
void send_32bits_data_by_irsir(u32 send_data)
{
	// �ȷ��͸�ʽͷ 9ms�ߵ�ƽ+4.5ms�͵�ƽ
	// __set_input_pull_up(); // �ߵ�ƽ
	P10D = 1;
	delay_ms(15);
	// __set_output_open_drain(); // �͵�ƽ
	P10D = 0;
	delay_ms(7); // ��ʱ4.5ms

	for (u8 i = 0; i < 32; i++)
	{
		if ((send_data >> i) & 0x01)
		{
			// ���Ҫ�����߼�1
			// __set_input_pull_up();	   // �ߵ�ƽ
			P10D = 1;
			delay_ms(6); //
			// __set_output_open_drain(); // �͵�ƽ
			P10D = 0;
			delay_ms(17); //
		}
		else
		{
			// ���Ҫ�����߼�0
			// __set_input_pull_up();	   // �ߵ�ƽ
			P10D = 1;
			delay_ms(6); //
			// __set_output_open_drain(); // �͵�ƽ
			P10D = 0;
			delay_ms(6); //
		}
	}

	// �������Ϊ�͵�ƽ
	// __set_output_open_drain(); // �͵�ƽ
	P10D = 0;
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
void adc_config(void)
{
	// ANSEL0 |= 0xc8; // P04 P11  P12 3  6  7  ģ������  1 ģ������  0  IO��
	// ADCR0 &= 0x07;
	// ADCR0 |= 0x78; // ͨ��7  12λ

	// �ر�������
	P11PU = 0;
	P11PD = 0;
	P11OE = 0;	// ����ģʽ
	P11ANS = 1; // ģ������

	ADCR0 = 0x0A;		// 12λ���ȡ�������ADCת������ʹ��ADC
	ADCR0 |= 0x03 << 5; // AIN6--P11
	// ADCR0 = 0x6A;

	// ADCR1 &= 0x00; // ����Ĵ���
	// ADCR1 |= 0xE1; // 125K����  �ڲ�3V�ο���ѹ
	ADCR1 = 0xE1; // 125K��������߾��ȣ�  �ڲ�3V�ο���ѹ
	ADCR2 = 0xFF; // ADC����ʱ��Ϊ15��ADCʱ��
	ADON = 1;	  // ʹ��ADC
}

// �л�adc��������
void adc_sel_pin(u8 adc_pin)
{
	// ���ݴ��Σ��л��ɶ�Ӧ��ͨ��
	switch (adc_pin)
	{
	case ADC_PIN_P11:
		ADCR0 |= 0x03 << 5; // AIN6--P11
		break;

	default:
		break;
	}

	delay_ms(1); // �л�adc�������ź�Ҫ��ʱһ��ʱ�䣬��ֹ����֮��ļ����
}

// ��ȡadc����ת�����ֵ
u16 adc_get_val(void)
{
	u8 cnt = 0; // adc�ɼ������ļ���
	u16 g_temp_value = 0;
	u16 g_tmpbuff = 0;
	u16 g_adcmax = 0;
	u16 g_adcmin = 0xFFFF;

	// �ɼ�20�Σ�ȥ��ǰ���β�������ȥ��һ�����ֵ��һ����Сֵ����ȡƽ��ֵ
	for (cnt = 0; cnt < 20; cnt++)
	{
		ADEOC = 0; // ���ADCת����ɱ�־λ������ADת��
		while (!ADEOC)
			;				 // �ȴ�ת�����
		g_temp_value = ADRH; // ȡ��ת�����ֵ
		g_temp_value = g_temp_value << 4 | (ADRL & 0x0F);
		if (cnt < 2)
			continue; // ����ǰ���β�����
		if (g_temp_value > g_adcmax)
			g_adcmax = g_temp_value; // ���
		if (g_temp_value < g_adcmin)
			g_adcmin = g_temp_value; // ��С
		g_tmpbuff += g_temp_value;
	}
	g_tmpbuff -= g_adcmax;			 // ȥ��һ�����
	g_tmpbuff -= g_adcmin;			 // ȥ��һ����С
	g_temp_value = (g_tmpbuff >> 4); // ����16��ȡƽ��ֵ

	return g_temp_value;
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

	// timer0_pwm_config();
	// timer1_pwm_config();
	// timer2_pwm_config();

	// key_config();
	// adc_config();

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

// ��������Ӧ�Ĺ���
void key_handle(void)
{
	if (key_press_flag)
	{
		// ����а������£���һ���ж����ĸ���������
		switch (key_press_flag)
		{
		case KEY_HEAT_PRESS:
			if (FLAG_IS_DEVICE_OPEN)
			{
				// ����豸�Ѿ����ڹ���״̬���ſ��Դ򿪼���
				if (0 == FLAG_IS_HEATING)
				{
					HEATING_ON(); // �򿪼���
					FLAG_IS_HEATING = 1;
				}
				else
				{
					HEATING_OFF(); // �رռ���
					FLAG_IS_HEATING = 0;
				}
			}
			break;

		case KEY_CHANGE_PRESS:
			
			break;

		case KEY_POWER_PRESS:
			if (0== FLAG_IS_DEVICE_OPEN )
			{
				//���δ�������ػ�->����
				FLAG_IS_DEVICE_OPEN = 1;
			}
			else
			{
				// ����->�ػ�
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
	// if (T3IF & T3IE)
	// {
	// 	T3IF = 0;
	// }
	//=======����======================
	// if (KBIF & KBIE)
	// {
	// 	KBIF = 0;
	// }
	//=======ADC=======================
	// if (ADIF & ADIE)
	// {
	// 	ADIF = 0;
	// }
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

	// adc�����ԣ�
	adc_val = adc_get_val();
	send_32bits_data_by_irsir((u32)adc_val);
	delay_ms(200);
}

#endif

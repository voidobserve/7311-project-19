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

		__asm;
		CLRWDT; // ι��ָ��
		__endasm;
		xms--;
	}
}

#if USE_MY_DEBUG
// ����32λ���ݣ���λ����
void send_32bits_data_by_irsir(u32 send_data)
{
	// �ȷ��͸�ʽͷ
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
#endif

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
	T0CR |= 0x02; // 4��Ƶ
	T0LOAD = 128 - 1;
	T0DATA = 25; // ռ�ձ� == T0DATA / T0LOAD
	PWM0OE = 0;	 // ��ֹPWM���
	// PWM0OE = 1;
	TC0EN = 1; // ������ʱ��

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
	// T0LOAD = 52 - 1;
	// T0DATA = 20; // ռ�ձ� == T1DATA / T1LOAD
	// // PWM0OE = 0;	 // ��ֹPWM���
	// PWM0OE = 1; // ʹ��PWM���
	// TC0EN = 1;
}

// ��ʱ��1pwm���ã�������� P17
void timer1_pwm_config(void)
{
	// ====================================================
	// �ӽ�15.645KHz�汾��ǰ��������FCPU = FOSC / 4
	T1CR |= 0x02; // 4��Ƶ
	T1LOAD = 128 - 1;
	T1DATA = 25; // ռ�ձ� == T1DATA / T1LOAD
	PWM1OE = 0;	 // ��ֹPWM1���
	TC1EN = 1;	 // ������ʱ��

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
	// T1LOAD = 52 - 1;
	// T1DATA = 20; // ռ�ձ� == T1DATA / T1LOAD
	// PWM1OE = 0;	 // ��ֹPWM1���
	// PWM1OE = 1;	 // ʹ��PWM1���
	// TC1EN = 1;	 // ������ʱ��
}

// // ������ʱ��1��pwm��Ĭ����PWM1
// void timer1_pwm_open(void)
// {
// 	PWM1OE = 1;
// }

// // �رն�ʱ��1��pwm
// void timer1_pwm_close(void)
// {
// 	PWM1OE = 0;
// }

// ��ʱ��2��PWM���ã�������� P15
void timer2_pwm_config(void)
{
	// ====================================================
	// �ӽ�15.645KHz�汾��ǰ��������FCPU = FOSC / 4
	// T2CR |= 0x02; // 4��Ƶ
	// T2LOAD = 128 - 1;
	// T2DATA = 25; // ռ�ձ� == T2DATA / T2LOAD
	// // PWM2OE = 0; // ��ֹPWM���
	// PWM2OE = 1;
	// TC2EN = 1; // ������ʱ��

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
	T2LOAD = 52 - 1;
	// T2DATA = 22; // ռ�ձ� == TxDATA / TxLOAD
	// T2DATA = 23; // ռ�ձ� == TxDATA / TxLOAD   ʵ�ʲ����43%ռ�ձ�
	T2DATA = 24; // ռ�ձ� == TxDATA / TxLOAD   ʵ�ʲ����46.15%ռ�ձ�
	PWM2OE = 0;	 // ��ֹPWM���
	// PWM2OE = 1;	 // ʹ��PWM���
	TC2EN = 1;
}

// ����������ŵ����ã�
void key_config(void)
{
	// �����Ƿ���ȵİ���������
#if USE_MY_DEBUG
	// ����P13�޷�������棬����ʹ��P05
	P05PU = 1;
	P05OE = 0;
#else
	P13PU = 1; // ����
	P13OE = 0; // ����ģʽ
#endif
	// P00PU = 1;
	// P00OE = 0;

	// ����/�ػ����������ã�һ��Ҫ�жϴ�����
	MINT00 = 1; // MINT00��MINT01���������INT0Ϊ�½����ж�
	MINT01 = 0;
	P11PU = 1;
	P11OE = 0;
	INT0IF = 0; // ����жϱ�־
	INT0IE = 1; // �ⲿ�ж�ʹ��

	// �л�ģʽ����������
	P01PU = 1;
	P01OE = 0;
}

// adc����
void adc_config(void)
{
	// ����Ƿ��г��ĵ�ѹ��
	P00PU = 0; // �ر�������
	P00PD = 0;
	P00OE = 0;	// ����ģʽ
	P00ANS = 1; // ģ������

	// ����ؽ�ѹ��ĵ�ѹ
	P02PU = 0; // �ر�������
	P02PD = 0;
	P02OE = 0;	// ����ģʽ
	P02ANS = 1; // ģ������

	ADCR0 = 0x0A; // 12λ���ȡ�������ADCת������ʹ��ADC
	// ADCR0 |= 0x03 << 5; // AIN6--P11

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
	case ADC_PIN_P00_AN0:
		ADCR0 &= ~(0x0F << 4); // ��ռĴ�����ͨ��ѡ��λ
		// ��պ��ͨ������ AIN0--P00
		break;

	case ADC_PIN_P02_AN1:
		ADCR0 &= ~(0x0F << 4); // ��ռĴ�����ͨ��ѡ��λ
		ADCR0 |= 0x01 << 4;	   // AIN1--P02;
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

	timer0_pwm_config();
	timer1_pwm_config();
	timer2_pwm_config();

	key_config();
	adc_config();

	GIE = 1;
}

// ����ɨ�躯��
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

		// �ȴ������ɿ�
		while (0 == KEY_POWER_PIN)
		{
			__asm;
			CLRWDT; // ι��ָ��
			__endasm;
		}
	}

	if (FLAG_IS_DEVICE_OPEN)
	{
		// ����豸��������ɨ�� ���Ƽ��ȵİ��� �� �����л�ģʽ�İ���
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

			while (0 == KEY_HEAT_PIN) // �ȴ������ɿ�
			{
				__asm;
				CLRWDT; // ι��ָ��
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

			while (0 == KEY_CHANGE_PIN) // �ȴ������ɿ�
			{
				__asm;
				CLRWDT; // ι��ָ��
				__endasm;
			}
		}
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
		case KEY_HEAT_PRESS: // ���Ȱ�������
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

		case KEY_CHANGE_PRESS: // ����ģʽ�İ�������
			// ��ʼΪ 89.9%ռ�ձȣ�ÿ��һ�� �� 89.9%->100%->80.5%->89.9%->...�����仯
			if (FLAG_IS_DEVICE_OPEN)
			{
				// ������������л�ģʽ
				if (MODE_1 == mode_flag)
				{
					// ����PWM��ռ�ձ�
					T0DATA = 255;
					T1DATA = 255; // 100%ռ�ձȣ�ȷ������TxLOAD��ֵ�Ϳ��ԣ�

					mode_flag = MODE_2;
				}
				else if (MODE_2 == mode_flag)
				{
					// ����PWM��ռ�ձ�
					T0DATA = 103;
					T1DATA = 103; // 80.5%ռ�ձ�

					mode_flag = MODE_3;
				}
				else if (MODE_3 == mode_flag)
				{
					// ����PWM��ռ�ձ�
					T0DATA = 115;
					T1DATA = 115; // 89.9%ռ�ձ�

					mode_flag = MODE_1;
				}
			}
			break;

		case KEY_POWER_PRESS: // ��Դ��������
			if (0 == FLAG_IS_DEVICE_OPEN)
			{
				// ���δ�������ػ�->����
				LED_WORKING_ON(); // �򿪵�Դָʾ��
				HEATING_ON();	  // �򿪼���
				FLAG_IS_DEVICE_OPEN = 1;
				FLAG_IS_HEATING = 1;

				// �趨��ת����ת��PWM�ĳ�ʼռ�ձ�
				T0DATA = 115;
				T1DATA = 115;		// ԼΪ 89.9%
				mode_flag = MODE_1; // ��һ���л�ģʽʱ������ MODE_2

				// �򿪿�����ת��PWM
				PWM0OE = 1;
			}
			else
			{
				// ����->�ػ�
				LED_WORKING_OFF(); // �رյ�Դָʾ��
				HEATING_OFF();	   // �رռ���
				FLAG_IS_DEVICE_OPEN = 0;
				FLAG_IS_HEATING = 0;

				mode_flag = MODE_1; // ��һ���л�ģʽʱ������ MODE_2

				// �ر� ��ת�ͷ�ת��PWM
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
	volatile u8 cnt = 0;				  // ����ֵ�����ڼ�����Ƿ����磬Ҳ�����ڼ���Ƿ����/�γ�������ļ���ֵ
	volatile u8 need_charge_cnt = 0;	  // ����ֵ�����ڼ���Ƿ�Ҫ���ٳ��
	volatile u8 need_charge_slow_cnt = 0; // ����ֵ�����ڼ���Ƿ�Ҫ��΢���ͳ���ٶ�
	volatile u8 flag_bat_is_empty = 0;	  // ��־λ�����ڼ���Ƿ�γ��˵��

	adc_sel_pin(ADC_PIN_P02_AN1); // �л�������ؽ�ѹ��ĵ�ѹ�ļ������
	for (i = 0; i < 10; i++)
	{
		adc_val = adc_get_val();
		// send_32bits_data_by_irsir(adc_val);
		if (FLAG_IS_IN_CHARGING)
		{
			// ������ڳ�磬����Ƿ������
			if (adc_val <= ADCDETECT_BAT_EMPTY) // �����һ�ε��һ��ĵ�ѹС��5V����Ϊ�ǰγ��˵��
			{
				flag_bat_is_empty = 1;
			}
			else if (adc_val >= ADCDETECT_BAT_FULL) // ������Ƿ�����
			{
				cnt++;
			}
			else if (adc_val < ADCDETECT_BAT_WILL_FULL) // ����Ƿ�Ҫ���ٳ��
			{
				need_charge_cnt++;
			}
			else if (adc_val >= ADCDETECT_BAT_WILL_FULL) // ����Ƿ�Ҫ��΢���ͳ���ٶ�
			{
				need_charge_slow_cnt++;
			}

			if (flag_bat_is_empty)
			{
				// �����⵽�γ��˵��
				LED_FULL_CHARGE_ON(); // ������س������ָʾ��
				LED_CHARGING_OFF();	  // �رճ��ָʾ��
				PWM2OE = 0;			  // �رտ�����ѹ��·��pwm
				FLAG_BAT_IS_NEED_CHARGE = 0;
				break;
			}
			else if (cnt >= 8)
			{
				// ����ڳ��ʱ�����֮ǰ��Ҫ��磬���ڼ�⵽������
				LED_FULL_CHARGE_ON(); // ������س������ָʾ��
				LED_CHARGING_OFF();	  // �رճ��ָʾ��
				PWM2OE = 0;			  // �رտ�����ѹ��·��pwm
				// FLAG_IS_IN_CHARGING = 0; // ���ܸ������־λ���㣨�������ɨ�������㣩
				FLAG_BAT_IS_NEED_CHARGE = 0;
				FLAG_BAT_IS_FULL = 1;
				break;
			}
			else if (need_charge_cnt >= 8)
			{
				// ����ڳ�磬�ҵ����Ҫ��磨��ص���С�� ADCDETECT_BAT_WILL_FULL����ؽ�Ҫ����ĵ�ѹ��
				T2DATA = 24;		   // ռ�ձ� == TxDATA / TxLOAD   ʵ�ʲ����46.15%ռ�ձ�
				LED_FULL_CHARGE_OFF(); // �رճ������ָʾ��
				FLAG_BAT_IS_NEED_CHARGE = 1;
				FLAG_BAT_IS_FULL = 0;

#if USE_MY_DEBUG
// send_32bits_data_by_irsir(0xA5A5);
#endif
				break;
			}
			else if (need_charge_slow_cnt >= 8 && 0 == FLAG_BAT_IS_FULL)
			{
				// �����Ҫ���ͳ���ٶ�
				T2DATA = 23;		   // ռ�ձ� == TxDATA / TxLOAD   ʵ�ʲ����41.54%��43%��44.62 ������ռ�ձ�
				LED_FULL_CHARGE_OFF(); // �رճ������ָʾ��
				FLAG_BAT_IS_NEED_CHARGE = 1;

#if USE_MY_DEBUG
				// P10D = ~P10D;
// send_32bits_data_by_irsir(0x5A5A);
#endif
				break;
			}
		} // if (FLAG_IS_IN_CHARGING)
		else // ���δ�ڳ�磬������Ƿ���Ҫ���
		{
			if (adc_val <= ADCDETECT_BAT_FULL - 130)
			{
				cnt++;
			}

			if (cnt >= 8)
			{
				// ���δ�ڳ�磬�ҵ����Ҫ��磨��ص���С�� ����-�������� ��
				FLAG_BAT_IS_NEED_CHARGE = 1;
				FLAG_BAT_IS_FULL = 0;
				break;
			}
		}
	} // for (i = 0; i < 10; i++)

	cnt = 0;
	adc_sel_pin(ADC_PIN_P00_AN0); // �л��������ĵ�ѹ�������
	for (i = 0; i < 10; i++)
	{
		adc_val = adc_get_val();
		if (FLAG_IS_IN_CHARGING)
		{
			// ������ڳ�磬����Ƿ�γ��˳����
			if (adc_val < ADCDETECT_CHARING_THRESHOLD)
			{
				cnt++;
			}

			if (cnt >= 8)
			{
				// ����ڳ��ʱ����⵽�γ��˳����
				LED_CHARGING_OFF();	   // �رճ��ָʾ��
				PWM2OE = 0;			   // �رտ�����ѹ��·��pwm
				LED_FULL_CHARGE_OFF(); // �رյ�س������ָʾ��
				FLAG_IS_IN_CHARGING = 0;

				break;
			} // if (cnt >= 8)

			if ((PWM2OE == 0) && FLAG_BAT_IS_NEED_CHARGE)
			{
				// ������ڳ��ʱ����⵽û�д򿪿��Ƴ�磨��ѹ����·��PWM�����ҵ����Ҫ���
				// �п����ǳ����˵磬�ر���pwm����ʹ����һ��ʱ�䣬���ºĵ磬����Ҫ���´򿪳��Ĺ���
				LED_CHARGING_ON(); // �������ָʾ��
				PWM2OE = 1;		   // ����������ѹ��·��pwm

				// P10D = ~P10D;
				// FLAG_IS_IN_CHARGING = 1; // ���ﲻ�ø������־λ��һ
			}

		} // if (FLAG_IS_IN_CHARGING)
		else
		{
			// ������ڳ�磬����Ƿ�����˳����
			if (adc_val >= ADCDETECT_CHARING_THRESHOLD)
			{
				cnt++;
			}

			if (cnt >= 8)
			{
				// if (FLAG_BAT_IS_NEED_CHARGE)
				{
					LED_CHARGING_ON(); // �������ָʾ��
					PWM2OE = 1;		   // ����������ѹ��·��pwm
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
		// �豸����ʱ���ſ�ʼ��ʱ���ж��Ƿ�Ҫת��
		turn_dir_ms_cnt += ONE_CYCLE_TIME_MS;
		if (turn_dir_ms_cnt >= 120000)
		{
			// �������2min
			if (0 == FLAG_DIR)
			{
				// �����ǰ����ת
				PWM0OE = 0; // �ر�������ת��pwm
				PWM1OE = 1; // ��������ת��pwm

				FLAG_DIR = 1; // ��ʾ��ǰΪ��ת
			}
			else
			{
				// �����ǰ�Ƿ�ת
				PWM1OE = 0; // �ر�������ת��pwm
				PWM0OE = 1; // ��������ת��pwm

				FLAG_DIR = 0; // ��ʾ��ǰΪ��ת
			}

			turn_dir_ms_cnt = 0;
		}
	}
	else
	{
		turn_dir_ms_cnt = 0; // �豸δ����ʱ����ռ���ֵ
	}
}

// �ػ����ʹ���
void shutdown_scan_handle(void)
{
	if (FLAG_IS_DEVICE_OPEN)
	{
		// ����豸��������ʼ��ʱ��15min���Զ��ػ�
		shut_down_ms_cnt += ONE_CYCLE_TIME_MS;
		if (shut_down_ms_cnt >= 900000)
		{
			// ���������15min���ػ���

			LED_WORKING_OFF(); // �رյ�Դָʾ��
			HEATING_OFF();	   // �رռ���
			FLAG_IS_HEATING = 0;
			mode_flag = MODE_3; // ��һ���л�ģʽʱ������ MODE_1
			// �ر� ��ת�ͷ�ת��PWM
			PWM0OE = 0;
			PWM1OE = 0;

			FLAG_IS_DEVICE_OPEN = 0;
		}
	}
	else
	{
		// ����豸δ��������ռ�ʱ
		shut_down_ms_cnt = 0;
	}
}

void low_power_scan_handle(void)
{
	if (FLAG_IS_DEVICE_OPEN)
	{
		// ����豸�Ѿ�������������͹���
		return;
	}

	// ������е����˵���豸û������
	// ������Ҫ�������ڳ������
	if (FLAG_IS_IN_CHARGING)
	{
		// ������ڳ�磬������͹��ģ���Ϊ���ܻ���Ҫ���PWM�����Ƴ��
		return;
	}

	GIE = 0;
	KBIF = 0;
	KBIE = 1;
	// ����ǰ�ر����� AD�� ʹ�ܻ������������ж�
	Nop();
	Nop();
	Stop();
	Nop();
	Nop();
	KBIE = 0;
	KBIF = 0;
	GIE = 1;
	// ���Ѻ�ʹ������رռ����жϣ�ʵ��Ӧ���û���������
}

void main(void)
{
	Sys_Init();
	// �ر�����ָʾ��
	LED_WORKING_OFF();
	LED_FULL_CHARGE_OFF();
	LED_CHARGING_OFF();

	// adc_sel_pin(ADC_PIN_P02_AN1); // �����ã�������ؽ�ѹ��ĵ�ѹ

	while (1)
	{
#if USE_MY_DEBUG
		// P10D = 1; // ����һ��ѭ�������ʱ��
#endif

		key_scan();
		key_handle();
		adc_scan_handle(); // ���ɨ��ʹ�����

		turn_dir_scan_handle();
		shutdown_scan_handle(); // �Զ��ػ����ʹ�����
		low_power_scan_handle();

		// adc_val = adc_get_val();
		// send_32bits_data_by_irsir(adc_val);
		// delay_ms(20);

#if USE_MY_DEBUG
		// P10D = 0; // ����һ��ѭ�������ʱ��
#endif

		__asm;
		CLRWDT; // ι��ָ��
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
	if (INT0IF & INT0IE)
	{
		INT0IF = 0;
	}
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

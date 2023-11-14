#include "Key/KeyPad.h"
#include "tim.h"
#include "GUI/gui.h"
#include "DAC/dac_cs.h"


//����KEY1:0x08,KEY2:0x04,KEY3:0x02,Key4:0x01
uint8_t bspKey_EventDn , perBspKey = 0xff;
uint32_t DeShake = 0; // ����ģʽ��־λ
#define BTN_DESHAKE_TIME		10	  // ����10ms
uint8_t Bsp_BTNScan(void)
{
	uint8_t key = 0;

	// ��4������ ��ϵ�һ���ֽ����ݵ����2λ
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3) == 1)
	{
		key |= 0x01;
	}
	key<<=1;
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)== 1)
	{
		key |= 0x01;
	}
	key<<=1;
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5)== 1)
	{
		key |= 0x01;
	}
	key<<=1;
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6)== 1)
	{
		key |= 0x01;
	}
	key = (~key) & 0x0f;  // ȡ����1��ʾ������Ч

	// ����״̬
	if(HAL_GetTick() < DeShake)
	{
		bspKey_EventDn = 0;
		return bspKey_EventDn;
	}
	if(perBspKey != key)
	{
		if(key != 0x00)		// �а�������
		{
			bspKey_EventDn = perBspKey ^ key; //���������ҳ��仯��λ��
		}
		else{
			// �����ɿ������ý�������ģʽ
			DeShake = HAL_GetTick() + BTN_DESHAKE_TIME;
			bspKey_EventDn = 0;
		}

		perBspKey = key;
	}
	else
	{
		bspKey_EventDn = 0;
	}

	return bspKey_EventDn;
}

uint8_t bspEncoder = Vertical_1, prebspEncoder = 0xff;
uint8_t SampRate_num = 0,preSampRate_num =0xff;
uint8_t Vpp_num1 = 0;
uint8_t Vpp_num2 = 0;
uint8_t DAC_Trigger_num = 11,preDAC_Trigger_num = 0xff;//��ʼΪ1950
uint8_t DAC_switch = 0 ;
uint8_t Trigger_Chx = 1;
uint8_t DAC_flag_Freq = 0;//DACƵ���޸ı�־λ
uint8_t DAC_flag_Vpp =0;//DAC���ֵ�޸ı�־λ

//������ɨ��
void Encoder_scan(void)
{
	uint16_t Encoder_number =0;
	if(prebspEncoder!=bspEncoder)
	{
		// ֹͣ��ʱ��
		HAL_TIM_Encoder_Stop(&htim3,TIM_CHANNEL_ALL); // htimΪ��ʱ�����
		switch(bspEncoder)
		{
			case Vertical_1://��ѹ6����λ
				// ���³�ʼ����ʱ���������µ�����ֵ
				htim3.Init.Period = 6-1;
				Encoder_number = Vpp_num1;
			break;
			case Vertical_2://��ѹ6����λ
				htim3.Init.Period = 6-1;
				Encoder_number = Vpp_num2;
			break;
			case SampRate://���� 7����λ
				htim3.Init.Period = 7-1;
				Encoder_number = SampRate_num;
			break;
			case DAC_Trigger://����38����λ
				htim3.Init.Period = 38-1;
				Encoder_number = DAC_Trigger_num;
			break;
			case DAC_Wave://DAC����3��ģʽ
				htim3.Init.Period = 3-1;
				Encoder_number = DAC.DAC_wave_form;
			break;
			case DAC_frq_HCF://DAC����Ƶ�ʵİ�λ
				htim3.Init.Period = 10-1;
				Encoder_number = DAC.Freq.HCF;
			break;
			case DAC_frq_TCF://DAC����Ƶ�ʵ�ʮλ
				htim3.Init.Period = 10-1;
				Encoder_number = DAC.Freq.TCF;
			break;
			case DAC_frq_UCF://DAC����Ƶ�ʵĸ�λ
				htim3.Init.Period = 10-1;
				Encoder_number = DAC.Freq.UCF;
			break;
			case DAC_signe://DAC����Ƶ�ʵķ���λ
				htim3.Init.Period = 3-1;
				Encoder_number = DAC.Freq.Signe;
			break;
			case DAC_Vpp_TV://DAC���η��ֵʮVλ
				htim3.Init.Period = 2-1;
				Encoder_number = DAC.Vpp.TV;
			break;
			case DAC_Vpp_UV://DAC���η��ֵ1Vλ
				htim3.Init.Period = 10-1;
				Encoder_number = DAC.Vpp.UV;
			break;
			case DAC_Vpp_HmV://DAC���η��ֵ��mVλ
				htim3.Init.Period = 10-1;
				Encoder_number = DAC.Vpp.HmV;
			break;
			case DAC_Vpp_TmV://DAC���η��ֵ��mVλ
				htim3.Init.Period = 10-1;
				Encoder_number = DAC.Vpp.TmV;
			break;
		}
			Lcd_Show_Data(0);
			Lcd_Show_Data(1);
			prebspEncoder =bspEncoder;
			// ������ʱ��
			HAL_TIM_Base_Init(&htim3);
			__HAL_TIM_GET_COUNTER(&htim3)  = Encoder_number;
			HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
	}
	else 
	{
		switch(bspEncoder)
		{
			case Vertical_1://��ѹ6����λ
				Vpp_num1 =  __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case Vertical_2://��ѹ6����λ
				Vpp_num2 =  __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case SampRate://���� 7����λ
				SampRate_num = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_Trigger://����38����λ
				DAC_Trigger_num = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_Wave://DAC����3��ģʽ
				DAC.DAC_wave_form  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_frq_HCF://DAC����Ƶ�ʵİ�λ
				DAC.Freq.HCF  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_frq_TCF://DAC����Ƶ�ʵ�ʮλ
				DAC.Freq.TCF  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_frq_UCF://DAC����Ƶ�ʵĸ�λ
				 DAC.Freq.UCF  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_signe://DAC����Ƶ�ʵķ���λ
				DAC.Freq.Signe  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_Vpp_TV://DAC���η��ֵʮVλ
				DAC.Vpp.TV  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_Vpp_UV://DAC���η��ֵ1Vλ
				DAC.Vpp.UV  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_Vpp_HmV://DAC���η��ֵ��mVλ
				DAC.Vpp.HmV  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_Vpp_TmV://DAC���η��ֵ��mVλ
				DAC.Vpp.TmV  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
		}
			Lcd_Show_Data(1);//ʵʱˢ��
	}

}



// ɨ����ϵ�λ�л�
void selectGear(uint8_t  Key_num)
{		
		// �ж��ж�������PC13�ܽ�
			switch(Key_num)
			{
				case 0x08: //KEY1�л�
					switch (bspEncoder) {
							case Vertical_1:
									bspEncoder = Vertical_2;
									break;
							case Vertical_2:
									bspEncoder = DAC_Trigger;
									break;
							case DAC_Trigger:
									bspEncoder =  SampRate ;
									break;
							case SampRate:
									bspEncoder = DAC_Wave ;
									break;
							case DAC_Wave:
									DAC_flag_Freq = 0;//Ƶ�ʽ����޸�
									bspEncoder = DAC_frq_HCF ;
									break;
							case DAC_frq_HCF:
									bspEncoder = DAC_frq_TCF ;
									break;
							case DAC_frq_TCF:
									bspEncoder = DAC_frq_UCF ;
									break;
							case DAC_frq_UCF:
									bspEncoder = DAC_signe ;
									break;
							case DAC_signe:
								DAC_flag_Freq =1;//Ƶ���˳��޸�
								DAC_flag_Vpp =0;//���ֵ�����޸�
									bspEncoder = DAC_Vpp_TV ;
									break;
							case DAC_Vpp_TV:
									bspEncoder = DAC_Vpp_UV ;
									break;
							case DAC_Vpp_UV:
									bspEncoder = DAC_Vpp_HmV ;
									break;
							case DAC_Vpp_HmV:
									bspEncoder = DAC_Vpp_TmV ;
									break;
							case DAC_Vpp_TmV:
								DAC_flag_Vpp =1;//���ֵ�˳��޸�
									bspEncoder = Vertical_1 ;
									break;
							}
					break;
				case 0x04://KEY2�л�
					switch (bspEncoder) {
						case Vertical_1:
									DAC_flag_Vpp = 0;//���ֵ�����޸�
									bspEncoder = DAC_Vpp_TmV ;
									break;
							case DAC_Vpp_TmV:
									bspEncoder = DAC_Vpp_HmV ;
									break;
							case DAC_Vpp_HmV:
									bspEncoder = DAC_Vpp_UV ;
									break;
							case DAC_Vpp_UV:
									bspEncoder = DAC_Vpp_TV ;
									break;
							case DAC_Vpp_TV:
									DAC_flag_Vpp = 1;//���ֵ����޸�
									DAC_flag_Freq = 0;//Ƶ�ʽ����޸�
									bspEncoder = DAC_signe ;
									break;
							case DAC_signe:
									bspEncoder = DAC_frq_UCF ;
									break;
							case DAC_frq_UCF:
									bspEncoder = DAC_frq_TCF ;
									break;
							case DAC_frq_TCF:
									bspEncoder = DAC_frq_HCF ;
									break;
							case DAC_frq_HCF:
								DAC_flag_Freq =1;//Ƶ���˳��޸�
									bspEncoder = DAC_Wave ;
									break;
							case DAC_Wave:
									bspEncoder = SampRate;
									break;
							case SampRate:
									bspEncoder = DAC_Trigger;
									break;
							case DAC_Trigger:
									bspEncoder = Vertical_2;
									break;
							case Vertical_2:
									bspEncoder = Vertical_1;
									break;
            }
					break;
				case 0x02://KEY3 ����ת��
					Trigger_Chx = (Trigger_Chx == 1) ? 2:1;
					break;
				case 0x01://KEY4 DAC����
					DAC_switch = ~DAC_switch; 
					break;
			}

}



void All_Key_scan(void)
{
	uint8_t Key_num =0;
	Key_num = Bsp_BTNScan();//�ĸ�����ɨ��
	selectGear(Key_num);
	Encoder_scan();
}




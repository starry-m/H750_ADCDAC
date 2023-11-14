#include "Key/KeyPad.h"
#include "tim.h"
#include "GUI/gui.h"
#include "DAC/dac_cs.h"


//返回KEY1:0x08,KEY2:0x04,KEY3:0x02,Key4:0x01
uint8_t bspKey_EventDn , perBspKey = 0xff;
uint32_t DeShake = 0; // 消抖模式标志位
#define BTN_DESHAKE_TIME		10	  // 消抖10ms
uint8_t Bsp_BTNScan(void)
{
	uint8_t key = 0;

	// 将4个按键 组合到一个字节数据的最低2位
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
	key = (~key) & 0x0f;  // 取反以1表示按键有效

	// 消抖状态
	if(HAL_GetTick() < DeShake)
	{
		bspKey_EventDn = 0;
		return bspKey_EventDn;
	}
	if(perBspKey != key)
	{
		if(key != 0x00)		// 有按键按下
		{
			bspKey_EventDn = perBspKey ^ key; //异或操作，找出变化的位。
		}
		else{
			// 按键松开，设置进入消抖模式
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
uint8_t DAC_Trigger_num = 11,preDAC_Trigger_num = 0xff;//初始为1950
uint8_t DAC_switch = 0 ;
uint8_t Trigger_Chx = 1;
uint8_t DAC_flag_Freq = 0;//DAC频率修改标志位
uint8_t DAC_flag_Vpp =0;//DAC峰峰值修改标志位

//编码器扫描
void Encoder_scan(void)
{
	uint16_t Encoder_number =0;
	if(prebspEncoder!=bspEncoder)
	{
		// 停止定时器
		HAL_TIM_Encoder_Stop(&htim3,TIM_CHANNEL_ALL); // htim为定时器句柄
		switch(bspEncoder)
		{
			case Vertical_1://电压6个档位
				// 重新初始化定时器并设置新的周期值
				htim3.Init.Period = 6-1;
				Encoder_number = Vpp_num1;
			break;
			case Vertical_2://电压6个档位
				htim3.Init.Period = 6-1;
				Encoder_number = Vpp_num2;
			break;
			case SampRate://采样 7个档位
				htim3.Init.Period = 7-1;
				Encoder_number = SampRate_num;
			break;
			case DAC_Trigger://触发38个档位
				htim3.Init.Period = 38-1;
				Encoder_number = DAC_Trigger_num;
			break;
			case DAC_Wave://DAC波形3个模式
				htim3.Init.Period = 3-1;
				Encoder_number = DAC.DAC_wave_form;
			break;
			case DAC_frq_HCF://DAC波形频率的百位
				htim3.Init.Period = 10-1;
				Encoder_number = DAC.Freq.HCF;
			break;
			case DAC_frq_TCF://DAC波形频率的十位
				htim3.Init.Period = 10-1;
				Encoder_number = DAC.Freq.TCF;
			break;
			case DAC_frq_UCF://DAC波形频率的个位
				htim3.Init.Period = 10-1;
				Encoder_number = DAC.Freq.UCF;
			break;
			case DAC_signe://DAC波形频率的符号位
				htim3.Init.Period = 3-1;
				Encoder_number = DAC.Freq.Signe;
			break;
			case DAC_Vpp_TV://DAC波形峰峰值十V位
				htim3.Init.Period = 2-1;
				Encoder_number = DAC.Vpp.TV;
			break;
			case DAC_Vpp_UV://DAC波形峰峰值1V位
				htim3.Init.Period = 10-1;
				Encoder_number = DAC.Vpp.UV;
			break;
			case DAC_Vpp_HmV://DAC波形峰峰值百mV位
				htim3.Init.Period = 10-1;
				Encoder_number = DAC.Vpp.HmV;
			break;
			case DAC_Vpp_TmV://DAC波形峰峰值百mV位
				htim3.Init.Period = 10-1;
				Encoder_number = DAC.Vpp.TmV;
			break;
		}
			Lcd_Show_Data(0);
			Lcd_Show_Data(1);
			prebspEncoder =bspEncoder;
			// 启动定时器
			HAL_TIM_Base_Init(&htim3);
			__HAL_TIM_GET_COUNTER(&htim3)  = Encoder_number;
			HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
	}
	else 
	{
		switch(bspEncoder)
		{
			case Vertical_1://电压6个档位
				Vpp_num1 =  __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case Vertical_2://电压6个档位
				Vpp_num2 =  __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case SampRate://采样 7个档位
				SampRate_num = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_Trigger://触发38个档位
				DAC_Trigger_num = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_Wave://DAC波形3个模式
				DAC.DAC_wave_form  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_frq_HCF://DAC波形频率的百位
				DAC.Freq.HCF  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_frq_TCF://DAC波形频率的十位
				DAC.Freq.TCF  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_frq_UCF://DAC波形频率的个位
				 DAC.Freq.UCF  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_signe://DAC波形频率的符号位
				DAC.Freq.Signe  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_Vpp_TV://DAC波形峰峰值十V位
				DAC.Vpp.TV  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_Vpp_UV://DAC波形峰峰值1V位
				DAC.Vpp.UV  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_Vpp_HmV://DAC波形峰峰值百mV位
				DAC.Vpp.HmV  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
			case DAC_Vpp_TmV://DAC波形峰峰值百mV位
				DAC.Vpp.TmV  = __HAL_TIM_GET_COUNTER(&htim3) ;
			break;
		}
			Lcd_Show_Data(1);//实时刷新
	}

}



// 扫描加上档位切换
void selectGear(uint8_t  Key_num)
{		
		// 判断中断来自于PC13管脚
			switch(Key_num)
			{
				case 0x08: //KEY1切换
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
									DAC_flag_Freq = 0;//频率进入修改
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
								DAC_flag_Freq =1;//频率退出修改
								DAC_flag_Vpp =0;//峰峰值进入修改
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
								DAC_flag_Vpp =1;//峰峰值退出修改
									bspEncoder = Vertical_1 ;
									break;
							}
					break;
				case 0x04://KEY2切换
					switch (bspEncoder) {
						case Vertical_1:
									DAC_flag_Vpp = 0;//峰峰值进入修改
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
									DAC_flag_Vpp = 1;//峰峰值完成修改
									DAC_flag_Freq = 0;//频率进入修改
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
								DAC_flag_Freq =1;//频率退出修改
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
				case 0x02://KEY3 触发转换
					Trigger_Chx = (Trigger_Chx == 1) ? 2:1;
					break;
				case 0x01://KEY4 DAC开关
					DAC_switch = ~DAC_switch; 
					break;
			}

}



void All_Key_scan(void)
{
	uint8_t Key_num =0;
	Key_num = Bsp_BTNScan();//四个按键扫描
	selectGear(Key_num);
	Encoder_scan();
}




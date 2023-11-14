//自建库
#include "DAC/dac_cs.h"
//系统库
#include "dac.h"
#include "tim.h"
#include "RELAY/relay.h"

DAC_ DAC;

uint16_t g_Wave[SIN_ROM_LENGTH];
uint16_t sin_rom[SIN_ROM_LENGTH]=
{
2048,
2304,
2557,
2801,
3034,
3251,
3449,
3625,
3776,
3900,
3995,
4059,
4091,
4091,
4059,
3995,
3900,
3776,
3625,
3449,
3251,
3034,
2801,
2557,
2304,
2048,
1791,
1538,
1294,
1061,
844,
646,
470,
319,
195,
100,
36,
4,
4,
36,
100,
195,
319,
470,
646,
844,
1061,
1294,
1538,
1791,
};


/*
*********************************************************************************************************
* 函 数 名: DAC_TIM
* 功能说明: 配置TIM6的触发时间
* 形 参: _DacFreq : DAC 样本更新频率
* 返 回 值: 无
*********************************************************************************************************
*/
void DAC_TIM(uint32_t _DacFreq)
{
	if(_DacFreq<100)
	{
		htim6.Init.Prescaler = 100-1;   /*分频比 = 100*/
		htim6.Init.Period = (48000/_DacFreq)-1;//240 000 000（240Mhz） /50（50个点为一个周期）/100（分频）
	}else/*大于1K的频率不分频*/
	{
		htim6.Init.Prescaler = 0;
		htim6.Init.Period = (4800000/_DacFreq)-1;//240 000 000（240Mhz） /50（50个点为一个周期）
	}
	HAL_TIM_Base_Init(&htim6);
}

/*
*********************************************************************************************************
* 函 数 名: dac1_SetSinWave
* 功能说明: DAC1 输出正弦波
* 形 参: _vpp : 幅度 100-19800;
* _freq : 频率
* 返 回 值: 无
*********************************************************************************************************
*/
void dac1_SetSinWave(uint16_t _vpp)
{
	uint32_t i;
	uint32_t dac;
	/* 调整正弦波幅度 */
	for (i = 0; i < 50; i++)
	{
	dac = (double)(sin_rom[i] *_vpp /19800);
	if (dac > 4095)
	{
	dac = 4095;
	}
	g_Wave[i] = dac+((4095-((double)_vpp *4095/19800))/2);
	}
}

/*
*********************************************************************************************************
* 函 数 名: dac1_SetRectWave
* 功能说明: DAC1 输出方波
* 形 参: _low : 低电平时 DAC, 
* _high : 高电平时 DAC
* _freq : 频率 Hz
* _duty : 占空比 2% - 98%, 调节步数 1%
* 返 回 值: 无
*********************************************************************************************************
*/
void dac1_SetRectWave(uint16_t _vpp)
{
	uint32_t i;
	uint16_t _low,_high;
	_low =  (double)(0 *_vpp /19800)+((4095-((double)_vpp *4095/19800))/2);
	_high =  (double)(4095 *_vpp /19800)+((4095-((double)_vpp *4095/19800))/2);
	for (i = 0; i < 25; i++)
	{
		g_Wave[i] = _low;
	}
	for (; i < 50; i++)
	{
		g_Wave[i] = _high;
	}

}

/*
*********************************************************************************************************
* 函 数 名: dac1_SetTriWave
* 功能说明: DAC1 输出三角波
* 形 参: _low : 低电平时 DAC, 
* _high : 高电平时 DAC
* _freq : 频率 Hz
* _duty : 占空比
* 返 回 值: 无
*********************************************************************************************************
*/
void dac1_SetTriWave(uint16_t _vpp)
{
uint32_t i;
uint16_t _low,_high;
uint16_t dac;
uint16_t m;
/* 构造三角波数组，100 个样本，从 _low 到 _high */
	m = 25;
	_low =  (double)(0 *_vpp /19800)+((4095-((double)_vpp *4095/19800))/2);
	_high =  (double)(4095 *_vpp /19800)+((4095-((double)_vpp *4095/19800))/2);
	for (i = 0; i < m; i++)
	{
		dac = _low + ((_high - _low) * i) / m;
		g_Wave[i] = dac;
	}
	for (; i < 50; i++)
	{
		dac = _high - ((_high - _low) * (i - m)) / (50 - m);
		g_Wave[i] = dac;
	}
}

//DAC发送直流电压
void Trigger_Open(uint16_t number)//0~4095
{
	//DAC
	float dac_voltage = 0; // dac 输出电压
	uint32_t dac_value = 0;  // dac 码值
	
	dac_voltage = number;//设置dac输出电压1v
	dac_value = (uint32_t)(dac_voltage*4095/3300);//将实际电压转换为码值，vref=3.3v，12位精度
	HAL_DAC_Start(&hdac1,DAC_CHANNEL_1);//dac1的通道2开启
	HAL_DAC_SetValue(&hdac1,DAC_CHANNEL_1,DAC_ALIGN_12B_R,dac_value);
	
}

void DAC_processing(uint8_t on_off,uint8_t Freq_on_off,uint8_t Vpp_on_off)
{	
	static uint8_t preon_off = 0xff,preDAC_wave=0xff,preDAC_Freq_on_off = 0xff,preVpp_on_off = 0xff,i =0;
	if(i == 0)
	{
		DAC.DAC_wave_form = 0;//正弦波
		DAC.Freq.HCF=0;
		DAC.Freq.TCF=0;
		DAC.Freq.UCF=1;
		DAC.Freq.Signe=0;
		DAC.DAC_Freq = 1000;//1K
		DAC.Vpp.TV=1;
		DAC.Vpp.UV=0;
		DAC.Vpp.HmV=0;
		DAC.Vpp.TmV=0;
		DAC.DAC_Vpp = 10000;//10V
		i =1;
	}
	if(preon_off  != on_off)//DAC的开关,仅仅只关心开关
	{
		if(on_off == 0)
		{
		HAL_TIM_Base_Stop(&htim6);//tim6关闭
		HAL_DAC_Stop_DMA(&hdac1,DAC_CHANNEL_2);
		}else{
		HAL_TIM_Base_Start(&htim6);//tim6开启
		HAL_DAC_Start_DMA(&hdac1,DAC_CHANNEL_2,(uint32_t *)g_Wave,50,DAC_ALIGN_12B_R);
		}
		preon_off = on_off;
	}
	if(preDAC_wave != DAC.DAC_wave_form)//仅仅关心DAC波形改变
	{
		if(DAC.DAC_wave_form == 0)//正弦波
		{
			dac1_SetSinWave(DAC.DAC_Vpp);
		}else if(DAC.DAC_wave_form == 1)//方波
		{
			dac1_SetRectWave(DAC.DAC_Vpp);
		}else if(DAC.DAC_wave_form == 2)//三角波
		{
			dac1_SetTriWave(DAC.DAC_Vpp);
		}
		preDAC_wave = DAC.DAC_wave_form;
	}
	
	//只关心DAC频率是否改变
	if(preDAC_Freq_on_off != Freq_on_off)
	{
		if(Freq_on_off == 1)
		{
			if(DAC.Freq.Signe == 0)//K 数值要*1000
			{
				DAC.DAC_Freq = ((DAC.Freq.HCF*100) + (DAC.Freq.TCF*10) + DAC.Freq.UCF)*1000;
				if(DAC.DAC_Freq>50000)//最高限制50K，如果大于这个值会被强制转换为50K并且各个位数重新赋值
				{
					DAC.Freq.HCF = 0,DAC.Freq.TCF =5,DAC.Freq.UCF =0;
					DAC.DAC_Freq = 50000;
				}
				if(DAC.DAC_Freq<1)//强制为1
				{
					DAC.Freq.UCF =1;
					DAC.Freq.Signe = 1;//切换为H
					DAC.DAC_Freq =1;
				}
			}else//H  数值不改变
			{
				DAC.DAC_Freq = ((DAC.Freq.HCF*100) + (DAC.Freq.TCF*10) + DAC.Freq.UCF);
				if(DAC.DAC_Freq<1)
				{
					DAC.Freq.UCF =1;
					DAC.DAC_Freq =1;
				}
			}
			DAC_TIM(DAC.DAC_Freq);
		}
		preDAC_Freq_on_off = Freq_on_off;
	}
	
	//仅仅关心峰峰值是否改变
	if(preVpp_on_off !=  Vpp_on_off)
	{
		if(Vpp_on_off == 1)
		{
			DAC.DAC_Vpp = (DAC.Vpp.TV*10000)+(DAC.Vpp.UV*1000) + (DAC.Vpp.HmV *100)+ (DAC.Vpp.TmV *10);
			if(DAC.DAC_Vpp>=19800)
			{
				DAC.DAC_Vpp = 19800;
				DAC.Vpp.TV = 1;
				DAC.Vpp.UV = 9;
				DAC.Vpp.HmV = 8;
				DAC.Vpp.TmV = 0;
			}
			if(DAC.DAC_Vpp<=900)
			{
				DAC.DAC_Vpp = DAC.DAC_Vpp*20;
				Relay3_On();
			}else
			{
				Relay3_Off();
			}
			if(DAC.DAC_wave_form == 0)//正弦波
			{
				dac1_SetSinWave(DAC.DAC_Vpp);
			}
			else if(DAC.DAC_wave_form == 1)//方波
			{
				dac1_SetRectWave(DAC.DAC_Vpp);
			}else if(DAC.DAC_wave_form == 2)//三角波
			{
				dac1_SetTriWave(DAC.DAC_Vpp);
			}
		}
		preVpp_on_off = Vpp_on_off;
	}
}



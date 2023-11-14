//用于处理ADC获取数据后的处理操作。
//ADC的回调
//TIM的触发回调


#include "ADC/DRP.h"
#include "tim.h"
#include "adc.h"
#include "Key/KeyPad.h"
#include "RELAY/relay.h"
#include "debug/debug.h"


uint32_t adc_dma_data1[ADC_DATA_LENGTH];//定义 adc1 采集数据存放数组
uint8_t adc_dma_complete_flag = notStarted; //adc1 数据 dma 采集完成标志，在 dma 采集完成回调函数设置

uint8_t Flag_Trigger =0;  //触发标志位
uint8_t Flag_overtime = 0;  //超时标志位

//对于y轴切换的计算初始化
void OSC_init(void)
{
	//波形1的继电器档位控制
	OSC.waveform1.gears[0] = 2;
	OSC.waveform1.gears[1] = 2;
	OSC.waveform1.gears[2] = 10;
	OSC.waveform1.gears[3] = 10;
	OSC.waveform1.gears[4] = 10;
	OSC.waveform1.gears[5] = 10;
	//波形1的波形比例因数
	OSC.waveform1.Sca_factor[0] = 3.3;
	OSC.waveform1.Sca_factor[1] = 1.65;
	OSC.waveform1.Sca_factor[2] = 13.2;
	OSC.waveform1.Sca_factor[3] = 6.6;
	OSC.waveform1.Sca_factor[4] = 3.3;
	OSC.waveform1.Sca_factor[5] = 1.32;
	//波形1的中心偏移值
	OSC.waveform1.offset[0] = 31.6;
	OSC.waveform1.offset[1] = 36.2;
	OSC.waveform1.offset[2] = 5.0;
	OSC.waveform1.offset[3] = 22.8;
	OSC.waveform1.offset[4] = 31.8;
	OSC.waveform1.offset[5] = 37.2;
	
	//波形2的继电器档位控制
	OSC.waveform2.gears[0] = 2;
	OSC.waveform2.gears[1] = 2;
	OSC.waveform2.gears[2] = 10;
	OSC.waveform2.gears[3] = 10;
	OSC.waveform2.gears[4] = 10;
	OSC.waveform2.gears[5] = 10;
	//波形2的波形比例因数
	OSC.waveform2.Sca_factor[0] = 3.3;
	OSC.waveform2.Sca_factor[1] = 1.65;
	OSC.waveform2.Sca_factor[2] = 13.2;
	OSC.waveform2.Sca_factor[3] = 6.6;
	OSC.waveform2.Sca_factor[4] = 3.3;
	OSC.waveform2.Sca_factor[5] = 1.32;
	//波形2的中心偏移值
	OSC.waveform2.offset[0] = 32.7;
	OSC.waveform2.offset[1] = 36.6;
	OSC.waveform2.offset[2] = 8.1;
	OSC.waveform2.offset[3] = 24.6;
	OSC.waveform2.offset[4] = 32.6;
	OSC.waveform2.offset[5] = 37.4;
}


//x轴ADC时钟更改
//x轴调节，用于调节采样率，具体见计算表格
void Osc_Time_Switch(uint8_t timer)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0}; //RCC时钟初始结构体
	ADC_Disable(&hadc1);
	ADC_Disable(&hadc2);
	HAL_Delay(2);
	
	switch(timer)
	{
		case 6://1.6ms 0.02Mhz  20000/512*5 =195Hz
			PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
			PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
			PeriphClkInitStruct.PLL3.PLL3N = 320;	//PLL分频后倍频,Max 512
			PeriphClkInitStruct.PLL3.PLL3R = 100;	//PLL分频,Max 128
			PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
			HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		
			hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV8;//adc内部分频,Max 256,只能倍数
			hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV8;//adc内部分频,Max 256,只能倍数
			HAL_ADC_Init(&hadc1);
			HAL_ADC_Init(&hadc2);
		break;
		case 5://640us 0.05Mhz  488Hz
			PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
			PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
			PeriphClkInitStruct.PLL3.PLL3N = 150;	//PLL分频后倍频,Max 512
			PeriphClkInitStruct.PLL3.PLL3R = 75;	//PLL分频,Max 128
			PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
			HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		
			hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;//adc内部分频,Max 256,只能倍数
			hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;//adc内部分频,Max 256,只能倍数
			HAL_ADC_Init(&hadc1);
			HAL_ADC_Init(&hadc2);
		break;
		case 4://320us 0.1mhz  976hz
			PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
			PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
			PeriphClkInitStruct.PLL3.PLL3N = 150;	//PLL分频后倍频,Max 512
			PeriphClkInitStruct.PLL3.PLL3R = 75;	//PLL分频,Max 128
			PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
			HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		
			hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
			hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
			HAL_ADC_Init(&hadc1);
			HAL_ADC_Init(&hadc2);
		break;
		case 3://160us 0.2Mhz  1953Hz
			PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
			PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
			PeriphClkInitStruct.PLL3.PLL3N = 200;	//PLL分频后倍频,Max 512
			PeriphClkInitStruct.PLL3.PLL3R = 50;	//PLL分频,Max 128
			PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
			HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		
			hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
			hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
			HAL_ADC_Init(&hadc1);
			HAL_ADC_Init(&hadc2);
		break;
		case 2://64us 0.5Mhz 4882Hz
			PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
			PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
			PeriphClkInitStruct.PLL3.PLL3N = 200;	//PLL分频后倍频,Max 512
			PeriphClkInitStruct.PLL3.PLL3R = 5;	//PLL分频,Max 128
			PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
			HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		
			hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;//adc内部分频,Max 256,只能倍数
			hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;//adc内部分频,Max 256,只能倍数
			HAL_ADC_Init(&hadc1);
			HAL_ADC_Init(&hadc2);
		break;
		case 1://32us 1Mhz 9765Hz
			PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
			PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
			PeriphClkInitStruct.PLL3.PLL3N = 200;	//PLL分频后倍频,Max 512
			PeriphClkInitStruct.PLL3.PLL3R = 5;	//PLL分频,Max 128
			PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
			HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		
			hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;//adc内部分频,Max 256,只能倍数
			hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;//adc内部分频,Max 256,只能倍数
			HAL_ADC_Init(&hadc1);
			HAL_ADC_Init(&hadc2);
		break;
		case 0://16us  2Mhz 19531Hz
			PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
			PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
			PeriphClkInitStruct.PLL3.PLL3N = 200;	//PLL分频后倍频,Max 512
			PeriphClkInitStruct.PLL3.PLL3R = 5;	//PLL分频,Max 128
			PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
			HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		
			hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
			hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
			HAL_ADC_Init(&hadc1);
			HAL_ADC_Init(&hadc2);
		break;
	}		
}

//打开定时器捕获作为触发
void Trigger_on(void)
{
		HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
		HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2);
}

//关闭定时器捕获作为触发
void Trigger_off(void)
{
		HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_1);
		HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_2);
}



//根据不同的触发通道开始不同时间的采集任务
void Trigger_ADC(uint8_t Chx) {
	if( Trigger_Chx == 1)//通道一触发
	{
		if(Chx == HAL_TIM_ACTIVE_CHANNEL_2)//通道1的触发
		{
			
			if(adc_dma_complete_flag == notStarted)//触发功能
			{
				Flag_Trigger = 1;
				adc_dma_complete_flag =inProgress;
				HAL_ADCEx_MultiModeStart_DMA(&hadc1,adc_dma_data1,ADC_DATA_LENGTH);//ADC的 dma 开始采集
			}
		}
	}
	if(Trigger_Chx == 2)
	{
		if(Chx == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			
			if(adc_dma_complete_flag == notStarted)//触发功能
			{
				Flag_Trigger = 1;
				adc_dma_complete_flag =inProgress;
				HAL_ADCEx_MultiModeStart_DMA(&hadc1,adc_dma_data1,ADC_DATA_LENGTH);//ADC的 dma 开始采集
			}
		}
	}
}
 
//捕获触发
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	static	unsigned int ccr1, ccr1_new, period_cnt1, CaptureNumber1=0;
	static  unsigned int ccr2, ccr2_new, period_cnt2, CaptureNumber2=0;
	if ( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	 {   
			Trigger_ADC(HAL_TIM_ACTIVE_CHANNEL_1);//触发
		 if(CaptureNumber1 == 0)
       {
         ccr1 = TIM2->CCR1; 
         CaptureNumber1 = 1;
       }
       else if(CaptureNumber1 == 1)  //判断是第几次进入IC1中断
       {
				 ccr1_new = TIM2->CCR1;
				 CaptureNumber1 = 0;
         if(ccr1_new > ccr1)//判断是否有溢出
         {
           period_cnt1 = ccr1_new - ccr1;
         }
         else
         {
           period_cnt1 = 0xFFFFFFFF - ccr1 + ccr1_new + 1;  //有溢出时的周期计数值计算
         }
         if(period_cnt1!=0)
         {
           OSC.waveform1.Freq = (float)8*240000000 / period_cnt1;  //通过周期计数值计算频率
         }
		    }
		}
   if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{ 
			Trigger_ADC(HAL_TIM_ACTIVE_CHANNEL_2);//触发			
			if(CaptureNumber2 == 0)
       {
         ccr2 = TIM2->CCR2; 
         CaptureNumber2 = 1;
       }
       else if(CaptureNumber2 == 1)  
			 {
				 ccr2_new = TIM2->CCR2;
         CaptureNumber2 = 0;
				 if(ccr2_new > ccr2)
					{
           period_cnt2 = ccr2_new - ccr2;
					}
         else
					{
           period_cnt2 = 0xFFFFFFFF - ccr2 + ccr2_new + 1;  
					}
         
         if(period_cnt2!=0)
					{
           OSC.waveform2.Freq = (float)8*240000000 / period_cnt2;  
					}
				}
		}
	
}


//ADC 回调函数，ADC 采集完成后进入回调函数
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc->Instance==ADC1)
	{
		adc_dma_complete_flag = completed;//采集完成标志
		Trigger_off();
	}
}


//=============================
//函数名称：LCD_FFZ
//函数返回：无
//参数说明：无
//功能概要：刷新ADC的峰峰值
//=========================
void LCD_FFZ(void)
{
	uint16_t adc_max = 0,adc_min = 0xffff;

	for(int ffz = 0; ffz <ADC_DATA_LENGTH;ffz++)
		{
			adc_max = adc_max > (adc_dma_data1[ffz]&0x0000ffff) ? adc_max :(adc_dma_data1[ffz]&0x0000ffff);
			adc_min = adc_min < (adc_dma_data1[ffz]&0x0000ffff)? adc_min : (adc_dma_data1[ffz]&0x0000ffff);
		}
		if(OSC.waveform1.gears[Vpp_num1]==2)
	{
			OSC.waveform1.Vpp= (adc_max-adc_min)*3300/65535/OSC.waveform1.gears[Vpp_num1];//求出峰峰值
	}else if (OSC.waveform1.gears[Vpp_num1] == 10)
	{
			OSC.waveform1.Vpp = (adc_max-adc_min)*3300/65535*OSC.waveform1.gears[Vpp_num1];//求出峰峰值
	}
	
	adc_max = 0,adc_min = 0xffff;
	for(int ffz = 0; ffz < ADC_DATA_LENGTH;ffz++)
	{
		adc_max = adc_max > (adc_dma_data1[ffz]>>16&0x0000ffff) ? adc_max :(adc_dma_data1[ffz]>>16&0x0000ffff);
		adc_min = adc_min < (adc_dma_data1[ffz]>>16&0x0000ffff)? adc_min : (adc_dma_data1[ffz]>>16&0x0000ffff);
	}
	if(OSC.waveform2.gears[Vpp_num2]==2)
	{
			OSC.waveform2.Vpp = (adc_max-adc_min)*3300/65535/OSC.waveform2.gears[Vpp_num2];//求出峰峰值
	}else if (OSC.waveform2.gears[Vpp_num2] == 10)
	{
			OSC.waveform2.Vpp = (adc_max-adc_min)*3300/65535*OSC.waveform2.gears[Vpp_num2];//求出峰峰值
	}
}


//=============================
//函数名称：TesT_offset
//函数返回：0：完成退出
//参数说明：uint32_t * g_adc_dma_data:DMA接收adc的数组
//		                  OSC_ *wave:示波器波形
//功能概要：测试偏移值
//=========================
uint8_t TesT_offset(uint32_t * g_adc_dma_data,OSC_ *wave)
{
	float buffer1[1000];
	float zongzhi1 = 0;
	float offset1 = 0;
	
	float buffer2[1000];
	float zongzhi2 = 0;
	float offset2 = 0;
	uint16_t j1 =0;
	while(1)
	{
		 //波形1
		 buffer1[j1] = (((float)(g_adc_dma_data[j1]&0x0000ffff)-32768)*wave->waveform1.Sca_factor[Vpp_num1]+32768)*179/65535;
		 //波形2
		 buffer2[j1] = (((float)(g_adc_dma_data[j1]>>16&0x0000ffff)-32768)*wave->waveform2.Sca_factor[Vpp_num2]+32768)*179/65535;
		j1++;
		zongzhi1 += buffer1[j1];
		zongzhi2 += buffer2[j1];
		 if(j1 >= 320)
		 {
			 zongzhi1 = zongzhi1/320;
			 zongzhi2 = zongzhi2/320;
			 offset1 = 130-zongzhi1;
			 offset2 = 130-zongzhi2;
			 printf("偏移量1：%.1f\n",offset1);
			 printf("偏移量2：%.1f\n",offset2);
			 return 0;
		 }
	}
}

//=============================
//函数名称：Waveform_Processing
//函数返回：无
//参数说明：*Show_Lin：存放数组的地址；
//         Sca_factor1：第一个波形的比例因数；
//         Sca_factor2：第二个波形的比例因数；
//         offset1：第一个波形所需要增加的偏移值；
//         offset2：第二个波形所需要增加的偏移值；
//功能概要：ADC数据处理成刷新在屏幕的值存放在数组中
//=========================
void Waveform_Processing(uint16_t * Show_Lin,float Sca_factor1,float Sca_factor2,float offset1,float offset2)
{
	for(uint16_t i=0;i<(320-1)/2;i++) //处理波形展示
	{
		Show_Lin[0+(i*2)] = ((((float)(adc_dma_data1[i+1]&0x0000ffff)-32768)*Sca_factor1+32768)*179/65535)+offset1;//adc的值放入BUff
		if(Show_Lin[0+(i*2)]>=219)
		{
			Show_Lin[0+(i*2)] = 219;
		}else if(Show_Lin[0+(i*2)]<=40)
		{
				Show_Lin[0+(i*2)] = 40;
		}
		Show_Lin[1+(i*2)] = ((((float)(adc_dma_data1[i+1]>>16&0x0000ffff)-32768)*Sca_factor2+32768)*179/65535)+offset2;//adc的值放入BUff
		if(Show_Lin[1+(i*2)]>=219)
		{
			Show_Lin[1+(i*2)] = 219;
		}else if(Show_Lin[1+(i*2)]<=40)
		{
				Show_Lin[1+(i*2)] = 40;
		}
	}
}

//对ADC的获取的数据进行处理
void ADC_Data_Processing(uint16_t * Show_Lin,OSC_ * wave)
{
	static uint8_t i =0;
	if(i == 0)//数据初始化
	{
		OSC_init();
		i =1;
	}
	(wave->waveform1.gears[Vpp_num1] == 2) ? Relay1_Off() : Relay1_On() ;

	(wave->waveform2.gears[Vpp_num2] == 2) ? Relay2_Off() : Relay2_On() ;
	//峰峰值计算
	LCD_FFZ();
	//偏移值的获取
	//TesT_offset(adc_dma_data1,&waveform1,&waveform2);
	//获取波形
	Waveform_Processing(Show_Lin,wave->waveform1.Sca_factor[Vpp_num1],wave->waveform2.Sca_factor[Vpp_num2],wave->waveform1.offset[Vpp_num1],wave->waveform2.offset[Vpp_num2]);
}

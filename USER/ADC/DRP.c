//���ڴ���ADC��ȡ���ݺ�Ĵ��������
//ADC�Ļص�
//TIM�Ĵ����ص�


#include "ADC/DRP.h"
#include "tim.h"
#include "adc.h"
#include "Key/KeyPad.h"
#include "RELAY/relay.h"
#include "debug/debug.h"


uint32_t adc_dma_data1[ADC_DATA_LENGTH];//���� adc1 �ɼ����ݴ������
uint8_t adc_dma_complete_flag = notStarted; //adc1 ���� dma �ɼ���ɱ�־���� dma �ɼ���ɻص���������

uint8_t Flag_Trigger =0;  //������־λ
uint8_t Flag_overtime = 0;  //��ʱ��־λ

//����y���л��ļ����ʼ��
void OSC_init(void)
{
	//����1�ļ̵�����λ����
	OSC.waveform1.gears[0] = 2;
	OSC.waveform1.gears[1] = 2;
	OSC.waveform1.gears[2] = 10;
	OSC.waveform1.gears[3] = 10;
	OSC.waveform1.gears[4] = 10;
	OSC.waveform1.gears[5] = 10;
	//����1�Ĳ��α�������
	OSC.waveform1.Sca_factor[0] = 3.3;
	OSC.waveform1.Sca_factor[1] = 1.65;
	OSC.waveform1.Sca_factor[2] = 13.2;
	OSC.waveform1.Sca_factor[3] = 6.6;
	OSC.waveform1.Sca_factor[4] = 3.3;
	OSC.waveform1.Sca_factor[5] = 1.32;
	//����1������ƫ��ֵ
	OSC.waveform1.offset[0] = 31.6;
	OSC.waveform1.offset[1] = 36.2;
	OSC.waveform1.offset[2] = 5.0;
	OSC.waveform1.offset[3] = 22.8;
	OSC.waveform1.offset[4] = 31.8;
	OSC.waveform1.offset[5] = 37.2;
	
	//����2�ļ̵�����λ����
	OSC.waveform2.gears[0] = 2;
	OSC.waveform2.gears[1] = 2;
	OSC.waveform2.gears[2] = 10;
	OSC.waveform2.gears[3] = 10;
	OSC.waveform2.gears[4] = 10;
	OSC.waveform2.gears[5] = 10;
	//����2�Ĳ��α�������
	OSC.waveform2.Sca_factor[0] = 3.3;
	OSC.waveform2.Sca_factor[1] = 1.65;
	OSC.waveform2.Sca_factor[2] = 13.2;
	OSC.waveform2.Sca_factor[3] = 6.6;
	OSC.waveform2.Sca_factor[4] = 3.3;
	OSC.waveform2.Sca_factor[5] = 1.32;
	//����2������ƫ��ֵ
	OSC.waveform2.offset[0] = 32.7;
	OSC.waveform2.offset[1] = 36.6;
	OSC.waveform2.offset[2] = 8.1;
	OSC.waveform2.offset[3] = 24.6;
	OSC.waveform2.offset[4] = 32.6;
	OSC.waveform2.offset[5] = 37.4;
}


//x��ADCʱ�Ӹ���
//x����ڣ����ڵ��ڲ����ʣ������������
void Osc_Time_Switch(uint8_t timer)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0}; //RCCʱ�ӳ�ʼ�ṹ��
	ADC_Disable(&hadc1);
	ADC_Disable(&hadc2);
	HAL_Delay(2);
	
	switch(timer)
	{
		case 6://1.6ms 0.02Mhz  20000/512*5 =195Hz
			PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
			PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL����25M��Ƶ,Max 63
			PeriphClkInitStruct.PLL3.PLL3N = 320;	//PLL��Ƶ��Ƶ,Max 512
			PeriphClkInitStruct.PLL3.PLL3R = 100;	//PLL��Ƶ,Max 128
			PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
			HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		
			hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV8;//adc�ڲ���Ƶ,Max 256,ֻ�ܱ���
			hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV8;//adc�ڲ���Ƶ,Max 256,ֻ�ܱ���
			HAL_ADC_Init(&hadc1);
			HAL_ADC_Init(&hadc2);
		break;
		case 5://640us 0.05Mhz  488Hz
			PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
			PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL����25M��Ƶ,Max 63
			PeriphClkInitStruct.PLL3.PLL3N = 150;	//PLL��Ƶ��Ƶ,Max 512
			PeriphClkInitStruct.PLL3.PLL3R = 75;	//PLL��Ƶ,Max 128
			PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
			HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		
			hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;//adc�ڲ���Ƶ,Max 256,ֻ�ܱ���
			hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;//adc�ڲ���Ƶ,Max 256,ֻ�ܱ���
			HAL_ADC_Init(&hadc1);
			HAL_ADC_Init(&hadc2);
		break;
		case 4://320us 0.1mhz  976hz
			PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
			PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL����25M��Ƶ,Max 63
			PeriphClkInitStruct.PLL3.PLL3N = 150;	//PLL��Ƶ��Ƶ,Max 512
			PeriphClkInitStruct.PLL3.PLL3R = 75;	//PLL��Ƶ,Max 128
			PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
			HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		
			hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc�ڲ���Ƶ,Max 256,ֻ�ܱ���
			hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc�ڲ���Ƶ,Max 256,ֻ�ܱ���
			HAL_ADC_Init(&hadc1);
			HAL_ADC_Init(&hadc2);
		break;
		case 3://160us 0.2Mhz  1953Hz
			PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
			PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL����25M��Ƶ,Max 63
			PeriphClkInitStruct.PLL3.PLL3N = 200;	//PLL��Ƶ��Ƶ,Max 512
			PeriphClkInitStruct.PLL3.PLL3R = 50;	//PLL��Ƶ,Max 128
			PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
			HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		
			hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc�ڲ���Ƶ,Max 256,ֻ�ܱ���
			hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc�ڲ���Ƶ,Max 256,ֻ�ܱ���
			HAL_ADC_Init(&hadc1);
			HAL_ADC_Init(&hadc2);
		break;
		case 2://64us 0.5Mhz 4882Hz
			PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
			PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL����25M��Ƶ,Max 63
			PeriphClkInitStruct.PLL3.PLL3N = 200;	//PLL��Ƶ��Ƶ,Max 512
			PeriphClkInitStruct.PLL3.PLL3R = 5;	//PLL��Ƶ,Max 128
			PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
			HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		
			hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;//adc�ڲ���Ƶ,Max 256,ֻ�ܱ���
			hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;//adc�ڲ���Ƶ,Max 256,ֻ�ܱ���
			HAL_ADC_Init(&hadc1);
			HAL_ADC_Init(&hadc2);
		break;
		case 1://32us 1Mhz 9765Hz
			PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
			PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL����25M��Ƶ,Max 63
			PeriphClkInitStruct.PLL3.PLL3N = 200;	//PLL��Ƶ��Ƶ,Max 512
			PeriphClkInitStruct.PLL3.PLL3R = 5;	//PLL��Ƶ,Max 128
			PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
			HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		
			hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;//adc�ڲ���Ƶ,Max 256,ֻ�ܱ���
			hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;//adc�ڲ���Ƶ,Max 256,ֻ�ܱ���
			HAL_ADC_Init(&hadc1);
			HAL_ADC_Init(&hadc2);
		break;
		case 0://16us  2Mhz 19531Hz
			PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
			PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL����25M��Ƶ,Max 63
			PeriphClkInitStruct.PLL3.PLL3N = 200;	//PLL��Ƶ��Ƶ,Max 512
			PeriphClkInitStruct.PLL3.PLL3R = 5;	//PLL��Ƶ,Max 128
			PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
			HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		
			hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc�ڲ���Ƶ,Max 256,ֻ�ܱ���
			hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc�ڲ���Ƶ,Max 256,ֻ�ܱ���
			HAL_ADC_Init(&hadc1);
			HAL_ADC_Init(&hadc2);
		break;
	}		
}

//�򿪶�ʱ��������Ϊ����
void Trigger_on(void)
{
		HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
		HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2);
}

//�رն�ʱ��������Ϊ����
void Trigger_off(void)
{
		HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_1);
		HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_2);
}



//���ݲ�ͬ�Ĵ���ͨ����ʼ��ͬʱ��Ĳɼ�����
void Trigger_ADC(uint8_t Chx) {
	if( Trigger_Chx == 1)//ͨ��һ����
	{
		if(Chx == HAL_TIM_ACTIVE_CHANNEL_2)//ͨ��1�Ĵ���
		{
			
			if(adc_dma_complete_flag == notStarted)//��������
			{
				Flag_Trigger = 1;
				adc_dma_complete_flag =inProgress;
				HAL_ADCEx_MultiModeStart_DMA(&hadc1,adc_dma_data1,ADC_DATA_LENGTH);//ADC�� dma ��ʼ�ɼ�
			}
		}
	}
	if(Trigger_Chx == 2)
	{
		if(Chx == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			
			if(adc_dma_complete_flag == notStarted)//��������
			{
				Flag_Trigger = 1;
				adc_dma_complete_flag =inProgress;
				HAL_ADCEx_MultiModeStart_DMA(&hadc1,adc_dma_data1,ADC_DATA_LENGTH);//ADC�� dma ��ʼ�ɼ�
			}
		}
	}
}
 
//���񴥷�
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	static	unsigned int ccr1, ccr1_new, period_cnt1, CaptureNumber1=0;
	static  unsigned int ccr2, ccr2_new, period_cnt2, CaptureNumber2=0;
	if ( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	 {   
			Trigger_ADC(HAL_TIM_ACTIVE_CHANNEL_1);//����
		 if(CaptureNumber1 == 0)
       {
         ccr1 = TIM2->CCR1; 
         CaptureNumber1 = 1;
       }
       else if(CaptureNumber1 == 1)  //�ж��ǵڼ��ν���IC1�ж�
       {
				 ccr1_new = TIM2->CCR1;
				 CaptureNumber1 = 0;
         if(ccr1_new > ccr1)//�ж��Ƿ������
         {
           period_cnt1 = ccr1_new - ccr1;
         }
         else
         {
           period_cnt1 = 0xFFFFFFFF - ccr1 + ccr1_new + 1;  //�����ʱ�����ڼ���ֵ����
         }
         if(period_cnt1!=0)
         {
           OSC.waveform1.Freq = (float)8*240000000 / period_cnt1;  //ͨ�����ڼ���ֵ����Ƶ��
         }
		    }
		}
   if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{ 
			Trigger_ADC(HAL_TIM_ACTIVE_CHANNEL_2);//����			
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


//ADC �ص�������ADC �ɼ���ɺ����ص�����
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc->Instance==ADC1)
	{
		adc_dma_complete_flag = completed;//�ɼ���ɱ�־
		Trigger_off();
	}
}


//=============================
//�������ƣ�LCD_FFZ
//�������أ���
//����˵������
//���ܸ�Ҫ��ˢ��ADC�ķ��ֵ
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
			OSC.waveform1.Vpp= (adc_max-adc_min)*3300/65535/OSC.waveform1.gears[Vpp_num1];//������ֵ
	}else if (OSC.waveform1.gears[Vpp_num1] == 10)
	{
			OSC.waveform1.Vpp = (adc_max-adc_min)*3300/65535*OSC.waveform1.gears[Vpp_num1];//������ֵ
	}
	
	adc_max = 0,adc_min = 0xffff;
	for(int ffz = 0; ffz < ADC_DATA_LENGTH;ffz++)
	{
		adc_max = adc_max > (adc_dma_data1[ffz]>>16&0x0000ffff) ? adc_max :(adc_dma_data1[ffz]>>16&0x0000ffff);
		adc_min = adc_min < (adc_dma_data1[ffz]>>16&0x0000ffff)? adc_min : (adc_dma_data1[ffz]>>16&0x0000ffff);
	}
	if(OSC.waveform2.gears[Vpp_num2]==2)
	{
			OSC.waveform2.Vpp = (adc_max-adc_min)*3300/65535/OSC.waveform2.gears[Vpp_num2];//������ֵ
	}else if (OSC.waveform2.gears[Vpp_num2] == 10)
	{
			OSC.waveform2.Vpp = (adc_max-adc_min)*3300/65535*OSC.waveform2.gears[Vpp_num2];//������ֵ
	}
}


//=============================
//�������ƣ�TesT_offset
//�������أ�0������˳�
//����˵����uint32_t * g_adc_dma_data:DMA����adc������
//		                  OSC_ *wave:ʾ��������
//���ܸ�Ҫ������ƫ��ֵ
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
		 //����1
		 buffer1[j1] = (((float)(g_adc_dma_data[j1]&0x0000ffff)-32768)*wave->waveform1.Sca_factor[Vpp_num1]+32768)*179/65535;
		 //����2
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
			 printf("ƫ����1��%.1f\n",offset1);
			 printf("ƫ����2��%.1f\n",offset2);
			 return 0;
		 }
	}
}

//=============================
//�������ƣ�Waveform_Processing
//�������أ���
//����˵����*Show_Lin���������ĵ�ַ��
//         Sca_factor1����һ�����εı���������
//         Sca_factor2���ڶ������εı���������
//         offset1����һ����������Ҫ���ӵ�ƫ��ֵ��
//         offset2���ڶ�����������Ҫ���ӵ�ƫ��ֵ��
//���ܸ�Ҫ��ADC���ݴ����ˢ������Ļ��ֵ�����������
//=========================
void Waveform_Processing(uint16_t * Show_Lin,float Sca_factor1,float Sca_factor2,float offset1,float offset2)
{
	for(uint16_t i=0;i<(320-1)/2;i++) //������չʾ
	{
		Show_Lin[0+(i*2)] = ((((float)(adc_dma_data1[i+1]&0x0000ffff)-32768)*Sca_factor1+32768)*179/65535)+offset1;//adc��ֵ����BUff
		if(Show_Lin[0+(i*2)]>=219)
		{
			Show_Lin[0+(i*2)] = 219;
		}else if(Show_Lin[0+(i*2)]<=40)
		{
				Show_Lin[0+(i*2)] = 40;
		}
		Show_Lin[1+(i*2)] = ((((float)(adc_dma_data1[i+1]>>16&0x0000ffff)-32768)*Sca_factor2+32768)*179/65535)+offset2;//adc��ֵ����BUff
		if(Show_Lin[1+(i*2)]>=219)
		{
			Show_Lin[1+(i*2)] = 219;
		}else if(Show_Lin[1+(i*2)]<=40)
		{
				Show_Lin[1+(i*2)] = 40;
		}
	}
}

//��ADC�Ļ�ȡ�����ݽ��д���
void ADC_Data_Processing(uint16_t * Show_Lin,OSC_ * wave)
{
	static uint8_t i =0;
	if(i == 0)//���ݳ�ʼ��
	{
		OSC_init();
		i =1;
	}
	(wave->waveform1.gears[Vpp_num1] == 2) ? Relay1_Off() : Relay1_On() ;

	(wave->waveform2.gears[Vpp_num2] == 2) ? Relay2_Off() : Relay2_On() ;
	//���ֵ����
	LCD_FFZ();
	//ƫ��ֵ�Ļ�ȡ
	//TesT_offset(adc_dma_data1,&waveform1,&waveform2);
	//��ȡ����
	Waveform_Processing(Show_Lin,wave->waveform1.Sca_factor[Vpp_num1],wave->waveform2.Sca_factor[Vpp_num2],wave->waveform1.offset[Vpp_num1],wave->waveform2.offset[Vpp_num2]);
}

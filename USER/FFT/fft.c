#include "FFT/fft.h"
//DSP
#include "arm_math.h"
#include "arm_const_structs.h"


/* ���� */
uint32_t ifftFlag = 0; 
uint32_t fftSize = 0;

/* ������������ */
#define TEST_LENGTH_SAMPLES 512 

static float32_t testOutput_f32[TEST_LENGTH_SAMPLES*2]; 
static float32_t testOutputMag_f32[TEST_LENGTH_SAMPLES*2]; 
static float32_t testInput_f32[TEST_LENGTH_SAMPLES*2];



/*
*********************************************************************************************************
*	�� �� ��: arm_rfft_f32_app
*	����˵��: ���ú���arm_rfft_fast_f32�����Ƶ
*	��    �Σ�uint32_t * g_adc_dma_data:adc������
						uint8_t CHx_selection:ͨ��ѡ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 float  *arm_rfft_f32_app(uint32_t * g_adc_dma_data,uint8_t Chx)
{
	uint16_t i;
	arm_rfft_fast_instance_f32 S;
	
	/* ���任 */
    ifftFlag = 0; 
	
	/* ��ʼ���ṹ��S�еĲ��� */
 	arm_rfft_fast_init_f32(&S, TEST_LENGTH_SAMPLES);
	
	if(1 == Chx)
	{
		for(i=0; i<512; i++)
		{
			testInput_f32[i] = g_adc_dma_data[i]&0x0000ffff;
		}
	}else
	{
		for(i=0; i<512; i++)
		{
			testInput_f32[i] = g_adc_dma_data[i]>>16&0x0000ffff;
		}
	}
	/* 512��ʵ���п���FFT */ 
	arm_rfft_fast_f32(&S, testInput_f32, testOutput_f32, ifftFlag);
	
	/* Ϊ�˷��������arm_cfft_f32����Ľ�����Աȣ����������512��ģֵ��ʵ�ʺ���arm_rfft_fast_f32
	   ֻ������256��  
	*/ 
 	arm_cmplx_mag_f32(testOutput_f32, testOutputMag_f32, TEST_LENGTH_SAMPLES);

	return testOutputMag_f32;
}


void ADC_FFT_Processing(uint32_t * g_adc_dma_data,uint16_t * Show_Lin,uint8_t Chx)
{
	static float * FFT_Storage;
	static uint16_t FFT_log_value[256];
	uint16_t adc_max = 0,adc_min = 0xffff;
	float ratio =0;
	FFT_Storage = arm_rfft_f32_app(g_adc_dma_data,Chx);
	for(uint16_t i = 0;i<256;i++)//�洢Ƶ��
	{
		FFT_log_value[i] = log(FFT_Storage[i]);
		adc_max = adc_max > FFT_log_value[i] ? adc_max : FFT_log_value[i];
		adc_min = adc_min < FFT_log_value[i] ? adc_min : FFT_log_value[i];
	}
	ratio = 179/(adc_max-adc_min);
	for(uint16_t i = 0;i<256;i++)//�洢Ƶ��
	{
		FFT_log_value[i] = FFT_log_value[i]*ratio-(adc_min*ratio)+40;
		if(FFT_log_value[i]>=219)
		{
			FFT_log_value[i] = 219;
		}else if(FFT_log_value[i]<=40)
		{
				FFT_log_value[i] = 40;
		}
		Show_Lin[i+320] = FFT_log_value[i];
	}

}

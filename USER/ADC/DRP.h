#ifndef __DRP_H
#define __DRP_H		
#include "stm32h7xx_hal.h"
#include "main.h"

enum //��������ǰ״̬
{
	inProgress,
	completed,
	notStarted,
};

#define ADC_DATA_LENGTH 1000 //����ɼ����ݳ���Ϊ 1000


extern uint32_t adc_dma_data1[];//���� adc1 �ɼ����ݴ������
extern uint8_t adc_dma_complete_flag; //adc1 ���� dma �ɼ���ɱ�־���� dma �ɼ���ɻص���������

extern uint8_t Flag_Trigger;  //������־λ
extern uint8_t Flag_overtime;  //��ʱ��־λ

void ADC_Data_Processing(uint16_t * Show_Lin,OSC_ * wave);
void Trigger_off(void);//������
void Trigger_on(void); //����kai
//ADCʱ���л�
void Osc_Time_Switch(uint8_t timer);

void OSC_init(void);
//��ȡƫ��ֵ
uint8_t TesT_offset(uint32_t * g_adc_dma_data,OSC_ *wave);
#endif






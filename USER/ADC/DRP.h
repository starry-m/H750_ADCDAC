#ifndef __DRP_H
#define __DRP_H		
#include "stm32h7xx_hal.h"
#include "main.h"

enum //编码器当前状态
{
	inProgress,
	completed,
	notStarted,
};

#define ADC_DATA_LENGTH 1000 //定义采集数据长度为 1000


extern uint32_t adc_dma_data1[];//定义 adc1 采集数据存放数组
extern uint8_t adc_dma_complete_flag; //adc1 数据 dma 采集完成标志，在 dma 采集完成回调函数设置

extern uint8_t Flag_Trigger;  //触发标志位
extern uint8_t Flag_overtime;  //超时标志位

void ADC_Data_Processing(uint16_t * Show_Lin,OSC_ * wave);
void Trigger_off(void);//触发关
void Trigger_on(void); //触发kai
//ADC时钟切换
void Osc_Time_Switch(uint8_t timer);

void OSC_init(void);
//获取偏移值
uint8_t TesT_offset(uint32_t * g_adc_dma_data,OSC_ *wave);
#endif






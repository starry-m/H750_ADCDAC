#ifndef __FFT_H
#define __FFT_H		

#include "stdint.h"
#include "stm32h7xx.h"                  // Device header


float  *arm_rfft_f32_app(uint32_t * ADC_array,uint8_t ADC_selection);

void ADC_FFT_Processing(uint32_t * g_adc_dma_data,uint16_t * Show_Lin,uint8_t Chx);
#endif






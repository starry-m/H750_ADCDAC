#ifndef __SYS_BSP_H__
#define __SYS_BSP_H__

#include <stdio.h>
#include "stm32h7xx.h"                  // Device header


enum //编码器当前状态
{
	Vertical_1 = 2,
	Vertical_2,
	SampRate,
	DAC_Trigger,
	DAC_Wave,
	DAC_frq_UCF,
	DAC_frq_TCF,
	DAC_frq_HCF,
	DAC_signe,
	DAC_Vpp_UV,
	DAC_Vpp_TV,
	DAC_Vpp_HmV,
	DAC_Vpp_TmV,
};

extern uint8_t bspEncoder, prebspEncoder;
extern uint8_t SampRate_num,preSampRate_num;

extern uint8_t Vpp_num1;
extern uint8_t Vpp_num2;
extern uint8_t DAC_Trigger_num,preDAC_Trigger_num;
extern uint8_t Trigger_Chx;
extern uint8_t DAC_switch;
extern uint8_t DAC_flag_Freq;//DAC频率修改标志位
extern uint8_t DAC_flag_Vpp;//DAC峰峰值修改标志位

void All_Key_scan(void);

#endif



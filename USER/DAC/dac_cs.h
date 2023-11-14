#ifndef __DAC_CS_H
#define __DAC_CS_H		
#include "stdint.h"

#define SIN_ROM_LENGTH	50


extern uint16_t g_Wave[];
extern uint16_t sin_rom[];

struct DAC_freq_
{
	uint8_t HCF;
	uint8_t TCF;
	uint8_t UCF;
	uint8_t Signe;
};

struct DAC_Vpp_
{
	uint8_t TV;
	uint8_t UV;
	uint8_t HmV;
	uint8_t TmV;
};

typedef struct DAC__{
	struct DAC_freq_ Freq;
	float DAC_Freq;
	struct DAC_Vpp_  Vpp;
	float DAC_Vpp;
	uint8_t DAC_wave_form;
}DAC_;


extern DAC_ DAC;

void Trigger_Open(uint16_t number);
void DAC_processing(uint8_t on_off,uint8_t Freq_on_off,uint8_t Vpp_on_off);
#endif






#ifndef __REFRESH_H
#define __REFRESH_H		
#include "stm32h7xx_hal.h"


extern uint16_t Show_GramA[];//����1
extern uint16_t Show_GramB[];//����2
extern uint8_t Show_AB;    //��һ�λ����Ƿ����

//��ʼˢ��LCD����Ļ
void LCD_Show_Start(void);
void LCD_Refresh(uint16_t * Show_Lin);
#endif






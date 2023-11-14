#ifndef __REFRESH_H
#define __REFRESH_H		
#include "stm32h7xx_hal.h"


extern uint16_t Show_GramA[];//缓存1
extern uint16_t Show_GramB[];//缓存2
extern uint8_t Show_AB;    //上一次缓存是否完成

//开始刷新LCD的屏幕
void LCD_Show_Start(void);
void LCD_Refresh(uint16_t * Show_Lin);
#endif






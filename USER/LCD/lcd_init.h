#ifndef __LCD_INIT_H
#define __LCD_INIT_H
#include "stm32h7xx_hal.h"



#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

//Alter
#if USE_HORIZONTAL==0||USE_HORIZONTAL==1  //横竖屏时的长高不唯一
#define LCD_W 240
#define LCD_H 320

#else
#define LCD_W 320
#define LCD_H 240
#endif

//#define LCD_W 240
//#define LCD_H 320
 


//-----------------LCD端口定义---------------- 


#define LCD_RES_Clr()  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET)//RES
#define LCD_RES_Set()  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET)

#define LCD_DC_Clr()   HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_RESET)//DC
#define LCD_DC_Set()   HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_SET)

#define LCD_BLK_Clr()		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_RESET)//BLK
#define LCD_BLK_Set()  	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_SET)//BLK

#define LCD_CS_Clr()		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET)//CS
#define LCD_CS_Set()  	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET)//CS

void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(uint8_t dat);//模拟SPI时序
void LCD_WR_DATA8(uint8_t dat);//写入一个字节
void LCD_WR_DATA(uint16_t dat);//写入两个字节
void LCD_WR_REG(uint8_t dat);//写入一个指令
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif





#ifndef __LCD_DMA_H
#define __LCD_DMA_H		
#include "stm32h7xx_hal.h"




#define USE_COLOR_INVERSION  1



//LCD的画笔颜色和背景色	   
extern uint16_t  POINT_COLOR;	//画笔颜色
extern uint16_t  BACK_COLOR;  //背景色 

//清空
void Lcd_Gram_Fill(uint16_t *dat,uint16_t color);

//在指定区域填充颜色
void Lcd_Clear(uint16_t *dat ,uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);

//画一个线段
void Lcd_Show_Lin(uint16_t *dat,uint16_t x,uint8_t pointa,uint8_t pointb,uint16_t color);

//显示一个字符
void LCD_ShowChar(uint16_t *dat,uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);

//显示字符串
void LCD_ShowString(uint16_t *dat,uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t mode,uint8_t *p);

//显示数字,高位为0,则不显示
void LCD_ShowNum(uint16_t *dat,uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size,uint8_t mode);

//绘制黑白取模图片
void Lcd_Show_Bmp(uint16_t *dat,uint8_t *bmp,uint16_t x,uint16_t y,uint16_t w,uint16_t h);

//LCD填充
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);


#ifdef USE_COLOR_INVERSION  //如果需要颜色翻转
//颜色翻转
//画笔颜色
#define WHITE         	 0x0000
#define BLACK         	 0xFFFF 
#define BLUE           	 0xFFE0 
#define BRED             0X07E0
#define GRED 			       0X001F
#define GBLUE			       0XF800
#define RED           	 0x07FF
#define MAGENTA       	 0x07E0
#define GREEN         	 0xF81F
#define CYAN          	 0x8000
#define YELLOW        	 0x001F
#define BROWN 			     0X43BF //棕色
#define BRRED 			     0X07F8 //棕红色
#define GRAY  			     0X7BCF //灰色
#define DARKBLUE      	 0XFE30	//深蓝色
#define LIGHTBLUE      	 0X8283	//浅蓝色  
#define GRAYBLUE       	 0XABA7 //灰蓝色
#define LIGHTGREEN     	 0X7BE0 //浅绿色
#define LGRAY 			     0X39E7 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0X59AE //浅灰蓝色(中间层颜色)
#define LBBLUE           0XD4ED //浅棕蓝色(选择条目的反色)

#else         //如果不需要颜色翻转
//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
#endif

#endif






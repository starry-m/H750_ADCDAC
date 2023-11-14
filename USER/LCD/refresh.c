#include "LCD/refresh.h"
#include "LCD/lcd_init.h"
#include "LCD/lcd_dma.h"
#include "GUI/gui.h"
#include "spi.h"
#include "lptim.h"

#define LCD_RAM_NUMBER  76800   
//屏幕显示缓存
uint16_t Show_GramA[LCD_RAM_NUMBER];//缓存1
uint16_t Show_GramB[LCD_RAM_NUMBER];//缓存2
uint8_t Show_AB;    //上一次缓存是否完成
uint8_t Lcd_Show_Over = 0;//上一次缓存是否完成
uint16_t *LCD_SHOW_RAM;

//开始刷新LCD的屏幕
void LCD_Show_Gram(uint16_t *dat)
{
    Lcd_Show_Over = 0;
    LCD_SHOW_RAM = dat;
    //设置地址
    LCD_Address_Set(0,0,LCD_W-1,LCD_H-1);//设置显示范围
    HAL_SPI_Transmit_DMA(&hspi4,(uint8_t *)LCD_SHOW_RAM,38400);
}


//SPI全部传输完成的回调函数
 void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)//SPI完全传输成功回调
{
	static uint8_t cishu = 0;
	if(++cishu<4)
	{
		HAL_SPI_Transmit_DMA(&hspi4,(uint8_t *)LCD_SHOW_RAM+38400*cishu,38400);
	}else
	{
		cishu = 0;
		Lcd_Show_Over = 1;
	}
}

void LCD_Show_Start(void)
{
	LCD_Init();
	Lcd_Gram_Fill(Show_GramA,BLACK);
	Lcd_Gram_Fill(Show_GramB,BLACK);
	Lcd_Show_Over = 1;
	HAL_LPTIM_Counter_Start_IT(&hlptim1,625);//20ms
}

void LCD_Refresh(uint16_t * Show_Lin)
{
	if(Lcd_Show_Over)//是否更新
	{
			if(Show_AB)
			{
				LCD_Show_Gram(Show_GramA);//BUff判断 为一显示A，0显示B
				Lcd_Show_LinA(Show_GramB,Show_Lin);
			}
			else{ LCD_Show_Gram(Show_GramB);
				Lcd_Show_LinA(Show_GramA,Show_Lin);
			}
				Show_AB =!Show_AB; 
	}
}

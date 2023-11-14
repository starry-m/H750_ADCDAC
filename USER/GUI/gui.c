//LCD刷新网格和波形数据

#include "GUI/gui.h"
#include "main.h"
#include "LCD/lcd_dma.h"
#include "Key/KeyPad.h"
#include "LCD/refresh.h"
#include "DAC/dac_cs.h"


///各种显示字符串的位置  
const char *ui_show1[25]=
{
	" 100mV  x2",
	" 200mV  x2",
	" 500mV /10",
	"1000mV /10",
	"2000mV /10",
	"5000mV /10",
	" 100mV  x2",
	" 200mV  x2",
	" 500mV /10",
	"1000mV /10",
	"2000mV /10",
	"5000mV /10",
	"  8us 125K",
	" 16us  62K",
	" 32us  31K",
	" 64us  13K",
	"160us 6.2K",
	"320us 3.1K",
	"640us 1.2K",
	"Sin",
  " Sq",
	"Tri",
	"K",
	"H",
};

///各种显示字符串的位置  
const char *ui_fixed[50]=
{
	"1:",
	"2:",
	"Scale",
	"Freq1",
	"Freq2",
	"Vpp1",
	"Vpp2",
	"T",
	"S",
	"D",
	"V",
	".",
};


//=============================
//函数名称：Lcd_Show_fixedData
//函数返回：无
//参数说明：无
//功能概要：Lcd刷新固定的数据
//=========================
void Lcd_Show_fixedData(void)
{
	for(uint8_t i =0;i<25;i++)
	{
		switch(i)
		{
			case 0://1:
				BACK_COLOR = LIGHTGREEN;
				LCD_ShowString(Show_GramA,0,0,320,20,16,0,(uint8_t *)ui_fixed[i]); 
				LCD_ShowString(Show_GramB,0,0,320,20,16,0,(uint8_t *)ui_fixed[i]);       
			break;
			case 1://2:
				BACK_COLOR = GREEN;
				LCD_ShowString(Show_GramA,110,0,320,20,16,0,(uint8_t *)ui_fixed[i]); 
				LCD_ShowString(Show_GramB,110,0,320,20,16,0,(uint8_t *)ui_fixed[i]);     
			break;
			case 2://Scale
				LCD_ShowString(Show_GramA,200,201,320,20,16,0,(uint8_t *)ui_fixed[i]); 
				LCD_ShowString(Show_GramB,200,201,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			break;		
			case 3://显示频率1
			 LCD_ShowString(Show_GramA,0,201,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			 LCD_ShowString(Show_GramB,0,201,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			break;
			case 4://显示频率2
			 LCD_ShowString(Show_GramA,100,201,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			 LCD_ShowString(Show_GramB,100,201,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			break;
			 case 5://显示峰峰值1
			 LCD_ShowString(Show_GramA,0,220,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			 LCD_ShowString(Show_GramB,0,220,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			break;
			case 6://显示峰峰值2
			 LCD_ShowString(Show_GramA,100,220,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			 LCD_ShowString(Show_GramB,100,220,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			break;
			case 7://T
			 LCD_ShowString(Show_GramA,270,0,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			 LCD_ShowString(Show_GramB,270,0,320,20,16,0,(uint8_t *)ui_fixed[i]);  
			break;
			case 8://S
			 LCD_ShowString(Show_GramA,220,0,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			 LCD_ShowString(Show_GramB,220,0,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			break;
			case 9://D
			 LCD_ShowString(Show_GramA,300,0,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			 LCD_ShowString(Show_GramB,300,0,320,20,16,0,(uint8_t *)ui_fixed[i]);   
			break;
			case 10://V
			  LCD_ShowString(Show_GramA,312,220,320,20,16,0,(uint8_t *)ui_fixed[i]); 
				LCD_ShowString(Show_GramB,312,220,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			break;
			case 11:// .
			  LCD_ShowString(Show_GramA,286,220,320,20,16,0,(uint8_t *)ui_fixed[i]); 
				LCD_ShowString(Show_GramB,286,220,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			break;
			case 12://100mv  x2
				LCD_ShowString(Show_GramA,16,0,320,20,16,0,(uint8_t *)ui_show1[Vpp_num1]); 
				LCD_ShowString(Show_GramB,16,0,320,20,16,0,(uint8_t *)ui_show1[Vpp_num1]);    
			break;
			case 13://100mv  x2
				LCD_ShowString(Show_GramA,126,0,320,20,16,0,(uint8_t *)ui_show1[Vpp_num2+6]); 
				LCD_ShowString(Show_GramB,126,0,320,20,16,0,(uint8_t *)ui_show1[Vpp_num2+6]); 
			break;
			case 14://16us
			 LCD_ShowString(Show_GramA,240,201,320,20,16,0,(uint8_t *)ui_show1[SampRate_num+12]); 
			 LCD_ShowString(Show_GramB,240,201,320,20,16,0,(uint8_t *)ui_show1[SampRate_num+12]);   
			break; 
			case 15://1950
			 LCD_ShowNum(Show_GramA,230,0,(DAC_Trigger_num+28)*50,4,16,0);
			 LCD_ShowNum(Show_GramB,230,0,(DAC_Trigger_num+28)*50,4,16,0);  
			break; 
			case 16://显示DAC默认显示正弦波
			 LCD_ShowString(Show_GramA,200,220,320,20,16,0,(uint8_t *)ui_show1[DAC.DAC_wave_form+12+7]); 
			 LCD_ShowString(Show_GramB,200,220,320,20,16,0,(uint8_t *)ui_show1[DAC.DAC_wave_form+12+7]);
			break;
			case 17://DAC波形频率百位显示
			 LCD_ShowNum(Show_GramA,230,220,DAC.Freq.HCF,1,16,0); 
			 LCD_ShowNum(Show_GramB,230,220,DAC.Freq.HCF,1,16,0);
			break;
			case 18://DAC波形频率十位显示
			 LCD_ShowNum(Show_GramA,238,220,DAC.Freq.TCF,1,16,0); 
			 LCD_ShowNum(Show_GramB,238,220,DAC.Freq.TCF,1,16,0);
			break;
			case 19://DAC波形频率个位显示
			 LCD_ShowNum(Show_GramA,246,220,DAC.Freq.UCF,1,16,0); 
			 LCD_ShowNum(Show_GramB,246,220,DAC.Freq.UCF,1,16,0);
			break;
			case 20://DAC波形频率符号位显示
			 LCD_ShowString(Show_GramA,254,220,320,20,16,0,(uint8_t *)ui_show1[DAC.Freq.Signe+12+7+3]); 
			 LCD_ShowString(Show_GramB,254,220,320,20,16,0,(uint8_t *)ui_show1[DAC.Freq.Signe+12+7+3]); 
			break;
			case 21://DAC波形峰峰值十V显示
			 LCD_ShowNum(Show_GramA,270,220, DAC.Vpp.TV,1,16,0); 
			 LCD_ShowNum(Show_GramB,270,220, DAC.Vpp.TV,1,16,0);
			break;
			case 22://DAC波形峰峰值1V显示
			 LCD_ShowNum(Show_GramA,278,220,DAC.Vpp.UV,1,16,0); 
			 LCD_ShowNum(Show_GramB,278,220,DAC.Vpp.UV,1,16,0);
			break;
			case 23://DAC波形峰峰值百mV显示
			 LCD_ShowNum(Show_GramA,294,220,DAC.Vpp.HmV,1,16,0); 
			 LCD_ShowNum(Show_GramB,294,220,DAC.Vpp.HmV,1,16,0);
			break;
			case 24://DAC波形峰峰值十mV显示
			 LCD_ShowNum(Show_GramA,304,220,DAC.Vpp.TmV,1,16,0); 
			 LCD_ShowNum(Show_GramB,304,220,DAC.Vpp.TmV,1,16,0);
			break;
		 default : /* 可选的 */
			 break;
		}
		BACK_COLOR = BLACK;
	}
	
}

//=============================
//函数名称：Lcd_Show_Data
//函数返回：无
//参数说明：Encoder_flag：标志位，用来表示Encoder，1：bspEncoder，0：prebspEncoder
//功能概要：实时按键刷新的高亮
//=========================
void Lcd_Show_Data(uint8_t Encoder_flag)
{
	uint8_t bspEncoder_flag = 0;
	if(Encoder_flag == 1)//bspEncoder，此时为编码器选中即为高亮
	{
		BACK_COLOR = DARKBLUE;
		bspEncoder_flag = bspEncoder;
	}else//否则就是prebspEncoder，关掉高亮
	{
		BACK_COLOR = BLACK;
		bspEncoder_flag = prebspEncoder;
	}
	switch(bspEncoder_flag)
	{
		case Vertical_1://刷新电压1的值
			LCD_ShowString(Show_GramA,16,0,320,20,16,0,(uint8_t *)ui_show1[Vpp_num1]); 
			LCD_ShowString(Show_GramB,16,0,320,20,16,0,(uint8_t *)ui_show1[Vpp_num1]);    
		break;
		case Vertical_2://刷新电压2的值
			LCD_ShowString(Show_GramA,126,0,320,20,16,0,(uint8_t *)ui_show1[Vpp_num2+6]); 
			LCD_ShowString(Show_GramB,126,0,320,20,16,0,(uint8_t *)ui_show1[Vpp_num2+6]); 
		break;
		case SampRate://刷新采样率
		 LCD_ShowString(Show_GramA,240,201,320,20,16,0,(uint8_t *)ui_show1[SampRate_num+12]); 
		 LCD_ShowString(Show_GramB,240,201,320,20,16,0,(uint8_t *)ui_show1[SampRate_num+12]);   
		break;   
		case DAC_Trigger://刷新触发位置
		 LCD_ShowNum(Show_GramA,230,0,(DAC_Trigger_num+28)*50,4,16,0);
		 LCD_ShowNum(Show_GramB,230,0,(DAC_Trigger_num+28)*50,4,16,0);
		break;	
		case DAC_Wave://显示DAC默认显示正弦波
		 LCD_ShowString(Show_GramA,200,220,320,20,16,0,(uint8_t *)ui_show1[DAC.DAC_wave_form+12+7]); 
		 LCD_ShowString(Show_GramB,200,220,320,20,16,0,(uint8_t *)ui_show1[DAC.DAC_wave_form+12+7]);
		break;
		case DAC_frq_HCF://DAC波形频率百位显示
		 LCD_ShowNum(Show_GramA,230,220,DAC.Freq.HCF,1,16,0); 
		 LCD_ShowNum(Show_GramB,230,220,DAC.Freq.HCF,1,16,0);
		break;
		case DAC_frq_TCF://DAC波形频率十位显示
		 LCD_ShowNum(Show_GramA,238,220,DAC.Freq.TCF,1,16,0); 
		 LCD_ShowNum(Show_GramB,238,220,DAC.Freq.TCF,1,16,0);
		break;
		case DAC_frq_UCF://DAC波形频率个位显示
		 LCD_ShowNum(Show_GramA,246,220,DAC.Freq.UCF,1,16,0); 
		 LCD_ShowNum(Show_GramB,246,220,DAC.Freq.UCF,1,16,0);
		break;
		case DAC_signe://DAC波形频率符号位显示
		 LCD_ShowString(Show_GramA,254,220,320,20,16,0,(uint8_t *)ui_show1[DAC.Freq.Signe+12+7+3]); 
		 LCD_ShowString(Show_GramB,254,220,320,20,16,0,(uint8_t *)ui_show1[DAC.Freq.Signe+12+7+3]); 
		break;
		case DAC_Vpp_TV://DAC波形峰峰值十V显示
		 LCD_ShowNum(Show_GramA,270,220, DAC.Vpp.TV,1,16,0); 
		 LCD_ShowNum(Show_GramB,270,220, DAC.Vpp.TV,1,16,0);
		break;
		case DAC_Vpp_UV://DAC波形峰峰值1V显示
		 LCD_ShowNum(Show_GramA,278,220,DAC.Vpp.UV,1,16,0); 
		 LCD_ShowNum(Show_GramB,278,220,DAC.Vpp.UV,1,16,0);
		break;
		case DAC_Vpp_HmV://DAC波形峰峰值百mV显示
		 LCD_ShowNum(Show_GramA,294,220,DAC.Vpp.HmV,1,16,0); 
		 LCD_ShowNum(Show_GramB,294,220,DAC.Vpp.HmV,1,16,0);
		break;
		case DAC_Vpp_TmV://DAC波形峰峰值十mV显示
		 LCD_ShowNum(Show_GramA,304,220,DAC.Vpp.TmV,1,16,0); 
		 LCD_ShowNum(Show_GramB,304,220,DAC.Vpp.TmV,1,16,0);
		break;
	}
	BACK_COLOR = BLACK;
}

//刷新频率
void Lcd_Show_Freq(uint16_t *dat)
{
	LCD_ShowString(dat,150,201,320,20,16,0,(uint8_t *)"      ");
	LCD_ShowNum(dat,150,201,OSC.waveform1.Freq,6,16,0);
	LCD_ShowString(dat,48,201,320,20,16,0,(uint8_t *)"      ");
	LCD_ShowNum(dat,48,201,OSC.waveform2.Freq,6,16,0);
	OSC.waveform1.Freq = 0;
	OSC.waveform2.Freq = 0;
}

//刷新峰峰值
void Lcd_Show_Vpp(uint16_t *dat)
{
	LCD_ShowString(dat,48,220,320,20,16,0,(uint8_t *)"     "); 
	LCD_ShowNum(dat,48,220,OSC.waveform1.Vpp,5,16,0);
	LCD_ShowString(dat,142,220,320,20,16,0,(uint8_t *)"     ");
	LCD_ShowNum(dat,142,220,OSC.waveform2.Vpp,5,16,0);
}

//刷新触发波形选择
void Lcd_Show_Trigger(uint16_t *dat)
{
	(Trigger_Chx == 1) ?	(
		BACK_COLOR = LIGHTGREEN ) :(
		BACK_COLOR = GREEN );
		LCD_ShowNum(dat,280,0,Trigger_Chx,1,16,0);
		BACK_COLOR = BLACK;
}

//DAC开启关闭标志位刷新
void Lcd_Show_DAC(uint16_t *dat)
{
	//D
	(DAC_switch != 0) ?	(
		BACK_COLOR = DARKBLUE ) :(
		BACK_COLOR = BLACK );
		LCD_ShowString(dat,300,0,320,20,16,0,(uint8_t *)"D"); 
		BACK_COLOR = BLACK;
	//频率
	 LCD_ShowNum(dat,230,220,DAC.Freq.HCF,1,16,0); 
	 LCD_ShowNum(dat,238,220,DAC.Freq.TCF,1,16,0);
	 LCD_ShowNum(dat,246,220,DAC.Freq.UCF,1,16,0); 
	 LCD_ShowString(dat,254,220,320,20,16,0,(uint8_t *)ui_show1[DAC.Freq.Signe+12+7+3]); 
	//峰峰值
	 LCD_ShowNum(dat,270,220, DAC.Vpp.TV,1,16,0); 
	 LCD_ShowNum(dat,278,220,DAC.Vpp.UV,1,16,0); 
	 LCD_ShowNum(dat,294,220,DAC.Vpp.HmV,1,16,0); 
	 LCD_ShowNum(dat,304,220,DAC.Vpp.TmV,1,16,0);

}




void LCD_Draw_grid(uint16_t *dat)
{
	uint16_t x,y;
		uint32_t i =0;
    //绘制简单线条
		for(y=0;y<21;y++)
		{
			for(x=0;x<320;x++) i++;
		}
    for(y=21;y<200;y++)
    {
        if((y-20)%36 == 0)
        {
            for(x=0;x<320;x++) dat[i++] = GRAY;
        }
        else
        {
            for(x=0;x<320;x++)
            {
                if(x%32 == 0) dat[i++] = GRAY;
                else dat[i++] = BLACK;
            }
        }
    }
		//画个线
		Lcd_Clear(dat,0,20,319,20,WHITE);
		Lcd_Clear(dat,0,200,319,200,WHITE);
}

	
/******************************************************************************
      函数说明：更改数组中的颜色使其显示相应的波形，数据
      入口数据：*dat需要修改的数组
								void * Show_Lin 修改后的波形值
      返回值：  无
******************************************************************************/
//简单显示
void Lcd_Show_LinA(uint16_t *dat,uint16_t*Show_Lin)
{
		static uint8_t i =0;
		if(i == 0)
		{
			//固定数据刷新之刷新一次
			Lcd_Show_fixedData();
			i=1;
		}
		LCD_Draw_grid(dat);//绘制网格
		Lcd_Show_Freq(dat);//刷新频率
		Lcd_Show_Vpp(dat);//刷新峰峰值
		Lcd_Show_Trigger(dat);//触发选中刷新
		Lcd_Show_DAC(dat);//DAC开关
		if(Show_Lin[15] != NULL)
		{
				//存放波形
			for(uint8_t x=0;x<158;x++)
			{
					Lcd_Show_Lin(dat,0+(x*2),Show_Lin[0+(x*2)],Show_Lin[0+((x+1)*2)],LIGHTGREEN );
					Lcd_Show_Lin(dat,1+(x*2),Show_Lin[1+(x*2)],Show_Lin[1+((x+1)*2)],GREEN );
			}
			//存放频谱
			for(uint16_t x=0;x<255;x++)
			{
					Lcd_Show_Lin(dat,x,Show_Lin[320+x],Show_Lin[320+x+1],RED );
			}
		}
}




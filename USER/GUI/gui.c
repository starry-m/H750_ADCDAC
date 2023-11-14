//LCDˢ������Ͳ�������

#include "GUI/gui.h"
#include "main.h"
#include "LCD/lcd_dma.h"
#include "Key/KeyPad.h"
#include "LCD/refresh.h"
#include "DAC/dac_cs.h"


///������ʾ�ַ�����λ��  
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

///������ʾ�ַ�����λ��  
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
//�������ƣ�Lcd_Show_fixedData
//�������أ���
//����˵������
//���ܸ�Ҫ��Lcdˢ�¹̶�������
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
			case 3://��ʾƵ��1
			 LCD_ShowString(Show_GramA,0,201,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			 LCD_ShowString(Show_GramB,0,201,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			break;
			case 4://��ʾƵ��2
			 LCD_ShowString(Show_GramA,100,201,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			 LCD_ShowString(Show_GramB,100,201,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			break;
			 case 5://��ʾ���ֵ1
			 LCD_ShowString(Show_GramA,0,220,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			 LCD_ShowString(Show_GramB,0,220,320,20,16,0,(uint8_t *)ui_fixed[i]); 
			break;
			case 6://��ʾ���ֵ2
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
			case 16://��ʾDACĬ����ʾ���Ҳ�
			 LCD_ShowString(Show_GramA,200,220,320,20,16,0,(uint8_t *)ui_show1[DAC.DAC_wave_form+12+7]); 
			 LCD_ShowString(Show_GramB,200,220,320,20,16,0,(uint8_t *)ui_show1[DAC.DAC_wave_form+12+7]);
			break;
			case 17://DAC����Ƶ�ʰ�λ��ʾ
			 LCD_ShowNum(Show_GramA,230,220,DAC.Freq.HCF,1,16,0); 
			 LCD_ShowNum(Show_GramB,230,220,DAC.Freq.HCF,1,16,0);
			break;
			case 18://DAC����Ƶ��ʮλ��ʾ
			 LCD_ShowNum(Show_GramA,238,220,DAC.Freq.TCF,1,16,0); 
			 LCD_ShowNum(Show_GramB,238,220,DAC.Freq.TCF,1,16,0);
			break;
			case 19://DAC����Ƶ�ʸ�λ��ʾ
			 LCD_ShowNum(Show_GramA,246,220,DAC.Freq.UCF,1,16,0); 
			 LCD_ShowNum(Show_GramB,246,220,DAC.Freq.UCF,1,16,0);
			break;
			case 20://DAC����Ƶ�ʷ���λ��ʾ
			 LCD_ShowString(Show_GramA,254,220,320,20,16,0,(uint8_t *)ui_show1[DAC.Freq.Signe+12+7+3]); 
			 LCD_ShowString(Show_GramB,254,220,320,20,16,0,(uint8_t *)ui_show1[DAC.Freq.Signe+12+7+3]); 
			break;
			case 21://DAC���η��ֵʮV��ʾ
			 LCD_ShowNum(Show_GramA,270,220, DAC.Vpp.TV,1,16,0); 
			 LCD_ShowNum(Show_GramB,270,220, DAC.Vpp.TV,1,16,0);
			break;
			case 22://DAC���η��ֵ1V��ʾ
			 LCD_ShowNum(Show_GramA,278,220,DAC.Vpp.UV,1,16,0); 
			 LCD_ShowNum(Show_GramB,278,220,DAC.Vpp.UV,1,16,0);
			break;
			case 23://DAC���η��ֵ��mV��ʾ
			 LCD_ShowNum(Show_GramA,294,220,DAC.Vpp.HmV,1,16,0); 
			 LCD_ShowNum(Show_GramB,294,220,DAC.Vpp.HmV,1,16,0);
			break;
			case 24://DAC���η��ֵʮmV��ʾ
			 LCD_ShowNum(Show_GramA,304,220,DAC.Vpp.TmV,1,16,0); 
			 LCD_ShowNum(Show_GramB,304,220,DAC.Vpp.TmV,1,16,0);
			break;
		 default : /* ��ѡ�� */
			 break;
		}
		BACK_COLOR = BLACK;
	}
	
}

//=============================
//�������ƣ�Lcd_Show_Data
//�������أ���
//����˵����Encoder_flag����־λ��������ʾEncoder��1��bspEncoder��0��prebspEncoder
//���ܸ�Ҫ��ʵʱ����ˢ�µĸ���
//=========================
void Lcd_Show_Data(uint8_t Encoder_flag)
{
	uint8_t bspEncoder_flag = 0;
	if(Encoder_flag == 1)//bspEncoder����ʱΪ������ѡ�м�Ϊ����
	{
		BACK_COLOR = DARKBLUE;
		bspEncoder_flag = bspEncoder;
	}else//�������prebspEncoder���ص�����
	{
		BACK_COLOR = BLACK;
		bspEncoder_flag = prebspEncoder;
	}
	switch(bspEncoder_flag)
	{
		case Vertical_1://ˢ�µ�ѹ1��ֵ
			LCD_ShowString(Show_GramA,16,0,320,20,16,0,(uint8_t *)ui_show1[Vpp_num1]); 
			LCD_ShowString(Show_GramB,16,0,320,20,16,0,(uint8_t *)ui_show1[Vpp_num1]);    
		break;
		case Vertical_2://ˢ�µ�ѹ2��ֵ
			LCD_ShowString(Show_GramA,126,0,320,20,16,0,(uint8_t *)ui_show1[Vpp_num2+6]); 
			LCD_ShowString(Show_GramB,126,0,320,20,16,0,(uint8_t *)ui_show1[Vpp_num2+6]); 
		break;
		case SampRate://ˢ�²�����
		 LCD_ShowString(Show_GramA,240,201,320,20,16,0,(uint8_t *)ui_show1[SampRate_num+12]); 
		 LCD_ShowString(Show_GramB,240,201,320,20,16,0,(uint8_t *)ui_show1[SampRate_num+12]);   
		break;   
		case DAC_Trigger://ˢ�´���λ��
		 LCD_ShowNum(Show_GramA,230,0,(DAC_Trigger_num+28)*50,4,16,0);
		 LCD_ShowNum(Show_GramB,230,0,(DAC_Trigger_num+28)*50,4,16,0);
		break;	
		case DAC_Wave://��ʾDACĬ����ʾ���Ҳ�
		 LCD_ShowString(Show_GramA,200,220,320,20,16,0,(uint8_t *)ui_show1[DAC.DAC_wave_form+12+7]); 
		 LCD_ShowString(Show_GramB,200,220,320,20,16,0,(uint8_t *)ui_show1[DAC.DAC_wave_form+12+7]);
		break;
		case DAC_frq_HCF://DAC����Ƶ�ʰ�λ��ʾ
		 LCD_ShowNum(Show_GramA,230,220,DAC.Freq.HCF,1,16,0); 
		 LCD_ShowNum(Show_GramB,230,220,DAC.Freq.HCF,1,16,0);
		break;
		case DAC_frq_TCF://DAC����Ƶ��ʮλ��ʾ
		 LCD_ShowNum(Show_GramA,238,220,DAC.Freq.TCF,1,16,0); 
		 LCD_ShowNum(Show_GramB,238,220,DAC.Freq.TCF,1,16,0);
		break;
		case DAC_frq_UCF://DAC����Ƶ�ʸ�λ��ʾ
		 LCD_ShowNum(Show_GramA,246,220,DAC.Freq.UCF,1,16,0); 
		 LCD_ShowNum(Show_GramB,246,220,DAC.Freq.UCF,1,16,0);
		break;
		case DAC_signe://DAC����Ƶ�ʷ���λ��ʾ
		 LCD_ShowString(Show_GramA,254,220,320,20,16,0,(uint8_t *)ui_show1[DAC.Freq.Signe+12+7+3]); 
		 LCD_ShowString(Show_GramB,254,220,320,20,16,0,(uint8_t *)ui_show1[DAC.Freq.Signe+12+7+3]); 
		break;
		case DAC_Vpp_TV://DAC���η��ֵʮV��ʾ
		 LCD_ShowNum(Show_GramA,270,220, DAC.Vpp.TV,1,16,0); 
		 LCD_ShowNum(Show_GramB,270,220, DAC.Vpp.TV,1,16,0);
		break;
		case DAC_Vpp_UV://DAC���η��ֵ1V��ʾ
		 LCD_ShowNum(Show_GramA,278,220,DAC.Vpp.UV,1,16,0); 
		 LCD_ShowNum(Show_GramB,278,220,DAC.Vpp.UV,1,16,0);
		break;
		case DAC_Vpp_HmV://DAC���η��ֵ��mV��ʾ
		 LCD_ShowNum(Show_GramA,294,220,DAC.Vpp.HmV,1,16,0); 
		 LCD_ShowNum(Show_GramB,294,220,DAC.Vpp.HmV,1,16,0);
		break;
		case DAC_Vpp_TmV://DAC���η��ֵʮmV��ʾ
		 LCD_ShowNum(Show_GramA,304,220,DAC.Vpp.TmV,1,16,0); 
		 LCD_ShowNum(Show_GramB,304,220,DAC.Vpp.TmV,1,16,0);
		break;
	}
	BACK_COLOR = BLACK;
}

//ˢ��Ƶ��
void Lcd_Show_Freq(uint16_t *dat)
{
	LCD_ShowString(dat,150,201,320,20,16,0,(uint8_t *)"      ");
	LCD_ShowNum(dat,150,201,OSC.waveform1.Freq,6,16,0);
	LCD_ShowString(dat,48,201,320,20,16,0,(uint8_t *)"      ");
	LCD_ShowNum(dat,48,201,OSC.waveform2.Freq,6,16,0);
	OSC.waveform1.Freq = 0;
	OSC.waveform2.Freq = 0;
}

//ˢ�·��ֵ
void Lcd_Show_Vpp(uint16_t *dat)
{
	LCD_ShowString(dat,48,220,320,20,16,0,(uint8_t *)"     "); 
	LCD_ShowNum(dat,48,220,OSC.waveform1.Vpp,5,16,0);
	LCD_ShowString(dat,142,220,320,20,16,0,(uint8_t *)"     ");
	LCD_ShowNum(dat,142,220,OSC.waveform2.Vpp,5,16,0);
}

//ˢ�´�������ѡ��
void Lcd_Show_Trigger(uint16_t *dat)
{
	(Trigger_Chx == 1) ?	(
		BACK_COLOR = LIGHTGREEN ) :(
		BACK_COLOR = GREEN );
		LCD_ShowNum(dat,280,0,Trigger_Chx,1,16,0);
		BACK_COLOR = BLACK;
}

//DAC�����رձ�־λˢ��
void Lcd_Show_DAC(uint16_t *dat)
{
	//D
	(DAC_switch != 0) ?	(
		BACK_COLOR = DARKBLUE ) :(
		BACK_COLOR = BLACK );
		LCD_ShowString(dat,300,0,320,20,16,0,(uint8_t *)"D"); 
		BACK_COLOR = BLACK;
	//Ƶ��
	 LCD_ShowNum(dat,230,220,DAC.Freq.HCF,1,16,0); 
	 LCD_ShowNum(dat,238,220,DAC.Freq.TCF,1,16,0);
	 LCD_ShowNum(dat,246,220,DAC.Freq.UCF,1,16,0); 
	 LCD_ShowString(dat,254,220,320,20,16,0,(uint8_t *)ui_show1[DAC.Freq.Signe+12+7+3]); 
	//���ֵ
	 LCD_ShowNum(dat,270,220, DAC.Vpp.TV,1,16,0); 
	 LCD_ShowNum(dat,278,220,DAC.Vpp.UV,1,16,0); 
	 LCD_ShowNum(dat,294,220,DAC.Vpp.HmV,1,16,0); 
	 LCD_ShowNum(dat,304,220,DAC.Vpp.TmV,1,16,0);

}




void LCD_Draw_grid(uint16_t *dat)
{
	uint16_t x,y;
		uint32_t i =0;
    //���Ƽ�����
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
		//������
		Lcd_Clear(dat,0,20,319,20,WHITE);
		Lcd_Clear(dat,0,200,319,200,WHITE);
}

	
/******************************************************************************
      ����˵�������������е���ɫʹ����ʾ��Ӧ�Ĳ��Σ�����
      ������ݣ�*dat��Ҫ�޸ĵ�����
								void * Show_Lin �޸ĺ�Ĳ���ֵ
      ����ֵ��  ��
******************************************************************************/
//����ʾ
void Lcd_Show_LinA(uint16_t *dat,uint16_t*Show_Lin)
{
		static uint8_t i =0;
		if(i == 0)
		{
			//�̶�����ˢ��֮ˢ��һ��
			Lcd_Show_fixedData();
			i=1;
		}
		LCD_Draw_grid(dat);//��������
		Lcd_Show_Freq(dat);//ˢ��Ƶ��
		Lcd_Show_Vpp(dat);//ˢ�·��ֵ
		Lcd_Show_Trigger(dat);//����ѡ��ˢ��
		Lcd_Show_DAC(dat);//DAC����
		if(Show_Lin[15] != NULL)
		{
				//��Ų���
			for(uint8_t x=0;x<158;x++)
			{
					Lcd_Show_Lin(dat,0+(x*2),Show_Lin[0+(x*2)],Show_Lin[0+((x+1)*2)],LIGHTGREEN );
					Lcd_Show_Lin(dat,1+(x*2),Show_Lin[1+(x*2)],Show_Lin[1+((x+1)*2)],GREEN );
			}
			//���Ƶ��
			for(uint16_t x=0;x<255;x++)
			{
					Lcd_Show_Lin(dat,x,Show_Lin[320+x],Show_Lin[320+x+1],RED );
			}
		}
}




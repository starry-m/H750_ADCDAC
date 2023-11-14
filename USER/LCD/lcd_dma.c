#include "LCD/lcd_dma.h"
#include "LCD/lcdfont_dma.h"
#include "LCD/lcd_init.h"

//LCD�Ļ�����ɫ�ͱ���ɫ	   
uint16_t  POINT_COLOR=WHITE;	//������ɫ
uint16_t  BACK_COLOR=BLACK;  //����ɫ 


/******************************************************************************
      ����˵������ָ�����������ɫ
      ������ݣ�dat ��Ҫ�ı������
								xsta,ysta   ��ʼ����
                xend,yend   ��ֹ����
								color       Ҫ������ɫ
      ����ֵ��  ��
******************************************************************************/
void Lcd_Clear(uint16_t *dat ,uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{
	uint32_t i=0;
	uint32_t Start = 0,End = 0;
	Start = ysta*LCD_W+xsta;
	End = yend*LCD_W+xend;
    //���ȫ��������
    for(i=Start;i<End;i++)
    {
        dat[i]  = color;
    }	
}

/******************************************************************************
      ����˵������������ȫ������Ϊһ����ɫ
      ������ݣ�dat ��Ҫ�ı������
								color       Ҫ������ɫ
      ����ֵ��  ��
******************************************************************************/
void Lcd_Gram_Fill(uint16_t *dat,uint16_t color)
{
    uint32_t i;
    uint16_t *buf=dat; 
    for(i=0;i<76800;i++) *buf++=color;
}

//m^n����
//����ֵ:m^n�η�.
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}


/******************************************************************************
      ����˵��������
			������ݣ�*dat ��Ҫ���ߵ�����
                x    x���λ��
                y    y���λ��
                color  ��ɫ    
      ����ֵ��  ��
******************************************************************************/
void LTDC_Draw_Point(uint16_t *dat, uint16_t x, uint16_t y, uint32_t color)
{
  *(dat + (LCD_W * (y) + x)) = color;
}

/******************************************************************************
      ����˵���������߶�
			������ݣ�*dat ��Ҫ���ߵ�����
                x    �ڼ����߶�
                pointa  ��a
								pointb  ��b
                color  ��ɫ    
      ����ֵ��  ��
******************************************************************************/
void Lcd_Show_Lin(uint16_t *dat,uint16_t x,uint8_t pointa,uint8_t pointb,uint16_t color)
{
    uint8_t i;
    if(pointa >= LCD_H) pointa=LCD_H-1;
    if(pointb >= LCD_H) pointb=LCD_H-1;
    
    if(pointa<pointb)
    {
        for(i=pointa;i<pointb;i++) *(dat+(239-i)*LCD_W+x) = color;
    }
    else if(pointa>pointb)
    {
        for(i=pointb;i<pointa;i++) *(dat+(239-i)*LCD_W+x) = color;
    }
    else *(dat+(239-pointa)*LCD_W+x) = color;
}

/******************************************************************************
      ����˵������ʾ����,��λΪ0,����ʾ
			������ݣ�*dat ��Ҫ���ߵ�����
                x    x���λ��
								y    y���λ��
								num  ��ֵ
								len  ����
								size  �ߴ� 12 16 24
                mode  ģʽ ��һ��ѡ0
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowNum(uint16_t *dat,uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size,uint8_t mode)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                LCD_ShowChar(dat, x + (size / 2)*t, y, ' ', size, mode);
                continue;
            }
            else enshow = 1;
        }
        LCD_ShowChar(dat, x + (size / 2)*t, y, temp + '0', size, mode);
    }
}


/******************************************************************************
      ����˵������ָ��λ����ʾһ���ַ�
			������ݣ�*dat ��Ҫ���ߵ�����
                x    x���λ��
								y    y���λ��
								num  ָ��ָ���8λ�ַ�
								size  �ߴ� 12 16 24
                mode  ģʽ ��һ��ѡ0
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChar(uint16_t *dat,uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{  
    uint8_t temp,t1,t;
    uint16_t y0=y;
    uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
    num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
    for(t=0;t<csize;t++)
    {   
        if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
        else if(size==16)temp=asc2_1608[num][t];	//����1608����
        else if(size==24)temp=asc2_2412[num][t];	//����2412����
        else return;								//û�е��ֿ�
        for(t1=0;t1<8;t1++)
        {			    
            if(temp&0x80)LTDC_Draw_Point(dat,x,y,POINT_COLOR);
            else if(mode==0)LTDC_Draw_Point(dat,x,y,BACK_COLOR);
            temp<<=1;
            y++;
            if(y>=LCD_H)return;		//��������
            if((y-y0)==size)
            {
                y=y0;
                x++;
                if(x>=LCD_W)return;	//��������
                break;
            }
        }  	 
    }  
} 


/******************************************************************************
      ����˵������ʾ�ַ���
			������ݣ�*dat ��Ҫ���ߵ�����
                x    x���λ��
								y    y���λ��
								width  һ�еĿ��
								height  һ�еĸ߶�
								size  �ߴ� 12 16 24
                mode  ģʽ ��һ��ѡ0
								*p �ַ�������
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowString(uint16_t *dat,uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t mode,uint8_t *p)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(dat,x,y,*p,size,mode);
        x+=size/2;
        p++;
    }  
}


/******************************************************************************
      ����˵�������ƺڰ�ȡģͼƬ
			������ݣ�*dat ��Ҫ���ߵ�����
								*bmp ת�����bmp����
                x    x���λ��
								y    y���λ��
								w  		��Ƭ�Ŀ��
								h    ��Ƭ�ĸ߶�
      ����ֵ��  ��
******************************************************************************/
void Lcd_Show_Bmp(uint16_t *dat,uint8_t *bmp,uint16_t x,uint16_t y,uint16_t w,uint16_t h)
{
    uint16_t *buf;
    uint16_t a,b;
    uint8_t i,temp;
    
    for(a=y;a<y+h;a++)
    {
        buf = dat + a*320 + x;
        for(b=x;b<x+w;b+=8)
        {
            temp = *bmp++;
            if(b+8 > x+w)
            {
                for(i=0;i<(x+w-b);i++)
                {
                    if(temp & 0x80) *buf++ = GRAY;
                    else *buf++ = BLACK;
                    temp = temp<<1;
                }
            }
            else
            {
                for(i=0;i<8;i++)
                {
                    if(temp & 0x80) *buf++ = GRAY;
                    else *buf++ = BLACK;
                    temp = temp<<1;
                }
            }
        }
    }
}



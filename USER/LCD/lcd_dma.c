#include "LCD/lcd_dma.h"
#include "LCD/lcdfont_dma.h"
#include "LCD/lcd_init.h"

//LCD的画笔颜色和背景色	   
uint16_t  POINT_COLOR=WHITE;	//画笔颜色
uint16_t  BACK_COLOR=BLACK;  //背景色 


/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：dat 需要改变的数组
								xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void Lcd_Clear(uint16_t *dat ,uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{
	uint32_t i=0;
	uint32_t Start = 0,End = 0;
	Start = ysta*LCD_W+xsta;
	End = yend*LCD_W+xend;
    //清除全部无线条
    for(i=Start;i<End;i++)
    {
        dat[i]  = color;
    }	
}

/******************************************************************************
      函数说明：整个数组全部整理为一个颜色
      入口数据：dat 需要改变的数组
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void Lcd_Gram_Fill(uint16_t *dat,uint16_t color)
{
    uint32_t i;
    uint16_t *buf=dat; 
    for(i=0;i<76800;i++) *buf++=color;
}

//m^n函数
//返回值:m^n次方.
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}


/******************************************************************************
      函数说明：画点
			入口数据：*dat 需要画线的数组
                x    x轴的位置
                y    y轴的位置
                color  颜色    
      返回值：  无
******************************************************************************/
void LTDC_Draw_Point(uint16_t *dat, uint16_t x, uint16_t y, uint32_t color)
{
  *(dat + (LCD_W * (y) + x)) = color;
}

/******************************************************************************
      函数说明：绘制线段
			入口数据：*dat 需要画线的数组
                x    第几个线段
                pointa  点a
								pointb  点b
                color  颜色    
      返回值：  无
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
      函数说明：显示数字,高位为0,则不显示
			入口数据：*dat 需要画线的数组
                x    x轴的位置
								y    y轴的位置
								num  数值
								len  长度
								size  尺寸 12 16 24
                mode  模式 ：一般选0
      返回值：  无
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
      函数说明：在指定位置显示一个字符
			入口数据：*dat 需要画线的数组
                x    x轴的位置
								y    y轴的位置
								num  指针指向的8位字符
								size  尺寸 12 16 24
                mode  模式 ：一般选0
      返回值：  无
******************************************************************************/
void LCD_ShowChar(uint16_t *dat,uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{  
    uint8_t temp,t1,t;
    uint16_t y0=y;
    uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
    num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
    for(t=0;t<csize;t++)
    {   
        if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
        else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
        else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
        else return;								//没有的字库
        for(t1=0;t1<8;t1++)
        {			    
            if(temp&0x80)LTDC_Draw_Point(dat,x,y,POINT_COLOR);
            else if(mode==0)LTDC_Draw_Point(dat,x,y,BACK_COLOR);
            temp<<=1;
            y++;
            if(y>=LCD_H)return;		//超区域了
            if((y-y0)==size)
            {
                y=y0;
                x++;
                if(x>=LCD_W)return;	//超区域了
                break;
            }
        }  	 
    }  
} 


/******************************************************************************
      函数说明：显示字符串
			入口数据：*dat 需要画线的数组
                x    x轴的位置
								y    y轴的位置
								width  一行的宽度
								height  一行的高度
								size  尺寸 12 16 24
                mode  模式 ：一般选0
								*p 字符串数据
      返回值：  无
******************************************************************************/
void LCD_ShowString(uint16_t *dat,uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t mode,uint8_t *p)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(dat,x,y,*p,size,mode);
        x+=size/2;
        p++;
    }  
}


/******************************************************************************
      函数说明：绘制黑白取模图片
			入口数据：*dat 需要画线的数组
								*bmp 转换后的bmp数组
                x    x轴的位置
								y    y轴的位置
								w  		照片的宽度
								h    照片的高度
      返回值：  无
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



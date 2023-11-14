/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "fatfs.h"
#include "lptim.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD/refresh.h"
#include "DELAY/delay.h"
#include "ADC/DRP.h"
#include "GUI/gui.h"
#include "Key/KeyPad.h"
#include "string.h"
#include "FFT/fft.h"
#include "DAC/dac_cs.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
 OSC_ OSC; //存放示波器的变量
 
uint16_t Show_Lin[640];

uint8_t LPTIM_COUNT =0,Pre_LPTIM_COUNT = 0xff;//低功耗的延时

uint8_t State =0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//低功耗回调函数
void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
	if(hlptim == &hlptim1)////低功耗定时器1，每20ms刷新一次
	{
			LPTIM_COUNT++;
	}
	if(hlptim == &hlptim2)////低功耗定时器2,超时标志位置一
	{
			Flag_overtime = 1;
	}
}
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
FATFS 	SD_FatFs; 		// 文件系统对象
FRESULT 	MyFile_Res;    // 操作结果 
//char SDPath[4];			// SD卡逻辑驱动路径

//	函数：FatFs_FileTest
//	功能：进行文件写入和读取测试
//
uint8_t  FatFs_FileTest(void)	//文件创建和写入测试
{
	uint8_t i = 0;
	uint16_t BufferSize = 0;	
	FIL	MyFile;			// 文件对象
	UINT 	MyFile_Num;		//	数据长度
	BYTE 	MyFile_WriteBuffer[] = "STM32 SD卡 文件系统测试 1234";	//要写入的数据
	BYTE 	MyFile_ReadBuffer[1024];	//要读出的数据
	
	printf("-------------FatFs 文件创建和写入测试---------------\r\n");
	
	MyFile_Res = f_open(&MyFile,"0:FatFs Test.txt",FA_CREATE_ALWAYS | FA_WRITE);	//打开文件，若不存在则创建该文件
	if(MyFile_Res == FR_OK)
	{
		printf("文件打开/创建成功，准备写入数据...\r\n");
		
		MyFile_Res = f_write(&MyFile,MyFile_WriteBuffer,sizeof(MyFile_WriteBuffer),&MyFile_Num);	//向文件写入数据
		if (MyFile_Res == FR_OK)	
		{
			printf("写入成功，写入内容为：\r\n");
			printf("%s\r\n",MyFile_WriteBuffer);
		}
		else
		{
			printf("文件写入失败，请检查SD卡或重新格式化!\r\n");
			f_close(&MyFile);	  //关闭文件	
			return ERROR;			
		}
		f_close(&MyFile);	  //关闭文件			
	}
	else
	{
		printf("无法打开/创建文件，请检查SD卡或重新格式化!\r\n");
		f_close(&MyFile);	  //关闭文件	
		return ERROR;		
	}
	
	printf("-------------FatFs 文件读取测试---------------\r\n");	
	
	BufferSize = sizeof(MyFile_WriteBuffer)/sizeof(BYTE);									// 计算写入的数据长度
	MyFile_Res = f_open(&MyFile,"0:FatFs Test.txt",FA_OPEN_EXISTING | FA_READ);	//打开文件，若不存在则创建该文件
	MyFile_Res = f_read(&MyFile,MyFile_ReadBuffer,BufferSize,&MyFile_Num);			// 读取文件
	if(MyFile_Res == FR_OK)
	{
		printf("文件读取成功，正在校验数据...\r\n");
		
		for(i=0;i<BufferSize;i++)
		{
			if(MyFile_WriteBuffer[i] != MyFile_ReadBuffer[i])		// 校验数据
			{
				printf("校验失败，请检查SD卡或重新格式化!\r\n");
				f_close(&MyFile);	  //关闭文件	
				return ERROR;
			}
		}
		printf("校验成功，读出的数据为：\r\n");
		printf("%s\r\n",MyFile_ReadBuffer);
	}	
	else
	{
		printf("无法读取文件，请检查SD卡或重新格式化!\r\n");
		f_close(&MyFile);	  //关闭文件	
		return ERROR;		
	}	
	
	f_close(&MyFile);	  //关闭文件	
	return SUCCESS;
}


	
//		FATFS *fs;		//定义结构体指针
//	uint32_t SD_CardCapacity = 0;		//SD卡的总容量
//	uint32_t SD_FreeCapacity = 0;		//SD卡空闲容量
//	DWORD fre_clust, fre_sect, tot_sect; 	//空闲簇，空闲扇区数，总扇区数

//	f_getfree("0:",&fre_clust,&fs);			//获取SD卡剩余的簇

//	tot_sect = (fs->n_fatent-2) * fs->csize;	//总扇区数量 = 总的簇 * 每个簇包含的扇区数
//	fre_sect = fre_clust * fs->csize;			//计算剩余的可用扇区数	   

//	SD_CardCapacity = tot_sect / 2048 ;	// SD卡总容量 = 总扇区数 * 512( 每扇区的字节数 ) / 1048576(换算成MB)
//	SD_FreeCapacity = fre_sect / 2048 ;	//计算剩余的容量，单位为M
//	printf("-------------------获取设备容量信息-----------------\r\n");		
//	printf("SD容量：%dMB\r\n",SD_CardCapacity);	
//	printf("SD剩余：%dMB\r\n",SD_FreeCapacity);
//	FatFs_FileTest();
//BMP头文件
#define u16 uint16_t
#define u32 uint32_t
#define u8 uint8_t

typedef __packed struct
{
    u16  bfType ;     //文件标志.只对'BM',用来识别BMP位图类型
    u32  bfSize ;	  //文件大小,占四个字节
    u16  bfReserved1 ;//保留
    u16  bfReserved2 ;//保留
    u32  bfOffBits ;  //从文件开始到位图数据(bitmap data)开始之间的的偏移量
}BITMAPFILEHEADER ;
//BMP信息头
typedef __packed struct
{
    u32 biSize ;		   		//说明BITMAPINFOHEADER结构所需要的字数。
    long  biWidth ;		   	//说明图象的宽度，以象素为单位 
    long  biHeight ;	   	//说明图象的高度，以象素为单位 
    u16  biPlanes ;	   		//为目标设备说明位面数，其值将总是被设为1 
    u16  biBitCount ;	   	//说明比特数/象素，其值为1、4、8、16、24、或32
/*说明图象数据压缩的类型。其值可以是下述值之一：
BI_RGB：没有压缩；
BI_RLE8：每个象素8比特的RLE压缩编码，压缩格式由2字节组成(重复象素计数和颜色索引)；  
BI_RLE4：每个象素4比特的RLE压缩编码，压缩格式由2字节组成
BI_BITFIELDS：每个象素的比特由指定的掩码决定。*/
    u32 biCompression ;  	
    u32 biSizeImage ;		//说明图象的大小，以字节为单位。当用BI_RGB格式时，可设置为0  
    long  biXPelsPerMeter ;	//说明水平分辨率，用象素/米表示
    long  biYPelsPerMeter ;	//说明垂直分辨率，用象素/米表示
    u32 biClrUsed ;	  	 	//说明位图实际使用的彩色表中的颜色索引数
    u32 biClrImportant ; 	//说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。 
}BITMAPINFOHEADER ;
//彩色表 
typedef __packed struct 
{
    u8 rgbBlue ;    //指定蓝色强度
    u8 rgbGreen ;	//指定绿色强度 
    u8 rgbRed ;	  	//指定红色强度 
    u8 rgbReserved ;//保留，设置为0 
}RGBQUAD ;
//整体信息头
typedef __packed struct
{ 
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;  
	RGBQUAD RGB_MASK[3];			//调色板用于存放RGB掩码.
}BITMAPINFO; 

BITMAPINFO bmp;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_LPTIM1_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_DAC1_Init();
  MX_TIM2_Init();
  MX_LPTIM2_Init();
  MX_TIM3_Init();
  MX_USART6_UART_Init();
  MX_TIM6_Init();
  MX_SPI4_Init();
  MX_USART1_UART_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
	//LCD开启
	delay_init(480);
	printf("sd卡测试\r\n");
	
	BYTE work[_MAX_SS]; 
	FATFS_LinkDriver(&SD_Driver, SDPath);		// 初始化驱动
	MyFile_Res = f_mount(&SD_FatFs,"0:",1);	//	挂载SD卡
	
	if (MyFile_Res == FR_OK)	//判断是否挂载成功
	{
		printf("\r\nSD文件系统挂载成功\r\n");
	}
	else
	{
	printf("\r\nSD 失败\r\n");
	}
	FIL	fnew;			// 文件对象
	uint16_t fnum;
	//打开文件，若不存在就创建
	MyFile_Res = f_open(&fnew, "0:fw234.bmp", FA_OPEN_ALWAYS | FA_WRITE );

	//文件打开成功
	if(MyFile_Res == FR_OK)
	{
		printf("\r\n开始写入BMP头信息\r\n");
		//填写文件信息头信息  
		bmp.bmfHeader.bfType = 0x4D42;			//bmp类型  "BM"
		bmp.bmfHeader.bfSize= 54 + 320*240*2;	//文件大小（信息结构体+像素数据）
		bmp.bmfHeader.bfReserved1 = 0x0000;		//保留，必须为0
		bmp.bmfHeader.bfReserved2 = 0x0000;  			
		bmp.bmfHeader.bfOffBits=54;				//位图信息结构体所占的字节数
		
		//填写位图信息头信息  
		bmp.bmiHeader.biSize=40;  			//位图信息头的大小
		bmp.bmiHeader.biWidth=320;  		//位图的宽度
		bmp.bmiHeader.biHeight=240;  		//图像的高度
		bmp.bmiHeader.biPlanes=1;  			//目标设别的级别，必须是1
		bmp.bmiHeader.biBitCount=16;        //每像素位数
		bmp.bmiHeader.biCompression=3;  	//RGB555格式
		bmp.bmiHeader.biSizeImage=320*240*2;//实际位图所占用的字节数（仅考虑位图像素数据）
		bmp.bmiHeader.biXPelsPerMeter=0;	//水平分辨率
		bmp.bmiHeader.biYPelsPerMeter=0; 	//垂直分辨率
		bmp.bmiHeader.biClrImportant=0;   	//说明图像显示有重要影响的颜色索引数目，0代表所有的颜色一样重要
		bmp.bmiHeader.biClrUsed=0;  		//位图实际使用的彩色表中的颜色索引数，0表示使用所有的调色板项
		
		//RGB565格式掩码
		bmp.RGB_MASK[0].rgbBlue = 0;
		bmp.RGB_MASK[0].rgbGreen = 0xF8;
		bmp.RGB_MASK[0].rgbRed = 0;
		bmp.RGB_MASK[0].rgbReserved = 0;
		
		bmp.RGB_MASK[1].rgbBlue = 0xE0;
		bmp.RGB_MASK[1].rgbGreen = 0x07;
		bmp.RGB_MASK[1].rgbRed = 0;
		bmp.RGB_MASK[1].rgbReserved = 0;
		
		bmp.RGB_MASK[2].rgbBlue = 0x1F;
		bmp.RGB_MASK[2].rgbGreen = 0;
		bmp.RGB_MASK[2].rgbRed = 0;
		bmp.RGB_MASK[2].rgbReserved = 0;
		
		//写文件头进文件  
		MyFile_Res= f_write(&fnew, &bmp, sizeof(bmp), &fnum);
		if( MyFile_Res != FR_OK )
		{
		printf("头部信息写入失败\r\n");
		}
		MyFile_Res = f_sync(&fnew);
				if( MyFile_Res != FR_OK )
		{
		printf("头部信息写入 同步失败  \r\n");
		}
	}
	printf("图像信息开始写入\r\n");
	uint16_t i,j;
	uint16_t color=0XFE11;
	for(i=0;i<240;i++)
{
	for(j=0;j<320;j++)
	{				
//		GPIOB->CRL=0X88888888;
//		OV7725_RCK=0;
//		color=OV7725_DATA;	//读数据--高8位
//		
//		OV7725_RCK=1; 
//		color<<=8;  
//		OV7725_RCK=0;
//		color|=OV7725_DATA;	//读数据	--低8位		(高低8+8位合并成一个u16发送)								
//		OV7725_RCK=1;
//		GPIOB->CRL=0X33333333;
		//LCD显示
//		LCD_WR_DATA(color);
		//写位图信息头进内存卡
		MyFile_Res=f_write(&fnew, &color, sizeof(color), &fnum);
		if( MyFile_Res != FR_OK )
		{
		printf("图像信息写入失败，%d,%d\r\n",i,j);
		}
		MyFile_Res = f_sync(&fnew);
		if( MyFile_Res != FR_OK )
		{
		printf("图像信息写入 同步失败  \r\n");
		}
	}
	printf("%d:行写完\r\n",i);
}

	printf("\r\n图片写入完成\r\n");
	while(1)
	{
	
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
		HAL_Delay(500);
	}
	LCD_Show_Start();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		//屏幕刷新
			if(Pre_LPTIM_COUNT!=LPTIM_COUNT)
			{
				LCD_Refresh(Show_Lin);
				Pre_LPTIM_COUNT = LPTIM_COUNT;
			}	
		//按键刷新
		if(LPTIM_COUNT%5 == 0 )//每5次刷新一次
		{
			All_Key_scan();
		}
			switch(State)
		{
			case 0://状态切换
				if(preSampRate_num != SampRate_num)//采样切换
				{
					Osc_Time_Switch(SampRate_num);
					preSampRate_num = SampRate_num;
				}
				if(preDAC_Trigger_num!=DAC_Trigger_num)//触发位置，默认1950mV
				{
					Trigger_Open((DAC_Trigger_num+28)*50);
					preDAC_Trigger_num = DAC_Trigger_num;
				}
				DAC_processing(DAC_switch,DAC_flag_Freq,DAC_flag_Vpp);//DAC处理
			State = 1;
			break;
			case 1://打开触发等待触发
				Flag_Trigger = 0;
				Flag_overtime =0;
				Trigger_on();//触发开
				HAL_LPTIM_Counter_Start_IT(&hlptim2,3120);//100ms左右
				State = 2;
			break;
			case 2:
				if(Flag_Trigger == 1)//产生触发
				{
					HAL_LPTIM_Counter_Stop_IT(&hlptim2);//关闭超时
					State = 4;//跳转等待ADC接收完成
				}
				if(Flag_overtime == 1 && Flag_Trigger == 0)//超时且未触发，关闭比较器和超时定时器
				{
					HAL_LPTIM_Counter_Stop_IT(&hlptim2);//关闭超时
					Trigger_off();//触发关
					State = 3;//跳转获取ADC
				}
			break;
			case 3://获取ADC数据
				if(adc_dma_complete_flag == notStarted)
				{
					adc_dma_complete_flag = inProgress;
					HAL_ADCEx_MultiModeStart_DMA(&hadc1,adc_dma_data1,ADC_DATA_LENGTH);
					State = 4;//等待接收完成
				}
			break;
			case 4:
					if(adc_dma_complete_flag == completed)//flag等于1表示采集完成
				{
					adc_dma_complete_flag = notStarted;//数据处理完成后ADC标志位置零
					State =5;
				}
			break;
			case 5 ://ADC输出处理为相应的波形.		
					ADC_Data_Processing(Show_Lin,&OSC);
					ADC_FFT_Processing(adc_dma_data1,Show_Lin,Trigger_Chx);
					State = 0;
				break;
		}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_CSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.CSIState = RCC_CSI_ON;
  RCC_OscInitStruct.CSICalibrationValue = RCC_CSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_CKPER;
  PeriphClkInitStruct.PLL3.PLL3M = 25;
  PeriphClkInitStruct.PLL3.PLL3N = 200;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 2;
  PeriphClkInitStruct.PLL3.PLL3R = 10;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.CkperClockSelection = RCC_CLKPSOURCE_CSI;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

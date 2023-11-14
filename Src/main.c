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
 OSC_ OSC; //���ʾ�����ı���
 
uint16_t Show_Lin[640];

uint8_t LPTIM_COUNT =0,Pre_LPTIM_COUNT = 0xff;//�͹��ĵ���ʱ

uint8_t State =0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//�͹��Ļص�����
void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
	if(hlptim == &hlptim1)////�͹��Ķ�ʱ��1��ÿ20msˢ��һ��
	{
			LPTIM_COUNT++;
	}
	if(hlptim == &hlptim2)////�͹��Ķ�ʱ��2,��ʱ��־λ��һ
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
FATFS 	SD_FatFs; 		// �ļ�ϵͳ����
FRESULT 	MyFile_Res;    // ������� 
//char SDPath[4];			// SD���߼�����·��

//	������FatFs_FileTest
//	���ܣ������ļ�д��Ͷ�ȡ����
//
uint8_t  FatFs_FileTest(void)	//�ļ�������д�����
{
	uint8_t i = 0;
	uint16_t BufferSize = 0;	
	FIL	MyFile;			// �ļ�����
	UINT 	MyFile_Num;		//	���ݳ���
	BYTE 	MyFile_WriteBuffer[] = "STM32 SD�� �ļ�ϵͳ���� 1234";	//Ҫд�������
	BYTE 	MyFile_ReadBuffer[1024];	//Ҫ����������
	
	printf("-------------FatFs �ļ�������д�����---------------\r\n");
	
	MyFile_Res = f_open(&MyFile,"0:FatFs Test.txt",FA_CREATE_ALWAYS | FA_WRITE);	//���ļ������������򴴽����ļ�
	if(MyFile_Res == FR_OK)
	{
		printf("�ļ���/�����ɹ���׼��д������...\r\n");
		
		MyFile_Res = f_write(&MyFile,MyFile_WriteBuffer,sizeof(MyFile_WriteBuffer),&MyFile_Num);	//���ļ�д������
		if (MyFile_Res == FR_OK)	
		{
			printf("д��ɹ���д������Ϊ��\r\n");
			printf("%s\r\n",MyFile_WriteBuffer);
		}
		else
		{
			printf("�ļ�д��ʧ�ܣ�����SD�������¸�ʽ��!\r\n");
			f_close(&MyFile);	  //�ر��ļ�	
			return ERROR;			
		}
		f_close(&MyFile);	  //�ر��ļ�			
	}
	else
	{
		printf("�޷���/�����ļ�������SD�������¸�ʽ��!\r\n");
		f_close(&MyFile);	  //�ر��ļ�	
		return ERROR;		
	}
	
	printf("-------------FatFs �ļ���ȡ����---------------\r\n");	
	
	BufferSize = sizeof(MyFile_WriteBuffer)/sizeof(BYTE);									// ����д������ݳ���
	MyFile_Res = f_open(&MyFile,"0:FatFs Test.txt",FA_OPEN_EXISTING | FA_READ);	//���ļ������������򴴽����ļ�
	MyFile_Res = f_read(&MyFile,MyFile_ReadBuffer,BufferSize,&MyFile_Num);			// ��ȡ�ļ�
	if(MyFile_Res == FR_OK)
	{
		printf("�ļ���ȡ�ɹ�������У������...\r\n");
		
		for(i=0;i<BufferSize;i++)
		{
			if(MyFile_WriteBuffer[i] != MyFile_ReadBuffer[i])		// У������
			{
				printf("У��ʧ�ܣ�����SD�������¸�ʽ��!\r\n");
				f_close(&MyFile);	  //�ر��ļ�	
				return ERROR;
			}
		}
		printf("У��ɹ�������������Ϊ��\r\n");
		printf("%s\r\n",MyFile_ReadBuffer);
	}	
	else
	{
		printf("�޷���ȡ�ļ�������SD�������¸�ʽ��!\r\n");
		f_close(&MyFile);	  //�ر��ļ�	
		return ERROR;		
	}	
	
	f_close(&MyFile);	  //�ر��ļ�	
	return SUCCESS;
}


	
//		FATFS *fs;		//����ṹ��ָ��
//	uint32_t SD_CardCapacity = 0;		//SD����������
//	uint32_t SD_FreeCapacity = 0;		//SD����������
//	DWORD fre_clust, fre_sect, tot_sect; 	//���дأ���������������������

//	f_getfree("0:",&fre_clust,&fs);			//��ȡSD��ʣ��Ĵ�

//	tot_sect = (fs->n_fatent-2) * fs->csize;	//���������� = �ܵĴ� * ÿ���ذ�����������
//	fre_sect = fre_clust * fs->csize;			//����ʣ��Ŀ���������	   

//	SD_CardCapacity = tot_sect / 2048 ;	// SD�������� = �������� * 512( ÿ�������ֽ��� ) / 1048576(�����MB)
//	SD_FreeCapacity = fre_sect / 2048 ;	//����ʣ�����������λΪM
//	printf("-------------------��ȡ�豸������Ϣ-----------------\r\n");		
//	printf("SD������%dMB\r\n",SD_CardCapacity);	
//	printf("SDʣ�ࣺ%dMB\r\n",SD_FreeCapacity);
//	FatFs_FileTest();
//BMPͷ�ļ�
#define u16 uint16_t
#define u32 uint32_t
#define u8 uint8_t

typedef __packed struct
{
    u16  bfType ;     //�ļ���־.ֻ��'BM',����ʶ��BMPλͼ����
    u32  bfSize ;	  //�ļ���С,ռ�ĸ��ֽ�
    u16  bfReserved1 ;//����
    u16  bfReserved2 ;//����
    u32  bfOffBits ;  //���ļ���ʼ��λͼ����(bitmap data)��ʼ֮��ĵ�ƫ����
}BITMAPFILEHEADER ;
//BMP��Ϣͷ
typedef __packed struct
{
    u32 biSize ;		   		//˵��BITMAPINFOHEADER�ṹ����Ҫ��������
    long  biWidth ;		   	//˵��ͼ��Ŀ�ȣ�������Ϊ��λ 
    long  biHeight ;	   	//˵��ͼ��ĸ߶ȣ�������Ϊ��λ 
    u16  biPlanes ;	   		//ΪĿ���豸˵��λ��������ֵ�����Ǳ���Ϊ1 
    u16  biBitCount ;	   	//˵��������/���أ���ֵΪ1��4��8��16��24����32
/*˵��ͼ������ѹ�������͡���ֵ����������ֵ֮һ��
BI_RGB��û��ѹ����
BI_RLE8��ÿ������8���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����(�ظ����ؼ�������ɫ����)��  
BI_RLE4��ÿ������4���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����
BI_BITFIELDS��ÿ�����صı�����ָ�������������*/
    u32 biCompression ;  	
    u32 biSizeImage ;		//˵��ͼ��Ĵ�С�����ֽ�Ϊ��λ������BI_RGB��ʽʱ��������Ϊ0  
    long  biXPelsPerMeter ;	//˵��ˮƽ�ֱ��ʣ�������/�ױ�ʾ
    long  biYPelsPerMeter ;	//˵����ֱ�ֱ��ʣ�������/�ױ�ʾ
    u32 biClrUsed ;	  	 	//˵��λͼʵ��ʹ�õĲ�ɫ���е���ɫ������
    u32 biClrImportant ; 	//˵����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ�� 
}BITMAPINFOHEADER ;
//��ɫ�� 
typedef __packed struct 
{
    u8 rgbBlue ;    //ָ����ɫǿ��
    u8 rgbGreen ;	//ָ����ɫǿ�� 
    u8 rgbRed ;	  	//ָ����ɫǿ�� 
    u8 rgbReserved ;//����������Ϊ0 
}RGBQUAD ;
//������Ϣͷ
typedef __packed struct
{ 
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;  
	RGBQUAD RGB_MASK[3];			//��ɫ�����ڴ��RGB����.
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
	//LCD����
	delay_init(480);
	printf("sd������\r\n");
	
	BYTE work[_MAX_SS]; 
	FATFS_LinkDriver(&SD_Driver, SDPath);		// ��ʼ������
	MyFile_Res = f_mount(&SD_FatFs,"0:",1);	//	����SD��
	
	if (MyFile_Res == FR_OK)	//�ж��Ƿ���سɹ�
	{
		printf("\r\nSD�ļ�ϵͳ���سɹ�\r\n");
	}
	else
	{
	printf("\r\nSD ʧ��\r\n");
	}
	FIL	fnew;			// �ļ�����
	uint16_t fnum;
	//���ļ����������ھʹ���
	MyFile_Res = f_open(&fnew, "0:fw234.bmp", FA_OPEN_ALWAYS | FA_WRITE );

	//�ļ��򿪳ɹ�
	if(MyFile_Res == FR_OK)
	{
		printf("\r\n��ʼд��BMPͷ��Ϣ\r\n");
		//��д�ļ���Ϣͷ��Ϣ  
		bmp.bmfHeader.bfType = 0x4D42;			//bmp����  "BM"
		bmp.bmfHeader.bfSize= 54 + 320*240*2;	//�ļ���С����Ϣ�ṹ��+�������ݣ�
		bmp.bmfHeader.bfReserved1 = 0x0000;		//����������Ϊ0
		bmp.bmfHeader.bfReserved2 = 0x0000;  			
		bmp.bmfHeader.bfOffBits=54;				//λͼ��Ϣ�ṹ����ռ���ֽ���
		
		//��дλͼ��Ϣͷ��Ϣ  
		bmp.bmiHeader.biSize=40;  			//λͼ��Ϣͷ�Ĵ�С
		bmp.bmiHeader.biWidth=320;  		//λͼ�Ŀ��
		bmp.bmiHeader.biHeight=240;  		//ͼ��ĸ߶�
		bmp.bmiHeader.biPlanes=1;  			//Ŀ�����ļ��𣬱�����1
		bmp.bmiHeader.biBitCount=16;        //ÿ����λ��
		bmp.bmiHeader.biCompression=3;  	//RGB555��ʽ
		bmp.bmiHeader.biSizeImage=320*240*2;//ʵ��λͼ��ռ�õ��ֽ�����������λͼ�������ݣ�
		bmp.bmiHeader.biXPelsPerMeter=0;	//ˮƽ�ֱ���
		bmp.bmiHeader.biYPelsPerMeter=0; 	//��ֱ�ֱ���
		bmp.bmiHeader.biClrImportant=0;   	//˵��ͼ����ʾ����ҪӰ�����ɫ������Ŀ��0�������е���ɫһ����Ҫ
		bmp.bmiHeader.biClrUsed=0;  		//λͼʵ��ʹ�õĲ�ɫ���е���ɫ��������0��ʾʹ�����еĵ�ɫ����
		
		//RGB565��ʽ����
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
		
		//д�ļ�ͷ���ļ�  
		MyFile_Res= f_write(&fnew, &bmp, sizeof(bmp), &fnum);
		if( MyFile_Res != FR_OK )
		{
		printf("ͷ����Ϣд��ʧ��\r\n");
		}
		MyFile_Res = f_sync(&fnew);
				if( MyFile_Res != FR_OK )
		{
		printf("ͷ����Ϣд�� ͬ��ʧ��  \r\n");
		}
	}
	printf("ͼ����Ϣ��ʼд��\r\n");
	uint16_t i,j;
	uint16_t color=0XFE11;
	for(i=0;i<240;i++)
{
	for(j=0;j<320;j++)
	{				
//		GPIOB->CRL=0X88888888;
//		OV7725_RCK=0;
//		color=OV7725_DATA;	//������--��8λ
//		
//		OV7725_RCK=1; 
//		color<<=8;  
//		OV7725_RCK=0;
//		color|=OV7725_DATA;	//������	--��8λ		(�ߵ�8+8λ�ϲ���һ��u16����)								
//		OV7725_RCK=1;
//		GPIOB->CRL=0X33333333;
		//LCD��ʾ
//		LCD_WR_DATA(color);
		//дλͼ��Ϣͷ���ڴ濨
		MyFile_Res=f_write(&fnew, &color, sizeof(color), &fnum);
		if( MyFile_Res != FR_OK )
		{
		printf("ͼ����Ϣд��ʧ�ܣ�%d,%d\r\n",i,j);
		}
		MyFile_Res = f_sync(&fnew);
		if( MyFile_Res != FR_OK )
		{
		printf("ͼ����Ϣд�� ͬ��ʧ��  \r\n");
		}
	}
	printf("%d:��д��\r\n",i);
}

	printf("\r\nͼƬд�����\r\n");
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
		//��Ļˢ��
			if(Pre_LPTIM_COUNT!=LPTIM_COUNT)
			{
				LCD_Refresh(Show_Lin);
				Pre_LPTIM_COUNT = LPTIM_COUNT;
			}	
		//����ˢ��
		if(LPTIM_COUNT%5 == 0 )//ÿ5��ˢ��һ��
		{
			All_Key_scan();
		}
			switch(State)
		{
			case 0://״̬�л�
				if(preSampRate_num != SampRate_num)//�����л�
				{
					Osc_Time_Switch(SampRate_num);
					preSampRate_num = SampRate_num;
				}
				if(preDAC_Trigger_num!=DAC_Trigger_num)//����λ�ã�Ĭ��1950mV
				{
					Trigger_Open((DAC_Trigger_num+28)*50);
					preDAC_Trigger_num = DAC_Trigger_num;
				}
				DAC_processing(DAC_switch,DAC_flag_Freq,DAC_flag_Vpp);//DAC����
			State = 1;
			break;
			case 1://�򿪴����ȴ�����
				Flag_Trigger = 0;
				Flag_overtime =0;
				Trigger_on();//������
				HAL_LPTIM_Counter_Start_IT(&hlptim2,3120);//100ms����
				State = 2;
			break;
			case 2:
				if(Flag_Trigger == 1)//��������
				{
					HAL_LPTIM_Counter_Stop_IT(&hlptim2);//�رճ�ʱ
					State = 4;//��ת�ȴ�ADC�������
				}
				if(Flag_overtime == 1 && Flag_Trigger == 0)//��ʱ��δ�������رձȽ����ͳ�ʱ��ʱ��
				{
					HAL_LPTIM_Counter_Stop_IT(&hlptim2);//�رճ�ʱ
					Trigger_off();//������
					State = 3;//��ת��ȡADC
				}
			break;
			case 3://��ȡADC����
				if(adc_dma_complete_flag == notStarted)
				{
					adc_dma_complete_flag = inProgress;
					HAL_ADCEx_MultiModeStart_DMA(&hadc1,adc_dma_data1,ADC_DATA_LENGTH);
					State = 4;//�ȴ��������
				}
			break;
			case 4:
					if(adc_dma_complete_flag == completed)//flag����1��ʾ�ɼ����
				{
					adc_dma_complete_flag = notStarted;//���ݴ�����ɺ�ADC��־λ����
					State =5;
				}
			break;
			case 5 ://ADC�������Ϊ��Ӧ�Ĳ���.		
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

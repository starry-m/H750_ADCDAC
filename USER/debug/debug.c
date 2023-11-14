#include "debug/debug.h"
#include "stm32h7xx_hal.h"
#include "usart.h"


UART_HandleTypeDef *huart_debug = &huart1; ///< Debug����

static uint8_t device_er[2] = {'E','R'};//���ش���ER
uint8_t USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

void reply_er(void);

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
uint16_t USART_RX_STA=0;       //����״̬���	
uint8_t aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���



#if   defined( __GNUC__ )//GCC
	/*	With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
		set to 'Yes') calls __io_putchar() */
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#elif defined( __CC_ARM )//ARMCC
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#else
	#error "Unknown compiler"
#endif
static uint8_t s_usart_tmp;


#if defined(__CC_ARM)
int fputc(int ch,FILE *f)
{
	s_usart_tmp=(uint8_t)(ch);
	HAL_UART_Transmit(huart_debug,&s_usart_tmp,1,1);
//	HAL_UART_Transmit_IT(huart_debug,&s_usart_tmp,1);
	return ch;
}
#elif defined( __GNUC__ )
int __io_putchar(int ch)
{
	s_usart_tmp=(uint8_t)(ch);
	HAL_UART_Transmit(huart_debug,&s_usart_tmp,1,1);
//	HAL_UART_Transmit_IT(huart_debug,&s_usart_tmp,1);
	return ch;
}
#endif





void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	if(huart->Instance==USART1)//����Ǵ���6
	{
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else 
				{	
					USART_RX_STA|=0x8000;	//��������� 
//					printf("USART_RX_BUF[%d]:%s\r\n",USART_RX_STA,USART_RX_BUF);	
				}				
			}
			else //��û�յ�0X0D
			{	
				if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))
					{	
						USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	 
							reply_er();
					}
				}		 
			}
		}

	}
	
}

//����6�жϷ������
void USART6_IRQHandler(void)                	
{ 
	uint32_t timeout=0;
	uint32_t maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(huart_debug);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(huart_debug) != HAL_UART_STATE_READY)//�ȴ�����
	{
	 timeout++;////��ʱ����
     if(timeout>maxDelay) 
		{ 
			reply_er();
			break;	
		}			 
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(huart_debug, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>maxDelay) 
	 { 
		 reply_er();
		break;	
	 }
	}
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
} 

void reply_er(void)
{
	HAL_UART_Transmit(huart_debug,(uint8_t*)device_er,sizeof(device_er),10);	
	while(__HAL_UART_GET_FLAG(huart_debug,UART_FLAG_TC)!=SET);		
	USART_RX_STA=0;
}

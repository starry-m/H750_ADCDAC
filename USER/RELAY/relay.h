#ifndef __RELAY_H__
#define __RELAY_H__

#include <stdio.h>
#include "stm32h7xx.h"                  // Device header

#define Relay1_GPIOx       	GPIOE
#define Relay1_GPIO_Pin    	7

#define Relay2_GPIOx				GPIOE
#define Relay2_GPIO_Pin			8

#define Relay3_GPIOx				GPIOE
#define Relay3_GPIO_Pin			9

#define Relay1_On()       (Relay1_GPIOx->BSRR = 1<<Relay1_GPIO_Pin) 			 //GPIOD GPIO_PIN3   ÖÃÒ»
#define Relay1_Off()      (Relay1_GPIOx->BSRR = (1<<Relay1_GPIO_Pin)<<16) //GPIOD GPIO_PIN3  ÖÃÁã

#define Relay2_On()       (Relay2_GPIOx->BSRR = 1<<Relay2_GPIO_Pin )			 //GPIOB GPIO_PIN9   ÖÃÒ»
#define Relay2_Off()      (Relay2_GPIOx->BSRR = (1<<Relay2_GPIO_Pin)<<16) //GPIOB GPIO_PIN9   ÖÃÁã

#define Relay3_On()       (Relay3_GPIOx->BSRR = 1<<Relay3_GPIO_Pin) 			 //GPIOB GPIO_PIN8   ÖÃÒ»
#define Relay3_Off()      (Relay3_GPIOx->BSRR = (1<<Relay3_GPIO_Pin)<<16) //GPIOB GPIO_PIN8   ÖÃÁã



#endif


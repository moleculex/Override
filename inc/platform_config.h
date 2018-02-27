#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"



//RN4020
#define RN4020_RCC_AHB			RCC_AHBPeriph_GPIOA

#define RN4020_RCC_APB			RCC_APB1Periph_USART2
#define RN4020_PORT				GPIOA
#define RN4020_TX_PIN			GPIO_Pin_2
#define RN4020_RX_PIN			GPIO_Pin_3
#define RN4020_TX_PINSOURCE		GPIO_PinSource2
#define RN4020_RX_PINSOURCE		GPIO_PinSource3
#define RN4020_USART			USART2
#define RN4020_USART_IRQN		USART2_IRQn
#define rn4020_read 			USART2_IRQHandler

#define RN4020_WAKE_HW_RCC_AHB		RCC_AHBPeriph_GPIOA
#define RN4020_WAKE_HW_PORT			GPIOA
#define RN4020_WAKE_HW_PIN			GPIO_Pin_5

#define RN4020_WAKE_SW_RCC_AHB		RCC_AHBPeriph_GPIOB
#define RN4020_WAKE_SW_PORT			GPIOB
#define RN4020_WAKE_SW_PIN			GPIO_Pin_7

#endif

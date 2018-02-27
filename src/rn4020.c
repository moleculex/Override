#include "rn4020.h"
#include "platform_config.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void rn4020_init(void)
{
	_rn4020.buffer_ptr = &_rn4020.buffer[0];

	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RN4020_RCC_AHB | RN4020_WAKE_HW_RCC_AHB | RN4020_WAKE_SW_RCC_AHB, ENABLE);
	RCC_APB1PeriphClockCmd(RN4020_RCC_APB, ENABLE);

	//WAKE
	GPIO_InitStructure.GPIO_Pin = RN4020_WAKE_HW_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RN4020_WAKE_HW_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RN4020_WAKE_SW_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RN4020_WAKE_SW_PORT, &GPIO_InitStructure);

	/* Connect USART pins to AF */
	GPIO_PinAFConfig(RN4020_PORT, RN4020_TX_PINSOURCE, GPIO_AF_7);
	GPIO_PinAFConfig(RN4020_PORT, RN4020_RX_PINSOURCE, GPIO_AF_7);

	GPIO_InitStructure.GPIO_Pin = RN4020_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RN4020_PORT, &GPIO_InitStructure);

	/* Configure USART Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = RN4020_RX_PIN;
	GPIO_Init(RN4020_PORT, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(RN4020_USART,&USART_InitStructure);

	USART_ITConfig(RN4020_USART, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = RN4020_USART_IRQN;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(RN4020_USART, ENABLE);
}

void rn4020_publish(void)
{
	GPIO_SetBits(RN4020_WAKE_HW_PORT, RN4020_WAKE_HW_PIN);
	GPIO_SetBits(RN4020_WAKE_SW_PORT, RN4020_WAKE_SW_PIN);
	vTaskDelay(1000);

	_rn4020.buffer_ptr = &_rn4020.buffer[0];
	rn4020_write("SF,1\r\n", 6);
	vTaskDelay(200);

	_rn4020.buffer_ptr = &_rn4020.buffer[0];
	rn4020_write("SS,C0000000\r\n", 13);
	vTaskDelay(200);

	_rn4020.buffer_ptr = &_rn4020.buffer[0];
	rn4020_write("SR,32000000\r\n", 13);
	vTaskDelay(200);

	_rn4020.buffer_ptr = &_rn4020.buffer[0];
	rn4020_write("R,1\r\n", 5);
	vTaskDelay(1000);

	_rn4020.buffer_ptr = &_rn4020.buffer[0];
	/*_rn4020.buffer_ptr = &_rn4020.buffer[0];
	rn4020_write("A\r\n", 3);
	vTaskDelay(1000);*/

	/*_rn4020.buffer_ptr = &_rn4020.buffer[0];
	rn4020_write("LS\r\n", 3);
	vTaskDelay(1000);*/
}

void rn4020_write(char *buffer, char length)
{
	for(uint8_t i = 0; i < length; i++)
	{
		USART_SendData(RN4020_USART, buffer[i]);
		while (USART_GetFlagStatus(RN4020_USART, USART_FLAG_TXE) == RESET);
	}
}

void rn4020_read(void)
{
	if(USART_GetITStatus(RN4020_USART, USART_IT_RXNE) != RESET)
	{
		char inchar = (char)USART_ReceiveData(RN4020_USART);
		*_rn4020.buffer_ptr++ = inchar;

	}
}

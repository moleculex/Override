#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "rn4020.h"

void test(void);

int main(void)
{

	SysTick_Config(72000);

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	xTaskCreate( (pdTASK_CODE)test, (const signed char * const)"test", 128, NULL, 2, NULL );

	vTaskStartScheduler();

	while(1);
}

void test(void)
{
	char *index;

	rn4020_init();
	rn4020_publish();


	for(;;)
	{
		index = strstr(_rn4020.buffer, "open");
		if(index)
		{
			_rn4020.buffer_ptr = &_rn4020.buffer[0];
			memset(_rn4020.buffer, '\0', sizeof(_rn4020.buffer));
			GPIO_SetBits(GPIOA, GPIO_Pin_7);
			GPIO_SetBits(GPIOA, GPIO_Pin_6);
			vTaskDelay(5000);
			GPIO_ResetBits(GPIOA, GPIO_Pin_7);
			GPIO_ResetBits(GPIOA, GPIO_Pin_6);
			//vTaskDelay(1000);
		}
		vTaskDelay(100);
	}
}

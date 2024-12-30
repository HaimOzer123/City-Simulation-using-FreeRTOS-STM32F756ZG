/*
 * print.c
 *
 *  Created on: Apr 28, 2021
 *      Author: e_tas
 */
/* Private user code ---------------------------------------------------------*/
#include "cmsis_os.h"
#include "stm32f7xx_hal.h"

#include <string.h>

#include <stdio.h>
#include <stdlib.h>


/* USER CODE BEGIN 0 */
//for printf
extern UART_HandleTypeDef huart3;

int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}

int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart3, (uint8_t *)ptr, len, 0xFFFF);
	return len;
}

// for scanf
int _read(int file, char *ptr, int len)
{
	int ch=0;
	HAL_UART_Receive(&huart3, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1,HAL_MAX_DELAY);
	if(ch==13)
	{
		ch=10;
		HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1,HAL_MAX_DELAY);
	}
	else if(ch==8)
	{
		ch=0x30;
		HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1,HAL_MAX_DELAY);
	}

	*ptr=ch;

	return 1;
}

#include "echo.h"
#include <stm32f1xx_hal.h>
#include <string.h>
#include <stdint.h>

extern UART_HandleTypeDef huart1;

void echo_str(const char* str){
	HAL_UART_Transmit(&huart1, (const uint8_t*)str, strlen(str), 10000);
}

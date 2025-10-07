#include "led.h"
#include <stm32f1xx_hal.h>

void led_init(void){
	// 已通过 CubeMX 配置过了
	/*
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	*/
}

void led_on(void){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

void led_off(void){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

void led_toggle(void){
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

void led_flash(uint8_t repetition){
	for(uint8_t i=0;i<repetition*2;i++){
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		HAL_Delay(50);
	}
}


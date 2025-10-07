/**
 * 直流减速电机
 * 
 */
#ifndef GEAR_MOTOR_H
#define GEAR_MOTOR_H

#include <stm32f1xx_hal.h>
#include <stdint.h>

typedef struct {
	GPIO_TypeDef* signal_port;
    uint16_t signal_pin;
} gear_motor_t;

void gear_motor_init(gear_motor_t* motor, GPIO_TypeDef* signal_port, uint16_t signal_pin);

#endif

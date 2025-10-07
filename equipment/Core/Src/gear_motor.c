#include "gear_motor.h"

void gear_motor_init(gear_motor_t* motor, GPIO_TypeDef* signal_port, uint16_t signal_pin){
	motor->signal_port = signal_port;
	motor->signal_pin = signal_pin;
}


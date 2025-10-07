/****************************************
DM542 驱动
=== Pulse/rev Table ===
Pulse/rev	SW5		SW6		SW7		SW8		binary		decimal		microsteps		remark
   200		on		on		on		on		00001111	0x0F		  \				1.8°
   400		off		on		on		on		00000111	0x07		  2				0.9°
   800		on		off		on		on		00001011	0x0B		  4				0.45°
  1600		off		off		on		on		00000011	0x03		  8				0.225°
  3200		on		on		off		on		00001101	0x0D		 16				0.1125°
  6400		off		on		off		on		00000101	0x05		 32				0.05625°
 12800		on		off		off		on		00001001	0x09		 64				0.028125°
 25600		off		off		off		on		00000001	0x01		128				0.0140625°
  1000		on		on		on		off		00001110	0x0E		  5				0.36°
  2000		off		on		on		off		00000110	0x06		 10				0.18°
  4000		on		off		on		off		00001010	0x0A		 20				0.09°
  5000		off		off		on		off		00000010	0x02		 25				0.072°
  8000		on		on		off		off		00001100	0x0C		 40				0.045°
 10000		off		on		off		off		00000100	0x04		 50				0.036°
 20000		on		off		off		off		00001000	0x08		100				0.018°
 25000		off		off		off		off		00000000	0x00		125				0.0144°

*******************************************/

#include "dm542_motor.h"
#include <stm32f1xx_hal.h>

static uint16_t min_arr_ = 200-1;
static uint16_t max_arr_ = 1200-1;
volatile static float duty_cycle_ = 0.50;

// 头文件没有写函数原型，类似 private 
/**
 * @brief 设置占空比，影响步进电机的力矩
 * @param htim 定时器的句柄
 * @Param duty_cycle 占空比，有效范围为 [0.00, 1.00]，无效值自动忽略
 * 当占空比为 0.00 或 1.00 时，无电平变化，步进电机不会旋转
 */
void set_duty_cycle(dm542_motor_t* motor, float duty_cycle){
	if(duty_cycle < 0.00 || duty_cycle > 1.00){
		return;
	}
	uint16_t arr = __HAL_TIM_GET_AUTORELOAD(motor->htim); // 获取 ARR 值，即方波周期 period
	uint16_t ccr = duty_cycle * (arr+1);	// 计算 CCR 值
	__HAL_TIM_SET_COMPARE(motor->htim, motor->channel, ccr);	// 写入 CCR
}

void dm542_motor_init(dm542_motor_t* motor, TIM_HandleTypeDef* htim, uint32_t channel,
	GPIO_TypeDef* ena_port, uint16_t ena_pin, GPIO_TypeDef* dir_port, uint16_t dir_pin)
{
	motor->htim = htim;
	motor->channel = channel;
	motor->ena_port = ena_port;
	motor->ena_pin = ena_pin;
	motor->dir_port = dir_port;
	motor->dir_pin = dir_pin;
	
	HAL_GPIO_WritePin(motor->ena_port, motor->ena_pin, GPIO_PIN_RESET); // 初始状态禁用电机
}

void dm542_motor_move(dm542_motor_t* motor, uint32_t pulse_cnt, dm523_motor_dir_t dir){
	/**
	 * 1. 确保定时器空闲
	 * 2. 设置方向
	 * 3. 使能电机
	 * 4. 配置 TIM1 （脉冲数量*单脉冲模式）
	 * 5. 设置占空比
	 * 6. 启动 PWM 波
	 */
	if (motor->htim->Instance->CR1 & TIM_CR1_CEN) { // 检查定时器是否使能
        return; // 定时器正在运行
    }
	
	// 停止并重置定时器
    HAL_TIM_PWM_Stop(motor->htim, motor->channel);
    __HAL_TIM_SET_COUNTER(motor->htim, 0);
	
	HAL_GPIO_WritePin(motor->dir_port, motor->dir_pin, (GPIO_PinState)dir); // 设置方向
	HAL_GPIO_WritePin(motor->ena_port, motor->ena_pin, GPIO_PIN_SET); // 使能电机

	// __HAL_TIM_SET_REPETITION_COUNTER(htim, pulses - 1); // invalid in C99
	motor->htim->Instance->RCR = pulse_cnt - 1; // 设置RCR (脉冲数量)
	
	set_duty_cycle(motor, duty_cycle_); // 设置占空比
	
	HAL_TIM_PWM_Start(motor->htim, motor->channel);
	HAL_TIM_OnePulse_Start(motor->htim, TIM_OPMODE_SINGLE); // motor->htim->Instance->CR1 |= TIM_CR1_OPM; // 单脉冲模式
	
	
    // motor->htim->Instance->CR1 |= TIM_CR1_CEN; // 启动计数器
    motor->is_active = 1;
}

void dm542_motor_move2(dm542_motor_t* motor, uint32_t pulse_cnt, dm523_motor_dir_t dir){
	/**
	 * 1. 停止定时器
	 * 2. 清除计数器CNT：定时器计数器寄存器(TIMx_CNT) 归零
	 * 3. 设置重复计数器RCR：脉冲数量
	 * 4. 启动单脉冲模式
	 * 5. 启动定时器
	 */
	HAL_TIM_PWM_Stop(motor->htim, motor->channel);
	__HAL_TIM_SET_COUNTER(motor->htim, 0);
	// __HAL_TIM_SET_REPETITION_COUNTER(motor->htim, pulse_count - 1);  // 重复计数器RCR（仅TIM1有）
	HAL_TIM_OnePulse_Start(motor->htim, TIM_OPMODE_SINGLE);
	HAL_TIM_PWM_Start(motor->htim, motor->channel);
}

void dm542_motor_stop(dm542_motor_t* motor){
    if (motor->is_active) {
        HAL_GPIO_WritePin(motor->ena_port, motor->ena_pin, GPIO_PIN_RESET); // 禁用电机
        // 停止定时器
        HAL_TIM_PWM_Stop(motor->htim, motor->channel);
        motor->htim->Instance->CR1 &= ~TIM_CR1_CEN;
        motor->is_active = 0;
    }
}

uint32_t dm542_motor_pulse_needed(uint16_t pulses_per_rev, float degrees){
	pulses_per_rev &= 0x0F;	// &= 00001111
	// 比例 degrees : 360° == ret_pulse : pulses_per_rev
	return (degrees*pulses_per_rev)/360;
}

void dm542_motor_speed_up(dm542_motor_t* motor, uint8_t value){
	uint16_t arr = __HAL_TIM_GET_AUTORELOAD(motor->htim);
	arr > value ? (arr -= value) : (arr = min_arr_);
	if(arr < min_arr_) arr = min_arr_;
	__HAL_TIM_SET_AUTORELOAD(motor->htim, arr);
	uint16_t ccr = duty_cycle_ * (arr+1);
	__HAL_TIM_SET_COMPARE(motor->htim, motor->channel, ccr);
}

void dm542_motor_speed_down(dm542_motor_t* motor, uint8_t value){
	uint16_t arr = __HAL_TIM_GET_AUTORELOAD(motor->htim);
	(arr + value) < max_arr_ ? (arr += value) : (arr = max_arr_);
	if(arr > max_arr_) arr = max_arr_;
	__HAL_TIM_SET_AUTORELOAD(motor->htim, arr);
	uint16_t ccr = duty_cycle_ * (arr+1);
	__HAL_TIM_SET_COMPARE(motor->htim, motor->channel, ccr);
}

int8_t dm542_motor_set_arr(dm542_motor_t* motor, uint16_t arr){
	if(arr >= min_arr_ && arr <= max_arr_){
		__HAL_TIM_SET_AUTORELOAD(motor->htim, arr);
		uint16_t ccr = duty_cycle_ * (arr+1);
		__HAL_TIM_SET_COMPARE(motor->htim, motor->channel, ccr);
		return 0;
	}
	return -1;
}

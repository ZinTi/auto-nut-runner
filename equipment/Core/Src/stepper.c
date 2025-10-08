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

#include "stepper.h"
#include <stm32f1xx_hal.h>

static uint16_t min_arr_ = 200-1;
static uint16_t max_arr_ = 1200-1;
volatile static float duty_cycle_ = 0.50;


// 私有函数声明
void set_duty_cycle(stepper_t* motor, float duty_cycle);
void stepper_pulse_finished_callback(stepper_t* motor);

void stepper_init(stepper_t* motor, TIM_HandleTypeDef* htim, uint32_t channel,
	GPIO_TypeDef* ena_port, uint16_t ena_pin, GPIO_TypeDef* dir_port, uint16_t dir_pin)
{
	motor->htim = htim;
	motor->channel = channel;
	motor->ena_port = ena_port;
	motor->ena_pin = ena_pin;
	motor->dir_port = dir_port;
	motor->dir_pin = dir_pin;
	
	motor->is_active = 0;
    motor->target_pulses = 0;
    motor->current_pulses = 0;
    motor->abort_flag = 0;
	
	HAL_GPIO_WritePin(motor->ena_port, motor->ena_pin, GPIO_PIN_RESET); // 初始状态禁用电机
}

void stepper_move(stepper_t* motor, uint32_t pulse_cnt, stepper_dir_t dir){
    // 检查是否正在运行，如果是则先停止
    if (motor->is_active) {
        stepper_stop(motor);
    }
    
    // 重置状态
    motor->abort_flag = 0;
    motor->current_pulses = 0;
    motor->target_pulses = pulse_cnt;
    
    // 设置方向
    HAL_GPIO_WritePin(motor->dir_port, motor->dir_pin, (GPIO_PinState)dir);
    
    // 使能电机
    HAL_GPIO_WritePin(motor->ena_port, motor->ena_pin, GPIO_PIN_SET);
    
    // 配置定时器为 PWM 模式，非单脉冲模式
    HAL_TIM_PWM_Stop(motor->htim, motor->channel);
    __HAL_TIM_SET_COUNTER(motor->htim, 0);
    
    // 设置占空比
    set_duty_cycle(motor, duty_cycle_);
    
    // 启用更新中断，用于脉冲计数
    __HAL_TIM_ENABLE_IT(motor->htim, TIM_IT_UPDATE);
    
    // 启动 PWM
    HAL_TIM_PWM_Start_IT(motor->htim, motor->channel);
    
    motor->is_active = 1;
}

void stepper_stop(stepper_t* motor){
    if (motor->is_active) {
        // 禁用定时器中断
        __HAL_TIM_DISABLE_IT(motor->htim, TIM_IT_UPDATE);
        
        // 停止 PWM
        HAL_TIM_PWM_Stop(motor->htim, motor->channel);
        
        // 禁用电机
        // HAL_GPIO_WritePin(motor->ena_port, motor->ena_pin, GPIO_PIN_RESET);
        
        motor->is_active = 0;
        motor->target_pulses = 0;
        motor->current_pulses = 0;
    }
}

// 中止函数（用于急停或新指令中断）
void stepper_abort(stepper_t* motor){
    motor->abort_flag = 1;
    stepper_stop(motor);
}

// 定时器更新中断回调函数（脉冲计数）
void stepper_update_callback(stepper_t* motor){
    if (motor->is_active) {
        motor->current_pulses++;
        
        // 检查是否达到目标脉冲数
        if (motor->current_pulses >= motor->target_pulses) {
            stepper_stop(motor);
            stepper_pulse_finished_callback(motor);
        }
        
        // 检查是否需要中止
        if (motor->abort_flag) {
            stepper_stop(motor);
        }
    }
}

// 脉冲完成回调函数（可扩展）
void stepper_pulse_finished_callback(stepper_t* motor){
    // 这里可以添加脉冲完成后的操作，如发送完成信号等
    // 例如：echo_str("Pulse finished");
}

/**
 * @brief 设置占空比，影响步进电机的力矩
 * @param htim 定时器的句柄
 * @Param duty_cycle 占空比，有效范围为 [0.00, 1.00]，无效值自动忽略
 * 当占空比为 0.00 或 1.00 时，无电平变化，步进电机不会旋转
 */
void set_duty_cycle(stepper_t* motor, float duty_cycle){
	if(duty_cycle < 0.00 || duty_cycle > 1.00){
		return;
	}
	uint16_t arr = __HAL_TIM_GET_AUTORELOAD(motor->htim); // 获取 ARR 值，即方波周期 period
	uint16_t ccr = duty_cycle * (arr+1);	// 计算 CCR 值
	__HAL_TIM_SET_COMPARE(motor->htim, motor->channel, ccr);	// 写入 CCR
}


uint32_t stepper_pulse_needed(uint16_t pulses_per_rev, float degrees){
	// 比例 degrees : 360° == ret_pulse : pulses_per_rev
	return (degrees*pulses_per_rev)/360;
}

int8_t stepper_set_arr(stepper_t* motor, uint16_t arr){
	if(arr >= min_arr_ && arr <= max_arr_){
		__HAL_TIM_SET_AUTORELOAD(motor->htim, arr);
		uint16_t ccr = duty_cycle_ * (arr+1);
		__HAL_TIM_SET_COMPARE(motor->htim, motor->channel, ccr);
		return 0;
	}
	return -1;
}

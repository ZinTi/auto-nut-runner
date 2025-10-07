/**
 * DM542 驱动，5718HB6401BZ 步进电机（1.8°步距角）
 * 
 * 考虑到通过 重复计数器RCR 发送指定脉冲从而控制步进电机旋转相应角度的方式 比使用“更新中断计数的方式”更方便，故而选择 RCR 方式控制
 * STM32F103C8T6 只有一个高级定时器 TIM1 , 普通定时器没有 RCR，而由于两个步进电机可以不同时工作，
 * 所以选择让两个电机共享 TIM1 定时器。
 */
#ifndef DM542_MOTOR_H
#define DM542_MOTOR_H

#include <stm32f1xx_hal.h>
#include <stdint.h>

typedef struct{
	TIM_HandleTypeDef* htim;
	uint32_t channel;
	GPIO_TypeDef* dir_port;
    uint16_t dir_pin;
    GPIO_TypeDef* ena_port;
    uint16_t ena_pin;

	uint8_t is_active;	// 状态标志
} dm542_motor_t;

/**
 * @brief  Stepper Direction FORWARD and Direction REVERSE enumeration
 */
typedef enum{
	DM542_DIR_FORWARD = 0u,
	DM542_DIR_REVERSE
} dm523_motor_dir_t;

/**
 * @brief 初始化电机结构体
 * @param motor 被初始化目标
 * @param htim 关联的定时器
 * @param channel PWM 输出通道
 * @param ena_port 使能端口
 * @param ena_pin 使能引脚
 * @param dir_port 方向端口
 * @param dir_pin 方向引脚
 */
void dm542_motor_init(dm542_motor_t* motor, TIM_HandleTypeDef* htim, uint32_t channel,
	GPIO_TypeDef* ena_port, uint16_t ena_pin, GPIO_TypeDef* dir_port, uint16_t dir_pin);

/**
 * @brief 移动电机
 * @param motor 被移动目标
 * @param pulse_cnt 脉冲数
 * @param dir 方向
 */
void dm542_motor_move(dm542_motor_t* motor, uint32_t pulse_cnt, dm523_motor_dir_t dir);

/**
 * @brief 输出脉冲
 * @param motor 被控制目标
 * @param pulse_cnt 脉冲数量
 * @param dir 方向
 */
void dm542_motor_move2(dm542_motor_t* motor, uint32_t pulse_cnt, dm523_motor_dir_t dir);

/**
 * @brief 停止电机
 * @param motor 被停止目标
 */
void dm542_motor_stop(dm542_motor_t* motor);

/**
 * @brief 步进电机旋转角度映射成相应的脉冲数量，即计算需要多少步才能旋转特定角度（DM542驱动支持 microsteps）
 * @param pulses_per_rev 电机每转步数，请依据实际的驱动拨码开关 5~8 号设置，
 * @param degrees 要转换的角度
 * @return pulses 数量，范围超过 uint32_t 将溢出
 * @retvar 0 当输入数据不合法例如 pulses_per_rev 不在范围内将返回此值，或 degrees 为 0 
 */
uint32_t dm542_motor_pulse_needed(uint16_t pulses_per_rev, float degrees);

/**
 * @brief 加速，通过减小 ARR 值增加 PWM 频率，从而提高转速
 * @param htim 定时器的句柄
 * @param value 减小值
 */
void dm542_motor_speed_up(dm542_motor_t* motor, uint8_t value);

/**
 * @brief 减速，通过增大 ARR 值降低 PWM 频率，从而降低转速
 * @param htim 定时器的句柄
 * @param value 减小值
 */
void dm542_motor_speed_down(dm542_motor_t* motor, uint8_t value);

/**
 * @brief 直接设置 arr 
 */
int8_t dm542_motor_set_arr(dm542_motor_t* motor, uint16_t arr);

#endif

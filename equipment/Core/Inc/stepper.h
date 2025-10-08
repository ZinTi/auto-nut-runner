/**
 * DM542 驱动，5718HB6401BZ 步进电机（1.8°步距角）
 * 
 */
#ifndef STEPPER_H
#define STEPPER_H

#include <stm32f1xx_hal.h>
#include <stdint.h>

typedef struct{
	TIM_HandleTypeDef* htim;
	uint32_t channel;
	GPIO_TypeDef* dir_port;
    uint16_t dir_pin;
    GPIO_TypeDef* ena_port;
    uint16_t ena_pin;

    uint8_t is_active;          // 状态标志
    uint32_t target_pulses;     // 目标脉冲数
    uint32_t current_pulses;    // 当前已输出脉冲数
    volatile uint8_t abort_flag; // 中止标志，用于急停或新指令中断
} stepper_t;

/**
 * @brief  Stepper Direction FORWARD and Direction REVERSE enumeration
 */
typedef enum{
	DM542_DIR_FORWARD = 0u,
	DM542_DIR_REVERSE
} stepper_dir_t;

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
void stepper_init(stepper_t* motor, TIM_HandleTypeDef* htim, uint32_t channel,
	GPIO_TypeDef* ena_port, uint16_t ena_pin, GPIO_TypeDef* dir_port, uint16_t dir_pin);

/**
 * @brief 移动电机
 * @param motor 被移动目标
 * @param pulse_cnt 脉冲数
 * @param dir 方向
 */
void stepper_move(stepper_t* motor, uint32_t pulse_cnt, stepper_dir_t dir);

/**
 * @brief 输出脉冲
 * @param motor 被控制目标
 * @param pulse_cnt 脉冲数量
 * @param dir 方向
 */
void stepper_move2(stepper_t* motor, uint32_t pulse_cnt, stepper_dir_t dir);

/**
 * @brief 停止电机
 * @param motor 被停止目标
 */
void stepper_stop(stepper_t* motor);

/**
 * @brief 中止当前电机运动
 * @param motor 要中止的电机
 */
void stepper_abort(stepper_t* motor);

/**
 * @brief 步进电机更新回调函数（用于定时器中断）
 * @param motor 步进电机实例
 */
void stepper_update_callback(stepper_t* motor);

/**
 * @brief 步进电机旋转角度映射成相应的脉冲数量，即计算需要多少步才能旋转特定角度（DM542驱动支持 microsteps）
 * @param pulses_per_rev 电机每转步数，请依据实际的驱动拨码开关 5~8 号设置，
 * @param degrees 要转换的角度
 * @return pulses 数量，范围超过 uint32_t 将溢出
 * @retvar 0 当输入数据不合法例如 pulses_per_rev 不在范围内将返回此值，或 degrees 为 0 
 */
uint32_t stepper_pulse_needed(uint16_t pulses_per_rev, float degrees);

/**
 * @brief 直接设置 arr 
 */
int8_t stepper_set_arr(stepper_t* motor, uint16_t arr);

#endif

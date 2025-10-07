/**
 * 电机控制器，包含两个步进电机与一个直流电机
 * 
 */

#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <stm32f1xx_hal.h>
#include "dm542_motor.h"
#include "gear_motor.h"

/**
 * @brief 初始化
 * @param htim1 步进电机高级定时器 TIM1
 */
void mctl_init(TIM_HandleTypeDef* htim1);

/**
 * @brief 移动转台步进电机向指定方向旋转指定角度
 * @param degrees 度数
 * @param dir 方向
 */
void mctl_move_turntable(float degrees, dm523_motor_dir_t dir);

/**
 * @brief 移动升降步进电机向指定方向旋转指定角度
 * @param degrees 度数
 * @param dir 方向
 */
void mctl_move_lifter(float degrees, dm523_motor_dir_t dir);

/**
 * @brief 刹车，即急停
 */
void mctl_brake(void);

/**
 * @brief 测试一个电机作业周期
 */
void mctl_auto(void);

#endif

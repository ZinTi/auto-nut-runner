/**
 * 有源蜂鸣器 (TMB12A05)
 */

#ifndef ACTIVE_BUZZER_H
#define ACTIVE_BUZZER_H

#include "stm32f1xx_hal.h"

/**
 * @brief 开启蜂鸣器（低电平触发）
 */
void active_buzzer_on(void);

/**
 * @brief 开启蜂鸣器（高电平关闭）
 */
void active_buzzer_off(void);

/**
 * @brief 翻转蜂鸣器
 */
void active_buzzer_toggle(void);

/**
 * @brief 蜂鸣器发出指定次数的哔声
 * @param count 哔声次数
 */
void active_buzzer_beep(uint8_t count);

/**
  * @brief 控制三引脚有源蜂鸣器播放节奏
  * @param duration_ms 节奏持续时间（毫秒）
  * @note 节奏包括短促"滴"声、长"嘟"声和急促双音，例如播放5s节奏音active_buzzer_rhythm(5000);
  * @retval 无
  */
void active_buzzer_rhythm(uint32_t duration_ms);

#endif

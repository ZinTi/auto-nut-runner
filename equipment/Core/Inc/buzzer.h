/**
 * 有源蜂鸣器 (TMB12A05)
 */

#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

/**
 * @brief 开启蜂鸣器（低电平触发）
 */
void buzzer_on(void);

/**
 * @brief 开启蜂鸣器（高电平关闭）
 */
void buzzer_off(void);

/**
 * @brief 翻转蜂鸣器
 */
void buzzer_toggle(void);

/**
 * @brief 蜂鸣器发出指定次数的哔声
 * @param count 哔声次数
 */
void buzzer_beep(uint8_t count);

/**
  * @brief 控制三引脚有源蜂鸣器播放节奏
  * @param duration_ms 节奏持续时间（毫秒）
  * @note 节奏包括短促"滴"声、长"嘟"声和急促双音，例如播放5s节奏音buzzer_rhythm(5000);
  * @retval 无
  */
void buzzer_rhythm(uint32_t duration_ms);

#endif

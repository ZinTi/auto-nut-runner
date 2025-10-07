#include "active_buzzer.h"

#define ACTIVE_BUZZER_PORT         GPIOA        // 蜂鸣器控制端口
#define ACTIVE_BUZZER_PIN          GPIO_PIN_2   // 蜂鸣器控制引脚


/**
 * @brief 开启蜂鸣器（低电平触发）
 */
void active_buzzer_on(void){
    HAL_GPIO_WritePin(ACTIVE_BUZZER_PORT, ACTIVE_BUZZER_PIN, GPIO_PIN_RESET);
}


/**
 * @brief 开启蜂鸣器（高电平关闭）
 */
void active_buzzer_off(void){
    HAL_GPIO_WritePin(ACTIVE_BUZZER_PORT, ACTIVE_BUZZER_PIN, GPIO_PIN_SET);
}

/**
 * @brief 翻转蜂鸣器
 */
void active_buzzer_toggle(void){
	HAL_GPIO_TogglePin(ACTIVE_BUZZER_PORT, ACTIVE_BUZZER_PIN);
}

/**
 * @brief 蜂鸣器发出指定次数的哔声
 * @param count 哔声次数
 */
void active_buzzer_beep(uint8_t count) {
    for (int i = 0; i < count*2; i++) {
        HAL_GPIO_TogglePin(ACTIVE_BUZZER_PORT, ACTIVE_BUZZER_PIN);
        HAL_Delay(100);
    }
    HAL_GPIO_WritePin(ACTIVE_BUZZER_PORT, ACTIVE_BUZZER_PIN, GPIO_PIN_SET); // 确保最后关闭蜂鸣器
}

/**
  * @brief 控制三引脚有源蜂鸣器播放节奏
  * @param duration_ms 节奏持续时间（毫秒）
  * @note 节奏包括短促"滴"声、长"嘟"声和急促双音，例如播放5s节奏音active_buzzer_rhythm(5000);
  * @retval 无
  */
void active_buzzer_rhythm(uint32_t duration_ms) {
    uint32_t start_tick = HAL_GetTick();
    const uint32_t quarter = 200;  // 基本节拍时长(ms)
    
    while((HAL_GetTick() - start_tick) < duration_ms) {
        // 短促"滴"声 (高频)
        HAL_GPIO_WritePin(ACTIVE_BUZZER_PORT, ACTIVE_BUZZER_PIN, GPIO_PIN_RESET);
        HAL_Delay(quarter/4);
        HAL_GPIO_WritePin(ACTIVE_BUZZER_PORT, ACTIVE_BUZZER_PIN, GPIO_PIN_SET);
        HAL_Delay(quarter/8);
        
        // 长"嘟"声 (中频)
        HAL_GPIO_WritePin(ACTIVE_BUZZER_PORT, ACTIVE_BUZZER_PIN, GPIO_PIN_RESET);
        HAL_Delay(quarter);
        HAL_GPIO_WritePin(ACTIVE_BUZZER_PORT, ACTIVE_BUZZER_PIN, GPIO_PIN_SET);
        HAL_Delay(quarter/4);
        
        // 急促双音 (低频组合)
        HAL_GPIO_WritePin(ACTIVE_BUZZER_PORT, ACTIVE_BUZZER_PIN, GPIO_PIN_RESET);
        HAL_Delay(quarter/2);
        HAL_GPIO_WritePin(ACTIVE_BUZZER_PORT, ACTIVE_BUZZER_PIN, GPIO_PIN_SET);
        HAL_Delay(quarter/8);
        HAL_GPIO_WritePin(ACTIVE_BUZZER_PORT, ACTIVE_BUZZER_PIN, GPIO_PIN_RESET);
        HAL_Delay(quarter/2);
        HAL_GPIO_WritePin(ACTIVE_BUZZER_PORT, ACTIVE_BUZZER_PIN, GPIO_PIN_SET);
        HAL_Delay(quarter/2);
    }
    // 确保结束时关闭蜂鸣器（高电平关闭）
    HAL_GPIO_WritePin(ACTIVE_BUZZER_PORT, ACTIVE_BUZZER_PIN, GPIO_PIN_SET);
}

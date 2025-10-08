#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

/************* 1. 引脚宏常量 ******************/

// 转台引脚：TIM1_CH1(PWM-PA8) ENA(PB0) DIR(PB1)
#define STP_T_CHANNEL		TIM_CHANNEL_1
#define STP_T_ENA_PORT		GPIOB
#define STP_T_ENA_PIN		GPIO_PIN_0
#define STP_T_DIR_PORT		GPIOB
#define STP_T_DIR_PIN		GPIO_PIN_1

// 升降引脚：TIM2_CH1(PWM-PA0) ENA(PB10) DIR(PB11)
#define STP_L_CHANNEL		TIM_CHANNEL_1
#define STP_L_ENA_PORT		GPIOB
#define STP_L_ENA_PIN		GPIO_PIN_10
#define STP_L_DIR_PORT		GPIOB
#define STP_L_DIR_PIN		GPIO_PIN_11

/************* 2. 声明全局变量 ******************/
// extern uint32_t system_clock;
// extern uint8_t operation_mode;
// extern float calibration_value;

extern volatile uint16_t pulses_per_rev_t; // 转台步进电机的 Pulses/rev 配置值
extern volatile uint16_t pulses_per_rev_l; // 升降步进电机的 Pulses/rev 配置值

/**************** 3. 访问函数 ****************/
/**
 * @brief 获取“转台步进/升降步进”的 每转脉冲 配置
 * @param option 步进选项，若为 't' 则表示 ppr_t，若为 'l' 则表示 ppr_l
 * @return 每转脉冲 值
 * @retval 若返回 0 则表示 option 不合法
 */
uint16_t get_ppr(char option);

/**
 * @brief 设置“转台步进/升降步进”的 每转脉冲 配置
 * @param option 步进选项，若为 't' 则表示 ppr_t，若为 'l' 则表示 ppr_l
 * @param ppr 新的 每转脉冲 值
 * @return 成功与否
 * @retval 1 设置成功
 * @retval 0 设置失败：option 不合法 或 ppr 不合法
 */
uint8_t set_ppr(char option, uint16_t ppr);


#endif

#include "config.h"

// uint32_t system_clock = 72000000;  // 72MHz默认时钟
// uint8_t operation_mode = DEFAULT_MODE;
// float calibration_value = 1.25f;

volatile uint16_t pulses_per_rev_t = 25600; // 转台步进电机的 Pulses/rev 配置值
volatile uint16_t pulses_per_rev_l = 25600; // 升降步进电机的 Pulses/rev 配置值

uint16_t get_ppr(char option){
    uint16_t result;
    
    // 进入临界区（禁用中断）
    __disable_irq();
    
    if(option == 't')
        result = pulses_per_rev_t;
    else if(option == 'l')
        result = pulses_per_rev_l;
    else
        result = 0u;
    
    // 退出临界区（启用中断）
    __enable_irq();
    
    return result;
}

uint8_t set_ppr(char option, uint16_t ppr){
    uint8_t success = 0u;
    
    // 进入临界区（禁用中断）
    __disable_irq();
    
    switch(ppr) {
        case 200: case 400: case 800: case 1600: case 3200: case 6400: case 12800: case 25600:
        case 1000: case 2000: case 4000: case 5000: case 8000: case 10000: case 20000: case 25000:
            if(option == 't') {
                pulses_per_rev_t = ppr;
                success = 1u;
            } else if(option == 'l') {
                pulses_per_rev_l = ppr;
                success = 1u;
            }
            break;
        default:
            success = 0u;
    }
    
    // 退出临界区（启用中断）
    __enable_irq();
    
    return success;
}


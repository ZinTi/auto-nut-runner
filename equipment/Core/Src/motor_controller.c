#include "motor_controller.h"
#include "config.h"

// 转盘：TIM1_CH1 PWM-PA8 ENA-PB0 DIR-PB1
#define STP_T_CHANNEL		TIM_CHANNEL_1
#define STP_T_ENA_PORT		GPIOB
#define STP_T_ENA_PIN		GPIO_PIN_0
#define STP_T_DIR_PORT		GPIOB
#define STP_T_DIR_PIN		GPIO_PIN_1
// 升降：TIM1_CH2 PWM-PA0 ENA-PB10 DIR-PB11
#define STP_L_CHANNEL		TIM_CHANNEL_2
#define STP_L_ENA_PORT		GPIOB
#define STP_L_ENA_PIN		GPIO_PIN_10
#define STP_L_DIR_PORT		GPIOB
#define STP_L_DIR_PIN		GPIO_PIN_11

static dm542_motor_t stepper_turntable;	// 转台步进电机
static dm542_motor_t stepper_lifter;		// 升降步进电机
static gear_motor_t gear_motor;	// 拧螺丝电机

void mctl_init(TIM_HandleTypeDef* htim1){
	dm542_motor_init(
		&stepper_turntable, htim1, STP_T_CHANNEL,
		STP_T_ENA_PORT, STP_T_ENA_PIN,
		STP_T_DIR_PORT, STP_T_DIR_PIN
	);

	dm542_motor_init(
		&stepper_lifter, htim1, STP_L_CHANNEL,
		STP_L_ENA_PORT, STP_L_ENA_PIN,
		STP_L_DIR_PORT, STP_L_DIR_PIN
	);

	dm542_motor_set_arr(&stepper_turntable, 999); // 自动重装寄存器，设置脉冲周期，影响脉冲发送频率快慢，即步进电机速度快慢
	dm542_motor_set_arr(&stepper_lifter, 999);
	
	UNUSED(gear_motor);
}

void mctl_move_turntable(float degrees, dm523_motor_dir_t dir){
	uint32_t pulse_cnt = dm542_motor_pulse_needed(get_ppr('t'), degrees);
	dm542_motor_move(&stepper_turntable, pulse_cnt, dir);
}

void mctl_move_lifter(float degrees, dm523_motor_dir_t dir){
	uint32_t pulse_cnt = dm542_motor_pulse_needed(get_ppr('l'), degrees);
	dm542_motor_move(&stepper_lifter, pulse_cnt, dir);
}

void mctl_brake(void){
	
}

void mctl_auto(void){
	uint32_t pulse1 = dm542_motor_pulse_needed(get_ppr('t'), 30); // 360/12
	uint32_t pulse2 = dm542_motor_pulse_needed(get_ppr('l'), 360);

	/**
	 * 1. 转台步进电机正转 30°
	 * 2. 延迟 3s
	* 3. 重复这一过程继续转...
	 */
	while(1){
		dm542_motor_move(&stepper_turntable, pulse1, DM542_DIR_FORWARD);
		
		// 等待运动完成
        while(stepper_turntable.htim->Instance->CR1 & TIM_CR1_CEN) {
            // 等待定时器停止
        }
		
		HAL_Delay(3000);
	}
	
	// dm542_motor_move(&stepper_lifter, pulse2, DM542_DIR_REVERSE);
}

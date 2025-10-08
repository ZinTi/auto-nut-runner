#include "cmd_handler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "led.h"
#include "buzzer.h"
#include "stepper.h"
#include "gear_motor.h"
#include "config.h"
#include "echo.h"

extern UART_HandleTypeDef huart1;
extern stepper_t turntable;		// 转台步进电机
extern stepper_t lifter;		// 升降步进电机
extern gear_motor_t gear_motor;				// 拧螺丝电机

void cmd_dispatcher(const char* cmd) {
	if('^' != cmd[0]){ // 不完整指令或非指令
		return;
	}
	switch(cmd[1]){
		case 'L':{
			cmd_process_led(cmd + 2);
			break;
		}
		case 'B':{
			cmd_process_buzzer(cmd + 2);
			break;
		}
		case 'C':{
			cmd_process_config(cmd + 2);
			break;
		}
		case 'T':{
			cmd_process_turntable(cmd + 2);
			break;
		}
		case 'E':{
			cmd_process_lifter(cmd + 2);
			break;
		}
		case 'K':{
			cmd_process_brake(cmd + 2);
			break;
		}
		default:{
			echo_str("Unknown cmd.");
		}
	}
	
}

void cmd_process_led(const char* cmd){
	switch(cmd[0]){
		case 'O':{
			led_on();
			echo_str("led on.\n");
			break;
		}
		case 'F':{
			led_off();
			echo_str("led off.\n");
			break;
		}
		case 'T':{
			led_toggle();
			echo_str("led toggle.\n");
			break;
		}
		case 'B':{
			uint8_t cnt = (int8_t)atoi(cmd+1);
			led_flash(cnt);
			char str[25];
			snprintf(str, 25, "led flash:%u.\n", cnt);
			echo_str(str);
			break;
		}
		default:{
			echo_str("Invalid led cmd.");
		}
	}

}

void cmd_process_buzzer(const char* cmd){
	switch(cmd[0]){
		case 'O':{
			buzzer_on();
			echo_str("buzzer on.\n");
			break;
		}
		case 'F':{
			buzzer_off();
			echo_str("buzzer off.\n");
			break;
		}
		case 'T':{
			buzzer_toggle();
			echo_str("buzzer toggle.\n");
			break;
		}
		case 'R':{
			buzzer_rhythm(1000); // 蜂鸣器播放 1s 节奏音
			echo_str("buzzer rhythm.\n");
			break;
		}
		case 'B':{
			uint8_t cnt = (int8_t)atoi(cmd+1);
			buzzer_beep(cnt);
			char str[25];
			snprintf(str, 25, "buzzer beep:%u.\n", cnt);
			echo_str(str);
			break;
		}
		default:{
			echo_str("Invalid buzzer cmd.");
		}
	}
}

void cmd_process_config(const char* cmd){
	switch(cmd[0]){
		case 'F':{
			char str[50];
			uint16_t ppr_t = get_ppr('t');
			uint16_t ppr_l = get_ppr('l');
			snprintf(str, 50, "T PPR:%u, L PPR:%u.", ppr_t, ppr_l);
			echo_str(str);
			break;
		}
		case 'M':{
			echo_str("Mode manual cmd.");
			break;
		}
		case 'A':{
			echo_str("Mode auto cmd.");
			break;
		}
		case 'T':{
			uint16_t ppr = (uint16_t)atoi(cmd+1);
			uint8_t ret = set_ppr('t', ppr);
			char str[25];
			if(1 == ret){
				snprintf(str, 25, "T PPR ok:%u.\n", ppr);
			} else {
				snprintf(str, 25, "T PPR err:%u.\n", ppr);
			}
			echo_str(str);
			break;
		}
		case 'L':{
			uint16_t ppr = (uint16_t)atoi(cmd+1);
			uint8_t ret = set_ppr('l', ppr);
			char str[25];
			if(1 == ret){
				snprintf(str, 25, "L PPR ok:%u.\n", ppr);
			} else {
				snprintf(str, 25, "L PPR err:%u.\n", ppr);
			}
			echo_str(str);
			break;
		}
		default:{
			echo_str("Invalid config cmd.");
		}
	}
}

void cmd_process_turntable(const char* cmd){
	// 如果有新指令，先中止当前运动
    if(turntable.is_active){
        stepper_abort(&turntable);
    }
	
	switch(cmd[0]){
		case 'F':{
			float degrees = (float)atof(cmd + 1);
			uint32_t pulse_cnt = stepper_pulse_needed(get_ppr('t'), degrees);
			stepper_move(&turntable, pulse_cnt, DM542_DIR_FORWARD);
			char str[25];
			snprintf(str, 25, "TF:%lu.\n", pulse_cnt);
			echo_str(str);
			break;
		}
		case 'R':{
			float degrees = (float)atof(cmd + 1);
			uint32_t pulse_cnt = stepper_pulse_needed(get_ppr('t'), degrees);
			stepper_move(&turntable, pulse_cnt, DM542_DIR_REVERSE);
			char str[25];
			snprintf(str, 25, "TR:%lu.\n", pulse_cnt);
			echo_str(str);
			break;
		}
		default:{
			echo_str("Invalid turntable cmd.");
		}
	}
}

void cmd_process_lifter(const char* cmd){
    // 如果有新指令，先中止当前运动
    if(lifter.is_active){
        stepper_abort(&lifter);
    }
	
	switch(cmd[0]){
		case 'F':{
			float degrees = (float)atof(cmd+1);
			uint32_t pulse_cnt = stepper_pulse_needed(get_ppr('l'), degrees);
			stepper_move(&lifter, pulse_cnt, DM542_DIR_FORWARD);
			char str[25];
			snprintf(str, 25, "LF:%lu.\n", pulse_cnt);
			echo_str(str);
			break;
		}
		case 'R':{
			float degrees = (float)atof(cmd+1);
			uint32_t pulse_cnt = stepper_pulse_needed(get_ppr('l'), degrees);
			stepper_move(&lifter, pulse_cnt, DM542_DIR_REVERSE);
			char str[25];
			snprintf(str, 25, "LR:%lu.\n", pulse_cnt);
			echo_str(str);
			break;
		}
		default:{
			echo_str("Invalid lifter cmd.");
		}
	}
}

void cmd_process_brake(const char* cmd){
	switch(cmd[0]){
		case 'A':{
            stepper_abort(&turntable);
            stepper_abort(&lifter);
            echo_str("All brake applied.\n");
			break;
		}
		case 'T':{
            // 转台急停
            stepper_abort(&turntable);
            echo_str("Turntable brake applied.\n");
			break;
		}
		case 'L':{
            // 升降急停
            stepper_abort(&lifter);
            echo_str("Lifter brake applied.\n");
			break;
		}
		default:{
			echo_str("Invalid brake cmd.");
		}
	}
}

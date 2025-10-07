#include "cmd_handler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "led.h"
#include "active_buzzer.h"
// #include "gear_motor.h"
#include "motor_controller.h"
#include "config.h"
#include "echo.h"

extern UART_HandleTypeDef huart1;

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
			snprintf(str, 25, "led flash:%d.\n", cnt);
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
			active_buzzer_on();
			echo_str("buzzer on.\n");
			break;
		}
		case 'F':{
			active_buzzer_off();
			echo_str("buzzer off.\n");
			break;
		}
		case 'T':{
			active_buzzer_toggle();
			echo_str("buzzer toggle.\n");
			break;
		}
		case 'R':{
			active_buzzer_rhythm(1000); // 蜂鸣器播放 1s 节奏音
			echo_str("buzzer rhythm.\n");
			break;
		}
		case 'B':{
			uint8_t cnt = (int8_t)atoi(cmd+1);
			active_buzzer_beep(cnt);
			char str[25];
			snprintf(str, 25, "buzzer beep:%d.\n", cnt);
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
			snprintf(str, 50, "T PPR:%d, L PPR:%d.", ppr_t, ppr_l);
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
				snprintf(str, 25, "T PPR ok:%d.\n", get_ppr('t'));
			} else {
				snprintf(str, 25, "T PPR err:%d.\n", get_ppr('t'));
			}
			echo_str(str);
			break;
		}
		case 'L':{
			uint16_t ppr = (uint16_t)atoi(cmd+1);
			uint8_t ret = set_ppr('l', ppr);
			char str[25];
			if(1 == ret){
				snprintf(str, 25, "L PPR ok:%d.\n", get_ppr('l'));
			} else {
				snprintf(str, 25, "L PPR err:%d.\n", get_ppr('l'));
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
	switch(cmd[0]){
		case 'F':{
			float degrees = (float)atof(cmd + 1);
			mctl_move_turntable(degrees, DM542_DIR_FORWARD);
			char str[25];
			snprintf(str, 25, "TF:%f.\n", degrees);
			echo_str(str);
			break;
		}
		case 'R':{
			float degrees = (float)atof(cmd + 1);
            mctl_move_turntable(degrees, DM542_DIR_REVERSE);
			char str[25];
			snprintf(str, 25, "TR:%f.\n", degrees);
			echo_str(str);
			break;
		}
		default:{
			echo_str("Invalid turntable cmd.");
		}
	}
}

void cmd_process_lifter(const char* cmd){
	switch(cmd[0]){
		case 'F':{
			float degrees = (float)atof(cmd+1);
			mctl_move_lifter(degrees, DM542_DIR_FORWARD);
			char str[25];
			snprintf(str, 25, "LF:%f.\n", degrees);
			echo_str(str);
			break;
		}
		case 'R':{
			float degrees = (float)atof(cmd+1);
            mctl_move_lifter(degrees, DM542_DIR_REVERSE);
			char str[25];
			snprintf(str, 25, "LR:%f.\n", degrees);
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
			mctl_brake();
			echo_str("mctl brake cmd.");
			break;
		}
		case 'T':{
			echo_str("t brake cmd.");
			break;
		}
		case 'L':{
			echo_str("l brake cmd.");
			break;
		}
		default:{
			echo_str("Invalid brake cmd.");
		}
	}
}

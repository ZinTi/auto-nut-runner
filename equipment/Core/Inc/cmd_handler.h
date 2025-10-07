/**
 命令解析与执行
1. 指令格式：

	"<起始符><设备><操作>[值]<结束符>"
	
	(1) 起始符为 '^'
	(2) 结束符为 '$'

2. 设备字符代码：

    - L		板载控制（LED）
    - B		蜂鸣器控制（Buzzer）
	- C		配置参数（Config）
    - T		转盘步进电机控制（Turntable）
	- E		升降步进电机控制（Elevator）
	- K		急停/刹车控制（braKe、breaK、Kill）
    - ......

3. 操作字符代码与示例：

(1) 板载 LED 指令格式（LED）:
    - LO			LED Turn on 板载 LED 开启
    - LF			LED Turn off 板载 LED 关闭
	- LT			LED Toggle 板载 LED 反转状态
    - LB3			LED Blink 板载 LED 闪烁 3 次

(2) 蜂鸣器指令格式（Buzzer）:
    - BO			Buzzer Turn on 蜂鸣器开启
    - BF			Buzzer Turn off 蜂鸣器关闭
	- BT			Buzzer Toggle 蜂鸣器反转状态
    - BR			Buzzer Rhythm 蜂鸣器播放节奏音
	- BB3			Buzzer Beep 蜂鸣器发出 3 次哔声

(3) 配置参数（Config）
	- CF			Config 获取所有配置值（Fetch）
	- CM			Config 配置运行模式为手动模式（Mode Manual）
	- CA			Config 配置运行模式为自动模式（Mode Auto）
	- CT25600		Config 配置转台步进电机“每转脉冲数” Pulse/rev 配置为 25600（微步配置）
	- CL200		Config 配置升降步进电机“每转脉冲数” Pulse/rev 配置为 200 （微步配置）

(4) 转盘步进电机控制（Turntable）
	- TF30.00		Turntable Forward 正向旋转 30 度
	- TR60.00		Turntable Reverse 反向旋转 60 度

(5) 升降步进电机控制（Elevator）
	- EF30.00		Elevator Forward 正向旋转 30 度
	- ER60.00		Elevator Reverse 反向旋转 60 度

(6) 紧急指令（Brake、break、kill）
	- KA			Kill All 所有机械结构全部急停
	- KT			Kill Turntable 转盘急停
	- KL			Kill Lifter	升降急停


(7) 直流电机控制（DC Gear Motor）
这部分由按钮操作

 */
#ifndef CMD_HANDLER_H
#define CMD_HANDLER_H

/**
 * @brief 指令分发处理（仲裁+路由）
 * @param cmd 指令
 * @return 无
 */
void cmd_dispatcher(const char* cmd);

// 处理板载 LED 指令
void cmd_process_led(const char* cmd);

// 处理蜂鸣器指令
void cmd_process_buzzer(const char* cmd);

// 处理配置指令
void cmd_process_config(const char* cmd);

// 处理转盘步进电机指令
void cmd_process_turntable(const char* cmd);

// 处理升降步进电机指令
void cmd_process_lifter(const char* cmd);

// 处理刹车指令
void cmd_process_brake(const char* cmd);

#endif

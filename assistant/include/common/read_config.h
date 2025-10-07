#ifndef READCONFIG_H
#define READCONFIG_H

#include <QString>

class ReadConfig final {
public:
    // 删除拷贝构造函数和赋值操作符
    ReadConfig(const ReadConfig&) = delete;
    ReadConfig& operator=(const ReadConfig&) = delete;

    // 获取单例实例的静态方法
    static ReadConfig& getInstance();

    void read_config_file(const QString &file_name);

    QString get_packet_prefix(void) const;
    QString get_packet_suffix(void) const;
    QString get_turntable_clockwise(void) const;
    QString get_turntable_anticlockwise(void) const;
    QString get_lifter_up(void) const;
    QString get_lifter_down(void) const;
    QString get_gearmotor_clockwise(void) const;
    QString get_gearmotor_anticlockwise(void) const;

    QString get_led_on(void) const;
    QString get_led_off(void) const;
    QString get_led_toggle(void) const;
    QString get_led_blink(void) const;

    QString get_buzzer_on(void) const;
    QString get_buzzer_off(void) const;
    QString get_buzzer_toggle(void) const;
    QString get_buzzer_rhythm(void) const;
    QString get_buzzer_beep(void) const;

    QString get_config_fetch(void) const;
    QString get_config_mode_manual(void) const;
    QString get_config_mode_auto(void) const;
    QString get_config_turntable_ppr(void) const;
    QString get_config_lifter_ppr(void) const;
    QString get_brake_all(void) const;
    QString get_brake_turntable(void) const;
    QString get_brake_lifter(void) const;

private:
    // 私有构造函数
    ReadConfig();
    ~ReadConfig() = default;

    QString packet_prefix_;
    QString packet_suffix_;

    QString turntable_clockwise_;
    QString turntable_anticlockwise_;
    QString lifter_up_;
    QString lifter_down_;
    QString gearmotor_clockwise_;
    QString gearmotor_anticlockwise_;

    QString led_on_;
    QString led_off_;
    QString led_toggle_;
    QString led_blink_;

    QString buzzer_on_;
    QString buzzer_off_;
    QString buzzer_toggle_;
    QString buzzer_rhythm_;
    QString buzzer_beep_;

    QString config_fetch_;
    QString config_mode_manual_;
    QString config_mode_auto_;
    QString config_turntable_ppr_;
    QString config_lifter_ppr_;

    QString brake_all_;
    QString brake_turntable_;
    QString brake_lifter_;
};

#endif //READCONFIG_H
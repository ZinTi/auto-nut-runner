#include "read_config.h"
#include <QSettings>

ReadConfig::ReadConfig() {
    // 构造函数内容可以留空，或者初始化一些默认值
}

ReadConfig& ReadConfig::getInstance() {
    static ReadConfig instance;
    return instance;
}

void ReadConfig::read_config_file(const QString &file_name){
    // 读取配置文件
    QSettings settings(file_name, QSettings::IniFormat);

    // 解析配置文件
    packet_prefix_ = settings.value("Packet/prefix").toString();
    packet_suffix_ = settings.value("Packet/suffix").toString();
    turntable_clockwise_ = settings.value("Turntable/clockwise").toString();
    turntable_anticlockwise_ = settings.value("Turntable/anticlockwise").toString();
    lifter_up_ = settings.value("Lifter/up").toString();
    lifter_down_ = settings.value("Lifter/down").toString();
    gearmotor_clockwise_ = settings.value("GearMotor/clockwise").toString();
    gearmotor_anticlockwise_ = settings.value("GearMotor/anticlockwise").toString();
    led_on_ = settings.value("LED/on").toString();
    led_off_ = settings.value("LED/off").toString();
    led_toggle_ = settings.value("LED/toggle").toString();
    led_blink_ = settings.value("LED/blink").toString();
    buzzer_on_ = settings.value("Buzzer/on").toString();
    buzzer_off_ = settings.value("Buzzer/off").toString();
    buzzer_toggle_ = settings.value("Buzzer/toggle").toString();
    buzzer_rhythm_ = settings.value("Buzzer/rhythm").toString();
    buzzer_beep_ = settings.value("Buzzer/beep").toString();
    config_fetch_ = settings.value("Config/fetch").toString();
    config_mode_manual_ = settings.value("Config/mode_manual").toString();
    config_mode_auto_ = settings.value("Config/mode_auto").toString();
    config_turntable_ppr_ = settings.value("Config/ppr_t").toString();
    config_lifter_ppr_ = settings.value("Config/ppr_l").toString();

    brake_all_ = settings.value("Brake/all").toString();
    brake_turntable_ = settings.value("Brake/turntable").toString();
    brake_lifter_ = settings.value("Brake/lifter").toString();
}

QString ReadConfig::get_packet_prefix(void) const{
    return packet_prefix_;
}

QString ReadConfig::get_packet_suffix(void) const{
    return packet_suffix_;
}

QString ReadConfig::get_turntable_clockwise(void) const{
    return turntable_clockwise_;
}

QString ReadConfig::get_turntable_anticlockwise(void) const{
    return turntable_anticlockwise_;
}

QString ReadConfig::get_lifter_up(void) const{
    return lifter_up_;
}

QString ReadConfig::get_lifter_down(void) const{
    return lifter_down_;
}

QString ReadConfig::get_gearmotor_clockwise(void) const{
    return gearmotor_clockwise_;
}

QString ReadConfig::get_gearmotor_anticlockwise(void) const{
    return gearmotor_anticlockwise_;
}


QString ReadConfig::get_led_on(void) const{
    return led_on_;
}

QString ReadConfig::get_led_off(void) const{
    return led_off_;
}

QString ReadConfig::get_led_toggle(void) const{
    return led_toggle_;
}

QString ReadConfig::get_led_blink(void) const{
    return led_blink_;
}

QString ReadConfig::get_buzzer_on(void) const{
    return buzzer_on_;
}

QString ReadConfig::get_buzzer_off(void) const{
    return buzzer_off_;
}

QString ReadConfig::get_buzzer_toggle(void) const{
    return buzzer_toggle_;
}

QString ReadConfig::get_buzzer_rhythm(void) const{
    return buzzer_rhythm_;
}

QString ReadConfig::get_buzzer_beep(void) const{
    return buzzer_beep_;
}

QString ReadConfig::get_config_fetch(void) const{
    return config_fetch_;
}

QString ReadConfig::get_config_mode_manual(void) const{
    return config_mode_manual_;
}

QString ReadConfig::get_config_mode_auto(void) const{
    return config_mode_auto_;
}

QString ReadConfig::get_config_turntable_ppr(void) const{
    return config_turntable_ppr_;
}

QString ReadConfig::get_config_lifter_ppr(void) const{
    return config_lifter_ppr_;
}

QString ReadConfig::get_brake_all(void) const{
    return brake_all_;
}

QString ReadConfig::get_brake_turntable(void) const{
    return brake_turntable_;
}

QString ReadConfig::get_brake_lifter(void) const{
    return brake_lifter_;
}

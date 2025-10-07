#include "sound_light_alert.h"
#include <QMessageBox>
#include "read_config.h"

SoundLightAlert::SoundLightAlert(QWidget *parent) : QWidget(parent) {
    setup_ui();
    connect_signals_slots();
}

SoundLightAlert::~SoundLightAlert() {

}

void SoundLightAlert::enable_btn_send(bool enable){
    if (enable) {
        btn_led_switch_->setEnabled(true);
        btn_led_toggle_->setEnabled(true);
        btn_led_flash_->setEnabled(true);
        btn_buzzer_switch_->setEnabled(true);
        btn_buzzer_toggle_->setEnabled(true);
        btn_buzzer_rhythm_->setEnabled(true);
        btn_buzzer_beep_->setEnabled(true);
    } else {
        btn_led_switch_->setEnabled(false);
        btn_led_toggle_->setEnabled(false);
        btn_led_flash_->setEnabled(false);
        btn_buzzer_switch_->setEnabled(false);
        btn_buzzer_toggle_->setEnabled(false);
        btn_buzzer_rhythm_->setEnabled(false);
        btn_buzzer_beep_->setEnabled(false);
    }
}

void SoundLightAlert::setup_ui() {
    gb_ = new QGroupBox("声光提醒", this);
    lbl_led_ = new QLabel("LED", this);
    lbl_buzzer_ = new QLabel("蜂鸣器", this);
    btn_led_switch_ = new QPushButton("On", this);
    btn_led_toggle_ = new QPushButton("Toggle", this);
    spin_led_flash_ = new QSpinBox(this);
    spin_led_flash_->setRange(1, 99);
    spin_led_flash_->setValue(3);
    btn_led_flash_ = new QPushButton("Blink", this);

    btn_buzzer_switch_ = new QPushButton("On", this);
    btn_buzzer_toggle_ = new QPushButton("Toggle", this);
    btn_buzzer_rhythm_ = new QPushButton("Rhythm", this);
    spin_buzzer_beep_ = new QSpinBox(this);
    spin_buzzer_beep_->setRange(1, 99);
    spin_buzzer_beep_->setValue(3);
    btn_buzzer_beep_ = new QPushButton("Beep", this);

    lyt_grid_ = new QGridLayout();
    lyt_grid_->addWidget(lbl_led_, 0, 0, 1, 1, Qt::AlignLeft);
    lyt_grid_->addWidget(btn_led_switch_, 0, 1, 1, 1, Qt::AlignLeft);
    lyt_grid_->addWidget(btn_led_toggle_, 0, 2, 1, 1, Qt::AlignLeft);
    lyt_grid_->addWidget(spin_led_flash_, 1, 1, 1, 1, Qt::AlignRight);
    lyt_grid_->addWidget(btn_led_flash_, 1, 2, 1, 1, Qt::AlignLeft);

    lyt_grid_->addWidget(lbl_buzzer_, 2, 0, 1, 1, Qt::AlignLeft);
    lyt_grid_->addWidget(btn_buzzer_switch_, 2, 1, 1, 1, Qt::AlignLeft);
    lyt_grid_->addWidget(btn_buzzer_toggle_, 2, 2, 1, 1, Qt::AlignLeft);
    lyt_grid_->addWidget(btn_buzzer_rhythm_, 3, 0, 1, 1, Qt::AlignLeft);
    lyt_grid_->addWidget(spin_buzzer_beep_, 3, 1, 1, 1, Qt::AlignRight);
    lyt_grid_->addWidget(btn_buzzer_beep_, 3, 2, 1, 1, Qt::AlignLeft);

    gb_->setLayout(lyt_grid_);

    lyt_main_ = new QVBoxLayout(this);
    lyt_main_->addWidget(gb_);
    setLayout(lyt_main_);

}

void SoundLightAlert::connect_signals_slots() {
    connect(btn_led_switch_, &QPushButton::clicked, this, &SoundLightAlert::on_btn_led_switch_clicked);
    connect(btn_led_toggle_, &QPushButton::clicked, this, &SoundLightAlert::on_btn_led_toggle_clicked);
    connect(btn_led_flash_, &QPushButton::clicked, this, &SoundLightAlert::on_btn_led_flash_clicked);
    connect(btn_buzzer_switch_, &QPushButton::clicked, this, &SoundLightAlert::on_btn_buzzer_switch_clicked);
    connect(btn_buzzer_toggle_, &QPushButton::clicked, this, &SoundLightAlert::on_btn_buzzer_toggle_clicked);
    connect(btn_buzzer_rhythm_, &QPushButton::clicked, this, &SoundLightAlert::on_btn_buzzer_rhythm_clicked);
    connect(btn_buzzer_beep_, &QPushButton::clicked, this, &SoundLightAlert::on_btn_buzzer_beep_clicked);
}

void SoundLightAlert::on_btn_led_switch_clicked() {
    QString packet_prefix = ReadConfig::getInstance().get_packet_prefix();
    QString packet_suffix = ReadConfig::getInstance().get_packet_suffix();

    QString command = (btn_led_switch_->text() == "On") ?
        (packet_prefix + ReadConfig::getInstance().get_led_on() + packet_suffix) :
        (packet_prefix + ReadConfig::getInstance().get_led_off()  + packet_suffix);
    emit sig_send_data(command.toUtf8());

    QString current_text = btn_led_switch_->text();
    QString new_text = (current_text == "On") ? ("Off") : ("On");
    btn_led_switch_->setText(new_text);

    // 设置背景色
    QString style = (new_text == "Off") ? "background-color: rgb(255, 182, 193);" : "";
    btn_led_switch_->setStyleSheet(style);
}

void SoundLightAlert::on_btn_led_toggle_clicked() {
    QString packet_prefix = ReadConfig::getInstance().get_packet_prefix();
    QString packet_suffix = ReadConfig::getInstance().get_packet_suffix();

    QString cmd = packet_prefix + ReadConfig::getInstance().get_led_toggle() + packet_suffix;
    emit sig_send_data(cmd.toUtf8());
}

void SoundLightAlert::on_btn_led_flash_clicked() {
    QString cmd = ReadConfig::getInstance().get_packet_prefix() + ReadConfig::getInstance().get_led_blink() +
        spin_led_flash_->text() + ReadConfig::getInstance().get_packet_suffix();
    emit sig_send_data(cmd.toUtf8());
}

void SoundLightAlert::on_btn_buzzer_switch_clicked() {
    QString packet_prefix = ReadConfig::getInstance().get_packet_prefix();
    QString packet_suffix = ReadConfig::getInstance().get_packet_suffix();

    QString command = (btn_buzzer_switch_->text() == "On") ?
        (packet_prefix + ReadConfig::getInstance().get_buzzer_on() + packet_suffix) :
        (packet_prefix + ReadConfig::getInstance().get_buzzer_off() + packet_suffix) ;
    emit sig_send_data(command.toUtf8());

    QString current_text = btn_buzzer_switch_->text();
    QString new_text = (current_text == "On") ? "Off" : "On";
    btn_buzzer_switch_->setText(new_text);

    // 设置背景色
    QString style = (new_text == "Off") ? "background-color: rgb(255, 182, 193);" : "";
    btn_buzzer_switch_->setStyleSheet(style);
}

void SoundLightAlert::on_btn_buzzer_toggle_clicked(){
    QString command = ReadConfig::getInstance().get_packet_prefix() +
        ReadConfig::getInstance().get_buzzer_toggle() +
        ReadConfig::getInstance().get_packet_suffix();
    emit sig_send_data(command.toUtf8());
}

void SoundLightAlert::on_btn_buzzer_rhythm_clicked(){
    QString command = ReadConfig::getInstance().get_packet_prefix() +
        ReadConfig::getInstance().get_buzzer_rhythm() +
        ReadConfig::getInstance().get_packet_suffix();
    emit sig_send_data(command.toUtf8());
}

void SoundLightAlert::on_btn_buzzer_beep_clicked(){
    QString command = ReadConfig::getInstance().get_packet_prefix() +
        ReadConfig::getInstance().get_buzzer_beep() + spin_buzzer_beep_->text() +
        ReadConfig::getInstance().get_packet_suffix();
    emit sig_send_data(command.toUtf8());
}

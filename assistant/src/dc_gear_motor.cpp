#include "dc_gear_motor.h"
#include "read_config.h"

DCGearMotor::DCGearMotor(QWidget *parent) : QWidget(parent) {
    setup_ui();
    connect_signals_slots();
}

DCGearMotor::~DCGearMotor() {

}

void DCGearMotor::enable_btn_send(bool enable){
    if (enable) {
        btn_forward_->setEnabled(true);
        btn_reverse_->setEnabled(true);
    } else {
        btn_forward_->setEnabled(false);
        btn_reverse_->setEnabled(false);
    }
}

void DCGearMotor::setup_ui() {
    gb_ = new QGroupBox("直流电机", this);
    lbl_forward_ = new QLabel("正转", this);
    lbl_reverse_ = new QLabel("反转", this);
    edit_forward_ = new QLineEdit(this);
    edit_forward_->setText("100");
    edit_reverse_ = new QLineEdit(this);
    edit_reverse_->setText("100");
    btn_forward_ = new QPushButton("正转", this);
    btn_reverse_ = new QPushButton("反转", this);

    lyt_btn_ = new QHBoxLayout();
    lyt_btn_->addWidget(btn_forward_);
    lyt_btn_->addWidget(btn_reverse_);

    lyt_form_ = new QFormLayout();
    lyt_form_->addRow(lbl_forward_, edit_forward_);
    lyt_form_->addRow(lbl_reverse_, edit_reverse_);
    lyt_form_->addRow(lyt_btn_);

    gb_->setLayout(lyt_form_);

    lyt_main_ = new QVBoxLayout(this);
    lyt_main_->addWidget(gb_);
    setLayout(lyt_main_);
}

void DCGearMotor::connect_signals_slots() {
    connect(btn_forward_, &QPushButton::clicked, this, &DCGearMotor::on_btn_forward_clicked);
    connect(btn_reverse_, &QPushButton::clicked, this, &DCGearMotor::on_btn_reverse_clicked);
}

void DCGearMotor::on_btn_forward_clicked() {
    QString cmd = ReadConfig::getInstance().get_packet_prefix() + ReadConfig::getInstance().get_gearmotor_clockwise() +
        edit_forward_->text() + ReadConfig::getInstance().get_packet_suffix();  // 直流电机正转命令
    emit sig_send_data(cmd.toUtf8());
}

void DCGearMotor::on_btn_reverse_clicked() {
    QString cmd = ReadConfig::getInstance().get_packet_prefix() + ReadConfig::getInstance().get_gearmotor_anticlockwise() +
        edit_reverse_->text() + ReadConfig::getInstance().get_packet_suffix();  // 直流电机反转命令
    emit sig_send_data(cmd.toUtf8());
}

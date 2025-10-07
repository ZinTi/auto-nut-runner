#include "stepper_ctl.h"
#include "read_config.h"

StepperCtl::StepperCtl(QWidget *parent) : QWidget(parent) {
    setup_ui();
    connect_signals_slots();
}

StepperCtl::~StepperCtl() {

}

void StepperCtl::setup_ui() {
    gb_ = new QGroupBox("步进控制", this);
    lbl_ppr_ = new QLabel("Pulse/rev", this);
    lbl_pulses_ = new QLabel("Pulses", this);
    lbl_degrees_ = new QLabel("Degrees°", this);
    edit_ppr_ = new QLineEdit("200", this);
    edit_ppr_->setStyleSheet(
        "QLineEdit {"
        "   background-color: rgb(30,100,255);"
        "   color: rgb(250,250,210);"
        "   border: none;"
        "}"
        );
    edit_ppr_->setReadOnly(true);
    edit_pulses_ = new QLineEdit(this);
    edit_pulses_->setStyleSheet(
        "QLineEdit {"
        "   background-color: rgb(30,100,255);"
        "   color: rgb(250,250,210);"
        "   border: none;"
        "}"
        );
    edit_pulses_->setReadOnly(true);
    edit_degrees_ = new QLineEdit("30.00", this);
    btn_forward_ = new QPushButton("正转", this);
    btn_reverse_ = new QPushButton("反转", this);

    lyt_btn_ = new QHBoxLayout();
    lyt_btn_->addWidget(btn_forward_);
    lyt_btn_->addWidget(btn_reverse_);

    lyt_form_ = new QFormLayout();
    lyt_form_->addRow(lbl_ppr_, edit_ppr_);
    lyt_form_->addRow(lbl_pulses_, edit_pulses_);
    lyt_form_->addRow(lbl_degrees_, edit_degrees_);
    lyt_form_->addRow(lyt_btn_);

    gb_->setLayout(lyt_form_);

    lyt_main_ = new QVBoxLayout(this);
    lyt_main_->addWidget(gb_);
    setLayout(lyt_main_);

    connect(edit_ppr_, &QLineEdit::textChanged, this, &StepperCtl::update_pulses);
    connect(edit_degrees_, &QLineEdit::textChanged, this, &StepperCtl::update_pulses);
    update_pulses(); // 初始计算
}

void StepperCtl::connect_signals_slots() {
    connect(btn_forward_, &QPushButton::clicked, this, &StepperCtl::on_btn_forward_clicked);
    connect(btn_reverse_, &QPushButton::clicked, this, &StepperCtl::on_btn_reverse_clicked);
}

void StepperCtl::on_btn_forward_clicked() {
    QString cmd = ReadConfig::getInstance().get_packet_prefix() + clockwise_cmd_ +
                 edit_degrees_->text() + ReadConfig::getInstance().get_packet_suffix();  // 正转命令格式
    emit sig_send_data(cmd.toUtf8());
}

void StepperCtl::on_btn_reverse_clicked() {
    QString cmd = ReadConfig::getInstance().get_packet_prefix() + anticlockwise_cmd_ +
                 edit_degrees_->text() + ReadConfig::getInstance().get_packet_suffix();  // 反转命令格式
    emit sig_send_data(cmd.toUtf8());
}

void StepperCtl::set_groupbox_title(const QString &title) {
    gb_->setTitle(title);
}

void StepperCtl::set_pulse_per_rev(uint16_t ppr_value) {
    edit_ppr_->setText(QString::number(ppr_value));
}

void StepperCtl::set_btn_text(const QString &left_btn_text, const QString &right_btn_text){
    btn_forward_->setText(left_btn_text);
    btn_reverse_->setText(right_btn_text);
}

void StepperCtl::set_btn_cmd(const QString &clockwise_cmd, const QString &anticlockwise_cmd){
    clockwise_cmd_ = clockwise_cmd;
    anticlockwise_cmd_ = anticlockwise_cmd;
}

void StepperCtl::enable_btn_send(bool enable){
    if (enable) {
        btn_forward_->setEnabled(true);
        btn_reverse_->setEnabled(true);
    } else {
        btn_forward_->setEnabled(false);
        btn_reverse_->setEnabled(false);
    }
}

void StepperCtl::update_pulses() {
    bool ok1, ok2;
    int ppr = edit_ppr_->text().toInt(&ok1);
    double degrees = edit_degrees_->text().toDouble(&ok2);

    if (ok1 && ok2) {
        double pulses = (degrees / 360.0) * ppr;
        edit_pulses_->setText(QString::number(pulses, 'f', 2));
    }
}

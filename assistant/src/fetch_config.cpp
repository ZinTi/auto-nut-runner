#include "fetch_config.h"
#include <QMessageBox>
#include "read_config.h"

FetchConfig::FetchConfig(QWidget *parent) : QWidget(parent) {
    current_mode_ = 0;
    setup_ui();
    connect_signals_slots();
}

FetchConfig::~FetchConfig() {
}

void FetchConfig::enable_btn_send(bool enable){
    btn_fetch_sysclk_->setEnabled(enable);
    btn_fetch_ppr_->setEnabled(enable);
    btn_fetch_speed_->setEnabled(enable);
    btn_fetch_mode_->setEnabled(enable);
}

void FetchConfig::setup_ui() {
    // 创建组件
    gb_gen_setting_ = new QGroupBox("获取配置", this);
    btn_fetch_sysclk_ = new QPushButton("查询系统时钟", gb_gen_setting_);
    btn_fetch_mode_ = new QPushButton("查询模式", gb_gen_setting_);
    btn_fetch_ppr_ = new QPushButton("查询PPR", gb_gen_setting_);
    btn_fetch_speed_ = new QPushButton("查询速度", gb_gen_setting_);

    // 创建布局
    lyt_line1_ = new QHBoxLayout();
    lyt_line1_->addWidget(btn_fetch_sysclk_);
    lyt_line1_->addWidget(btn_fetch_mode_);
    lyt_line2_ = new QHBoxLayout();
    lyt_line2_->addWidget(btn_fetch_ppr_);
    lyt_line2_->addWidget(btn_fetch_speed_);
    lyt_v_ = new QVBoxLayout();
    lyt_v_->addLayout(lyt_line1_);
    lyt_v_->addLayout(lyt_line2_);

    // 设置分组框布局
    gb_gen_setting_->setLayout(lyt_v_);

    // 设置主布局
    lyt_main_ = new QVBoxLayout(this);
    lyt_main_->addWidget(gb_gen_setting_);
    setLayout(lyt_main_);
}

void FetchConfig::connect_signals_slots() {
    connect(btn_fetch_sysclk_, &QPushButton::clicked, this, &FetchConfig::on_btn_fetch_sysclk_clicked);
    connect(btn_fetch_ppr_, &QPushButton::clicked, this, &FetchConfig::on_btn_fetch_ppr_clicked);
    connect(btn_fetch_speed_, &QPushButton::clicked, this, &FetchConfig::on_btn_fetch_speed_clicked);
    connect(btn_fetch_mode_, &QPushButton::clicked, this, &FetchConfig::on_btn_fetch_mode_clicked);
}

void FetchConfig::on_btn_fetch_sysclk_clicked() {
    QString command = "^GS$";  // 获取系统时钟命令
    emit sig_send_data(command.toUtf8());
    QMessageBox::information(this, "Fetch SysClk", "获取系统时钟");
}

void FetchConfig::on_btn_fetch_ppr_clicked() {
    QString packet_prefix = ReadConfig::getInstance().get_packet_prefix();
    QString packet_suffix = ReadConfig::getInstance().get_packet_suffix();

    QString command = packet_prefix + ReadConfig::getInstance().get_config_fetch() + packet_suffix;  // 获取PPR命令
    emit sig_send_data(command.toUtf8());
}

void FetchConfig::on_btn_fetch_speed_clicked() {
    QString packet_prefix = ReadConfig::getInstance().get_packet_prefix();
    QString packet_suffix = ReadConfig::getInstance().get_packet_suffix();

    // QString command = packet_prefix + ReadConfig::getInstance().get_config_speed() + packet_suffix;  // 获取PPR命令
    // emit sig_send_data(command.toUtf8());
    QMessageBox::information(this, "Fetch Speed", "功能未实现");
}

void FetchConfig::on_btn_fetch_mode_clicked() {
    QMessageBox::information(this, "Fetch Mode", "功能未实现");
}

#include "main_window.h"
#include "read_config.h" // 读取配置文件

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setup_ui();
    connect_signals_slots();

    on_tx_encoding_changed(); // 初始化发送显示格式
    on_serial_state_changed(false); // 初始状态设置为禁用
}

MainWindow::~MainWindow() {
    // 断开所有信号槽连接
    disconnect(microstep_conf_t_, nullptr, this, nullptr);
    disconnect(microstep_conf_l_, nullptr, this, nullptr);
    disconnect(batch_cmd_, nullptr, serial_port_conf_, nullptr);

    delete microstep_conf_t_;
    delete microstep_conf_l_;
    delete serial_port_conf_;
}

void MainWindow::setup_ui() {
    // 创建中央部件
    wgt_central_ = new QWidget(this);
    setCentralWidget(wgt_central_);

    lbl_title_ = new QLabel("自动拧螺丝设备-调试配置助手", wgt_central_);
    lbl_title_->setAlignment(Qt::AlignHCenter);

    microstep_conf_t_ = new MicrostepConfig(wgt_central_);
    microstep_conf_t_->set_groupbox_title("转台微步配置（DM542）");
    microstep_conf_t_->set_ppr_cmd(ReadConfig::getInstance().get_config_turntable_ppr());
    microstep_conf_l_ = new MicrostepConfig(wgt_central_);
    microstep_conf_l_->set_groupbox_title("升降微步配置（DM542）");
    microstep_conf_l_->set_ppr_cmd(ReadConfig::getInstance().get_config_lifter_ppr());

    serial_port_conf_ = new SerialPortConfig(wgt_central_);

    detail_display_ = new DetailDisplay(wgt_central_);
    batch_cmd_ = new BatchCommand(wgt_central_);

    stepper_ctl_t_ = new StepperCtl(wgt_central_);
    stepper_ctl_t_->set_groupbox_title("转台步进控制");
    stepper_ctl_t_->set_btn_text("顺时针", "逆时针");
    stepper_ctl_t_->set_btn_cmd(
        ReadConfig::getInstance().get_turntable_clockwise(),
        ReadConfig::getInstance().get_turntable_anticlockwise()
    );
    stepper_ctl_l_ = new StepperCtl(wgt_central_);
    stepper_ctl_l_->set_groupbox_title("升降步进控制");
    stepper_ctl_l_->set_btn_text("上升", "下降");
    stepper_ctl_l_->set_btn_cmd(
        ReadConfig::getInstance().get_lifter_up(),
        ReadConfig::getInstance().get_lifter_down()
    );
    dc_motor_ = new DCGearMotor(wgt_central_);
    sound_light_ = new SoundLightAlert(wgt_central_);

    lyt_ctl_ = new QHBoxLayout();
    lyt_ctl_->addWidget(stepper_ctl_t_);
    lyt_ctl_->addWidget(stepper_ctl_l_);
    lyt_ctl_->addWidget(dc_motor_);
    lyt_ctl_->addWidget(sound_light_);

    lyt_left_ = new QVBoxLayout();
    lyt_left_->addWidget(detail_display_);
    lyt_left_->addWidget(batch_cmd_);
    lyt_left_->addLayout(lyt_ctl_);

    fetch_config_ = new FetchConfig(wgt_central_);
    g_set_ = new GeneralSetting(wgt_central_);

    lyt_right_ = new QVBoxLayout();
    lyt_right_->addWidget(serial_port_conf_);
    lyt_right_->addWidget(g_set_);

    lyt_work_ = new QHBoxLayout();
    lyt_work_->addLayout(lyt_left_, 3);
    lyt_work_->addLayout(lyt_right_, 1);

    lyt_bottom_ = new QHBoxLayout();
    lyt_bottom_->addWidget(microstep_conf_t_);
    lyt_bottom_->addWidget(microstep_conf_l_);
    lyt_bottom_->addWidget(fetch_config_);

    lyt_main_ = new QVBoxLayout(wgt_central_);
    lyt_main_->addWidget(lbl_title_);
    lyt_main_->addLayout(lyt_work_);
    lyt_main_->addLayout(lyt_bottom_);

    // 设置中央部件的布局
    wgt_central_->setLayout(lyt_main_);
}

void MainWindow::connect_signals_slots(){
    // PPR改变
    connect(microstep_conf_t_, &MicrostepConfig::sig_ppr_changed, this, &MainWindow::on_microstep_t_ppr_changed);
    connect(microstep_conf_l_, &MicrostepConfig::sig_ppr_changed, this, &MainWindow::on_microstep_l_ppr_changed);

    // 连接批处理命令发送信号到串口和显示控件
    connect(batch_cmd_, &BatchCommand::sig_send_data, serial_port_conf_, &SerialPortConfig::send_data);
    connect(batch_cmd_, &BatchCommand::sig_send_data, this, &MainWindow::on_data_sent);

    // 连接串口接收信号到显示控件
    connect(serial_port_conf_, &SerialPortConfig::data_received, this, &MainWindow::on_data_received);

    // 连接发送区编码改变信号
    connect(batch_cmd_->get_encoding_config(), &EncodingConfig::sig_encoding_changed, this, &MainWindow::on_tx_encoding_changed);

    // 发送（两个步进电机、一个直流电机、一个声光提醒、两个微步配置）控制信号到串口和显示控件、
    connect(stepper_ctl_t_, &StepperCtl::sig_send_data, serial_port_conf_, &SerialPortConfig::send_data);
    connect(stepper_ctl_t_, &StepperCtl::sig_send_data, this, &MainWindow::on_data_sent);
    connect(stepper_ctl_l_, &StepperCtl::sig_send_data, serial_port_conf_, &SerialPortConfig::send_data);
    connect(stepper_ctl_l_, &StepperCtl::sig_send_data, this, &MainWindow::on_data_sent);
    connect(dc_motor_, &DCGearMotor::sig_send_data, serial_port_conf_, &SerialPortConfig::send_data);
    connect(dc_motor_, &DCGearMotor::sig_send_data, this, &MainWindow::on_data_sent);
    connect(sound_light_, &SoundLightAlert::sig_send_data, serial_port_conf_, &SerialPortConfig::send_data);
    connect(sound_light_, &SoundLightAlert::sig_send_data, this, &MainWindow::on_data_sent);
    connect(microstep_conf_t_, &MicrostepConfig::sig_send_data, serial_port_conf_, &SerialPortConfig::send_data);
    connect(microstep_conf_t_, &MicrostepConfig::sig_send_data, this, &MainWindow::on_data_sent);
    connect(microstep_conf_l_, &MicrostepConfig::sig_send_data, serial_port_conf_, &SerialPortConfig::send_data);
    connect(microstep_conf_l_, &MicrostepConfig::sig_send_data, this, &MainWindow::on_data_sent);

    // 配置、获取配置信号到串口和显示控件
    connect(fetch_config_, &FetchConfig::sig_send_data, serial_port_conf_, &SerialPortConfig::send_data);
    connect(fetch_config_, &FetchConfig::sig_send_data, this, &MainWindow::on_data_sent);
    connect(g_set_, &GeneralSetting::sig_send_data, serial_port_conf_, &SerialPortConfig::send_data);
    connect(g_set_, &GeneralSetting::sig_send_data, this, &MainWindow::on_data_sent);

    connect(serial_port_conf_, &SerialPortConfig::serial_opened, this, [this]() {
        this->on_serial_state_changed(true);
    });
    connect(serial_port_conf_, &SerialPortConfig::serial_closed, this, [this]() {
        this->on_serial_state_changed(false);
    });
}

void MainWindow::on_microstep_t_ppr_changed() {
    uint16_t ppr = microstep_conf_t_->get_pulse_per_rev();
    stepper_ctl_t_->set_pulse_per_rev(ppr);
}

void MainWindow::on_microstep_l_ppr_changed() {
    uint16_t ppr = microstep_conf_l_->get_pulse_per_rev();
    stepper_ctl_l_->set_pulse_per_rev(ppr);
}

void MainWindow::on_data_sent(const QByteArray &data) {
    detail_display_->append_message(data, true);
}

void MainWindow::on_data_received(const QByteArray &data) {
    detail_display_->append_message(data, false);
}

void MainWindow::on_tx_encoding_changed() {
    EncodingConfig *tx_enc = batch_cmd_->get_encoding_config();
    detail_display_->set_tx_display_format(tx_enc->get_mode(), tx_enc->get_encoding());
}

void MainWindow::on_serial_state_changed(bool opened) {
    microstep_conf_t_->enable_btn_send(opened);
    microstep_conf_l_->enable_btn_send(opened);
    g_set_->enable_btn_send(opened);
    batch_cmd_->enable_btn_send(opened);
    stepper_ctl_t_->enable_btn_send(opened);
    stepper_ctl_l_->enable_btn_send(opened);
    dc_motor_->enable_btn_send(opened);
    sound_light_->enable_btn_send(opened);
    fetch_config_->enable_btn_send(opened);
}

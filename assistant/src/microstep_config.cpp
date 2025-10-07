/****************************************
DM542 驱动
=== Pulse/rev Table ===
Pulse/rev	SW5		SW6		SW7		SW8		binary		decimal		microsteps		remark
   200		on		on		on		on		00001111	0x0F		  \				1.8°
   400		off		on		on		on		00000111	0x07		  2				0.9°
   800		on		off		on		on		00001011	0x0B		  4				0.45°
  1600		off		off		on		on		00000011	0x03		  8				0.225°
  3200		on		on		off		on		00001101	0x0D		 16				0.1125°
  6400		off		on		off		on		00000101	0x05		 32				0.05625°
 12800		on		off		off		on		00001001	0x09		 64				0.028125°
 25600		off		off		off		on		00000001	0x01		128				0.0140625°
  1000		on		on		on		off		00001110	0x0E		  5				0.36°
  2000		off		on		on		off		00000110	0x06		 10				0.18°
  4000		on		off		on		off		00001010	0x0A		 20				0.09°
  5000		off		off		on		off		00000010	0x02		 25				0.072°
  8000		on		on		off		off		00001100	0x0C		 40				0.045°
 10000		off		on		off		off		00000100	0x04		 50				0.036°
 20000		on		off		off		off		00001000	0x08		100				0.018°
 25000		off		off		off		off		00000000	0x00		125				0.0144°

*******************************************/

#include "microstep_config.h"
#include <QMessageBox>
#include "read_config.h"

// 静态映射数组定义
const int MicrostepConfig::index_to_sw_state[16] = {15, 7, 11, 3, 13, 5, 9, 1, 14, 6, 10, 2, 12, 4, 8, 0};
const int MicrostepConfig::sw_state_to_index[16] = {15, 7, 11, 3, 13, 5, 9, 1, 14, 6, 10, 2, 12, 4, 8, 0};


MicrostepConfig::MicrostepConfig(QWidget *parent) : QWidget(parent) {
    setup_ui();
    connect_signals_slots();
}

MicrostepConfig::~MicrostepConfig() {

}


void MicrostepConfig::setup_ui() {
    gb_microstep_ = new QGroupBox("步进电机细分微步设置", this);
    btn_sw_ = new QPushButton("拨码全开", this); // “全开/全关” 按钮，即 “全选/全不选” 切换按钮
    chkb_sw5_ = new QCheckBox("sw5", this);
    chkb_sw6_ = new QCheckBox("sw6", this);
    chkb_sw7_ = new QCheckBox("sw7", this);
    chkb_sw8_ = new QCheckBox("sw8", this);
    lbl_ppr_ = new QLabel("Pulse/rev", this);  // pulse per revolution
    cbob_ppr_ = new QComboBox(this);
    cbob_ppr_->addItems({
        "200","400","800","1600","3200","6400","12800","25600",
        "1000","2000","4000","5000","8000","10000","20000","25000"
    });
    btn_config_ = new QPushButton("配置", this);
    lyt_sw_ = new QHBoxLayout();
    lyt_sw_->addWidget(chkb_sw5_);
    lyt_sw_->addWidget(chkb_sw6_);
    lyt_sw_->addWidget(chkb_sw7_);
    lyt_sw_->addWidget(chkb_sw8_);
    lyt_grid_ = new QGridLayout();
    lyt_grid_->addWidget(btn_sw_, 0, 0, 1, 1);
    lyt_grid_->addLayout(lyt_sw_, 0, 1, 1, 2);
    lyt_grid_->addWidget(lbl_ppr_, 1, 0, 1, 1);
    lyt_grid_->addWidget(cbob_ppr_, 1, 1, 1, 1);
    lyt_grid_->addWidget(btn_config_, 1, 2, 1, 1);
    // 设置分组框的布局
    gb_microstep_->setLayout(lyt_grid_);

    // 创建主布局并添加分组框
    lyt_main_ = new QVBoxLayout(this);
    lyt_main_->addWidget(gb_microstep_);
    setLayout(lyt_main_);

    // 默认值
    chkb_sw5_->setCheckState(Qt::Checked);
    chkb_sw6_->setCheckState(Qt::Checked);
    chkb_sw7_->setCheckState(Qt::Checked);
    chkb_sw8_->setCheckState(Qt::Checked);
    cbob_ppr_->setCurrentText("200");
    btn_sw_->setText("拨码全关"); // 因为默认是全部勾选状态
}

void MicrostepConfig::connect_signals_slots() {
    // 连接“拨码全开/关”按钮
    connect(btn_sw_, &QPushButton::clicked, this, &MicrostepConfig::on_btn_sw_clicked);
    // 连接复选框信号
    connect(chkb_sw5_, &QCheckBox::checkStateChanged, this, &MicrostepConfig::on_sw_checkbox_changed);
    connect(chkb_sw6_, &QCheckBox::checkStateChanged, this, &MicrostepConfig::on_sw_checkbox_changed);
    connect(chkb_sw7_, &QCheckBox::checkStateChanged, this, &MicrostepConfig::on_sw_checkbox_changed);
    connect(chkb_sw8_, &QCheckBox::checkStateChanged, this, &MicrostepConfig::on_sw_checkbox_changed);
    // 连接组合框信号
    connect(cbob_ppr_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MicrostepConfig::on_ppr_combobox_changed);
    // 连接配置按钮
    connect(btn_config_, &QPushButton::clicked, this, &MicrostepConfig::on_btn_config_clicked);
}

void MicrostepConfig::set_groupbox_title(const QString &title) {
    gb_microstep_->setTitle(title);
}

uint8_t MicrostepConfig::get_switch_state() {
    uint8_t state = 0;
    state |= (chkb_sw5_->isChecked() ? 1 : 0) << 3;
    state |= (chkb_sw6_->isChecked() ? 1 : 0) << 2;
    state |= (chkb_sw7_->isChecked() ? 1 : 0) << 1;
    state |= (chkb_sw8_->isChecked() ? 1 : 0) << 0;
    return state;
}

uint16_t MicrostepConfig::get_pulse_per_rev() {
    return static_cast<uint16_t>(cbob_ppr_->currentText().toUInt());
}

void MicrostepConfig::set_ppr_cmd(const QString &cmd){
    ppr_cmd_ = cmd;
}

void MicrostepConfig::enable_btn_send(bool enable) {
    btn_config_->setEnabled(enable);
}

void MicrostepConfig::on_sw_checkbox_changed() {
    // 计算当前开关状态对应的索引
    uint8_t currentState = get_switch_state();
    int index = sw_state_to_index[currentState];

    // 阻塞组合框信号避免循环触发
    cbob_ppr_->blockSignals(true);
    cbob_ppr_->setCurrentIndex(index);
    cbob_ppr_->blockSignals(false);
    emit sig_ppr_changed(get_pulse_per_rev());
}

void MicrostepConfig::on_ppr_combobox_changed(int index) {
    // 获取对应索引的开关状态
    int state = index_to_sw_state[index];

    // 阻塞复选框信号避免循环触发
    chkb_sw5_->blockSignals(true);
    chkb_sw6_->blockSignals(true);
    chkb_sw7_->blockSignals(true);
    chkb_sw8_->blockSignals(true);

    // 设置各个复选框的状态
    chkb_sw5_->setChecked(state & (1 << 3));
    chkb_sw6_->setChecked(state & (1 << 2));
    chkb_sw7_->setChecked(state & (1 << 1));
    chkb_sw8_->setChecked(state & (1 << 0));

    // 解除信号阻塞
    chkb_sw5_->blockSignals(false);
    chkb_sw6_->blockSignals(false);
    chkb_sw7_->blockSignals(false);
    chkb_sw8_->blockSignals(false);
    emit sig_ppr_changed(get_pulse_per_rev());
}

void MicrostepConfig::on_btn_sw_clicked() {
    // 检查当前是否已经是全开状态
    bool allChecked = chkb_sw5_->isChecked() &&
                      chkb_sw6_->isChecked() &&
                      chkb_sw7_->isChecked() &&
                      chkb_sw8_->isChecked();

    // 阻塞复选框信号避免触发on_sw_checkbox_changed
    chkb_sw5_->blockSignals(true);
    chkb_sw6_->blockSignals(true);
    chkb_sw7_->blockSignals(true);
    chkb_sw8_->blockSignals(true);

    if (allChecked) {
        // 如果是全开状态，则全关
        chkb_sw5_->setChecked(false);
        chkb_sw6_->setChecked(false);
        chkb_sw7_->setChecked(false);
        chkb_sw8_->setChecked(false);
        btn_sw_->setText("拨码全开");
    } else {
        // 如果不是全开状态，则全开
        chkb_sw5_->setChecked(true);
        chkb_sw6_->setChecked(true);
        chkb_sw7_->setChecked(true);
        chkb_sw8_->setChecked(true);
        btn_sw_->setText("拨码全关");
    }

    // 解除信号阻塞
    chkb_sw5_->blockSignals(false);
    chkb_sw6_->blockSignals(false);
    chkb_sw7_->blockSignals(false);
    chkb_sw8_->blockSignals(false);

    // 手动触发复选框变化信号，确保组合框同步更新
    on_sw_checkbox_changed();
}

void MicrostepConfig::on_btn_config_clicked() {
    QString cmd = ReadConfig::getInstance().get_packet_prefix() +
                ppr_cmd_ + cbob_ppr_->currentText() +
                ReadConfig::getInstance().get_packet_suffix();  // 反转命令格式
    emit sig_send_data(cmd.toUtf8());
}

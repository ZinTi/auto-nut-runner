#include "general_setting.h"
#include <QMessageBox>
#include "read_config.h"

GeneralSetting::GeneralSetting(QWidget *parent) : QWidget(parent) {
    current_mode_ = OperationMode::Auto;
    setup_ui();
    connect_signals_slots();
}

GeneralSetting::~GeneralSetting() {
}

void GeneralSetting::enable_btn_send(bool enable){
    btn_emergency_->setEnabled(enable);
    btn_mode_->setEnabled(enable);
    btn_custom1_->setEnabled(enable);
    btn_custom2_->setEnabled(enable);
    btn_custom3_->setEnabled(enable);
    btn_custom4_->setEnabled(enable);
    btn_custom5_->setEnabled(enable);
}

void GeneralSetting::setup_ui() {
    // 创建组件
    gb_gen_setting_ = new QGroupBox("通用操作", this);
    btn_mode_ = new QPushButton(gb_gen_setting_);
    if (current_mode_ == OperationMode::Auto) {
        this->btn_mode_->setText("Auto");
    } else if (current_mode_ == OperationMode::Manual) {
        this->btn_mode_->setText("Manual");
    } else {
        this->current_mode_ = OperationMode::Manual;
        this->btn_mode_->setText("Manual");
    }
    btn_emergency_ = new QPushButton("急停", gb_gen_setting_);
    btn_emergency_->setStyleSheet(
        "QPushButton {"
        "   background-color: red;"
        "   color: white;"
        "   border: none;"
        "   padding: 6px;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #FFA500;" // 橙黄色
        "}"
        "QPushButton:pressed {"
        "   background-color: #FF8C00;" // 按下时稍暗的橙色
        "}"
    );

    QString prefix = ReadConfig::getInstance().get_packet_prefix();
    QString suffix = ReadConfig::getInstance().get_packet_suffix();
    txt_tips_ = new QTextEdit(
         QString(
            "下方是自定义控件：<br/>"
            "1. 左侧为指令，右侧按钮发送；<br/>"
            "2. 底部控件可修改按钮文本；<br/>"
            "3. 指令以 %1 为首，以 %2 结尾；<br/>"
         ).arg(prefix).arg(suffix),
         this
    );
    txt_tips_->setPlaceholderText("备注");

    edit_custom1_ = new QLineEdit("^XX$", this);
    edit_custom2_ = new QLineEdit("^XX$", this);
    edit_custom3_ = new QLineEdit("^XX$", this);
    edit_custom4_ = new QLineEdit("^XX$", this);
    edit_custom5_ = new QLineEdit("^XX$", this);
    edit_custom1_->setFixedWidth(100);
    edit_custom2_->setFixedWidth(100);
    edit_custom3_->setFixedWidth(100);
    edit_custom4_->setFixedWidth(100);
    edit_custom5_->setFixedWidth(100);
    btn_custom1_ = new QPushButton("自定义1", this);
    btn_custom2_ = new QPushButton("自定义2", this);
    btn_custom3_ = new QPushButton("自定义3", this);
    btn_custom4_ = new QPushButton("自定义4", this);
    btn_custom5_ = new QPushButton("自定义5", this);

    cbob_select_btn_ = new QComboBox(this);
    cbob_select_btn_->addItems({"1","2","3","4","5"});
    edit_backup_ = new QLineEdit(this);
    edit_backup_->setPlaceholderText("按键功能");
    btn_ch_text_ = new QPushButton("备注", this);

    // 创建布局
    lyt_grid_ = new QGridLayout();
    lyt_grid_->addWidget(btn_emergency_, 0, 0, 2, 2, Qt::AlignTop);
    lyt_grid_->addWidget(btn_mode_, 2, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);

    lyt_form_ = new QFormLayout();
    lyt_form_->addRow(txt_tips_);
    lyt_form_->addRow(edit_custom1_, btn_custom1_);
    lyt_form_->addRow(edit_custom2_, btn_custom2_);
    lyt_form_->addRow(edit_custom3_, btn_custom3_);
    lyt_form_->addRow(edit_custom4_, btn_custom4_);
    lyt_form_->addRow(edit_custom5_, btn_custom5_);

    lyt_h_ = new QHBoxLayout();
    lyt_h_->addWidget(cbob_select_btn_);
    lyt_h_->addWidget(edit_backup_);
    lyt_h_->addWidget(btn_ch_text_);

    lyt_v_ = new QVBoxLayout();
    lyt_v_->setAlignment(Qt::AlignTop);
    lyt_v_->addLayout(lyt_grid_);
    lyt_v_->addLayout(lyt_form_);
    lyt_v_->addLayout(lyt_h_);

    // 设置分组框布局
    gb_gen_setting_->setLayout(lyt_v_);

    // 设置主布局
    lyt_main_ = new QVBoxLayout(this);
    lyt_main_->addWidget(gb_gen_setting_);
    setLayout(lyt_main_);
}

void GeneralSetting::connect_signals_slots() {
    connect(btn_emergency_, &QPushButton::clicked, this, &GeneralSetting::on_btn_emergency_clicked);
    connect(btn_mode_, &QPushButton::clicked, this, &GeneralSetting::on_switch_mode);
    connect(btn_ch_text_, &QPushButton::clicked, this, &GeneralSetting::on_btn_ch_text_clicked);

    // 连接自定义按钮
    signal_mapper_ = new QSignalMapper(this);
    connect(btn_custom1_, &QPushButton::clicked, signal_mapper_, qOverload<>(&QSignalMapper::map));
    connect(btn_custom2_, &QPushButton::clicked, signal_mapper_, qOverload<>(&QSignalMapper::map));
    connect(btn_custom3_, &QPushButton::clicked, signal_mapper_, qOverload<>(&QSignalMapper::map));
    connect(btn_custom4_, &QPushButton::clicked, signal_mapper_, qOverload<>(&QSignalMapper::map));
    connect(btn_custom5_, &QPushButton::clicked, signal_mapper_, qOverload<>(&QSignalMapper::map));

    signal_mapper_->setMapping(btn_custom1_, 0);
    signal_mapper_->setMapping(btn_custom2_, 1);
    signal_mapper_->setMapping(btn_custom3_, 2);
    signal_mapper_->setMapping(btn_custom4_, 3);
    signal_mapper_->setMapping(btn_custom5_, 4);

    connect(signal_mapper_, &QSignalMapper::mappedInt, this, &GeneralSetting::on_custom_button_clicked);
}

OperationMode GeneralSetting::on_switch_mode() {
    if (current_mode_ == OperationMode::Auto) {
        current_mode_ = OperationMode::Manual;
        btn_mode_->setText("Manual");
        QString cmd = ReadConfig::getInstance().get_packet_prefix() +
            ReadConfig::getInstance().get_config_mode_auto() +
            ReadConfig::getInstance().get_packet_suffix();
        emit sig_send_data(cmd.toUtf8());
    } else if (current_mode_ == OperationMode::Manual) {
        current_mode_ = OperationMode::Auto;
        this->btn_mode_->setText("Auto");
        QString cmd = ReadConfig::getInstance().get_packet_prefix() +
            ReadConfig::getInstance().get_config_mode_manual() +
            ReadConfig::getInstance().get_packet_suffix();
        emit sig_send_data(cmd.toUtf8());
    }
    return current_mode_;
}

void GeneralSetting::on_btn_emergency_clicked() {
    QString cmd = ReadConfig::getInstance().get_packet_prefix() +
        ReadConfig::getInstance().get_brake_all() +
        ReadConfig::getInstance().get_packet_suffix();
    emit sig_send_data(cmd.toUtf8());
}

void GeneralSetting::on_custom_button_clicked(int index) {
    QLineEdit* edit = nullptr;
    switch (index) {
    case 0: edit = edit_custom1_; break;
    case 1: edit = edit_custom2_; break;
    case 2: edit = edit_custom3_; break;
    case 3: edit = edit_custom4_; break;
    case 4: edit = edit_custom5_; break;
    default: break;
    }

    if (edit && !validate_command(edit->text())) {
        QMessageBox::warning(this, "格式错误", "指令必须以^开头，以$结尾");
        return;
    }

    emit sig_send_data(edit->text().toUtf8());
}

void GeneralSetting::on_btn_ch_text_clicked() {
    int index = cbob_select_btn_->currentIndex();
    QString new_text = edit_backup_->text();

    if (new_text.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "按钮文本不能为空");
        return;
    }

    switch (index) {
    case 0: btn_custom1_->setText(new_text); break;
    case 1: btn_custom2_->setText(new_text); break;
    case 2: btn_custom3_->setText(new_text); break;
    case 3: btn_custom4_->setText(new_text); break;
    case 4: btn_custom5_->setText(new_text); break;
    default: break;
    }
}

bool GeneralSetting::validate_command(const QString &command) {
    return command.startsWith('^') && command.endsWith('$');
}

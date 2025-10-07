#include "batch_command.h"
#include <QStringConverter>
#include "text_converter.h"
#include "read_config.h"

BatchCommand::BatchCommand(QWidget *parent) : QWidget(parent) {
    setup_ui();

    is_hex_updating_ = false;
    is_text_updating_ = false;

    // 连接信号槽
    connect(txt_hex_, &QTextEdit::textChanged, this, &BatchCommand::on_txt_hex_text_changed);
    connect(txt_text_, &QTextEdit::textChanged, this, &BatchCommand::on_txt_text_text_changed);
    connect(enc_conf_tx_, &EncodingConfig::sig_encoding_changed, this, &BatchCommand::on_encoding_changed);
    connect(btn_clear_, &QPushButton::clicked, this, &BatchCommand::on_btn_clear_clicked);
    connect(btn_send_, &QPushButton::clicked, this, &BatchCommand::on_btn_send_clicked);

    // 连接所有控制按钮到同一个槽
    QList<QPushButton*> control_buttons = {
        btn_t_forward_, btn_t_reverse_, btn_l_forward_, btn_l_reverse_,
        btn_rep_begin_, btn_rep_end_, btn_delay_
    };
    for (auto btn : control_buttons) {
        connect(btn, &QPushButton::clicked, this, &BatchCommand::on_ctl_btn_clicked);
    }

    // 禁用未实现的功能按键
    spin_repetition_->setEnabled(false);
    edit_delay_->setEnabled(false);
    btn_rep_begin_->setEnabled(false);
    btn_rep_end_->setEnabled(false);
    btn_delay_->setEnabled(false);
}

BatchCommand::~BatchCommand() {
}

void BatchCommand::enable_btn_send(bool enable){
    if (enable) {
        btn_send_->setEnabled(true);
    } else {
        btn_send_->setEnabled(false);
    }
}

EncodingConfig* BatchCommand::get_encoding_config() const {
    return enc_conf_tx_;
}

void BatchCommand::setup_ui() {
    gb_ = new QGroupBox("发送区（批处理命令）", this);
    txt_hex_ = new HexTextEdit(gb_);
    txt_hex_->setPlaceholderText("Hexadecimal");
    txt_text_ = new QTextEdit(gb_);
    txt_text_->setPlaceholderText("Text");
    enc_conf_tx_ = new EncodingConfig(gb_);
    enc_conf_tx_->set_group_box_title("字符编码");
    enc_conf_tx_->set_mode_text("发送模式");
    btn_clear_ = new QPushButton("清空", gb_);
    btn_send_ = new QPushButton("发送", gb_);

    lbl_turntable_ = new QLabel("转台角度°", this);
    lbl_lifter_ = new QLabel("升降角度°", this);
    lbl_repeat_ = new QLabel("重复", this);
    lbl_delay_ = new QLabel("延迟(ms)", this);
    edit_t_degrees_ = new QLineEdit("30.00", this);
    edit_l_degrees_ = new QLineEdit("360.00", this);
    spin_repetition_ = new QSpinBox(this);
    edit_delay_ = new QLineEdit("1000", this);

    btn_t_forward_ = new QPushButton("顺时针", this);
    btn_t_reverse_ = new QPushButton("逆时针", this);
    btn_l_forward_ = new QPushButton("上升", this);
    btn_l_reverse_ = new QPushButton("下降", this);
    btn_rep_begin_ = new QPushButton("BEGIN", this);
    btn_rep_end_ = new QPushButton("END", this);
    btn_delay_ = new QPushButton("延时", this);

    lyt_grid_right_ = new QGridLayout();
    lyt_grid_right_->addWidget(enc_conf_tx_, 0, 0, 2, 2);
    lyt_grid_right_->addWidget(btn_clear_, 2, 0, 1, 1);
    lyt_grid_right_->addWidget(btn_send_, 2, 1, 1, 1);

    lyt_h_top_ = new QHBoxLayout();
    lyt_h_top_->addWidget(txt_hex_, 1);
    lyt_h_top_->addWidget(txt_text_, 1);
    lyt_h_top_->addLayout(lyt_grid_right_);

    lyt_grid_bottom_ = new QGridLayout();
    lyt_grid_bottom_->addWidget(lbl_turntable_, 0, 0, 1, 1);
    lyt_grid_bottom_->addWidget(edit_t_degrees_, 0, 1, 1, 2);
    lyt_grid_bottom_->addWidget(btn_t_forward_, 0, 3, 1, 1);
    lyt_grid_bottom_->addWidget(btn_t_reverse_, 0, 4, 1, 1);
    lyt_grid_bottom_->addWidget(lbl_repeat_, 0, 5, 1, 1);
    lyt_grid_bottom_->addWidget(spin_repetition_, 0, 6, 1, 1);
    lyt_grid_bottom_->addWidget(btn_rep_begin_, 0, 7, 1, 1);
    lyt_grid_bottom_->addWidget(btn_rep_end_, 0, 8, 1, 1);
    lyt_grid_bottom_->addWidget(lbl_lifter_, 1, 0, 1, 1);
    lyt_grid_bottom_->addWidget(edit_l_degrees_, 1, 1, 1, 2);
    lyt_grid_bottom_->addWidget(btn_l_forward_, 1, 3, 1, 1);
    lyt_grid_bottom_->addWidget(btn_l_reverse_, 1, 4, 1, 1);
    lyt_grid_bottom_->addWidget(lbl_delay_, 1, 5, 1, 1);
    lyt_grid_bottom_->addWidget(edit_delay_, 1, 6, 1, 2);
    lyt_grid_bottom_->addWidget(btn_delay_, 1, 8, 1, 1);

    lyt_v_ = new QVBoxLayout();
    lyt_v_->addLayout(lyt_h_top_);
    lyt_v_->addLayout(lyt_grid_bottom_);

    lyt_gb_ = new QVBoxLayout();
    lyt_gb_->addLayout(lyt_v_);

    gb_->setLayout(lyt_gb_);

    lyt_main_ = new QVBoxLayout(this);
    lyt_main_->addWidget(gb_);
    setLayout(lyt_main_);

}

void BatchCommand::on_txt_hex_text_changed() {
    if (is_text_updating_) return;
    is_hex_updating_ = true;
    update_text_from_hex();
    is_hex_updating_ = false;
}

void BatchCommand::on_txt_text_text_changed() {
    if (is_hex_updating_) return;
    is_text_updating_ = true;
    update_hex_from_text();
    is_text_updating_ = false;
}

void BatchCommand::on_encoding_changed() {
    update_hex_from_text();  // 编码改变时更新hex显示
}

void BatchCommand::update_text_from_hex() {
    if (is_text_updating_) return;
    is_hex_updating_ = true;

    QString encoding = enc_conf_tx_->get_encoding();
    QByteArray hex_data = txt_hex_->get_hex_data(); // 获取原始字节数据

    if (hex_data.isEmpty()) {     // 空数据检查
        txt_text_->clear();
        is_hex_updating_ = false;
        return;
    }

    QString text;
    if (encoding == "GBK") {
        text = TextConverter::gbk_to_string(hex_data);
    } else if (encoding == "UTF-8") {
        text = TextConverter::utf8_to_string(hex_data);
    } else {
        text = txt_hex_->get_text_from_hex(encoding); // 默认行为
    }
    txt_text_->setText(text);

    is_hex_updating_ = false;
}

void BatchCommand::update_hex_from_text() {
    if (is_hex_updating_) return;
    is_text_updating_ = true;

    QString encoding = enc_conf_tx_->get_encoding();
    QString text = txt_text_->toPlainText();

    if (text.isEmpty()) {
        txt_hex_->clear();
        is_text_updating_ = false;
        return;
    }

    QByteArray data;
    if (encoding == "GBK") {
        data = TextConverter::string_to_gbk(text);
    } else if (encoding == "UTF-8") {
        data = TextConverter::string_to_utf8(text);
    } else {
        // 默认行为，使用 HexTextEdit 的方法
        txt_hex_->set_hex_from_text(text, encoding);
        is_text_updating_ = false;
        return;
    }
    // 将数据转换为十六进制字符串并设置到 txt_hex_
    QString hex = data.toHex(' ').toUpper();
    txt_hex_->setText(hex);

    is_text_updating_ = false;
}

void BatchCommand::on_btn_clear_clicked() {
    txt_hex_->clear();
    txt_text_->clear();
}

void BatchCommand::on_ctl_btn_clicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    QString cmd;

    QString packet_prefix = ReadConfig::getInstance().get_packet_prefix();
    QString packet_suffix = ReadConfig::getInstance().get_packet_suffix();

    if (btn == btn_t_forward_) {
        cmd = packet_prefix + ReadConfig::getInstance().get_turntable_clockwise() +
            edit_t_degrees_->text() + packet_suffix;
    } else if (btn == btn_t_reverse_) {
        cmd = packet_prefix + ReadConfig::getInstance().get_turntable_anticlockwise() +
            edit_t_degrees_->text() + packet_suffix;
    } else if (btn == btn_l_forward_) {
        cmd = packet_prefix + ReadConfig::getInstance().get_lifter_up() +
            edit_l_degrees_->text() + packet_suffix;
    } else if (btn == btn_l_reverse_) {
        cmd = packet_prefix + ReadConfig::getInstance().get_lifter_down() +
            edit_l_degrees_->text() + packet_suffix;
    } else if (btn == btn_delay_) {
        cmd = packet_prefix + "XX" + edit_delay_->text() + packet_suffix;
    } else if (btn == btn_rep_begin_) {
        cmd = "{";
    } else if (btn == btn_rep_end_) {
        cmd = "}";
    }

    append_command(cmd);
}

void BatchCommand::on_btn_send_clicked() {
    QString mode = enc_conf_tx_->get_mode();
    QByteArray data;

    if (mode == "HEX") {
        data = txt_hex_->get_hex_data();
    } else {
        QString text = txt_text_->toPlainText();
        QString encoding = enc_conf_tx_->get_encoding();
        if (encoding == "GBK") {
            data = TextConverter::string_to_gbk(text);
        } else {
            data = text.toUtf8(); // 默认UTF-8
        }
    }

    emit sig_send_data(data); // 发射信号
}

void BatchCommand::append_command(const QString &command) {
    // 移除自动换行：使用 insertPlainText 而不是 append
    if (enc_conf_tx_->get_mode() == "HEX") {
        QString current_hex = txt_hex_->toPlainText();
        if (!current_hex.isEmpty() && !current_hex.endsWith(' ')) {
            current_hex += " ";
        }

        QString encoding = enc_conf_tx_->get_encoding();
        QByteArray data = txt_hex_->get_bytes_from_text(encoding);
        data.append(command.toUtf8()); // 直接追加字节数据

        QString new_hex = data.toHex(' ').toUpper();
        txt_hex_->setText(new_hex);
    } else {
        // 在文本模式下直接插入，不添加换行
        txt_text_->insertPlainText(command);
    }
}



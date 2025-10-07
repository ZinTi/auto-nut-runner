#include "encoding_config.h"

EncodingConfig::EncodingConfig(QWidget *parent) : QWidget(parent) {
    setup_ui();

    // 编码选项改变时发送信号
    connect(cbob_enc_, &QComboBox::currentTextChanged, this, &EncodingConfig::sig_encoding_changed);
}

EncodingConfig::~EncodingConfig() {

}

void EncodingConfig::setup_ui() {
    gb_ = new QGroupBox("编码配置", this);
    gb_->setMinimumHeight(80);
    lbl_mode_ = new QLabel("收发模式", this);
    lbl_enc_ = new QLabel("文本编码", this);
    cbob_mode_ = new QComboBox(this);
    cbob_mode_->addItems({
        "HEX", "TEXT"
    });
    cbob_mode_->setCurrentText("TEXT");

    cbob_enc_ = new QComboBox(this);
    cbob_enc_->addItems({
        "GBK", "UTF-8"
    });
    cbob_enc_->setCurrentText("UTF-8");

    // 连接模式切换信号
    connect(cbob_mode_, &QComboBox::currentTextChanged,
            this, &EncodingConfig::on_mode_changed);

    // 初始状态设置
    on_mode_changed(cbob_mode_->currentText());

    lyt_form_ = new QFormLayout();
    lyt_form_->addRow(lbl_mode_, cbob_mode_);
    lyt_form_->addRow(lbl_enc_, cbob_enc_);

    gb_->setLayout(lyt_form_);

    lyt_main_ = new QVBoxLayout(this);
    lyt_main_->addWidget(gb_);
    setLayout(lyt_main_);
}

void EncodingConfig::set_group_box_title(const QString &title) {
    gb_->setTitle(title);
}

void EncodingConfig::set_mode_text(const QString &text) {
    lbl_mode_->setText(text);
}

QString EncodingConfig::get_mode() const {
    return cbob_mode_->currentText();
}

QString EncodingConfig::get_encoding() const {
    return cbob_enc_->currentText();
}

void EncodingConfig::on_mode_changed(const QString &mode) {
    if (mode == "HEX") {
        // HEX模式下禁用编码选择
        cbob_enc_->setEnabled(false);
    } else {
        // TEXT模式下启用编码选择
        cbob_enc_->setEnabled(true);
    }
    emit sig_encoding_changed();  // 在模式改变时也发射信号（因为模式改变会影响编码）
}

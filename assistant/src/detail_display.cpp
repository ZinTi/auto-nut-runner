#include "detail_display.h"
#include "text_converter.h"

DetailDisplay::DetailDisplay(QWidget *parent) : QWidget(parent) {
    // 初始化发送显示格式
    tx_mode_ = "HEX";
    tx_encoding_ = "UTF-8";
    setup_ui();
    connect_signals_slots();
}

DetailDisplay::~DetailDisplay() {
}

void DetailDisplay::setup_ui() {
    gb_ = new QGroupBox(this);
    txt_ = new QTextEdit(gb_);
    txt_->setReadOnly(true);

    enc_conf_rx_ = new EncodingConfig(gb_);
    enc_conf_rx_->set_group_box_title("字符编码");
    enc_conf_rx_->set_mode_text("接收模式");

    btn_clear_ = new QPushButton("清空", gb_);
    btn_show_tx_ = new QPushButton(gb_);
    update_display_state();

    lyt_grid_ = new QGridLayout();
    lyt_grid_->addWidget(enc_conf_rx_,0,0,2,2);
    lyt_grid_->addWidget(btn_clear_,2,0,1,1);
    lyt_grid_->addWidget(btn_show_tx_,2,1,1,1);

    lyt_gb_ = new QHBoxLayout();
    lyt_gb_->addWidget(txt_, 1);
    lyt_gb_->addLayout(lyt_grid_);

    gb_->setLayout(lyt_gb_);

    lyt_main_ = new QVBoxLayout(this);
    lyt_main_->addWidget(gb_);
    setLayout(lyt_main_);
}

void DetailDisplay::update_display_state() {
    if(enable_show_tx_) {
        btn_show_tx_->setText("隐藏发送");
        gb_->setTitle("通信原文（接收+发送）");
    } else {
        btn_show_tx_->setText("显示发送");
        gb_->setTitle("接收区");
    }
}

void DetailDisplay::connect_signals_slots() {
    connect(btn_clear_, &QPushButton::clicked, this, &DetailDisplay::on_btn_clear_clicked);
    connect(btn_show_tx_, &QPushButton::clicked, this, &DetailDisplay::on_btn_show_tx_clicked);
}

void DetailDisplay::on_btn_clear_clicked() {
    this->txt_->clear();
}

void DetailDisplay::on_btn_show_tx_clicked() {
    enable_show_tx_ = !enable_show_tx_;
    update_display_state(); // 调用更新函数
}

void DetailDisplay::append_message(const QByteArray &data, bool is_tx, DisplayFormat format) {
    // 如果是发送消息且不允许显示发送，则直接返回
    if (is_tx && !enable_show_tx_) {
        return;
    }

    // 根据消息来源设置前缀和颜色
    QString prefix = is_tx ? "[TX] " : "[RX] ";
    QString color = is_tx ? "magenta" : "blue";

    // 根据格式转换数据
    QString content;
    switch (format) {
        case HEX:
            content = data.toHex(' ').toUpper();
            prefix += "HEX: ";
            break;
        case UTF8:
            content = QString::fromUtf8(data);
            prefix += "UTF8: ";
            break;
        case GBK:
            content = QString::fromLocal8Bit(data); // 假设系统本地编码为GBK
            prefix += "GBK: ";
            break;
    }

    // 构建带格式的HTML文本
    QString formatted_text = QString("<font color='%1'>%2</font>%3")
                                 .arg(color)
                                 .arg(prefix)
                                 .arg(content.toHtmlEscaped());

    // 追加到文本控件
    txt_->append(formatted_text);
}

void DetailDisplay::append_message(const QByteArray &data, bool is_tx) {
    // 如果是发送消息且不允许显示发送，则直接返回
    if (is_tx && !enable_show_tx_) {
        return;
    }

    // 根据消息来源设置前缀和颜色
    QString prefix = is_tx ? "[TX] " : "[RX] ";
    QString color = is_tx ? "magenta" : "blue";

    // 根据格式转换数据
    QString content;
    if (is_tx) {
        // 发送消息使用发送区的显示设置
        if (tx_mode_ == "HEX") {
            content = data.toHex(' ').toUpper();
            prefix += "HEX: ";
        } else {
            if (tx_encoding_ == "GBK") {
                content = TextConverter::gbk_to_string(data);
                prefix += "GBK: ";
            } else {
                content = TextConverter::utf8_to_string(data);
                prefix += "UTF8: ";
            }
        }
    } else {
        // 接收消息使用接收区的显示设置
        QString mode = enc_conf_rx_->get_mode();
        QString encoding = enc_conf_rx_->get_encoding();

        if (mode == "HEX") {
            content = data.toHex(' ').toUpper();
            prefix += "HEX: ";
        } else {
            if (encoding == "GBK") {
                content = TextConverter::gbk_to_string(data);
                prefix += "GBK: ";
            } else {
                content = TextConverter::utf8_to_string(data);
                prefix += "UTF8: ";
            }
        }
    }

    // 构建带格式的HTML文本
    QString formatted_text = QString("<font color='%1'>%2</font>%3")
                                 .arg(color)
                                 .arg(prefix)
                                 .arg(content.toHtmlEscaped());

    // 追加到文本控件
    txt_->append(formatted_text);
}

void DetailDisplay::set_tx_display_format(const QString &mode, const QString &encoding) {
    tx_mode_ = mode;
    tx_encoding_ = encoding;
}

#include "serial_port_config.h"
#include <QMessageBox>
#include <QDebug>

SerialPortConfig::SerialPortConfig(QWidget *parent) : QWidget(parent), is_serial_open_(false) {
    serial_port_ = new QSerialPort(this);  // 先创建 QSerialPort 对象
    setup_ui();
}

SerialPortConfig::~SerialPortConfig() {
    close_serial_port(); // 无论是否打开都尝试关闭
}

bool SerialPortConfig::is_serial_open() const {
    return is_serial_open_;
}

void SerialPortConfig::setup_ui() {
    gb_serial_ = new QGroupBox("串口配置", this);
    lbl_port_num_ = new QLabel("串口号", this);
    lbl_baud_rate_ = new QLabel("波特率", this);
    lbl_word_length_ = new QLabel("数据位", this);
    lbl_stop_bits_ = new QLabel("停止位", this);
    lbl_parity_ = new QLabel("校验位", this);
    lbl_enable_ = new QLabel("操作", this);

    // 使用SerialComboBox而不是QComboBox
    cbob_port_num_ = new SerialComboBox(this);
    refresh_serial_ports();  // 初始刷新串口列表

    cbob_baud_rate_ = new QComboBox(this);
    cbob_baud_rate_->addItems({
        "2400", "4800", "9600", "19200", "38400", "57600", "115200"
    });
    cbob_baud_rate_->setCurrentText("115200");

    cbob_word_length_ = new QComboBox(this);
    cbob_word_length_->addItems({
        "5", "6", "7", "8"
    });
    cbob_word_length_->setCurrentText("8");

    cbob_stop_bits_ = new QComboBox(this);
    cbob_stop_bits_->addItems({
        "1", "1.5", "2"
    });
    cbob_stop_bits_->setCurrentText("1");

    cbob_parity_ = new QComboBox(this);
    cbob_parity_->addItems({
        "None", "Odd", "Even"
    });
    cbob_parity_->setCurrentText("None");

    btn_enable_ = new QPushButton("打开串口", this);
    connect(btn_enable_, &QPushButton::clicked, this, &SerialPortConfig::on_btn_enable_clicked);
    connect(serial_port_, &QSerialPort::readyRead, this, &SerialPortConfig::read_serial_data);

    lyt_form_ = new QFormLayout();
    lyt_form_->addRow(lbl_port_num_, cbob_port_num_);
    lyt_form_->addRow(lbl_baud_rate_, cbob_baud_rate_);
    lyt_form_->addRow(lbl_word_length_, cbob_word_length_);
    lyt_form_->addRow(lbl_stop_bits_, cbob_stop_bits_);
    lyt_form_->addRow(lbl_parity_, cbob_parity_);
    lyt_form_->addRow(lbl_enable_, btn_enable_);

    gb_serial_->setLayout(lyt_form_);

    lyt_main_ = new QVBoxLayout(this);
    lyt_main_->addWidget(gb_serial_);
    setLayout(lyt_main_);

    update_ui_state();  // 初始化UI状态
}

void SerialPortConfig::refresh_serial_ports() {
    try {
        QString current_text = cbob_port_num_->currentText();
        cbob_port_num_->clear();

        // 获取可用串口
        QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
        for (const QSerialPortInfo &port : ports) {
            // 统一使用 " - " 作为分隔符
            cbob_port_num_->addItem(port.portName() + " - " + port.description());
        }

        // 尝试恢复之前的选择
        int index = cbob_port_num_->findText(current_text);
        if (index >= 0) {
            cbob_port_num_->setCurrentIndex(index);
        } else if (cbob_port_num_->count() > 0) {
            cbob_port_num_->setCurrentIndex(0);
        }
    } catch (...) {
        QMessageBox::critical(nullptr, "严重错误", "Exception in refresh_serial_ports");
        // 可以在这里添加更详细的错误处理
    }
}

void SerialPortConfig::on_btn_enable_clicked() {
    if (is_serial_open_) {
        close_serial_port();
    } else {
        if (open_serial_port()) {
            is_serial_open_ = true;
            emit serial_opened();
        }
    }
    update_ui_state();
}

bool SerialPortConfig::open_serial_port() {
    if (cbob_port_num_->count() == 0) {
        QMessageBox::warning(this, "错误", "没有可用的串口设备");
        return false;
    }

    // 从组合框中提取端口名（去掉描述部分）
    QString port_text = cbob_port_num_->currentText();
    QString port_name = port_text.split(" - ").first();

    serial_port_->setPortName(port_name);
    serial_port_->setBaudRate(cbob_baud_rate_->currentText().toInt());

    // 设置数据位
    switch (cbob_word_length_->currentText().toInt()) {
    case 5: serial_port_->setDataBits(QSerialPort::Data5); break;
    case 6: serial_port_->setDataBits(QSerialPort::Data6); break;
    case 7: serial_port_->setDataBits(QSerialPort::Data7); break;
    case 8: serial_port_->setDataBits(QSerialPort::Data8); break;
    default: serial_port_->setDataBits(QSerialPort::Data8); break;
    }

    // 设置停止位
    if (cbob_stop_bits_->currentText() == "1.5") {
        serial_port_->setStopBits(QSerialPort::OneAndHalfStop);
    } else if (cbob_stop_bits_->currentText() == "2") {
        serial_port_->setStopBits(QSerialPort::TwoStop);
    } else {
        serial_port_->setStopBits(QSerialPort::OneStop);
    }

    // 设置校验位
    if (cbob_parity_->currentText() == "Odd") {
        serial_port_->setParity(QSerialPort::OddParity);
    } else if (cbob_parity_->currentText() == "Even") {
        serial_port_->setParity(QSerialPort::EvenParity);
    } else {
        serial_port_->setParity(QSerialPort::NoParity);
    }

    // 尝试打开串口
    if (serial_port_->open(QIODevice::ReadWrite)) {
        return true;
    } else {
        QMessageBox::warning(this, "错误", QString("无法打开串口: %1").arg(serial_port_->errorString()));
        return false;
    }
}

void SerialPortConfig::close_serial_port() {
    if (serial_port_->isOpen()) {
        serial_port_->close();
    }
    is_serial_open_ = false;
    emit serial_closed();
}

void SerialPortConfig::update_ui_state() {
    // 根据串口状态更新UI
    bool is_open = is_serial_open_;

    btn_enable_->setText(is_open ? "关闭串口" : "打开串口");

    // 设置按钮背景色
    if (is_open) {
        btn_enable_->setStyleSheet(
            "QPushButton {"
            "   background-color: rgb(255, 80, 80);" // 正常状态颜色
            "   border: 1px solid #8f8f91;"
            "   border-radius: 3px;"
            "   padding: 2px;"
            "}"
            "QPushButton:hover {"
            "   background-color: rgb(255, 120, 120);" // 悬停颜色
            "}"
            "QPushButton:pressed {"
            "   background-color: rgb(255, 160, 160);" // 按下颜色
            "}"
            "QPushButton:disabled {"
            "   background-color: #cccccc;" // 禁用状态颜色
            "   color: #888888;"
            "}"
            );
    } else {
        // 恢复默认样式，但保留一些基本样式
        btn_enable_->setStyleSheet(
            "QPushButton {"
            "   background-color: palette(button);" // 使用系统按钮颜色
            "   border: 1px solid #8f8f91;"
            "   border-radius: 3px;"
            "   padding: 2px;"
            "}"
            "QPushButton:hover {"
            "   background-color: palette(light);" // 使用系统高亮颜色
            "}"
            "QPushButton:pressed {"
            "   background-color: palette(dark);" // 使用系统暗色
            "}"
            );
    }

    // 串口打开时禁用配置选项，关闭时启用
    cbob_port_num_->setEnabled(!is_open);
    cbob_baud_rate_->setEnabled(!is_open);
    cbob_word_length_->setEnabled(!is_open);
    cbob_stop_bits_->setEnabled(!is_open);
    cbob_parity_->setEnabled(!is_open);
}

void SerialPortConfig::read_serial_data() {
    if (serial_port_->bytesAvailable() > 0) {
        try {
            QByteArray data = serial_port_->readAll();
            if (!data.isEmpty()) {
                emit data_received(data);
            }
        } catch (...) {
            QMessageBox::critical(nullptr, "严重错误", "Exception in read_serial_data");
            qWarning() << "Error reading serial data";
        }
    }
}

void SerialPortConfig::send_data(const QByteArray &data) {
    if (!is_serial_open_ || !serial_port_->isOpen()) {
        QMessageBox::warning(this,
                            "串口错误",
                            "发送失败：串口未连接或已断开\n\n"
                            "请先打开串口再尝试发送数据");
        return;
    }

    try {
        if (serial_port_->write(data) == -1) {
            QMessageBox::warning(this,
                                "写入错误",
                                QString("写入串口数据失败: %1")
                                .arg(serial_port_->errorString()));
            qWarning() << "Failed to write data to serial port";
        }
    } catch (...) {
        QMessageBox::critical(this,
                             "严重错误",
                             "发送数据时发生异常");
        qWarning() << "Error writing to serial port";
    }
}

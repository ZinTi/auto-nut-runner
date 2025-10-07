#ifndef SERIAL_PORT_CONFIG_H
#define SERIAL_PORT_CONFIG_H

#include <QWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QFormLayout>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "serial_combo_box.h"

class SerialPortConfig : public QWidget {
    Q_OBJECT

public:
    explicit SerialPortConfig(QWidget *parent = nullptr);
    ~SerialPortConfig();

    bool is_serial_open() const;  // 串口状态查询
    QSerialPort* get_serial_port() const { return serial_port_; }
    void send_data(const QByteArray &data);

signals:
    void serial_opened();  // 串口打开信号
    void serial_closed();  // 串口关闭信号
    void data_received(const QByteArray &data);

private slots:
    void on_btn_enable_clicked();  // 打开/关闭串口按钮点击槽
    void read_serial_data();

private:
    void setup_ui();
    void refresh_serial_ports();  // 刷新串口列表
    bool open_serial_port();      // 打开串口
    void close_serial_port();     // 关闭串口
    void update_ui_state();       // 更新UI状态

    QGroupBox* gb_serial_;
    QLabel* lbl_port_num_;
    QLabel* lbl_baud_rate_;
    QLabel* lbl_word_length_;
    QLabel* lbl_stop_bits_;
    QLabel* lbl_parity_;
    QLabel* lbl_enable_;
    SerialComboBox* cbob_port_num_;
    QComboBox* cbob_baud_rate_;
    QComboBox* cbob_word_length_;
    QComboBox* cbob_stop_bits_;
    QComboBox* cbob_parity_;
    QPushButton* btn_enable_;
    QFormLayout* lyt_form_;
    QVBoxLayout* lyt_main_;

    QSerialPort* serial_port_;     // 串口对象
    bool is_serial_open_;          // 串口打开状态
};

#endif // SERIAL_PORT_CONFIG_H

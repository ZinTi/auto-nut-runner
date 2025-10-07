#ifndef DC_GEAR_MOTOR_H
#define DC_GEAR_MOTOR_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

class DCGearMotor : public QWidget {
    Q_OBJECT

public:
    explicit DCGearMotor(QWidget *parent = nullptr);
    ~DCGearMotor();

    /**
     * @brief 启用或禁用所有发送按钮
     * @param enable 布尔值，true为启用，false为禁用
     */
    void enable_btn_send(bool enable);

signals:
    void sig_send_data(const QByteArray &data);  // 添加发送信号

private slots:
    void on_btn_forward_clicked();  // 正转按钮点击
    void on_btn_reverse_clicked();  // 反转按钮点击

private:
    void setup_ui();
    void connect_signals_slots();

    QGroupBox* gb_;
    QLabel* lbl_forward_;
    QLabel* lbl_reverse_;
    QLineEdit* edit_forward_;
    QLineEdit* edit_reverse_;
    QPushButton* btn_forward_;
    QPushButton* btn_reverse_;

    QFormLayout* lyt_form_;
    QHBoxLayout* lyt_btn_;
    QVBoxLayout* lyt_main_;
};

#endif // DC_GEAR_MOTOR_H

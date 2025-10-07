#ifndef STEPPER_CTL_H
#define STEPPER_CTL_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

class StepperCtl : public QWidget {
    Q_OBJECT

public:
    explicit StepperCtl(QWidget *parent = nullptr);
    ~StepperCtl();

    void set_groupbox_title(const QString &title);
    void set_pulse_per_rev(uint16_t ppr_value);
    void set_btn_text(const QString &left_btn_text, const QString &right_btn_text);
    void set_btn_cmd(const QString &clockwise_cmd, const QString &anticlockwise_cmd);
    /**
     * @brief 启用或禁用所有发送按钮
     * @param enable 布尔值，true为启用，false为禁用
     */
    void enable_btn_send(bool enable);

signals:
    void sig_send_data(const QByteArray &data);

private slots:
    void update_pulses();
    void on_btn_forward_clicked();  // 正转按钮点击
    void on_btn_reverse_clicked();  // 反转按钮点击

private:
    void setup_ui();
    void connect_signals_slots();  // 添加信号槽连接函数

    QString clockwise_cmd_;
    QString anticlockwise_cmd_;

    QGroupBox* gb_;
    QLabel* lbl_ppr_;
    QLabel* lbl_pulses_;
    QLabel* lbl_degrees_;
    QLineEdit* edit_ppr_;
    QLineEdit* edit_pulses_;
    QLineEdit* edit_degrees_;
    QPushButton* btn_forward_;
    QPushButton* btn_reverse_;

    QFormLayout* lyt_form_;
    QHBoxLayout* lyt_btn_;
    QVBoxLayout* lyt_main_;
};

#endif // STEPPER_CTL_H

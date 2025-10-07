#ifndef SOUND_LIGHT_ALERT_H
#define SOUND_LIGHT_ALERT_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

class SoundLightAlert : public QWidget {
    Q_OBJECT

public:
    explicit SoundLightAlert(QWidget *parent = nullptr);
    ~SoundLightAlert();

    /**
     * @brief 启用或禁用所有发送按钮
     * @param enable 布尔值，true为启用，false为禁用
     */
    void enable_btn_send(bool enable);

signals:
    void sig_send_data(const QByteArray &data);

private slots:
    void on_btn_led_switch_clicked();
    void on_btn_led_toggle_clicked();
    void on_btn_led_flash_clicked();
    void on_btn_buzzer_switch_clicked();
    void on_btn_buzzer_toggle_clicked();
    void on_btn_buzzer_rhythm_clicked();
    void on_btn_buzzer_beep_clicked();

private:
    void setup_ui();
    void connect_signals_slots();

    QString led_on_cmd_;
    QString led_off_cmd_;
    QString led_toggle_cmd_;
    QString led_flash_prefix_;
    QString buzzer_on_cmd_;
    QString buzzer_off_cmd_;
    QString buzzer_toggle_cmd_;
    QString buzzer_rhythm_cmd_;
    QString buzzer_beep_prefix_;

    QGroupBox* gb_;
    QLabel* lbl_led_;
    QLabel* lbl_buzzer_;

    QPushButton* btn_led_switch_;
    QPushButton* btn_led_toggle_;
    QSpinBox* spin_led_flash_;
    QPushButton* btn_led_flash_;
    QPushButton* btn_buzzer_switch_;
    QPushButton* btn_buzzer_toggle_;
    QPushButton* btn_buzzer_rhythm_;
    QSpinBox* spin_buzzer_beep_;
    QPushButton* btn_buzzer_beep_;

    QGridLayout* lyt_grid_;
    QVBoxLayout* lyt_main_;
};

#endif // SOUND_LIGHT_ALERT_H

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "microstep_config.h"
#include "general_setting.h"
#include "serial_port_config.h"
#include "detail_display.h"
#include "batch_command.h"
#include "stepper_ctl.h"
#include "dc_gear_motor.h"
#include "sound_light_alert.h"
#include "fetch_config.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_microstep_t_ppr_changed();
    void on_microstep_l_ppr_changed();
    void on_data_sent(const QByteArray &data);
    void on_data_received(const QByteArray &data);
    void on_tx_encoding_changed();
    void on_serial_state_changed(bool opened);

private:
    void setup_ui();
    void connect_signals_slots();

    QWidget* wgt_central_;

    QLabel* lbl_title_; // 标题

    MicrostepConfig* microstep_conf_t_;
    MicrostepConfig* microstep_conf_l_;
    GeneralSetting* g_set_;
    SerialPortConfig* serial_port_conf_;
    DetailDisplay* detail_display_;
    BatchCommand* batch_cmd_;
    StepperCtl* stepper_ctl_t_;
    StepperCtl* stepper_ctl_l_;
    DCGearMotor* dc_motor_;
    SoundLightAlert* sound_light_;
    FetchConfig* fetch_config_;

    QVBoxLayout* lyt_main_;
    QHBoxLayout* lyt_work_;
    QVBoxLayout* lyt_left_;
    QVBoxLayout* lyt_right_;
    QHBoxLayout* lyt_ctl_;
    QHBoxLayout* lyt_bottom_;

};
#endif // MAIN_WINDOW_H

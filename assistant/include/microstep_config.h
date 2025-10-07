#ifndef MICROSTEP_CONFIG_H
#define MICROSTEP_CONFIG_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <stdint.h>

class MicrostepConfig : public QWidget {
    Q_OBJECT

public:
    explicit MicrostepConfig(QWidget *parent = nullptr);
    ~MicrostepConfig();

    void set_groupbox_title(const QString &title);
    uint8_t get_switch_state();
    uint16_t get_pulse_per_rev();
    void set_ppr_cmd(const QString &cmd);
    void enable_btn_send(bool enable);

signals:
    void sig_ppr_changed(uint16_t ppr_value);
    void sig_send_data(const QByteArray &data);

private slots:
    void on_sw_checkbox_changed();
    void on_ppr_combobox_changed(int index);
    void on_btn_sw_clicked();
    void on_btn_config_clicked();

private:
    void setup_ui();
    void connect_signals_slots();

    QString ppr_cmd_;    // pulse per revolution

    // 静态映射数组
    static const int index_to_sw_state[16];
    static const int sw_state_to_index[16];

    QGroupBox* gb_microstep_;
    QPushButton* btn_sw_;   // 全开或全关
    QCheckBox* chkb_sw5_;
    QCheckBox* chkb_sw6_;
    QCheckBox* chkb_sw7_;
    QCheckBox* chkb_sw8_;
    QLabel* lbl_ppr_;  // pulse per revolution
    QComboBox* cbob_ppr_;
    QPushButton* btn_config_;   // 提交配置

    QHBoxLayout* lyt_sw_;
    QGridLayout* lyt_grid_;
    QVBoxLayout* lyt_main_;

};

#endif // MICROSTEP_CONFIG_H

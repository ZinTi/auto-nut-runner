#ifndef GENERAL_SETTING_H
#define GENERAL_SETTING_H

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QSignalMapper>

enum class OperationMode {
    Auto,     // 自动模式
    Manual    // 手动模式
};

class GeneralSetting : public QWidget {
    Q_OBJECT

public:
    explicit GeneralSetting(QWidget *parent = nullptr);
    ~GeneralSetting();

    /**
     * @brief 启用或禁用所有发送按钮
     * @param enable 布尔值，true为启用，false为禁用
     */
    void enable_btn_send(bool enable);

signals:
    void sig_send_data(const QByteArray &data);

private slots:
    void on_btn_emergency_clicked();
    OperationMode on_switch_mode();
    void on_custom_button_clicked(int index);
    void on_btn_ch_text_clicked();

private:
    OperationMode current_mode_;
    QSignalMapper* signal_mapper_;

    void setup_ui();
    void connect_signals_slots();
    bool validate_command(const QString &command);

    QGroupBox* gb_gen_setting_;
    QPushButton* btn_emergency_;    // 急停
    QPushButton* btn_mode_; // 自动或手动模式

    QTextEdit* txt_tips_;

    QLineEdit* edit_custom1_;
    QLineEdit* edit_custom2_;
    QLineEdit* edit_custom3_;
    QLineEdit* edit_custom4_;
    QLineEdit* edit_custom5_;
    QPushButton* btn_custom1_;
    QPushButton* btn_custom2_;
    QPushButton* btn_custom3_;
    QPushButton* btn_custom4_;
    QPushButton* btn_custom5_;

    QComboBox* cbob_select_btn_;
    QLineEdit* edit_backup_;
    QPushButton* btn_ch_text_;

    QVBoxLayout* lyt_v_;
    QGridLayout* lyt_grid_;
    QFormLayout* lyt_form_;
    QHBoxLayout* lyt_h_;
    QVBoxLayout* lyt_main_;
};

#endif // GENERAL_SETTING_H

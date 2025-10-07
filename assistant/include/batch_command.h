#ifndef BATCH_COMMAND_H
#define BATCH_COMMAND_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include "hex_text_edit.h"
#include "encoding_config.h"

class BatchCommand : public QWidget {
    Q_OBJECT

public:
    explicit BatchCommand(QWidget *parent = nullptr);
    ~BatchCommand();

    /**
     * @brief 启用或禁用发送按钮
     * @param enable 布尔值，true为启用，false为禁用
     */
    void enable_btn_send(bool enable);

    /**
     * @brief 获取编码配置对象
     * @return 指向EncodingConfig对象的指针
     */
    EncodingConfig* get_encoding_config() const;

private slots:
    void on_txt_hex_text_changed();
    void on_txt_text_text_changed();
    void on_encoding_changed();
    void on_btn_clear_clicked();
    void on_ctl_btn_clicked();  // 控制按键快捷追加命令
    void on_btn_send_clicked();

signals:
    void sig_send_data(const QByteArray &data);

private:
    void setup_ui();
    void update_text_from_hex();
    void update_hex_from_text();
    void append_command(const QString &command);
    bool is_hex_updating_;
    bool is_text_updating_;

    QGroupBox* gb_;
    HexTextEdit* txt_hex_;    // HEX 文本框
    QTextEdit* txt_text_;   //  TEXT 文本框（UTF-8、GBK/GB18030）
    EncodingConfig* enc_conf_tx_;   // 编码配置
    QPushButton* btn_clear_;
    QPushButton* btn_send_;

    QLabel* lbl_turntable_;
    QLabel* lbl_lifter_;
    QLabel* lbl_repeat_;
    QLabel* lbl_delay_;
    QLineEdit* edit_t_degrees_;
    QLineEdit* edit_l_degrees_;
    QSpinBox* spin_repetition_;
    QLineEdit* edit_delay_;

    QPushButton* btn_t_forward_;
    QPushButton* btn_t_reverse_;
    QPushButton* btn_l_forward_;
    QPushButton* btn_l_reverse_;
    QPushButton* btn_rep_begin_;
    QPushButton* btn_rep_end_;
    QPushButton* btn_delay_;

    QGridLayout* lyt_grid_right_;
    QHBoxLayout* lyt_h_top_;
    QGridLayout* lyt_grid_bottom_;
    QVBoxLayout* lyt_v_;
    QVBoxLayout* lyt_gb_;
    QVBoxLayout* lyt_main_;

};

#endif // BATCH_COMMAND_H

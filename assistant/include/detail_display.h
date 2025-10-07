#ifndef DETAIL_DISPLAY_H
#define DETAIL_DISPLAY_H

#include <QWidget>
#include <QTextEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include "encoding_config.h"

/**
 * @brief 详细显示控件类，用于显示接收和发送的历史消息
 *
 * 支持HEX、UTF-8和GBK三种显示格式，可根据设置显示或隐藏发送消息
 */
class DetailDisplay : public QWidget {
    Q_OBJECT

public:
    explicit DetailDisplay(QWidget *parent = nullptr);
    ~DetailDisplay();


    enum DisplayFormat {
        HEX,
        UTF8,
        GBK
    };

    void append_message(const QByteArray &data, bool is_tx, DisplayFormat format);

    /**
     * @brief 追加消息到显示区域
     * @param data 要显示的数据
     * @param is_tx 是否为发送消息(true:发送, false:接收)
     */
    void append_message(const QByteArray &data, bool is_tx);

    /**
     * @brief 设置发送消息的显示格式
     * @param mode 显示模式("HEX"或"TEXT")
     * @param encoding 编码格式("UTF-8"或"GBK")
     */
    void set_tx_display_format(const QString &mode, const QString &encoding);

private slots:
    void on_btn_clear_clicked();
    void on_btn_show_tx_clicked();

private:
    void setup_ui();
    void connect_signals_slots();
    void update_display_state();

    bool enable_show_tx_ = true;
    QString tx_mode_;      // 发送消息显示模式
    QString tx_encoding_;  // 发送消息编码格式

    QGroupBox* gb_;
    QTextEdit* txt_;
    EncodingConfig* enc_conf_rx_;
    QPushButton* btn_clear_;
    QPushButton* btn_show_tx_;  // 启用显示发送的消息

    QVBoxLayout* lyt_main_;
    QHBoxLayout* lyt_gb_;
    QGridLayout* lyt_grid_;

};

#endif // DETAIL_DISPLAY_H

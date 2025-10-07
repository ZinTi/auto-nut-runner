#ifndef FETCH_CONFIG_H
#define FETCH_CONFIG_H

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>


class FetchConfig : public QWidget {
    Q_OBJECT

public:
    explicit FetchConfig(QWidget *parent = nullptr);
    ~FetchConfig();

    /**
     * @brief 启用或禁用所有发送按钮
     * @param enable 布尔值，true为启用，false为禁用
     */
    void enable_btn_send(bool enable);

signals:
    void sig_send_data(const QByteArray &data);

private slots:
    void on_btn_fetch_sysclk_clicked();
    void on_btn_fetch_ppr_clicked();
    void on_btn_fetch_mode_clicked();
    void on_btn_fetch_speed_clicked();

private:
    int current_mode_;

    void setup_ui();
    void connect_signals_slots();

    QGroupBox* gb_gen_setting_;
    QPushButton* btn_fetch_sysclk_; // 获取系统时钟
    QPushButton* btn_fetch_ppr_;
    QPushButton* btn_fetch_speed_;
    QPushButton* btn_fetch_mode_;

    QVBoxLayout* lyt_v_;
    QHBoxLayout* lyt_line1_;
    QHBoxLayout* lyt_line2_;
    QVBoxLayout* lyt_main_;
};

#endif // FETCH_CONFIG_H

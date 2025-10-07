#ifndef ENCODING_CONFIG_H
#define ENCODING_CONFIG_H

#include <QWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

class EncodingConfig : public QWidget {
    Q_OBJECT

public:
    explicit EncodingConfig(QWidget *parent = nullptr);
    ~EncodingConfig();

    void set_group_box_title(const QString &title);
    void set_mode_text(const QString &text);
    QString get_mode() const;
    QString get_encoding() const;

signals:
    /**
     * @brief 编码或模式改变时发出的信号
     */
    void sig_encoding_changed();

private slots:
    void on_mode_changed(const QString &mode);

private:
    void setup_ui();

    QGroupBox* gb_;
    QLabel* lbl_mode_;
    QLabel* lbl_enc_;
    QComboBox* cbob_mode_;
    QComboBox* cbob_enc_;

    QVBoxLayout* lyt_main_;
    QFormLayout* lyt_form_;
};

#endif // ENCODING_CONFIG_H

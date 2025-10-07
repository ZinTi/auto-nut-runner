#ifndef HEX_TEXT_EDIT_H
#define HEX_TEXT_EDIT_H

#include <QTextEdit>
#include <QKeyEvent>
#include <QInputMethodEvent>
#include <string>

class HexTextEdit : public QTextEdit{
    Q_OBJECT

public:
    explicit HexTextEdit(QWidget *parent = nullptr);
    ~HexTextEdit();

    QByteArray get_hex_data() const;
    std::string get_hex_data_std_string() const;

    void set_text_from_bytes(const QByteArray &data, const QString &encoding);
    QByteArray get_bytes_from_text(const QString &encoding) const;
    void set_hex_from_text(const QString &text, const QString &encoding);
    QString get_text_from_hex(const QString &encoding) const;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void inputMethodEvent(QInputMethodEvent *event) override;
    void insertFromMimeData(const QMimeData *source) override;

private:
    bool is_valid_hex_char(const QChar &ch) const;
    bool is_valid_hex_string(const QString &text) const;
    QString filter_hex_text(const QString &text) const;
};

#endif // HEX_TEXT_EDIT_H

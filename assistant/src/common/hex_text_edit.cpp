#include "hex_text_edit.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMimeData>
#include <QClipboard>
#include <QApplication>
#include <QDebug>

HexTextEdit::HexTextEdit(QWidget *parent) : QTextEdit(parent) {
    // 设置输入限制：只允许输入十六进制字符和空格
    // 虽然QTextEdit没有直接的setValidator方法，但我们在事件处理中会进行验证
}

HexTextEdit::~HexTextEdit() {
}

void HexTextEdit::keyPressEvent(QKeyEvent *event) {
    // 处理控制键（退格、删除、方向键等）
    if (event->key() == Qt::Key_Backspace ||
        event->key() == Qt::Key_Delete ||
        event->key() == Qt::Key_Left ||
        event->key() == Qt::Key_Right ||
        event->key() == Qt::Key_Up ||
        event->key() == Qt::Key_Down ||
        event->key() == Qt::Key_Home ||
        event->key() == Qt::Key_End ||
        event->key() == Qt::Key_PageUp ||
        event->key() == Qt::Key_PageDown ||
        event->modifiers() & Qt::ControlModifier) {
        QTextEdit::keyPressEvent(event);
        return;
    }

    // 检查输入的字符是否合法
    QString text = event->text();
    if (text.isEmpty()) {
        QTextEdit::keyPressEvent(event);
        return;
    }

    // 过滤文本，只保留合法字符
    QString filtered_text = filter_hex_text(text);
    if (filtered_text.isEmpty()) {
        // 没有合法字符，忽略输入
        return;
    }

    // 插入过滤后的文本
    insertPlainText(filtered_text);
}

void HexTextEdit::inputMethodEvent(QInputMethodEvent *event) {
    // 处理输入法事件，防止通过输入法输入非法字符
    QString commit_string = event->commitString();
    if (!commit_string.isEmpty()) {
        QString filtered_text = filter_hex_text(commit_string);
        if (!filtered_text.isEmpty()) {
            insertPlainText(filtered_text);
        }
    }
    event->ignore(); // 忽略原始事件，防止插入非法字符
}

void HexTextEdit::insertFromMimeData(const QMimeData *source){
    // 重写粘贴操作，过滤粘贴内容
    if (source->hasText()) {
        QString text = source->text();
        QString filtered_text = filter_hex_text(text);
        if (!filtered_text.isEmpty()) {
            insertPlainText(filtered_text);
        }
    }
}

bool HexTextEdit::is_valid_hex_char(const QChar &ch) const{
    return (ch >= '0' && ch <= '9') ||
           (ch >= 'A' && ch <= 'F') ||
           (ch >= 'a' && ch <= 'f') ||
           (ch == ' '); // 允许空格
}

bool HexTextEdit::is_valid_hex_string(const QString &text) const{
    QRegularExpression reg_exp("^([0-9A-Fa-f]{0,2}\\s)*[0-9A-Fa-f]{0,2}$");
    return reg_exp.match(text).hasMatch();
}

QString HexTextEdit::filter_hex_text(const QString &text) const{
    QString result;
    for (int i = 0; i < text.length(); ++i) {
        QChar ch = text.at(i);
        if (is_valid_hex_char(ch)) {
            // 转换为大写，保持一致性
            result.append(ch.toUpper());
        }
    }
    return result;
}

QByteArray HexTextEdit::get_hex_data() const{
    QString text = toPlainText();
    // 移除所有空格和非十六进制字符
    text.remove(QRegularExpression("[^0-9A-Fa-f]"));

    // 将字符串转换为十六进制数据
    QByteArray hex_data;
    for (int i = 0; i < text.length(); i += 2) {
        QString byte_str = text.mid(i, 2);
        if (byte_str.length() == 2) {
            bool ok;
            char byte = static_cast<char>(byte_str.toInt(&ok, 16));
            if (ok) {
                hex_data.append(byte);
            }
        }
    }

    return hex_data;
}

std::string HexTextEdit::get_hex_data_std_string() const{
    QByteArray data = get_hex_data();
    return std::string(data.constData(), data.length());
}

void HexTextEdit::set_text_from_bytes(const QByteArray &data, const QString &encoding){
    auto encoding_enum = QStringConverter::encodingForName(encoding.toUtf8());
    if (encoding_enum) {
        QStringDecoder decoder(*encoding_enum);
        QString text = decoder.decode(data);
        setText(text);
    }
}

QByteArray HexTextEdit::get_bytes_from_text(const QString &encoding) const{
    QString text = toPlainText();
    auto encoding_enum = QStringConverter::encodingForName(encoding.toUtf8());
    if (encoding_enum) {
        QStringEncoder encoder(*encoding_enum);
        return encoder.encode(text);
    }
    return QByteArray();
}

void HexTextEdit::set_hex_from_text(const QString &text, const QString &encoding){
    auto encoding_enum = QStringConverter::encodingForName(encoding.toUtf8());
    if (encoding_enum) {
        QStringEncoder encoder(*encoding_enum);
        QByteArray data = encoder.encode(text);
        QString hex = data.toHex(' ').toUpper();
        setText(hex);
    }
}

QString HexTextEdit::get_text_from_hex(const QString &encoding) const{
    QByteArray hex_data = get_hex_data();
    auto encoding_enum = QStringConverter::encodingForName(encoding.toUtf8());
    if (encoding_enum) {
        QStringDecoder decoder(*encoding_enum);
        return decoder.decode(hex_data);
    }
    return QString();
}

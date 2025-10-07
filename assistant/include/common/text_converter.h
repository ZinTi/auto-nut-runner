#ifndef TEXT_CONVERTER_H
#define TEXT_CONVERTER_H

#include <QByteArray>
#include <QString>

class TextConverter {
public:
    static QByteArray utf8_to_gbk(const QByteArray &utf8_data);
    static QByteArray gbk_to_utf8(const QByteArray &gbk_data);
    static QByteArray string_to_gbk(const QString &str);
    static QString gbk_to_string(const QByteArray &gbk_data);
    static QString utf8_to_string(const QByteArray &utf8_data);
    static QByteArray string_to_utf8(const QString &str);

private:
    TextConverter() = delete;  // 禁止实例化
};

#endif // TEXT_CONVERTER_H

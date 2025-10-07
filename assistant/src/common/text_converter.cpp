#include "text_converter.h"

QByteArray TextConverter::utf8_to_gbk(const QByteArray &utf8_data) {
    return QString::fromUtf8(utf8_data).toLocal8Bit();
}

QByteArray TextConverter::gbk_to_utf8(const QByteArray &gbk_data) {
    return QString::fromLocal8Bit(gbk_data).toUtf8();
}

QByteArray TextConverter::string_to_gbk(const QString &str) {
    return str.toLocal8Bit();
}

QString TextConverter::gbk_to_string(const QByteArray &gbk_data) {
    return QString::fromLocal8Bit(gbk_data);
}

QString TextConverter::utf8_to_string(const QByteArray &utf8_data) {
    return QString::fromUtf8(utf8_data);
}

QByteArray TextConverter::string_to_utf8(const QString &str) {
    return str.toUtf8();
}


/* 使用示例
// UTF-8转GBK
QByteArray gbk_data = TextConverter::utf8_to_gbk("你好".toUtf8());

// GBK转QString
QString text = TextConverter::gbk_to_string(gbk_data);

// QString转GBK
QByteArray gbk_output = TextConverter::string_to_gbk(text);

*/

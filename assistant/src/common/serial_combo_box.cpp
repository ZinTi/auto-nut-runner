#include "serial_combo_box.h"
#include <QSerialPortInfo>

SerialComboBox::SerialComboBox(QWidget *parent) : QComboBox(parent) {
}

SerialComboBox::~SerialComboBox() {
}

void SerialComboBox::showPopup() {
    int index = 0;
    QString currText = currentText();

    clear(); // 清除显示

    // 扫描显示可用串口列表
    QList<QSerialPortInfo> serials = QSerialPortInfo::availablePorts();
    foreach (QSerialPortInfo info, serials) {
        // 使用 " - " 作为分隔符
        addItem(info.portName() + " - " + info.description());
    }

    // 定位显示项
    index = findText(currText);
    if(index < 0) {
        index = 0;
    }
    setCurrentIndex(index);

    QComboBox::showPopup();  // 调用父类显示列表
}

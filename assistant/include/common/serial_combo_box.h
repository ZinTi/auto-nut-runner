#ifndef SERIAL_COMBO_BOX_H
#define SERIAL_COMBO_BOX_H

#include <QComboBox>

class SerialComboBox : public QComboBox{
    Q_OBJECT

public:
    explicit SerialComboBox(QWidget *parent = nullptr);
    ~SerialComboBox();

    void showPopup() override;

private:

};

#endif // SERIAL_COMBO_BOX_H

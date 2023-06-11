#include "max_connection_spinbox_frame.h"

MaxConnectionSpinBoxFrame::MaxConnectionSpinBoxFrame(MainWindow *parentUi)
{
    this->setObjectName("Max Connection Frame");

    maxConnectionSpinBox = new QSpinBox();
    maxConnectionSpinBox->setMaximumWidth(40);
    maxConnectionSpinBox->setEnabled(false);
    maxConnectionSpinBox->setMinimum(1);
    maxConnectionSpinBox->setMaximum(100);

    dataLabel = new QLabel();

    this->parentUi = parentUi;

    dataLabel->setText("Макс. подключений:");
}

void MaxConnectionSpinBoxFrame::createInterface()
{
    QHBoxLayout *frameLayout = new QHBoxLayout();
    this->setLayout(frameLayout);

    frameLayout->addWidget(dataLabel);
    frameLayout->addWidget(maxConnectionSpinBox);
}

void MaxConnectionSpinBoxFrame::enableInteface(bool value){
    maxConnectionSpinBox->setEnabled(value);
}

QMap<QString, QVariant> MaxConnectionSpinBoxFrame::getValue()
{
    QMap<QString, QVariant> valueMap;   //  переменная для возврата
    //  сообщение в консоль
    consoleMessage = "<font color = red>!!!<\\font> <br/> <font color = black><\\font>Установлено новое кол-во пользователей: "+QString::number(maxConnectionSpinBox->value())+"<br/><font color = red>!!!<\\font>";

    valueMap.insert(consoleMessage, maxConnectionSpinBox->value());
    return valueMap;
}

void MaxConnectionSpinBoxFrame::setValue(QVariant value)
{
    maxConnectionSpinBox->setValue(value.toInt());
}

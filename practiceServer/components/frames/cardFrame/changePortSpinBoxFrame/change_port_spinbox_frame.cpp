#include "change_port_spinbox_frame.h"

#include <QHBoxLayout>
#include <QMap>

ChangePortSpinBoxFrame::ChangePortSpinBoxFrame(MainWindow *parentUi)
{
    //  конструктор карточки
    this->setObjectName("Change port spinbox Frame");
    changePortSpinBox = new QSpinBox;
    changePortSpinBox->setMaximumWidth(95);
    changePortSpinBox->setEnabled(false);
    changePortSpinBox->setMaximum(65535);
    changePortSpinBox->setMinimum(1024);

    dataLabel = new QLabel;

    this->parentUi = parentUi;

    dataLabel->setText("Сменить порт: (1024-65535)");
}

void ChangePortSpinBoxFrame::createInterface()
{
    //  создание интерфейса
    QHBoxLayout *frameLayout = new QHBoxLayout();   //  слой выравнивания
    this->setLayout(frameLayout);

    frameLayout->addWidget(dataLabel);
    frameLayout->addWidget(changePortSpinBox);
}

void ChangePortSpinBoxFrame::enableInteface(bool value){
    changePortSpinBox->setEnabled(value);
}

QMap<QString, QVariant> ChangePortSpinBoxFrame::getValue()
{
    QMap<QString, QVariant> valueMap;   //  переменная для возврата
    //  сообщение в консоль
    consoleMessage = "<font color = red>!!!<\\font> <br/> <font color = black><\\font>Установлен новый порт: "+changePortSpinBox->text()+"<br/><font color = red>!!!<\\font>";

    valueMap.insert(consoleMessage, changePortSpinBox->text());
    return valueMap;
}

void ChangePortSpinBoxFrame::setValue(QVariant value)
{
    changePortSpinBox->setValue(value.toInt());
}

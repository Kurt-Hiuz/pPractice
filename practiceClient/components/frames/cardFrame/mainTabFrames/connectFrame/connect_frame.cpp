#include "connect_frame.h"

ConnectFrame::ConnectFrame(MainWindow *ui)
{
    this->setObjectName("ConnectFrame");
    parentUi = ui;

    IPLineEdit = new QLineEdit();
    IPLineEdit->setObjectName("IPLineEdit");
    IPLineEdit->setText("127.0.0.1");
    IPLineEdit->setPlaceholderText("Введите IP");

    portLineEdit = new QLineEdit();
    portLineEdit->setObjectName("portLineEdit");
    portLineEdit->setPlaceholderText("Введите порт");

    connectToServerPuchButton = new QPushButton("Подключиться");
    connect(connectToServerPuchButton, &QPushButton::clicked, this, &ConnectFrame::slotConnectToServerPushButtonClicked);
}

void ConnectFrame::createInterface()
{
    QVBoxLayout *mainVLayout = new QVBoxLayout();
    this->setLayout(mainVLayout);

    QFrame *lineEditsFrame = new QFrame();
    QHBoxLayout *lineEditsHLayout = new QHBoxLayout();
    lineEditsFrame->setLayout(lineEditsHLayout);
    lineEditsHLayout->addWidget(IPLineEdit);
    lineEditsHLayout->addWidget(portLineEdit);

    mainVLayout->addWidget(lineEditsFrame);
    mainVLayout->addWidget(connectToServerPuchButton);
}

QMap<QString, QVariant> ConnectFrame::getValue()
{
    QMap<QString, QVariant> mapValue;
    mapValue[IPLineEdit->objectName()] = IPLineEdit->text();
    mapValue[portLineEdit->objectName()] = portLineEdit->text();
    return mapValue;
}

void ConnectFrame::setValue(QVariant value)
{

}

void ConnectFrame::switchEnabledInteface()
{
    connectToServerPuchButton->setEnabled(!connectToServerPuchButton->isEnabled());
}

void ConnectFrame::slotConnectToServerPushButtonClicked()
{
    if(IPLineEdit->text().isEmpty()){
        IPLineEdit->setStyleSheet("background-color: red");
        IPLineEdit->setPlaceholderText("IP пустой!");
        return;
    }
    if(portLineEdit->text().isEmpty()){
        portLineEdit->setStyleSheet("background-color: red");
        portLineEdit->setPlaceholderText("Порт пустой!");
        return;
    }

    IPLineEdit->setStyleSheet("background-color: white");
    portLineEdit->setStyleSheet("background-color: white");

    parentUi->setEnableInteface();
}

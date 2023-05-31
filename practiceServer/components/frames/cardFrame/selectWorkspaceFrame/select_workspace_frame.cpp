#include "select_workspace_frame.h"

SelectWorkspaceFrame::SelectWorkspaceFrame(MainWindow *parentUi)
{
    this->setObjectName("Choose workspace directory Frame");

    chooseWorkspaceDirPushButton = new QPushButton("Выбрать рабочую папку");
    connect(chooseWorkspaceDirPushButton, &QPushButton::clicked, parentUi, &MainWindow::on_chooseWorkspaceDirPushButton_clicked);

    //  кнопка автоматически растягивается в зависимости от длины текста
    chooseWorkspaceDirPushButton->adjustSize();

    dataLabel = new QLabel();
    dataLabel->setText("Нет рабочей папки!");
    dataLabel->setObjectName("Data Label");

    this->parentUi = parentUi;
}

void SelectWorkspaceFrame::createInterface(){
    QHBoxLayout *frameLayout = new QHBoxLayout();
    this->setLayout(frameLayout);

    frameLayout->addWidget(chooseWorkspaceDirPushButton);
    frameLayout->addWidget(dataLabel);
}

QMap<QString, QVariant> SelectWorkspaceFrame::getValue(){
    QMap<QString, QVariant> valueMap;   //  переменная для возврата
    //  сообщение в консоль
    consoleMessage = "<font color = red>!!!<\\font> <br/> <font color = black><\\font>Установлена новая рабочая папка: "+dataLabel->text()+"<br/><font color = red>!!!<\\font>";

    valueMap.insert(consoleMessage, dataLabel->text());
    return valueMap;
}

void SelectWorkspaceFrame::setValue(QVariant value){
    dataLabel->setText(value.toString());
}

void SelectWorkspaceFrame::enableInteface()
{
    return;
}

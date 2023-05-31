#include "select_workspace_frame.h"

SelectWorkspaceFrame::SelectWorkspaceFrame(MainWindow *ui)
{
    this->parentUi = ui;
    dataLabel = new QLabel("Не выбрана рабочая папка");

    chooseWorkspaceDirPushButton = new QPushButton("Выбрать папку");
    connect(chooseWorkspaceDirPushButton, &QPushButton::clicked, parentUi, &MainWindow::on_chooseWorkspaceDirPushButton_clicked);
}

void SelectWorkspaceFrame::createInterface()
{
    QHBoxLayout *mainHLayout = new QHBoxLayout();
    this->setLayout(mainHLayout);

    mainHLayout->addWidget(chooseWorkspaceDirPushButton);
    mainHLayout->addWidget(dataLabel);
}

QMap<QString, QVariant> SelectWorkspaceFrame::getValue()
{
    QMap<QString, QVariant> valueMap;   //  переменная для возврата
    //  сообщение в консоль
    consoleMessage = "<font color = red>!!!<\\font> <br/> <font color = black><\\font>Установлена новая рабочая папка: "+dataLabel->text()+"<br/><font color = red>!!!<\\font>";

    valueMap.insert(consoleMessage, dataLabel->text());
    return valueMap;
}

void SelectWorkspaceFrame::setValue(QVariant value)
{
    dataLabel->setText(value.toString());
}

void SelectWorkspaceFrame::switchEnabledInteface()
{

}

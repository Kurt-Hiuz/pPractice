#include "possible_processing_frame.h"

PossibleProcessingFrame::PossibleProcessingFrame(MainWindow *ui)
{
    this->setObjectName("PossibleProcessingFrame");
    parentUi = ui;

    chooseProcessingComboBox = new QComboBox();
    chooseProcessingComboBox->setObjectName("chooseProcessingComboBox");
    chooseProcessingComboBox->setEnabled(false);

    chooseProcessingPushButton = new QPushButton("Выбрать");
    chooseProcessingPushButton->setObjectName("chooseProcessingPushButton");
    chooseProcessingPushButton->setEnabled(false);

    connect(chooseProcessingPushButton, &QPushButton::clicked, parentUi, &MainWindow::on_chooseProcessingPushButton_clicked);
}

void PossibleProcessingFrame::createInterface()
{
    QHBoxLayout *mainHLayout = new QHBoxLayout();
    this->setLayout(mainHLayout);

    mainHLayout->addWidget(chooseProcessingComboBox);
    mainHLayout->addWidget(chooseProcessingPushButton);
}

QMap<QString, QVariant> PossibleProcessingFrame::getValue()
{
    QMap<QString, QVariant> valueMap;   //  переменная для возврата
    //  сообщение в консоль
    consoleMessage = "<font color = red>!!!<\\font> <br/> <font color = black><\\font>Установлены новые обработки! <br/><font color = red>!!!<\\font>";

    valueMap.insert(consoleMessage, chooseProcessingComboBox->currentData());
    return valueMap;
}

void PossibleProcessingFrame::setValue(QVariant value)
{
    QMap<QString,QVariant> possibleProcessingData = value.toMap();

    chooseProcessingComboBox->clear();
    //  самая первая обработка - это отсутствие обработки
    chooseProcessingComboBox->addItem("Нет обработки", "");
    for(auto it = possibleProcessingData.begin(); it != possibleProcessingData.end(); it++){
        chooseProcessingComboBox->addItem(it.key(), it.value());
    }
}

void PossibleProcessingFrame::switchEnabledInteface()
{
    chooseProcessingComboBox->setEnabled(!chooseProcessingComboBox->isEnabled());
    chooseProcessingPushButton->setEnabled(!chooseProcessingPushButton->isEnabled());
}

QString PossibleProcessingFrame::getCurrentData()
{
    return chooseProcessingComboBox->currentData().toString();
}

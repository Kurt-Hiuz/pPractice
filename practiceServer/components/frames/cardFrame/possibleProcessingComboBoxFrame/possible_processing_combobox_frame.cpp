#include "possible_processing_combobox_frame.h"

PossibleProcessingComboBoxFrame::PossibleProcessingComboBoxFrame(MainWindow *parentUi)
{
    this->setObjectName("Possible processing Frame");

    possibleProcessingComboBox = new QComboBox();
    possibleProcessingComboBox->setEnabled(false);

    dataLabel = new QLabel();

    updateProcessingPushButton = new QPushButton("Редактировать");
    connect(updateProcessingPushButton, &QPushButton::clicked, this, &PossibleProcessingComboBoxFrame::on_updateProcessingPushButton_clicked);
    updateProcessingPushButton->setEnabled(false);

    this->parentUi = parentUi;

    dataLabel->setText("Возможные обработки:");
}

void PossibleProcessingComboBoxFrame::on_updateProcessingPushButton_clicked()
{
    updateProcessingDialog = new UpdateProcessingDialog(parentUi);
    updateProcessingDialog->createInterface();
    updateProcessingDialog->show();
}

void PossibleProcessingComboBoxFrame::createInterface()
{
    QHBoxLayout *frameLayout = new QHBoxLayout();
    this->setLayout(frameLayout);

    frameLayout->addWidget(dataLabel);
    frameLayout->addWidget(possibleProcessingComboBox);
    frameLayout->addWidget(updateProcessingPushButton);
}

void PossibleProcessingComboBoxFrame::enableInteface(bool value){
    updateProcessingPushButton->setEnabled(value);
    possibleProcessingComboBox->setEnabled(value);
}

QMap<QString, QVariant> PossibleProcessingComboBoxFrame::getValue()
{
    QMap<QString, QVariant> comboBoxData;   //  переменная для данных выпадающего списка
    for(int i = 0; i < possibleProcessingComboBox->count(); i++){
        comboBoxData[possibleProcessingComboBox->itemText(i)] = possibleProcessingComboBox->itemData(i);
    }

    QMap<QString, QVariant> valueMap;   //  переменная для возврата
    //  сообщение в консоль
    consoleMessage = "<font color = red>!!!<\\font> <br/> <font color = black><\\font>Установлены новые обработки! <br/><font color = red>!!!<\\font>";

    valueMap.insert(consoleMessage, comboBoxData);
    return valueMap;
}

void PossibleProcessingComboBoxFrame::setValue(QVariant value)
{
    possibleProcessingComboBox->clear();
    QMap<QString, QVariant> comboBoxValue = value.toMap();

    for(auto valueMap = comboBoxValue.begin(); valueMap != comboBoxValue.end(); valueMap++){
        possibleProcessingComboBox->addItem(valueMap.key(), valueMap.value());
    }
}

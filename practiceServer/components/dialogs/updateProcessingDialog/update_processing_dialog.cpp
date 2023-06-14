#include "update_processing_dialog.h"

UpdateProcessingDialog::UpdateProcessingDialog(MainWindow *parentUi)
{
    this->setObjectName("Update processing Dialog");
    this->parentUi = parentUi;

    this->setModal(true);

    dataLabel = new QLabel("Отредактируйте данные:");
    jsonDataTextEdit = new QTextEdit();

    openJsonFilePushButton = new QPushButton("Открыть");
    connect(openJsonFilePushButton, &QPushButton::clicked, this, &UpdateProcessingDialog::on_openJsonFilePushButton_clicked);

    closeDialogPushButton = new QPushButton("Закрыть");
    connect(closeDialogPushButton, &QPushButton::clicked, this, &UpdateProcessingDialog::on_closeDialogPushButton_clicked);

    applyDataPushButton = new QPushButton("Подтвердить");
    connect(applyDataPushButton, &QPushButton::clicked, this, &UpdateProcessingDialog::on_applyDataPushButton_clicked);
}

void UpdateProcessingDialog::createInterface()
{
    QVBoxLayout *frameLayout = new QVBoxLayout();
    this->setLayout(frameLayout);

    frameLayout->addWidget(dataLabel);
    frameLayout->addWidget(openJsonFilePushButton);
    frameLayout->addWidget(jsonDataTextEdit);
    frameLayout->addWidget(closeDialogPushButton);
    frameLayout->addWidget(applyDataPushButton);
}

QVariant UpdateProcessingDialog::getValue()
{
    return 0;
}

void UpdateProcessingDialog::on_openJsonFilePushButton_clicked()
{
    QString openFileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Json File"),
                                                    QString(),
                                                    tr("JSON (*.json)"));
    QFileInfo fileInfo(openFileName);
    QDir::setCurrent(fileInfo.path());

    QFile jsonFile(openFileName);
    if(!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        dataLabel->setText("Невозможно открыть файл!");
        qDebug() << "Невозможно открыть файл";
        return;
    }
    QByteArray saveData = jsonFile.readAll();

    QJsonDocument jsonDocument(QJsonDocument::fromJson(saveData));

    m_currentJsonObject = jsonDocument.object();

    this->jsonDataTextEdit->clear();
    //  выгружаем данные с json файла в форматированном виде
    this->jsonDataTextEdit->setText(QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented));

    jsonFile.close();

    qDebug() << openFileName;
    qDebug() << QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented);
    jsonFilePath = openFileName;
}

void UpdateProcessingDialog::on_closeDialogPushButton_clicked()
{
    this->close();
}

void UpdateProcessingDialog::on_applyDataPushButton_clicked()
{
    QByteArray jsonData  = jsonDataTextEdit->toPlainText().toUtf8();

    if(!m_jsonParser.isJson(jsonData)){
        dataLabel->setText("Документ содержит ошибку");
        return;
    }

    QFile possibleProcessingFile(jsonFilePath);

    if (!possibleProcessingFile.open(QIODevice::WriteOnly)) {
        dataLabel->setText("Невозможно открыть файл для сохранения");
    }

    QTextStream writeStream(&possibleProcessingFile);

    writeStream << jsonDataTextEdit->toPlainText();

    possibleProcessingFile.close();

    dataLabel->setText("Запись сохранена!");
}

#include "file_frame.h"

FileFrame::FileFrame(MainWindow *ui)
{
    this->setObjectName("FileFrame");
    parentUi = ui;

    filePathLabel = new QLabel();
    filePathLabel->setObjectName("filePathLabel");

    completer = new QCompleter(this);   //  создаем completer

    fileSystemModel = new QFileSystemModel();    //  создаем модель системы
    fileSystemModel->setRootPath(QDir::currentPath() + "\\");    //  устанавливаем разделитель

    completer->setModel(fileSystemModel);    //  completer'у устанавливаем модель

    completer->setCompletionMode(QCompleter::CompletionMode(0));    //  подсказки во всплывающем окне (2 - как выделенный текст, идущий далее)
    completer->setModelSorting(QCompleter::ModelSorting(2));    //  НЕ чувствительно к регистру (0 -  не сортировать, 1 - чувствительно к регистру)

    filePathLineEdit = new QLineEdit();
    filePathLineEdit->setObjectName("filePathLineEdit");
    filePathLineEdit->setCompleter(completer);  //  устанавливам completer
    filePathLineEdit->setPlaceholderText("Путь до файла");
    filePathLineEdit->setEnabled(false);

    openFilePushButton = new QPushButton("Открыть проводник");
    openFilePushButton->setObjectName("openFilePushButton");
    openFilePushButton->setEnabled(false);
    connect(openFilePushButton, &QPushButton::clicked, this, &FileFrame::openFilePushButtonClicked);

    sendFilePushButton = new QPushButton("Отправить файл");
    sendFilePushButton->setObjectName("sendFilePushButton");
    sendFilePushButton->setEnabled(false);
    connect(sendFilePushButton, &QPushButton::clicked, this, &FileFrame::sendFilePushButtonClicked);
}

void FileFrame::createInterface()
{
    QVBoxLayout *mainVLayout = new QVBoxLayout();
    this->setLayout(mainVLayout);

    QFrame *filePathFrame = new QFrame();
    QHBoxLayout *filePathHLayout = new QHBoxLayout();
    filePathFrame->setLayout(filePathHLayout);
    filePathHLayout->addWidget(filePathLineEdit);
    filePathHLayout->addWidget(openFilePushButton);

    mainVLayout->addWidget(filePathLabel);
    mainVLayout->addWidget(filePathFrame);
    mainVLayout->addWidget(sendFilePushButton);
}

QMap<QString, QVariant> FileFrame::getValue()
{

}

void FileFrame::setValue(QVariant value)
{
    filePathLabel->setText(value.toString());
}

void FileFrame::setEnabledInteface(bool value)
{
    filePathLabel->clear();
    filePathLineEdit->setEnabled(value);
    openFilePushButton->setEnabled(value);
    sendFilePushButton->setEnabled(value);
}

void FileFrame::sendFilePushButtonClicked()
{
    QString filePath;   //  определяем наш путь к файлу
    filePath = filePathLineEdit->text();    //  сохраняем в переменную путь к файлу
    filePathLineEdit->clear();  //  очищаем поле ввода пути файла после выбора нажатии отправки

    emit parentUi->signalSendFileToServer(filePath); //  отправляем серверу файл
}

void FileFrame::openFilePushButtonClicked()    //  по нажатию на "Открыть проводник"
{
    QString filePath;
    filePath = QFileDialog::getOpenFileName(this, "Выбор файла");   //  открываем диалоговое окно с заголовком "Выбор файла" и по умолчанию ставим путь C:/
    filePathLineEdit->setText(filePath);   //  устанавливаем путь в LineEdit для наглядности
}

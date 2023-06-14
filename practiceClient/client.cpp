#include "client.h"

Client::Client(){
    connect(this, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
    connect(this, &QTcpSocket::disconnected, this, &QTcpSocket::deleteLater);   //  при отключении сокет сам удалится

    nextBlockSize = 0;

    readyReadManager = new ReadyReadManager();
    connect(readyReadManager, &ReadyReadManager::signalMessageRRManagerClient, this, &Client::slotMessageServer);
    connect(readyReadManager, &ReadyReadManager::signalStatusRRManagerClient, this, &Client::slotStatusClient);
    connect(readyReadManager, &ReadyReadManager::signalSetCBData, this, &Client::slotSetCBData);
    connect(readyReadManager, &ReadyReadManager::signalSendBufferToServer, this, &Client::slotSendBufferToServer);
    connect(readyReadManager, &ReadyReadManager::signalSendToServer, this, &Client::slotSendToServer);
    connect(readyReadManager, &ReadyReadManager::signalDeleteSendedFile, this, &Client::slotDeleteSendedFile);
    connect(readyReadManager, &ReadyReadManager::signalEnableInterface, this, &Client::signalEnableInterface);
}

void Client::setWorkspaceManager(WorkspaceManager *newWorkspaceManager)
{
    this->workspaceManager = newWorkspaceManager;

    connect(workspaceManager, &WorkspaceManager::signalSiftFiles, this, &Client::slotSiftFiles);
}

void Client::slotSiftFiles(QStringList &filesList)
{
    for(int fileI = 0; fileI < filesList.size(); fileI++){
        QFileInfo currentFile(filesList.at(fileI));
        QString filePath = currentFile.filePath();
        slotSendFileToServer(filePath);
        workspaceManager->deleteFile(currentFile.fileName());
    }
}

void Client::slotSendTextToServer(QString &message, QString &senderName)
{
    Data.clear();   //  чистим массив байт
    QDataStream out(&Data, QIODevice::WriteOnly);   //  генерируем поток вывода
    out.setVersion(QDataStream::Qt_6_2);    //  устанавливаем последнюю версию
    out <<  quint64(0) << QString("Message") << message << senderName;   //  собираем сообщение из размер_сообщения << тип_сообщения << строка << отправитель
    out.device()->seek(0);  //  передвигаемся в начало
    out << quint64(Data.size() - sizeof(quint64));  //  избавляемся от зарезервированных двух байт в начале каждого сообщения
    this->write(Data);    //  записываем данные в сокет
}

void Client::slotSendFileToServer(QString &filePath)
{
    Data.clear();   //  чистим массив байт от мусора
    file = new QFile(filePath);   //  определяем файл, чтобы поработать с его свойствами и данными
    fileSize = file->size();     //  определяем размер файла
    QFileInfo fileInfo(file->fileName());    //  без этой строки название файла будет хранить полный путь до него
    fileName = fileInfo.fileName();     //  записываем название файла

    emit signalSetFilePathLabel("Size: "+QString::number(fileSize)+" Name: "+fileName);  //  простое уведомление пользователя о размере и имени файла, если мы смогли его открыть

    if(!file->open(QIODevice::ReadOnly)){ //  открываем файл для только чтения
        emit signalSetFilePathLabel("Файл не открывается для передачи");
        return;
    }
    file->close();  //  Открытие-закрытие было сделано для проверки, можем ли мы вообще отправить файл

    this->waitForBytesWritten();  //  мы ждем того, чтобы все байты записались

    QDataStream out(&Data, QIODevice::WriteOnly);   //  определяем поток отправки
    out.setVersion(QDataStream::Qt_6_2);
    out << quint64(0) << QString("File") << fileName << fileSize;   //  отправляем название файла и его размер
    out.device()->seek(0);
    //  избавляемся от зарезервированных двух байт в начале каждого сообщения
    out << quint64(Data.size() - sizeof(quint64));   //  определяем размер сообщения
    this->write(Data);

    readyReadManager->setFileClientFileRequest(filePath);
}

void Client::slotSetClientFolders(QMap<QString, QString> &subFolders)
{
    for(auto it = subFolders.begin(); it != subFolders.end(); it++){
        if(it.key() == "Root"){
            this->workspaceFolder = it.value();  //  установили новую директорию
            continue;
        }

        if(it.key() == "Entry"){
            entryFolder = it.value();    //  папка для файлов извне
            readyReadManager->setEntryFolder(entryFolder);

            fileSystemWatcher = new QFileSystemWatcher;
            fileSystemWatcher->addPath(entryFolder);    //  устанавливаем на слежку папку для приходящих извне файлов

            connect(fileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, &Client::slotEntryFolderChanged);

            continue;
        }
        if(it.key() == "Processed"){
            processedFolder = it.value();    //  папка для хранения конечных файлов
            continue;
        }
    }
}

void Client::slotReadyRead()
{
    QDataStream in(this); //  создание объекта "in", помогающий работать с данными в сокете
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status()==QDataStream::Ok){
        while(true){    //  цикл для расчета размера блока
            if(nextBlockSize == 0){ //  размер блока пока неизвестен
                if(this->bytesAvailable() < 8){   //  и не должен быть меньше 8-и байт
                   break;  //  иначе выходим из цикла, т.е. размер посчитать невозможно
                }
                in >> nextBlockSize;    //  считываем размер блока в правильном исходе
            }
            if(this->bytesAvailable() < nextBlockSize){   //  когда уже известен размер блока, мы сравниваем его с количеством байт, которые пришли от сервера
                break;
            }
            //  надо же, мы до сих пор в цикле, все хорошо

            QString typeOfMessage;
            in >> typeOfMessage;    //  определение типа сообщения
            I_MessageManager *messageManager = readyReadManager->identifyMessage(typeOfMessage);

            if(messageManager->typeOfMessage() == "No type"){
                emit signalStatusClient("Была произведена попытка отправки сообщения неизвестного типа!");
                nextBlockSize = 0;
                return;
            }

            messageManager->processData(in);

            nextBlockSize = 0;  //  обнуляем для новых сообщений
            if(this->bytesAvailable() == 0){
                break;  //  выходим, делать больше нечего
            }
        }
    }
}

void Client::slotMessageServer(QString message)
{
    emit signalMessageTextBrowser(message);
}

void Client::slotStatusClient(QString status)
{
    emit signalStatusClient(status);
}

void Client::slotSetCBData(QMap<QString, QVariant> &possibleProcessingData)
{
    emit signalSetCBDataForm(possibleProcessingData);
}

void Client::slotSendBufferToServer(QByteArray &data)
{
    this->waitForBytesWritten();  //  мы ждем того, чтобы все байты записались
    Data.clear();   //  чистим массив байт
    QDataStream out(&Data, QIODevice::WriteOnly);   //  генерируем поток вывода
    out.setVersion(QDataStream::Qt_6_2);    //  устанавливаем последнюю версию
    out << quint64(0) << QString("File") << data;   //  собираем сообщение из размер_сообщения << тип_сообщения << строка << отправитель
    out.device()->seek(0);  //  передвигаемся в начало
    out << quint64(Data.size() - sizeof(quint64));  //  избавляемся от зарезервированных двух байт в начале каждого сообщения
    this->write(Data);    //  записываем данные в сокет
}

void Client::slotSendToServer(QString typeOfMsg, QString str)
{
    Data.clear();   //  чистим массив байт
    QDataStream out(&Data, QIODevice::WriteOnly);   //  генерируем поток вывода
    out.setVersion(QDataStream::Qt_6_2);    //  устанавливаем последнюю версию
    out <<  quint64(0) << typeOfMsg << str;   //  собираем сообщение из размер_сообщения << тип_сообщения << строка << отправитель
    out.device()->seek(0);  //  передвигаемся в начало
    out << quint64(Data.size() - sizeof(quint64));  //  избавляемся от зарезервированных двух байт в начале каждого сообщения
    this->write(Data);    //  записываем данные в сокет
}

void Client::slotEntryFolderChanged(const QString &fileName)
{
}

void Client::slotDeleteSendedFile(QString &fileName)
{
    workspaceManager->deleteFile(fileName);
}



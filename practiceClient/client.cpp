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
//    connect(readyReadManager, &ReadyReadManager::signalSendToAllClientsServer, this, &Client::slotSendToAllClients);
//    connect(readyReadManager, &ReadyReadManager::signalSendToOneRRManager, this, &Client::slotSendToOneClie
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
    qDebug() << "Client::slotSendFileToServer:      sending data size: " << Data.size() - sizeof(quint64);
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

    qDebug() << "Client::slotSetClientFolders:        " << workspaceFolder;
    qDebug() << "Client::slotSetClientFolders:        " << entryFolder << processedFolder;
}

void Client::slotReadyRead()
{
    QDataStream in(this); //  создание объекта "in", помогающий работать с данными в сокете
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status()==QDataStream::Ok){
        while(true){    //  цикл для расчета размера блока
            if(nextBlockSize == 0){ //  размер блока пока неизвестен
                qDebug() << "Client::slotReadyRead:     nextBlockSize == 0";
                if(this->bytesAvailable() < 8){   //  и не должен быть меньше 8-и байт
                    qDebug() << "Data < 8, break";
                    break;  //  иначе выходим из цикла, т.е. размер посчитать невозможно
                }
                in >> nextBlockSize;    //  считываем размер блока в правильном исходе
                qDebug() << "Client::slotReadyRead:     nextBlockSize:  " << nextBlockSize;
                qDebug() << "Client::slotReadyRead:     this->bytesAvailable():  " << this->bytesAvailable();
            }
            if(this->bytesAvailable() < nextBlockSize){   //  когда уже известен размер блока, мы сравниваем его с количеством байт, которые пришли от сервера
                qDebug() << "Client::slotReadyRead:     Data not full | socket->bytesAvailable() = "+QString::number(socket->bytesAvailable()) + " | nextBlockSize = "+QString::number(nextBlockSize);    //  если данные пришли не полностью
                break;
            }
            //  надо же, мы до сих пор в цикле, все хорошо

            QString typeOfMessage;
            in >> typeOfMessage;    //  определение типа сообщения
            qDebug() << "Client::slotReadyRead:     typeOfMessage:  "   <<  typeOfMessage;

            I_MessageManager *messageManager = readyReadManager->identifyMessage(typeOfMessage);

            if(messageManager->typeOfMessage() == "No type"){
                qDebug() << "Client::slotReadyRead:     No type";
                emit signalStatusClient("Была произведена попытка отправки сообщения неизвестного типа!");
                nextBlockSize = 0;
                return;
            }

            messageManager->processData(in);

    //            if(typeOfMessage == "File"){    //  отправляется файл

    //                // mapRequest["002"] << fileName << fileSize

    //                if(fileName.isEmpty()){    //  если файла не существует
    //                    in >> fileName;  //  записываем из потока название файла
    //                    in >> fileSize; //  считываем его размер

    //                    if(fileSize < blockData){   //  если размер файла меньше выделенного блока
    //                        blockData = fileSize;   //  устанавливаем размер блока ровно по файлу (передача произойдет в один этап)
    //                    } else {
    //                        blockData = 1000000;  //  устанавливаем по умолчанию (на случай последующей передачи, если размер файла будет куда больше)
    //                    }

    //                    file = new QFile;     //  определяем файл
    //                    file->setFileName(fileName);    //  устанавливаем имя файла
    //                    QDir::setCurrent(rawInformationDirectory);  //  устанавливаем путь сохранения на рабочем столе


    //                    SendToServer(mapRequest["103"],"Downloading new part of processing file to "+socket->localAddress().toString()+"...");    //  запрашиваем первую часть файла
    //                }
    //            }

    //            if(typeOfMessage == "Request part of file"){    //  если серверу нужна еще одна часть файла
    //                QString str;    //  определяем переменную, в которую сохраним уведомление от запроса
    //                in >> str;  //  выводим в переменную сообщение

    //                qDebug() << str;  //  выводим в консоль
    //                ui->textBrowser->append(QTime::currentTime().toString()+" | "+str); //  выводим клиенту

    //                nextBlockSize = 0;  //  заранее обнуляем размер сообщения
    //                SendPartOfFile();   //  вызываем соответствующий метод отправки
    //            }

    //            if(typeOfMessage == "Request part of processing file"){
    //                if((fileSize - file->size()) < blockData){  //  если разница между плановым и текущим размером файла меньше блока байтов
    //                    blockData = fileSize - file->size();    //  мы устанавливаем такой размер для блока (разницу)
    //                }

    //                bytes = new char[blockData];   //  выделяем байты под файл, то есть передача пройдет в несколько этапов

    //                in >> bytes;    //  считываем байты

    //                if(file->open(QIODevice::Append)){  //  записываем в конец файла
    //                    file->write(bytes, blockData);    //  записываем в файл
    //                } else {
    //                    qDebug() << "Не удается открыть файл "+fileName;
    //                }

    //                if(file->size() < fileSize){    //  если размер до сих пор не полон
    //                    qDebug() << "Текущий размер файла "+fileName+" от "+QString::number(socket->socketDescriptor())+" = "+QString::number(file->size())+"\n"+"Ожидаемый размер = "+QString::number(fileSize);

    //                    //  SendToAllClients(mapRequest["102"],"<font color = black><\\font>Downloading new part of file...<font color = black><\\font>");    //  запрашиваем новую часть файл
    //                    SendToServer(mapRequest["103"],"<font color = black><\\font>Downloading new part of processing file...<font color = black><\\font>");    //  запрашиваем первую часть файла
    //                } else {
    //                    //  оформляем чат на стороне Сервера
    //                    //  уведомление о "кто: какой файл" при сигнале "012" - File downloaded
    //                    qDebug() << "Server: send file by name \""+fileName+"\"";
    //                    //  SendToAllClients(mapRequest["012"],"<font color = green><\\font>User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": send file by name \""+fileName+"\" \n"+delimiter);

    //                    SendToServer(mapRequest["012"], "<font color = green><\\font>file \""+fileName+"\" downloaded \n"+delimiter);

    //                    //  TODO: при отправке всем происходит баг в задержке сообщений. решить
    //                    //  SendToAllClients(mapRequest["001"], "<font color = green><\\font>User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": send file by name \""+fileName+"\" \n"+delimiter);

    //                    file->close();  //  закрываем файл
    //                    file = nullptr; //  удаляем файл
    //                    fileName.clear();   //  очищаем его название
    //                    fileSize = 0;   //  очищаем его размер
    //                    blockData = 1000000;  //  устанавливаем прежний размер байтов
    //                    delete[] bytes; //  удаляем байты из кучи
    //                    nextBlockSize = 0;  //  обнуляем для новых сообщений

    //                    return; //  выходим
    //                }

    //                file->close();   //закрываем файл
    //                if(bytes != nullptr){   //  удаляем байты из кучи, делая проверку на случай двойного удаления
    //                    delete[] bytes;
    //                    bytes = nullptr;
    //                }
    //                nextBlockSize = 0;  //  обнуляем для новых сообщений
    //            }

    //            if(typeOfMessage == "File downloaded"){ //  если файл полностью скачался
    //                QString str;    //  определяем переменную, в которую сохраним данные
    //                in >> str;  //  выводим в переменную сообщение
    //                qDebug() << "File "+fileName+" downloaded";   //  выводим консоль, какой файл был загружен
    //                ui->textBrowser->append(QTime::currentTime().toString()+" | "+str);  //  и то же самое клиенту
    //                file->close();
    //                delete file; //  удаляем файл
    //                file = nullptr;
    //                fileName.clear();   //  очищаем его название
    //                delete[] bytes; //  удаляем байты из кучи
    //                nextBlockSize = 0;  //  обнуляем для новых сообщений
    //            }

    //            if(typeOfMessage == "Possible treatments ComboBox data"){
    //                possibleTreatments.clear();
    //                ui->chooseTreatmentComboBox->clear();
    //                in >> possibleTreatments;  //  выводим в глобальную переменную map из доступных обработок
    //                qDebug() << possibleTreatments;

    //                for(auto item = possibleTreatments.begin(); item != possibleTreatments.end(); ++item)
    //                {
    //                    //  Вставляем в комбобокс "в конец, читаемый текст, префикс"
    //                    ui->chooseTreatmentComboBox->insertItem(ui->chooseTreatmentComboBox->count(), item.value(), item.key());
    //                }
    //                nextBlockSize = 0;  //  обнуляем для новых сообщений
    //            }

    //            if(typeOfMessage == "Disconnect"){
    //                QString str;
    //                in >> str;

    //                qDebug() << "Disconnect";
    //                ui->textBrowser->append(str);
    //                setEnabledInterface(true);
    //                socket->disconnectFromHost();
    //            }

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
    qDebug() << "Client::slotSendBufferToServer:    sending blockSize = " << quint64(Data.size() - sizeof(quint64));
    out << quint64(Data.size() - sizeof(quint64));  //  избавляемся от зарезервированных двух байт в начале каждого сообщения
    this->write(Data);    //  записываем данные в сокет
    qDebug() << "Client::slotSendBufferToServer:    Data size = " << Data.size();
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
    qDebug() << "Client::slotEntryFolderChanged:    " << fileName;
}



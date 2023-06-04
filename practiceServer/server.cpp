#include "server.h"

Server::Server(bool &server_started){
    if(this->listen(QHostAddress::Any, generatedServerPort)){  //  статус будет передаваться, когда сервер будет прослушивать любой из адресов
        server_started = true;  //  меняем состояние сервера
        qDebug() << "Server::Server:        start on "+QString::number(generatedServerPort)+" port";    //  уведомляем в консоли
    } else {
        server_started = false; //  иначе что-то пошло не так
    }
    nextBlockSize = 0;  //  обнуляем размер сообщения в самом начале работы

    /// Глоссарий, описывающий тип отправляемого сообщения
    /// первое число определяет тип (0 - простой сигнал о чем-то \ 1 - запрос чего-то)
    /// второе число определяет конец какого-то действия, если оно в несколько этапов, например, передача файла
    /// третье и последующие числа определяют тип передаваемых данных

    mapRequest[""] = "";  //  ничего не нужно
    mapRequest["000"] = "Disconnect";   //  сигнал на отключение
    mapRequest["001"] = "Message";   //  отправляется простое сообщение
    mapRequest["0011"] = "Message from someone";    //  отправляется сообщение от кого-то конкретного
    mapRequest["002"] = "File";  //  отправляется файл (определяем начало процесса передачи файла)
    mapRequest["102"] = "Request part of file";  //  запрос на еще одну часть файла
    mapRequest["103"] = "Request part of processing file";  //  запрос на еще одну часть обрабатываемого файла
    mapRequest["012"] = "File downloaded";  //  файл загружен полностью (определяем конец процесса передачи файла)
    mapRequest["004"] = "Possible processing ComboBox data";    //  отправка данных по доступным обработкам
    mapRequest["0041"] = "Set treatment on client";     //  закрепление возможной обработки за сокетом

    readyReadManager = new ReadyReadManager();
    connect(readyReadManager, &ReadyReadManager::signalStatusRRManagerServer, this, &Server::slotStatusServer);
    connect(readyReadManager, &ReadyReadManager::signalSendToAllClientsServer, this, &Server::slotSendToAllClients);
    connect(readyReadManager, &ReadyReadManager::signalSendToOneRRManager, this, &Server::slotSendToOneClient);
    connect(readyReadManager, &ReadyReadManager::signalSendBufferToClient, this, &Server::slotSendBufferToClient);
    connect(readyReadManager, &ReadyReadManager::signalSetClientProcessing, this, &Server::slotSetClientProcessing);
}

void Server::SendPossibleProcessing(QTcpSocket* socketToSend, QMap<QString,QVariant> possibleProcessingData)
{
    Data.clear();   //  может быть мусор
    QDataStream out(&Data, QIODevice::WriteOnly);   //  объект out, режим работы только для записи, иначе ничего работать не будет
    out.setVersion(QDataStream::Qt_6_2);
    out << quint64(0) << mapRequest["004"] << possibleProcessingData;  //  отправляем в поток размер_сообщения, тип-сообщения и строку при необходимости
    out.device()->seek(0);  //  в начало потока
    out << quint64(Data.size() - sizeof(quint64));  //  высчитываем размер сообщения
    socketToSend->write(Data);    //  отправляем по сокету данные
}

void Server::slotEntryFolderChanged(const QString &folderName)
{
//    qDebug() << "Server::slotEntryFolderChanged:        ";
//    processingManager = new ProcessingManager();
//    QFileInfoList list = processingManager->entryFiles(folderName);     //  получаем список файлов директории

//    for (int i = 0; i < list.size(); i++) {
//        QFileInfo fileInfo = list.at(i);
//        qDebug() << "Server::slotEntryFolderChanged:        " << qPrintable(QString("%1 %2 %3").arg(fileInfo.size(), 10).arg(fileInfo.fileName(), 5).arg(fileInfo.lastModified().toString()));   //  выводим в формате "размер имя"
////        SendFileToClient(fileInfo.filePath());  //  отправляем файл клиенту
//    }
//    qDebug() << "Server::slotEntryFolderChanged:        " << folderName;
//    qDebug() << "Server::slotEntryFolderChanged:        " << "================";     // переводим строку

//    processingManager = nullptr;    //  освобождаем память
}

void Server::slotSendBufferToClient(QTcpSocket *socketToSend, QByteArray &buffer)
{
    socketToSend->waitForBytesWritten();  //  мы ждем того, чтобы все байты записались
    Data.clear();   //  чистим массив байт
    QDataStream out(&Data, QIODevice::WriteOnly);   //  генерируем поток вывода
    out.setVersion(QDataStream::Qt_6_2);    //  устанавливаем последнюю версию
    out << quint64(0) << QString("File") << buffer;   //  собираем сообщение из размер_сообщения << тип_сообщения << строка << отправитель
    out.device()->seek(0);  //  передвигаемся в начало
    qDebug() << "Server::slotSendBufferToClient:    sending blockSize = " << quint64(Data.size() - sizeof(quint64));
    out << quint64(Data.size() - sizeof(quint64));  //  избавляемся от зарезервированных двух байт в начале каждого сообщения
    socketToSend->write(Data);    //  записываем данные в сокет
    qDebug() << "Server::slotSendBufferToClient:    Data size = " << Data.size();
}

void Server::slotSetClientProcessing(QTcpSocket *socket, QString currentProcessing)
{
    mapSockets[socket] = currentProcessing;
    qDebug() << "Server::setClientProcessing:     mapSockets = " << mapSockets;
    if(currentProcessing != ""){
        emit signalStatusServer("<font color = blue><\\font>Клиент "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+" выбрал \""+currentProcessing+"\" обработку</hr>");
    } else {
        emit signalStatusServer("<font color = red><\\font>Client on "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+" убрал обработку</hr>");
    }
}

void Server::slotSocketDisplayed(QTcpSocket* displayedSocket)
{
    SendPossibleProcessing(displayedSocket, possibleProcessing);
}

void Server::slotDisconnectSocket(int socketDiscriptorToDelete) //  обработчик принудительного удаления сокетов
{
    for(auto item = mapSockets.begin(); item != mapSockets.end(); item++){  //  пробегаемся по сокетам
        if(item.key()->socketDescriptor() == socketDiscriptorToDelete){     //  ищем совпадение по сокету
            SendToOneClient(item.key(), mapRequest["000"], "Disconnect from host"); //  отправляем клиенту сигнал на отключение

            qDebug() << "Server::slotDisconnectSocket       " << "pop quantity of clients: "+QString::number(mapSockets.size());
            break;
        }
    }
}

void Server::slotUpdatePossibleProcessing(QVariant newPossibleProcessingData)
{
    this->possibleProcessing.clear();

    QMapIterator<QString, QVariant> i(newPossibleProcessingData.toMap());

    while(i.hasNext()){
        i.next();
        this->possibleProcessing.insert(i.key(), i.value());
    }

    for(auto it = mapSockets.begin(); it != mapSockets.end(); it++){
        SendPossibleProcessing(it.key(), possibleProcessing);
    }

    emit signalStatusServer("Список обработок отправлен всем клиентам!");
    qDebug() << "Server::slotUpdatePossibleProcessing:      " << this->possibleProcessing;
}

void Server::slotSetServerFolders(QMap<QString, QString> &subFolders)
{
    for(auto it = subFolders.begin(); it != subFolders.end(); it++){

        if(it.key() == "Root"){
            this->workspaceFolder = it.value();  //  установили новую директорию
            continue;
        }

        if(it.key() == "Entry"){
            entryFolder = it.value();    //  папка для файлов извне
            readyReadManager->setEntryFolder(entryFolder);

//            fileSystemWatcher = new QFileSystemWatcher;
//            fileSystemWatcher->addPath(entryFolder);    //  устанавливаем на слежку папку для приходящих извне файлов

//            connect(fileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, &Server::slotEntryFolderChanged);
            continue;
        }

        if(it.key() == "Storage"){
            storageFolder = it.value();    //  папка для хранения конечных файлов
            continue;
        }

        if(it.key() == "Expectation"){
            expectationFolder = it.value();    //  папка для файлов в состоянии ожидания
            continue;
        }
    }

    qDebug() << "Server::slotSetServerFolders:      all Data folders:   " << entryFolder << storageFolder << expectationFolder;
}

void Server::incomingConnection(qintptr socketDescriptor){  //  обработчик нового подключения
    socket = new QTcpSocket;    //  создание нового сокета под нового клиента
    socket->setSocketDescriptor(socketDescriptor);  //  устанавливаем в него дескриптор (- неотрицательное число, идентифицирующее  поток ввода-вывода)
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);  //  связка готовности чтения
    connect(socket, &QTcpSocket::disconnected, this, &Server::slotDisconnect); //  связка удаления клиента

    mapSockets[socket] = "";    //  клиент пока не показал, что он умеет делать
    emit signalStatusServer("new client on " + QString::number(socketDescriptor));   //  уведомление о подключении
    emit signalAddSocketToListWidget(socket);    //  отображаем на форме в clientsListWidget этот сокет
    SendToAllClients(mapRequest["001"], "new client on " + QString::number(socketDescriptor));
    SendPossibleProcessing(socket, possibleProcessing);
    qDebug() << "Server::incomingConnection:        new client on " << socketDescriptor;
    qDebug() << "Server::incomingConnection:        push quantity of clients: "+QString::number(mapSockets.size());
}

void Server::slotReadyRead(){
    socket = (QTcpSocket*)sender(); //  записываем именно тот сокет, с которого пришел запрос
    QDataStream in(socket); //  создание объекта "in", помогающий работать с данными в сокете
    in.setVersion(QDataStream::Qt_6_2); //  установка версии, чтобы не было ошибок
    if(in.status() != QDataStream::Ok){ //  если у нас нет ошибок в состоянии работы in
        emit signalStatusServer("Ошибка чтения потока данных!");    //  при ошибке чтения сообщения
        return;
    }

    while(true){    //  цикл для расчета размера блока
        if(nextBlockSize == 0){ //  размер блока пока неизвестен
            qDebug() << "Server::slotReadyRead:     nextBlockSize == 0";
            qDebug() << "Server::slotReadyRead:     size of waiting bytes" << socket->bytesAvailable();   //  выводим размер ожидающих байтов
            if(socket->bytesAvailable() < 8){   //  и не должен быть меньше 8-и байт
                qDebug() << "Server::slotReadyRead:     Data < 8, break";
                break;  //  иначе выходим из цикла, т.е. размер посчитать невозможно
            }
            in >> nextBlockSize;    //  считываем размер блока в правильном исходе
            qDebug() << "Server::slotReadyRead:     nextBlockSize: " << nextBlockSize;
            qDebug() << "Server::slotReadyRead:     размер данных:  " << nextBlockSize - 26;
        }
        if(socket->bytesAvailable() < nextBlockSize){   //  когда уже известен размер блока, мы сравниваем его с количеством байт, которые пришли от сервера
            qDebug() << "Server::slotReadyRead:     Data not full | socket->bytesAvailable() = "+QString::number(socket->bytesAvailable()) + " | nextBlockSize = "+QString::number(nextBlockSize);    //  если данные пришли не полностью
            break;
        }
        //  надо же, мы до сих пор в цикле, все хорошо

        QString typeOfMess;
        in >> typeOfMess;   //  считываем тип сообщения

        qDebug() << "Server::slotReadyRead:     остаток после чтения с in: " << socket->bytesAvailable();
        qDebug() << "Server::slotReadyRead:     Тип сообщения:     " << typeOfMess;
        I_MessageManager *messageManager = readyReadManager->identifyMessage(typeOfMess);

        if(messageManager->typeOfMessage() == "No type"){
            emit signalStatusServer("Была произведена попытка отправки сообщения неизвестного типа!");
            nextBlockSize = 0;
            return;
        }

        messageManager->processData(in, socket);

//        if(typeOfMess == "Message"){     //  если тип данных "Message"
//            QString str;    //  создаем переменную строки
//            in >> str;  //  записываем в нее строку из объекта in, чтобы проверить содержимое
//            qDebug() << "Server::slotReadyRead:     остаток после чтения str: " << socket->bytesAvailable();
//            Server::signalStatusServer("User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+str);     //  оформляем чат на стороне Сервера

//            SendToAllClients(mapRequest["001"],"<font color = black><\\font>User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+str.remove(0,5)+delimiter);      //  мы просто избавляемся от префикса "MESS:" и пересылаем клиенту сообщение
//        }

//        if(typeOfMess == "Message from someone"){   //  если сообщение с конкретным отправителем
//            QString str, someone;    //  создаем переменную строки и отправителя
//            in >> str >> someone;   //  считываем
//            Server::signalStatusServer(someone+" "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+str);     //  оформляем чат на стороне Сервера
//            SendToAllClients(mapRequest["001"],"<font color = black><\\font>"+someone+" "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+str.remove(0,5)+delimiter);      //  мы просто избавляемся от префикса "MESS:" и пересылаем клиенту сообщение
//        }

//        if(typeOfMess == "File"){    //  отправляется файл

//            // mapRequest["002"] << fileName << fileSize

//            if(fileName.isEmpty()){    //  если файла не существует
//                in >> fileName;  //  записываем из потока название файла
//                in >> fileSize; //  считываем его размер

//                if(fileSize < blockData){   //  если размер файла меньше выделенного блока
//                    blockData = fileSize;   //  устанавливаем размер блока ровно по файлу (передача произойдет в один этап)
//                } else {
//                    blockData = 1000000;  //  устанавливаем по умолчанию (на случай последующей передачи, если размер файла будет куда больше)
//                }

//                file = new QFile;     //  определяем файл
//                file->setFileName(fileName);    //  устанавливаем имя файла

//                //  этой функции entryFolder определяется в Server::slotNewWorkspaceFolder
//                //  поскольку этот слот всегда происходит раньше вызова Server::slotReadyRead
//                QDir::setCurrent(entryFolder);  //  устанавливаем путь сохранения
//                Server::signalStatusServer("Файл "+fileName+" создан на сервере");  //  уведомляем

//                SendToOneClient(socket, mapRequest["102"],"Downloading new part of file...");    //  запрашиваем первую часть файла
//            }
//        }

//        if(typeOfMess == "Request part of processing file"){    //  от нас запрашивается новая часть файла
//            QString str;    //  определяем переменную, в которую сохраним уведомление от запроса
//            in >> str;  //  выводим в переменную сообщение

//            qDebug() << "Server::slotReadyRead:     str = " << str;  //  выводим в консоль
//            Server::signalStatusServer(str); //  выводим клиенту

//            SendPartOfFile();   //  вызываем соответствующий метод отправки
//            nextBlockSize = 0;  //  заранее обнуляем размер сообщения
//        }

//        if(typeOfMess == "Request part of file"){   //  отправляется часть файла
//            if((fileSize - file->size()) < blockData){  //  если разница между плановым и текущим размером файла меньше блока байтов
//                blockData = fileSize - file->size();    //  мы устанавливаем такой размер для блока (разницу)
//            }

//            bytes = new char[blockData];   //  выделяем байты под файл, то есть передача пройдет в несколько этапов

//            in >> bytes;    //  считываем байты

//            if(file->open(QIODevice::Append)){  //  записываем в конец файла
//                file->write(bytes, blockData);    //  записываем в файл
//            } else {
//                Server::signalStatusServer("Не удается открыть файл "+fileName);
//            }

//            if(file->size() < fileSize){    //  если размер до сих пор не полон
//                Server::signalStatusServer("Текущий размер файла "+fileName+" от "+QString::number(socket->socketDescriptor())+" = "+QString::number(file->size())+"\n"+"Ожидаемый размер = "+QString::number(fileSize));

//                //  SendToAllClients(mapRequest["102"],"<font color = black><\\font>Downloading new part of file...<font color = black><\\font>");    //  запрашиваем новую часть файл
//                SendToOneClient(socket, mapRequest["102"],"<font color = black><\\font>Downloading new part of file...<font color = black><\\font>");    //  запрашиваем первую часть файла
//            } else {
//                //  оформляем чат на стороне Сервера
//                //  уведомление о "кто: какой файл" при сигнале "012" - File downloaded
//                Server::signalStatusServer("User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": send file by name \""+fileName+"\"");
//                //  SendToAllClients(mapRequest["012"],"<font color = green><\\font>User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": send file by name \""+fileName+"\" \n"+delimiter);

//                SendToOneClient(socket, mapRequest["012"], "<font color = green><\\font>file \""+fileName+"\" downloaded \n"+delimiter);

//                //  TODO: при отправке всем происходит баг в задержке сообщений. решить
//                //  SendToAllClients(mapRequest["001"], "<font color = green><\\font>User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": send file by name \""+fileName+"\" \n"+delimiter);

//                file->close();  //  закрываем файл
//                file = nullptr; //  удаляем файл
//                fileName.clear();   //  очищаем его название
//                fileSize = 0;   //  очищаем его размер
//                blockData = 1000000;  //  устанавливаем прежний размер байтов
//                delete[] bytes; //  удаляем байты из кучи
//                nextBlockSize = 0;  //  обнуляем для новых сообщений

//                return; //  выходим
//            }

//            file->close();   //закрываем файл
//            if(bytes != nullptr){   //  удаляем байты из кучи, делая проверку на случай двойного удаления
//                delete[] bytes;
//                bytes = nullptr;
//            }

//        }   //  конец, если отправляется файл

//        if(typeOfMess == "File downloaded"){ //  если файл полностью скачался
//            QString str;    //  определяем переменную, в которую сохраним данные
//            in >> str;  //  выводим в переменную сообщение
//            qDebug() << "Server::slotReadyRead:     File "+fileName+" downloaded";   //  выводим консоль, какой файл был загружен
//            Server::signalStatusServer(str);  //  и то же самое клиенту
//            file->close();
//            delete file; //  удаляем файл
//            file = nullptr;
//            fileName.clear();   //  очищаем его название
//            delete[] bytes; //  удаляем байты из кучи
//            nextBlockSize = 0;  //  обнуляем для новых сообщений
//        }

//        if(typeOfMess == "Set treatment on client"){
//            QString currentTreatment;
//            in >> currentTreatment;
//            mapSockets[socket] = currentTreatment;
//            qDebug() << "Server::slotReadyRead:     mapSockets = " << mapSockets;
//            if(currentTreatment != ""){
//                Server::signalStatusServer("<font color = blue><\\font>Client on "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+" choose "+currentTreatment+" for processing"+delimiter);
//            } else {
//                Server::signalStatusServer("<font color = red><\\font>Client on "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+" removed the processing "+delimiter);
//            }
//        }

        nextBlockSize = 0;  //  обнуляем для новых сообщений
//        if(socket->bytesAvailable() == 0){
            break;  //  выходим, делать больше нечего
//        }

//        delete messageManager;
    }   //  конец while
}

void Server::slotDisconnect()
{
    QTcpSocket* disconnectedSocket = static_cast<QTcpSocket*>(QObject::sender());
    mapSockets.remove(disconnectedSocket);
    qDebug() << "Server::slotDisconnect:        pop quantity of clients: "+QString::number(mapSockets.size());
    SendToAllClients(mapRequest["001"], "<font color = red><\\font>User  "+disconnectedSocket->localAddress().toString()+": has disconnected <hr/>");
    Server::signalStatusServer("<font color = red><\\font>User  "+disconnectedSocket->localAddress().toString()+": has disconnected <hr/>");
    Server::signalDeleteSocketFromListWidget(mapSockets);
    disconnectedSocket->deleteLater();  //  оставляем удаление сокета программе
}

void Server::SendToAllClients(QString typeOfMsg, QString str){ //  отправка клиенту сообщений
    Data.clear();   //  может быть мусор

    QDataStream out(&Data, QIODevice::WriteOnly);   //  объект out, режим работы только для записи, иначе ничего работать не будет
    out.setVersion(QDataStream::Qt_6_2);
    out << quint64(0) << typeOfMsg << str+delimiter;  //  отправляем в поток размер_сообщения, тип-сообщения и строку при необходимости
    out.device()->seek(0);  //  в начало потока
    out << quint64(Data.size() - sizeof(quint64));  //  высчитываем размер сообщения

    for(auto it = mapSockets.begin();it != mapSockets.end(); ++it)  //  пробегаемся по всем сокетам и
    {
        it.key()->write(Data);    //  отправляем по соответствующему сокету данные
    }
}

void Server::SendToOneClient(QTcpSocket* socket, QString typeOfMsg, QString str)
{
    Data.clear();   //  может быть мусор

    QDataStream out(&Data, QIODevice::WriteOnly);   //  объект out, режим работы только для записи, иначе ничего работать не будет
    out.setVersion(QDataStream::Qt_6_2);
    out << quint64(0) << typeOfMsg << str;  //  отправляем в поток размер_сообщения, тип-сообщения и строку при необходимости
    out.device()->seek(0);  //  в начало потока
    out << quint64(Data.size() - sizeof(quint64));  //  высчитываем размер сообщения
    socket->write(Data);    //  отправляем по сокету данные
}

void Server::SendFileToClient(QString filePath)
{
    Data.clear();   //  чистим массив байт от мусора
    file = new QFile(filePath);   //  определяем файл, чтобы поработать с его свойствами и данными
    fileSize = file->size();     //  определяем размер файла
    QFileInfo fileInfo(file->fileName());    //  без этой строки название файла будет хранить полный путь до него
    fileName = fileInfo.fileName();     //  записываем название файла

    Server::signalStatusServer("Size: "+QString::number(fileSize)+" Name: "+fileName);  //  простое уведомление пользователя о размере и имени файла, если мы смогли его найти

    if(fileSize < blockData){   //  если размер файла меньше выделенного блока
        blockData = fileSize;
    } else {    //  если мы еще раз отправляем какой-нибудь файл
        blockData = 1000000;  //  возвращаем дефолтное значение
    }
    bytes = new char[blockData];   //  выделяем байты под файл, то есть передача пройдет в несколько этапов

    for(auto iteratorSocket = mapSockets.begin(); iteratorSocket != mapSockets.end(); iteratorSocket++){
        //  проверяем только начало с префиксом у сокета, потому что в эту функцию попадают файлы по фильтру
        if(fileName.startsWith(iteratorSocket.value())){
            socket = iteratorSocket.key();  //  присуждаем переменной ссылку на сокет

            if(file->open(QIODevice::ReadOnly)){ //  открываем файл для только чтения
                socket->waitForBytesWritten();  //  мы ждем того, чтобы все байты записались

                QDataStream out(&Data, QIODevice::WriteOnly);   //  определяем поток отправки
                out.setVersion(QDataStream::Qt_6_2);
                out << quint64(0) << mapRequest["002"] << fileName << fileSize;   //  отправляем название файла и его размер
                out.device()->seek(0);
                //  избавляемся от зарезервированных двух байт в начале каждого сообщения
                out << quint64(Data.size() - sizeof(quint64));   //  определяем размер сообщения
                qDebug() << "Server::SendFileToClient:      sending file data size: " << Data.size() - sizeof(quint64);
                socket->write(Data);
            } else {
                Server::signalStatusServer("File"+fileName+" not open :(");
            }
        }
    }
}

void Server::SendPartOfFile()
{
    if((fileSize - file->pos()) < blockData){   //  если остаток файла будет меньше блока байт
        blockData = fileSize - file->pos();     //  мы просто будем читать этот остаток
    }

    socket->waitForBytesWritten();  //  мы ждем того, чтобы все байты записались
    Data.clear();

    qDebug() << "Server::SendPartOfFile:        read " << file->read(bytes, blockData);     //  читаем файл и записываем данные в байты
    qDebug() << "Server::SendPartOfFile:        block: "+QString::number(blockData);   //  нужно, чтобы видеть текущий размер блоков


    QByteArray buffer;
    buffer = buffer.fromRawData(bytes, blockData);

    qDebug() << "Server::SendPartOfFile:        block size" << blockData << "buffer size" << buffer.size();

    QDataStream out(&Data, QIODevice::WriteOnly);   //  определяем поток отправки
    out.setVersion(QDataStream::Qt_6_2);
    out << quint64(0) << mapRequest["103"] << buffer;   //  отправляем байты
    out.device()->seek(0);
    //  избавляемся от зарезервированных двух байт в начале каждого сообщения
    qDebug() << "Server::SendPartOfFile:        sending blockSize = " << quint64(Data.size() - sizeof(quint64));
    out << quint64(Data.size() - sizeof(quint64));   //  определяем размер сообщения
    socket->write(Data);
    qDebug() << "Server::SendPartOfFile:        Data size = " << Data.size();
}

void Server::slotStatusServer(QString status)
{
    emit signalStatusServer(status);
}

void Server::slotSendToAllClients(QString typeOfMsg, QString str)
{
    qDebug() << "Server::slotSendToAllClients:      typeOfMsg: " << typeOfMsg << "  str: "<< str;
    SendToAllClients(typeOfMsg, str);
}

void Server::slotSendToOneClient(QTcpSocket *sendSocket, QString typeOfMsg, QString str)
{
    SendToOneClient(sendSocket, typeOfMsg, str);
}


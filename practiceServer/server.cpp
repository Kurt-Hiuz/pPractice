#include "server.h"

Server::Server(bool &server_started, int serverPort){
    if(serverPort < 1024){
        generatedServerPort = QRandomGenerator::global()->bounded(1024, 65535);
    } else {
        generatedServerPort = serverPort;
    }

    if(this->listen(QHostAddress::Any, generatedServerPort)){  //  статус будет передаваться, когда сервер будет прослушивать любой из адресов
        server_started = true;  //  меняем состояние сервера
        qDebug() << "Server::Server:        start on "+QString::number(generatedServerPort)+" port";    //  уведомляем в консоли
    } else {
        server_started = false; //  иначе что-то пошло не так
    }

    nextBlockSize = 0;  //  обнуляем размер сообщения в самом начале работы

    readyReadManager = new ReadyReadManager();
    connect(readyReadManager, &ReadyReadManager::signalStatusRRManagerServer, this, &Server::signalStatusServer);
    connect(readyReadManager, &ReadyReadManager::signalChatNewMessage, this, &Server::signalChatNewMessage);
    connect(readyReadManager, &ReadyReadManager::signalSendToAllClientsServer, this, &Server::slotSendToAllClients);
    connect(readyReadManager, &ReadyReadManager::signalSendToOneRRManager, this, &Server::slotSendToOneClient);
    connect(readyReadManager, &ReadyReadManager::signalSendBufferToClient, this, &Server::slotSendBufferToClient);
    connect(readyReadManager, &ReadyReadManager::signalSetClientProcessing, this, &Server::slotSetClientProcessing);
    connect(readyReadManager, &ReadyReadManager::signalDeleteSendedFile, this, &Server::slotDeleteSendedFile);
    connect(readyReadManager, &ReadyReadManager::signalDeleteExpectationFile, this, &Server::slotDeleteExpectationFile);
    connect(readyReadManager, &ReadyReadManager::signalSaveData, this, &Server::slotSaveData);
    connect(readyReadManager, &ReadyReadManager::signalCheckExpectationFolder, this, &Server::slotCheckExpectationFolder);
}

void Server::setWorkspaceManager(WorkspaceManager *newWorkspaceManager)
{
    this->workspaceManager = newWorkspaceManager;
    qDebug() << "Server::setWorkspaceManager:   установлен workspaceManager";
    connect(workspaceManager, &WorkspaceManager::signalSiftFiles, this, &Server::slotSiftFiles);
}

void Server::setMaxConnections(int count)
{
    this->setMaxPendingConnections(count);
    this->maxConnections = count;
}

void Server::SendPossibleProcessing(QTcpSocket* socketToSend, QMap<QString,QVariant> possibleProcessingData)
{
    Data.clear();   //  может быть мусор
    QDataStream out(&Data, QIODevice::WriteOnly);   //  объект out, режим работы только для записи, иначе ничего работать не будет
    out.setVersion(QDataStream::Qt_6_2);
    out << quint64(0) << QString("Possible processing ComboBox data") << possibleProcessingData;  //  отправляем в поток размер_сообщения, тип-сообщения и строку при необходимости
    out.device()->seek(0);  //  в начало потока
    out << quint64(Data.size() - sizeof(quint64));  //  высчитываем размер сообщения
    socketToSend->write(Data);    //  отправляем по сокету данные
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
        emit signalStatusServer("<font color = red><\\font>Клиент "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+" убрал обработку</hr>");
    }
}

void Server::slotSiftFiles(QStringList &filesList)
{
    for(int fileI = 0; fileI < filesList.size(); fileI++){
        QFileInfo currentFile(filesList.at(fileI));

        if(mapSockets.size() == 0){
            emit signalStatusServer(workspaceManager->copyToExpectation(currentFile.filePath()));
            emit signalStatusServer(workspaceManager->deleteEntryFile(currentFile.fileName()));
            continue;
        }

        for(auto it = mapSockets.begin(); it != mapSockets.end(); it++){

            qDebug() << "Server::slotSiftFiles  it.value(): " << it.value();
            qDebug() << "Server::slotSiftFiles currentFile.fileName(): " << currentFile.fileName();
            if(currentFile.fileName().endsWith(it.value()) && it.value() != ""){
                qDebug() << "Server::slotSiftFiles:     " << currentFile.filePath();
                emit signalStatusServer(workspaceManager->copyToSended(currentFile.filePath()));
                SendFileToClient(it.key(), currentFile.filePath());
            } else {
                emit signalStatusServer(workspaceManager->copyToExpectation(currentFile.filePath()));
                emit signalStatusServer(workspaceManager->deleteEntryFile(currentFile.fileName()));
            }
        }
    }
}

void Server::slotDeleteSendedFile(QString &fileName)
{
    emit signalStatusServer(workspaceManager->deleteEntryFile(fileName));
}

void Server::slotDeleteExpectationFile(QString &fileName)
{
    emit signalStatusServer(workspaceManager->deleteExpectationFile(fileName));
}

void Server::slotSaveData(QString fileName)
{
    emit signalStatusServer(workspaceManager->saveProcessingFile(fileName));
}

void Server::slotCheckExpectationFolder(QTcpSocket *checkingSocket)
{
    QStringList filesList = workspaceManager->getExpectationFolderFiles();

    for(int fileI = 0; fileI < filesList.size(); fileI++){
        QFileInfo currentFile(filesList.at(fileI));
        QString processing = mapSockets[checkingSocket];
        qDebug() << "Server::slotCheckExpectationFolder  обработка: " << processing;
        qDebug() << "Server::slotCheckExpectationFolder currentFile.fileName(): " << currentFile.fileName();
        if(currentFile.fileName().endsWith(processing) && processing != ""){
            qDebug() << "Server::slotCheckExpectationFolder:     " << currentFile.filePath();
            emit signalStatusServer(workspaceManager->copyToSended(currentFile.filePath()));
            SendFileToClient(checkingSocket, currentFile.filePath());
        }
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
            socket = item.key();
            SendToOneClient(item.key(), QString("Disconnect"), "Отключен с хоста"); //  отправляем клиенту сигнал на отключение
            qDebug() << "Server::slotDisconnectSocket       " << "pop quantity of clients: "+QString::number(mapSockets.size());
            break;
        }
    }

    mapSockets.remove(socket);
    emit signalStatusServer("<font color = red><\\font>Пользователь  "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": принудительно отключён <br/>");
    emit signalDeleteSocketFromListWidget(mapSockets);
    socket->disconnectFromHost();
}

void Server::slotDisconnectAll(QString reason)
{
    SendToAllClients("Message", "Принудительное отключение всех клиентов: "+reason);
    for(auto item = mapSockets.begin(); item != mapSockets.end(); item++){  //  пробегаемся по сокетам
        SendToOneClient(item.key(), QString("Disconnect"), "Отключен с хоста"); //  отправляем клиенту сигнал на отключение
    }
    mapSockets.clear();
    emit signalDeleteSocketFromListWidget(mapSockets);
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

        if(it.key() == "SendedFiles"){
            sendedFilesFolder = it.value();
            continue;
        }
    }

    qDebug() << "Server::slotSetServerFolders:      all Data folders:   " << entryFolder << storageFolder << expectationFolder;
}

void Server::slotSendMessage(QString message)
{
    SendToAllClients("Message", message);
}

void Server::incomingConnection(qintptr socketDescriptor){  //  обработчик нового подключения
    socket = new QTcpSocket;    //  создание нового сокета под нового клиента
    socket->setSocketDescriptor(socketDescriptor);  //  устанавливаем в него дескриптор (- неотрицательное число, идентифицирующее  поток ввода-вывода)
    if(this->mapSockets.size() == this->maxConnections){
        SendToOneClient(socket, QString("Disconnect"), "Отключен с хоста");
        socket->disconnectFromHost();
        return;
    }

    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);  //  связка готовности чтения
    connect(socket, &QTcpSocket::disconnected, this, &Server::slotDisconnect); //  связка удаления клиента

    mapSockets[socket] = "";    //  клиент пока не показал, что он умеет делать
    emit signalStatusServer("Новый клиент " + QString::number(socketDescriptor));   //  уведомление о подключении
    emit signalAddSocketToListWidget(socket);    //  отображаем на форме в clientsListWidget этот сокет
    SendToAllClients(QString("Message"), "Новый клиент " + QString::number(socketDescriptor));
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

        QString typeOfMess;
        in >> typeOfMess;

        qDebug() << "Server::slotReadyRead:     остаток после чтения с in: " << socket->bytesAvailable();
        qDebug() << "Server::slotReadyRead:     Тип сообщения:     " << typeOfMess;
        I_MessageManager *messageManager = readyReadManager->identifyMessage(typeOfMess);

        if(messageManager->typeOfMessage() == "No type"){
            emit signalStatusServer("Была произведена попытка отправки сообщения неизвестного типа!");
            nextBlockSize = 0;
            return;
        }

        messageManager->processData(in, socket);

        nextBlockSize = 0;  //  обнуляем для новых сообщений
        break;
    }
}

void Server::slotDisconnect()
{
    QTcpSocket* disconnectedSocket = static_cast<QTcpSocket*>(QObject::sender());
    mapSockets.remove(disconnectedSocket);
    qDebug() << "Server::slotDisconnect:        pop quantity of clients: "+QString::number(mapSockets.size());
    SendToAllClients(QString("Message"), "<font color = red><\\font>Пользователь  "+QString::number(socket->socketDescriptor())+" "+disconnectedSocket->localAddress().toString()+": отключился <br/>");
    emit signalStatusServer("<font color = red><\\font>Пользователь  "+QString::number(socket->socketDescriptor())+" "+disconnectedSocket->localAddress().toString()+": отключился <br/>");
    emit signalDeleteSocketFromListWidget(mapSockets);
    disconnectedSocket->disconnectFromHost();  //  оставляем удаление сокета программе
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

void Server::SendFileToClient(QTcpSocket* socket, QString filePath)
{
    Data.clear();   //  чистим массив байт от мусора
    QFile someFile(filePath);
    fileSize = someFile.size();     //  определяем размер файла
    QFileInfo fileInfo(someFile.fileName());    //  без этой строки название файла будет хранить полный путь до него
    fileName = fileInfo.fileName();     //  записываем название файла

    emit signalStatusServer("Размер файла: "+QString::number(fileSize)+" Название: "+fileName);  //  простое уведомление пользователя о размере и имени файла, если мы смогли его найти

    if(!someFile.open(QIODevice::ReadOnly)){ //  открываем файл для только чтения
        emit signalStatusServer("Файл "+fileInfo.fileName()+" не открывается для передачи");
        return;
    }
    someFile.close();  //  Открытие-закрытие было сделано для проверки, можем ли мы вообще отправить файл

    socket->waitForBytesWritten();  //  мы ждем того, чтобы все байты записались

    QDataStream out(&Data, QIODevice::WriteOnly);   //  определяем поток отправки
    out.setVersion(QDataStream::Qt_6_2);
    out << quint64(0) << QString("File") << fileName << fileSize;   //  отправляем название файла и его размер
    out.device()->seek(0);
    //  избавляемся от зарезервированных двух байт в начале каждого сообщения
    out << quint64(Data.size() - sizeof(quint64));   //  определяем размер сообщения
    qDebug() << "Server::SendFileToClient:      Отправляемый размер данных: " << Data.size() - sizeof(quint64);
    socket->write(Data);

    readyReadManager->setFileClientFileRequest(filePath);
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
    out << quint64(0) << QString("Request part of processing file") << buffer;   //  отправляем байты
    out.device()->seek(0);
    //  избавляемся от зарезервированных двух байт в начале каждого сообщения
    qDebug() << "Server::SendPartOfFile:        sending blockSize = " << quint64(Data.size() - sizeof(quint64));
    out << quint64(Data.size() - sizeof(quint64));   //  определяем размер сообщения
    socket->write(Data);
    qDebug() << "Server::SendPartOfFile:        Data size = " << Data.size();
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


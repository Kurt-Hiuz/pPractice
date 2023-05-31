#include "ready_read_manager.h"

ReadyReadManager::ReadyReadManager()
{
    serverMessageManager = new ServerMessageManager();
    connect(serverMessageManager, &ServerMessageManager::signalMessageRRManager, this, &ReadyReadManager::slotMessageRRManager);

    possibleProcessingManager = new PossibleProcessingManager();
    connect(possibleProcessingManager, &PossibleProcessingManager::signalStatusRRManager, this, &ReadyReadManager::slotStatusRRManager);
    connect(possibleProcessingManager, &PossibleProcessingManager::signalSetCBDataRRManager, this, &ReadyReadManager::slotSetCBDataRRManager);

    clientFileRequestPartManager = new ClientFileRequestPartManager();
    connect(clientFileRequestPartManager, &ClientFileRequestPartManager::signalStatusRRManager, this, &ReadyReadManager::slotStatusRRManager);
    connect(clientFileRequestPartManager, &ClientFileRequestPartManager::signalSendBufferRRManager, this, &ReadyReadManager::slotSendBufferRRManager);

    fileDownloadedManager = new FileDownloadedManager();
    connect(fileDownloadedManager, &FileDownloadedManager::signalStatusRRManager, this, &ReadyReadManager::slotStatusRRManager);
    connect(fileDownloadedManager, &FileDownloadedManager::signalClearFileData, clientFileRequestPartManager, &ClientFileRequestPartManager::slotClearFileData);

    serverFileManager = new ServerFileManager();
    connect(serverFileManager, &ServerFileManager::signalStatusRRManager, this, &ReadyReadManager::slotStatusRRManager);
    connect(serverFileManager, &ServerFileManager::signalSendToServer, this, &ReadyReadManager::slotSendToServer);

    nullManager = new NullManager();
    messageManagers[serverMessageManager->typeOfMessage()] = serverMessageManager;
    messageManagers[possibleProcessingManager->typeOfMessage()] = possibleProcessingManager;
    messageManagers[clientFileRequestPartManager->typeOfMessage()] = clientFileRequestPartManager;
    messageManagers[fileDownloadedManager->typeOfMessage()] = fileDownloadedManager;
    messageManagers[serverFileManager->typeOfMessage()] = serverFileManager;
}

I_MessageManager *ReadyReadManager::identifyMessage(QString typeOfMess)
{
    /// Создать список всех typeOfMess и пробегаться по нему
    /// При совпадении создавать new конструктор()
    /// иначе возвращать nullManager
    for(auto it = messageManagers.begin(); it != messageManagers.end(); it++){
        if(it.key() == typeOfMess){
            return it.value();
        }
    }
    return nullManager;
}

void ReadyReadManager::setEntryFolder(QString &entryFolder)
{
    serverFileManager->setEntryFolderName(entryFolder);
}

void ReadyReadManager::setFileClientFileRequest(QString &filePath)
{
    qDebug() << "ReadyReadManager::setFileClientFileRequest:    " << filePath;
    clientFileRequestPartManager->setFilePath(filePath);
}

void ReadyReadManager::slotMessageRRManager(QString message)
{
    qDebug() << "ReadyReadManager::slotMessageRRManager:     " << message;
    emit signalMessageRRManagerClient(message);
}

void ReadyReadManager::slotStatusRRManager(QString status)
{
    qDebug() << "ReadyReadManager::slotStatusRRManager:     " << status;
    emit signalStatusRRManagerClient(status);
}

void ReadyReadManager::slotSetCBDataRRManager(QMap<QString, QVariant> &possibleProcessingData)
{
    qDebug() << "ReadyReadManager::slotSetCBDataRRManager:     possibleProcessingData.size():" << possibleProcessingData.size();
    emit signalSetCBData(possibleProcessingData);
}

void ReadyReadManager::slotSendToAllClientsRRManager(QString typeOfMsg, QString str)
{
    qDebug() << "ReadyReadManager::slotSendToAllClientsRRManager:     " << typeOfMsg << " | " << str;
    emit signalSendToAllClientsServer(typeOfMsg, str);
}

void ReadyReadManager::slotSendBufferRRManager(QByteArray &buffer)
{
    qDebug() << "ReadyReadManager::slotSendBufferRRManager:     buffer.size(): " << buffer.size();
    emit signalSendBufferToServer(buffer);
}

void ReadyReadManager::slotSendToServer(QString typeOfMsg, QString str)
{
    qDebug() << "ReadyReadManager::slotSendToServer:    " << typeOfMsg << " | " << str;
    emit signalSendToServer(typeOfMsg, str);
}

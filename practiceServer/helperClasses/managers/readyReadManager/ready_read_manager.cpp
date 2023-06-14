#include "ready_read_manager.h"

ReadyReadManager::ReadyReadManager()
{
    clientsMessageManager = new ClientsMessageManager();
    connect(clientsMessageManager, &ClientsMessageManager::signalStatusRRManager, this, &ReadyReadManager::signalStatusRRManagerServer);
    connect(clientsMessageManager, &ClientsMessageManager::signalChatNewMessage, this, &ReadyReadManager::signalChatNewMessage);
    connect(clientsMessageManager, &ClientsMessageManager::signalSendToAllClientsRRManager, this, &ReadyReadManager::signalSendToAllClientsServer);

    clientsFileManager = new ClientsFileManager();
    connect(clientsFileManager, &ClientsFileManager::signalStatusRRManager, this, &ReadyReadManager::signalStatusRRManagerServer);
    connect(clientsFileManager, &ClientsFileManager::signalSendToOneRRManager, this, &ReadyReadManager::signalSendToOneRRManager);
    connect(clientsFileManager, &ClientsFileManager::signalSaveData, this, &ReadyReadManager::signalSaveData);

    serverRequestPartFileManager = new ServerRequestPartFileManager();
    connect(serverRequestPartFileManager, &ServerRequestPartFileManager::signalStatusRRManager, this, &ReadyReadManager::signalStatusRRManagerServer);
    connect(serverRequestPartFileManager, &ServerRequestPartFileManager::signalSendBufferRRManager, this, &ReadyReadManager::signalSendBufferToClient);

    fileDownloadedManager = new FileDownloadedManager();
    connect(fileDownloadedManager, &FileDownloadedManager::signalStatusRRManager, this, &ReadyReadManager::signalStatusRRManagerServer);
    connect(fileDownloadedManager, &FileDownloadedManager::signalClearFileData, serverRequestPartFileManager, &ServerRequestPartFileManager::slotClearFileData);
    connect(fileDownloadedManager, &FileDownloadedManager::signalDeleteSendedFile, this, &ReadyReadManager::signalDeleteSendedFile);
    connect(fileDownloadedManager, &FileDownloadedManager::signalDeleteExpectationFile, this, &ReadyReadManager::signalDeleteExpectationFile);

    clientsProcessingManager = new ClientsProcessingManager();
    connect(clientsProcessingManager, &ClientsProcessingManager::signalSetClientProcessing, this, &ReadyReadManager::signalSetClientProcessing);
    connect(clientsProcessingManager, &ClientsProcessingManager::signalCheckExpectationFolder, this, &ReadyReadManager::signalCheckExpectationFolder);

    nullManager = new NullManager();
    messageManagers[clientsMessageManager->typeOfMessage()] = clientsMessageManager;
    messageManagers[clientsFileManager->typeOfMessage()] = clientsFileManager;
    messageManagers[fileDownloadedManager->typeOfMessage()] = fileDownloadedManager;
    messageManagers[clientsProcessingManager->typeOfMessage()] = clientsProcessingManager;
    messageManagers[serverRequestPartFileManager->typeOfMessage()] = serverRequestPartFileManager;
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
    clientsFileManager->setEntryFolderName(entryFolder);
}

void ReadyReadManager::setFileClientFileRequest(QString &filePath)
{
    qDebug() << "ReadyReadManager::setFileClientFileRequest:    " << filePath;
    serverRequestPartFileManager->setFilePath(filePath);
}

#include "ready_read_manager.h"

ReadyReadManager::ReadyReadManager()
{
    serverMessageManager = new ServerMessageManager();
    connect(serverMessageManager, &ServerMessageManager::signalMessageRRManager, this, &ReadyReadManager::signalMessageRRManagerClient);

    possibleProcessingManager = new PossibleProcessingManager();
    connect(possibleProcessingManager, &PossibleProcessingManager::signalStatusRRManager, this, &ReadyReadManager::signalStatusRRManagerClient);
    connect(possibleProcessingManager, &PossibleProcessingManager::signalSetCBDataRRManager, this, &ReadyReadManager::signalSetCBData);

    clientFileRequestPartManager = new ClientFileRequestPartManager();
    connect(clientFileRequestPartManager, &ClientFileRequestPartManager::signalStatusRRManager, this, &ReadyReadManager::signalStatusRRManagerClient);
    connect(clientFileRequestPartManager, &ClientFileRequestPartManager::signalSendBufferRRManager, this, &ReadyReadManager::signalSendBufferToServer);

    fileDownloadedManager = new FileDownloadedManager();
    connect(fileDownloadedManager, &FileDownloadedManager::signalStatusRRManager, this, &ReadyReadManager::signalStatusRRManagerClient);
    connect(fileDownloadedManager, &FileDownloadedManager::signalClearFileData, clientFileRequestPartManager, &ClientFileRequestPartManager::slotClearFileData);
    connect(fileDownloadedManager, &FileDownloadedManager::signalDeleteSendedFile, this, &ReadyReadManager::signalDeleteSendedFile);

    serverFileManager = new ServerFileManager();
    connect(serverFileManager, &ServerFileManager::signalStatusRRManager, this, &ReadyReadManager::signalStatusRRManagerClient);
    connect(serverFileManager, &ServerFileManager::signalSendToServer, this, &ReadyReadManager::signalSendToServer);

    disconnectManager = new DisconnectManager();
    connect(disconnectManager, &DisconnectManager::signalStatusRRManager, this, &ReadyReadManager::signalStatusRRManagerClient);
    connect(disconnectManager, &DisconnectManager::signalEnableInterface, this, &ReadyReadManager::signalEnableInterface);

    nullManager = new NullManager();
    messageManagers[serverMessageManager->typeOfMessage()] = serverMessageManager;
    messageManagers[possibleProcessingManager->typeOfMessage()] = possibleProcessingManager;
    messageManagers[clientFileRequestPartManager->typeOfMessage()] = clientFileRequestPartManager;
    messageManagers[fileDownloadedManager->typeOfMessage()] = fileDownloadedManager;
    messageManagers[serverFileManager->typeOfMessage()] = serverFileManager;
    messageManagers[disconnectManager->typeOfMessage()] = disconnectManager;
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
    clientFileRequestPartManager->setFilePath(filePath);
}


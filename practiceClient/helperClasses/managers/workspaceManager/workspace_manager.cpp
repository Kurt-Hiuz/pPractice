#include "workspace_manager.h"

WorkspaceManager::WorkspaceManager()
{

}

bool WorkspaceManager::createWorkspaceFolders()
{
    //  возвращаем значение, создались ли все папки
    return  QDir().mkdir(processedFolder) &&
            QDir().mkdir(entryFolder);
}

void WorkspaceManager::setRootFolder(QString incomingRootFolder)
{
    this->rootFolder = incomingRootFolder;

    this->processedFolder = incomingRootFolder+"/Processed";
    this->entryFolder = incomingRootFolder+"/Entry";

    QStringList folders = {processedFolder, entryFolder};

    this->workspaceWatcher = new QFileSystemWatcher();

    QStringList watchingFolders = workspaceWatcher->addPaths(folders);
    QString status;

    if(watchingFolders == folders){
        status = "Все папки отслеживаются";

        QMap<QString, QString> subFolders;

        subFolders.insert("Root", rootFolder);
        subFolders.insert("Entry", entryFolder);
        subFolders.insert("Processed", processedFolder);

        emit signalSetClientFolders(subFolders);

    } else {
        status = "Не все папки отслеживаются";
    }
    emit signalStatusClient(status);

    connect(workspaceWatcher, &QFileSystemWatcher::directoryChanged, this, &WorkspaceManager::workspaceDirectoryChanged);
}

void WorkspaceManager::workspaceDirectoryChanged(const QString &fodlerName)
{
    return;
}

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

    this->workspaceWatcher = new QFileSystemWatcher();

    QString status;

    if(workspaceWatcher->addPath(rootFolder)){
        status = "Корневая папка отслеживается";

        QMap<QString, QString> subFolders;
        subFolders.insert("Root", rootFolder);
        subFolders.insert("Entry", entryFolder);
        subFolders.insert("Processed", processedFolder);
        qDebug() << "WorkspaceManager::setRootFolder    subFolders:" << subFolders;
        emit signalSetClientFolders(subFolders);
    } else {
        status = "Корневая папка НЕ отслеживается";
    }

    emit signalStatusClient(status);

    m_entryManager = new EntryManager(entryFolder);
    connect(m_entryManager, &EntryManager::signalNewEntryFile, this, &WorkspaceManager::slotNewEntryFile);

    m_processedManager = new ProcessedManager(processedFolder);
    connect(m_processedManager, &ProcessedManager::signalProcessedFiles, this, &WorkspaceManager::slotProcessedFiles);
    connect(m_processedManager, &ProcessedManager::signalFolderStatus, this, &WorkspaceManager::signalStatusClient);

    connect(workspaceWatcher, &QFileSystemWatcher::directoryChanged, this, &WorkspaceManager::workspaceDirectoryChanged);
}

QString WorkspaceManager::setEntryWatcher()
{
    if(m_entryManager->setWatcher()){
        return QString("Папка для входящих файлов отслеживается!");
    }
    return QString("Папка для входящих файлов НЕ отслеживается!");
}
QString WorkspaceManager::setProcessedWatcher()
{
    if(m_processedManager->setWatcher()){
        return QString("Папка для обработанных файлов отслеживается!");
    }
    return QString("Папка для обработанных файлов НЕ отслеживается!");
}

void WorkspaceManager::workspaceDirectoryChanged(const QString &fodlerName)
{
    return;
}

void WorkspaceManager::slotNewEntryFile(QFileInfo &fileInfo)
{
    emit signalStatusClient("Поступил новый файл:   "+fileInfo.fileName());
}

void WorkspaceManager::slotProcessedFiles(QStringList &fileInfoList)
{
    qDebug() << "WorkspaceManager::slotEntryFiles   fileList:" << fileInfoList;
    emit signalSiftFiles(fileInfoList);
}

void WorkspaceManager::deleteFile(QString fileName)
{
    if(m_processedManager->removeFile(fileName)){
        qDebug() << "WorkspaceManager::copyToExpectation:   файл " << fileName << " удален в папке";
        return;
    }

    qDebug() << "WorkspaceManager::copyToExpectation:   файл " << fileName << " не удален в папке";
}


#include "workspace_manager.h"

WorkspaceManager::WorkspaceManager()
{

}

bool WorkspaceManager::createWorkspaceFolders()
{
    //  возвращаем значение, создались ли все папки
    return  QDir().mkdir(settingsFolder) &&
            QDir().mkdir(dataFolder) &&
            QDir().mkdir(entryFolder) &&
            QDir().mkdir(expectationFolder) &&
            QDir().mkdir(storageFolder) &&
            QDir().mkdir(sendedFilesFolder);
}

QString WorkspaceManager::saveSettings(QJsonObject m_currentJsonObject)
{
    return m_settingsManager->setSettings(m_currentJsonObject);
}

QString WorkspaceManager::createSettingFiles()
{
    if(m_settingsManager->createSettingsFiles()){
        return QString("Файлы настроек созданы в /Settings");
    }

    return QString("Файлы настроек НЕ созданы");
}

void WorkspaceManager::setRootFolder(QString incomingRootFolder)
{
    this->rootFolder = incomingRootFolder;

    this->settingsFolder = incomingRootFolder+"/Settings";
    this->dataFolder = incomingRootFolder+"/Data";
    this->entryFolder = dataFolder+"/Entry";
    this->expectationFolder = dataFolder+"/Expectation";
    this->storageFolder = dataFolder+"/Storage";
    this->sendedFilesFolder = dataFolder+"/SendedFiles";

//    QStringList folders = {settingsFolder, dataFolder, entryFolder, expectationFolder, storageFolder};

//    this->workspaceWatcher = new QFileSystemWatcher();
//    workspaceWatcher->addPaths(folders);

//    if(workspaceWatcher->addPath(incomingRootFolder)){
//        emit signalStatusServer("Корневая папка не отслеживается!");
////        qDebug() << "WorkspaceManager::setRootFolder:   folders:   " << folders;
////        qDebug() << "WorkspaceManager::setRootFolder:   workspaceWatcher->directories():   " << workspaceWatcher->directories();
//    } else {
//        connect(workspaceWatcher, &QFileSystemWatcher::directoryChanged, this, &WorkspaceManager::workspaceDirectoryChanged);
//    }

    //  создаем map для всех папок
    QMap<QString, QString> subFolders;

    subFolders.insert("Root", rootFolder);
    subFolders.insert("Entry", entryFolder);
    subFolders.insert("Expectation", expectationFolder);
    subFolders.insert("Storage", storageFolder);
    subFolders.insert("SendedFiles", sendedFilesFolder);

    //  слот обработает контейнер папок
    emit signalSetServerFolders(subFolders);

    m_settingsManager = new SettingsManager(settingsFolder);
    connect(m_settingsManager, &SettingsManager::processingFileChangedSignal, this, &WorkspaceManager::workspaceFileChanged);

    m_entryManager = new EntryManager(entryFolder);
    connect(m_entryManager, &EntryManager::signalEntryFiles, this, &WorkspaceManager::slotEntryFiles);
    connect(m_entryManager, &EntryManager::signalClearEntryFolder, this, &WorkspaceManager::slotClearEntryFolder);

    m_expectationManager = new ExpectationManager(expectationFolder);

    m_sendedFileManager = new SendedFileManager(sendedFilesFolder);
}

QString WorkspaceManager::setEntryWatcher()
{
    if(m_entryManager->setWatcher()){
        return QString("Папка для входящих файлов отслеживается!");
    }
    return QString("Папка для входящих файлов НЕ отслеживается!");
}

void WorkspaceManager::deleteFile(QString fileName)
{
    if(m_entryManager->removeFile(fileName)){
        qDebug() << "WorkspaceManager::copyToExpectation:   файл " << fileName << " удален в папке";
        return;
    }

    qDebug() << "WorkspaceManager::copyToExpectation:   файл " << fileName << " не удален в папке";
}

void WorkspaceManager::copyToExpectation(QString filePath)
{
    if(m_expectationManager->createFile(filePath)){
        qDebug() << "WorkspaceManager::copyToExpectation:   файл " << filePath << " создан в папке";
        return;
    }

    qDebug() << "WorkspaceManager::copyToExpectation:   файл не создан в папке";
}

void WorkspaceManager::copyToSended(QString filePath)
{
    if(m_sendedFileManager->createFile(filePath)){
        qDebug() << "WorkspaceManager::copyToSended:   файл " << filePath << " создан в папке";
        return;
    }

    qDebug() << "WorkspaceManager::copyToSended:   файл не создан в папке";
}

void WorkspaceManager::workspaceFileChanged(const QString &fileName)
{
    emit signalUpdateUiComboBox(fileName);
}

void WorkspaceManager::workspaceDirectoryChanged(const QString &folderName)
{
    qDebug() << "WorkspaceManager::workspaceDirectoryChanged:   " << folderName;
}

void WorkspaceManager::slotEntryFiles(QStringList &fileInfoList)
{
    qDebug() << "WorkspaceManager::slotEntryFiles   fileList:" << fileInfoList;
    emit signalSiftFiles(fileInfoList);
}

void WorkspaceManager::slotClearEntryFolder(QString message, QFileInfoList &fileInfoList)
{
    emit signalClearEntryFolder(message);

    //  пробегаемся по всем файлам /Entry для переноса в /Expectation
    for(auto file : fileInfoList){
        copyToExpectation(file.absoluteFilePath());
        deleteFile(file.fileName());
    }
}

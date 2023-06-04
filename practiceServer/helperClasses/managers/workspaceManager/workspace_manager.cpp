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
            QDir().mkdir(storageFolder);
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

    //  слот обработает контейнер папок
    emit signalSetServerFolders(subFolders);

    m_settingsManager = new SettingsManager(settingsFolder);
    connect(m_settingsManager, &SettingsManager::processingFileChangedSignal, this, &WorkspaceManager::workspaceFileChanged);

    m_entryManager = new EntryManager(entryFolder);
}

QString WorkspaceManager::setEntryWatcher()
{
    if(m_entryManager->setWatcher()){
        return QString("Папка для входящих файлов отслеживается!");
    }
    return QString("Папка для входящих файлов НЕ отслеживается!");
}

void WorkspaceManager::workspaceFileChanged(const QString &fileName)
{
    emit signalUpdateUiComboBox(fileName);
}

void WorkspaceManager::workspaceDirectoryChanged(const QString &folderName)
{
    qDebug() << "WorkspaceManager::workspaceDirectoryChanged:   " << folderName;
}

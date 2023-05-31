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

void WorkspaceManager::setRootFolder(QString incomingRootFolder)
{
    this->rootFolder = incomingRootFolder;

    this->settingsFolder = incomingRootFolder+"/Settings";
    this->dataFolder = incomingRootFolder+"/Data";
    this->entryFolder = dataFolder+"/Entry";
    this->expectationFolder = dataFolder+"/Expectation";
    this->storageFolder = dataFolder+"/Storage";

    QStringList folders = {settingsFolder, dataFolder, entryFolder, expectationFolder, storageFolder};

    this->workspaceWatcher = new QFileSystemWatcher();

    QStringList watchingFolders = workspaceWatcher->addPaths(folders);
    QString status;

    if(watchingFolders == folders){
        status = "Все папки отслеживаются";
        QMap<QString, QString> subFolders;

        subFolders.insert("Root", rootFolder);
        subFolders.insert("Entry", entryFolder);
        subFolders.insert("Expectation", expectationFolder);
        subFolders.insert("Storage", storageFolder);

        emit signalSetServerFolders(subFolders);
    } else {
        status = "Не все папки отслеживаются";
    }
    emit signalStatusServer(status);

    connect(workspaceWatcher, &QFileSystemWatcher::directoryChanged, this, &WorkspaceManager::workspaceDirectoryChanged);

    m_settingsManager = new SettingsManager(settingsFolder);

    connect(m_settingsManager, &SettingsManager::processingFileChangedSignal, this, &WorkspaceManager::workspaceFileChanged);
}

void WorkspaceManager::workspaceFileChanged(const QString &fileName)
{
    emit signalUpdateUiComboBox(fileName);
}

void WorkspaceManager::workspaceDirectoryChanged(const QString &fodlerName)
{
    return;
}

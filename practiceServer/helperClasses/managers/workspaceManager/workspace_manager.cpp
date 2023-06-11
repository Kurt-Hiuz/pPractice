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
    connect(m_settingsManager, &SettingsManager::signalProcessingFileChanged, this, &WorkspaceManager::slotWorkspaceFileChanged);
    connect(m_settingsManager, &SettingsManager::signalSettingsFileChanged, this, &WorkspaceManager::signalSettingsFileChanged);

    m_entryManager = new EntryManager(entryFolder);
    connect(m_entryManager, &EntryManager::signalEntryFiles, this, &WorkspaceManager::slotEntryFiles);
    connect(m_entryManager, &EntryManager::signalClearEntryFolder, this, &WorkspaceManager::slotClearEntryFolder);

    m_expectationManager = new ExpectationManager(expectationFolder);

    m_storageManager = new StorageManager(storageFolder);

    m_sendedFileManager = new SendedFileManager(sendedFilesFolder);
}

QString WorkspaceManager::setEntryWatcher()
{
    if(m_entryManager->setWatcher()){
        return QString("Папка для входящих файлов отслеживается!");
    }
    return QString("Папка для входящих файлов НЕ отслеживается!");
}

QString WorkspaceManager::deleteEntryFile(QString fileName)
{
    if(m_entryManager->removeFile(fileName)){
        qDebug() << "WorkspaceManager::deleteEntryFile:   файл " << fileName << " удален в папке";
        return QString("Файл "+fileName+" удален в папке");
    }

    qDebug() << "WorkspaceManager::deleteEntryFile:   файл " << fileName << " не удален в папке";
    return QString("Файл "+fileName+" НЕ удален в папке");
}

QString WorkspaceManager::deleteExpectationFile(QString fileName)
{
    if(m_expectationManager->removeFile(fileName)){
        qDebug() << "WorkspaceManager::deleteExpectationFile:   файл " << fileName << " удален в папке";
        return QString("Файл "+fileName+" удален в папке");
    }

    qDebug() << "WorkspaceManager::deleteExpectationFile:   файл " << fileName << " не удален в папке";
    return QString("Файл "+fileName+" НЕ удален в папке");
}

QString WorkspaceManager::saveProcessingFile(QString fileName)
{
    QString beforeFileName = fileName.remove("processed_");
    QString beforeSendedFile = m_sendedFileManager->getFile(beforeFileName);
    QString afterEntryFile = m_entryManager->getFile("processed_"+fileName);

    if(beforeSendedFile == ""){
        return QString("Не найден файл до "+beforeFileName);
    }

    if(afterEntryFile == ""){
        return QString("Не найден файл после "+fileName);
    }

    QString answer = m_storageManager->saveFiles(beforeSendedFile, afterEntryFile);
    if(answer != "OK"){
        return QString("Не удалось сохранить файлы. "+answer);
    }

    QString testFileName = "processed_"+fileName;
    if(!m_entryManager->removeFile(testFileName)){
        return QString("Не удалось удалить файл /Entry/processed_"+fileName);
    }

    if(!m_sendedFileManager->removeFile(beforeFileName)){
        return QString("Не удалось удалить файл /SendedFiles/"+beforeFileName);
    }

    if(!m_expectationManager->removeFile(beforeFileName)){
        return QString("Не удалось удалить файл /Expectation/"+beforeFileName);
    }

    return QString("Файлы категории "+beforeFileName+" сохранены!");
}

QStringList WorkspaceManager::getExpectationFolderFiles()
{
    return m_expectationManager->getFiles();
}

QString WorkspaceManager::copyToExpectation(QString filePath)
{
    if(m_expectationManager->createFile(filePath)){
        qDebug() << "WorkspaceManager::copyToExpectation:   файл " << filePath << " создан в папке";
        return QString("Файл "+filePath+" создан в папке ожидания");
    }

    qDebug() << "WorkspaceManager::copyToExpectation:   файл не создан в папке";
    return QString("Файл "+filePath+" НЕ создан в папке ожидания");
}

QString WorkspaceManager::copyToSended(QString filePath)
{
    if(m_sendedFileManager->createFile(filePath)){
        qDebug() << "WorkspaceManager::copyToSended:   файл " << filePath << " создан в папке";
        return QString("Файл "+filePath+" создан в папке отправленных файлов");
    }

    qDebug() << "WorkspaceManager::copyToSended:   файл не создан в папке";
    return QString("Файл "+filePath+" НЕ создан в папке отправленных файлов");
}

void WorkspaceManager::slotWorkspaceFileChanged(const QString &fileName)
{
    emit signalUpdateUiComboBox(fileName);
}

void WorkspaceManager::slotWorkspaceDirectoryChanged(const QString &folderName)
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
        deleteEntryFile(file.fileName());
    }
}

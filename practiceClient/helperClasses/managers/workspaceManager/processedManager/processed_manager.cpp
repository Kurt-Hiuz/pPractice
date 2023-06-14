#include "processed_manager.h"

ProcessedManager::ProcessedManager(QString rootFolder)
{
    this->rootFolder = rootFolder;
}

bool ProcessedManager::setWatcher()
{
    //  при повторном вызове этой функции создается новый наблюдатель
    //  эта проверка позволит выйти из функции, минуя дублирования наблюдателей
    if(processedFilesWatcher != nullptr){
        return true;
    }

    processedFilesWatcher = new QFileSystemWatcher();

    if(processedFilesWatcher->addPath(rootFolder)){
        connect(processedFilesWatcher, &QFileSystemWatcher::directoryChanged, this, &ProcessedManager::slotProcessedDirectoryChanged);
        return true;
    }

    return false;
}

bool ProcessedManager::removeFile(QString fileName)
{
    QFileInfo fileInfo(rootFolder+"/"+fileName);
    // установим текущую рабочую директорию, где будет файл, без QFileInfo может не заработать
    QDir::setCurrent(fileInfo.path());
    // Создаём объект файла и открываем его на запись
    QFile fileToDelete(rootFolder+"/"+fileName);

    return fileToDelete.remove();
}

void ProcessedManager::slotProcessedDirectoryChanged(const QString &folderName)
{
    QDir workWithDirectory(rootFolder);
    QFileInfoList listFiles = workWithDirectory.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);     //  получаем список файлов директории

    QList<QString> nowFilesList = {};

    for(auto file : listFiles){
        if(file.fileName().startsWith("processed_")){
            nowFilesList.append(file.absoluteFilePath());
        }
    }

    if(nowFilesList.count() == 0){
        //  иначе будет дублирование сообщений
        return;
    }

    if(nowFilesList.count() != 1){
        QFileInfoList entryFiles = workWithDirectory.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
        emit signalFolderStatus("В Entry несколько файлов! Они были пересланы в папку ожидания.");

        return;
    }

    emit signalProcessedFiles(nowFilesList);
}

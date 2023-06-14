#include "sended_file_manager.h"

SendedFileManager::SendedFileManager(QString rootFolder)
{
    this->rootFolder = rootFolder;
}

bool SendedFileManager::createFile(QString filePath)
{
    QFileInfo fileInfo(filePath);
    // установим текущую рабочую директорию, где будет файл, без QFileInfo может не заработать
    QDir::setCurrent(fileInfo.path());
    // Создаём объект файла и открываем его на запись
    QFile newFile(filePath);

    return newFile.copy(filePath, rootFolder+"/"+fileInfo.fileName());
}

QString SendedFileManager::getFile(QString fileName)
{
    QFileInfo fileInfo(rootFolder+"/"+fileName);

    if(fileInfo.exists()){
        return rootFolder+"/"+fileName;
    }
    return QString("");
}

bool SendedFileManager::removeFile(QString fileName)
{
    QFileInfo fileInfo(rootFolder+"/"+fileName);
    // установим текущую рабочую директорию, где будет файл, без QFileInfo может не заработать
    QDir::setCurrent(fileInfo.path());
    // Создаём объект файла
    QFile fileToDelete(rootFolder+"/"+fileName);

    return fileToDelete.remove();
}

bool SendedFileManager::setWatcher()
{
    //  при повторном вызове этой функции создается новый наблюдатель
    //  эта проверка позволит выйти из функции, минуя дублирования наблюдателей
    if(sendedFilesWatcher != nullptr){
        return true;
    }

    sendedFilesWatcher = new QFileSystemWatcher();

    if(sendedFilesWatcher->addPath(rootFolder)){
        connect(sendedFilesWatcher, &QFileSystemWatcher::directoryChanged, this, &SendedFileManager::slotSendedDirectoryChanged);
        return true;
    }

    return false;
}

void SendedFileManager::slotSendedDirectoryChanged(const QString &folderName)
{

}

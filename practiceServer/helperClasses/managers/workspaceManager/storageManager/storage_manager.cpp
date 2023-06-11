#include "storage_manager.h"

StorageManager::StorageManager(QString rootFolder)
{
    this->rootFolder = rootFolder;
}

QString StorageManager::saveFiles(QString beforeFilePath, QString afterFilePath)
{
    QFileInfo beforeFileInfo(beforeFilePath);
    // установим текущую рабочую директорию, где будет файл, без QFileInfo может не заработать
    QDir::setCurrent(beforeFileInfo.path());
    // Создаём объект файла и открываем его на запись
    QFile beforeFile(beforeFilePath);

    //  обозначаем новую папку для сохранения
    QString newDirectory = rootFolder+"/"+beforeFileInfo.baseName();
    if(QDir().mkdir(newDirectory)){
        if(!QDir().mkdir(newDirectory+"/Before")){
            return "Папка не создана /Before";
        }

        if(!beforeFile.copy(beforeFilePath, newDirectory+"/Before/"+beforeFileInfo.fileName())){
            return "Не удалось скопировать файл: "+beforeFilePath+" в "+newDirectory+"/"+beforeFileInfo.fileName();
        }

        QFileInfo afterFileInfo(afterFilePath);
        // установим текущую рабочую директорию, где будет файл, без QFileInfo может не заработать
        QDir::setCurrent(afterFileInfo.path());
        // Создаём объект файла и открываем его на запись
        QFile afterFile(afterFilePath);

        if(!QDir().mkdir(newDirectory+"/After")){
            return "Папка не создана /After";
        }

        if(!afterFile.copy(afterFilePath, newDirectory+"/After/"+afterFileInfo.fileName())){
            return "Не удалось скопировать файл: "+afterFilePath+" в "+newDirectory+"/"+afterFileInfo.fileName();
        }
        return "OK";
    }
    return "Не удалось создать папку: "+rootFolder+"/"+beforeFileInfo.fileName();
}

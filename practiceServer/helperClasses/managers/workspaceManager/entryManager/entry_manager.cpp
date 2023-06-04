#include "entry_manager.h"

EntryManager::EntryManager(QString rootFolder)
{
    this->rootFolder = rootFolder;
}

bool EntryManager::setWatcher()
{
    //  при повторном вызове этой функции создается новый наблюдатель
    //  эта проверка позволит выйти из функции, минуя дублирования наблюдателей
    if(entryFilesWatcher != nullptr){
        return true;
    }

    entryFilesWatcher = new QFileSystemWatcher();

    if(entryFilesWatcher->addPath(rootFolder)){
        connect(entryFilesWatcher, &QFileSystemWatcher::directoryChanged, this, &EntryManager::slotEntryDirectoryChanged);
        return true;
    }

    return false;
}

void EntryManager::slotEntryDirectoryChanged(const QString &folderName)
{
    qDebug() << "EntryManager::slotEntryDirectoryChanged:        ";
    processingManager = new ProcessingManager();
    QFileInfoList list = processingManager->entryFiles(folderName);     //  получаем список файлов директории

    for (int i = 0; i < list.size(); i++) {
        QFileInfo fileInfo = list.at(i);
        qDebug() << "EntryManager::slotEntryDirectoryChanged:        " << qPrintable(QString("%1 %2 %3").arg(fileInfo.size(), 10).arg(fileInfo.fileName(), 5).arg(fileInfo.lastModified().toString()));   //  выводим в формате "размер имя"
//          SendFileToClient(fileInfo.filePath());  //  отправляем файл клиенту
    }
    qDebug() << "EntryManager::slotEntryDirectoryChanged:        " << folderName;
    qDebug() << "EntryManager::slotEntryDirectoryChanged:        " << "================";     // переводим строку

    if(processingManager != nullptr){
        delete processingManager;
        processingManager = nullptr;
    }
}

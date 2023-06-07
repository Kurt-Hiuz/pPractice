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

//        //  определяем папку Entry
//        QDir entryDirectory(rootFolder);
//        //  сохраняем текущие файлы
//        currentEntryInfo = entryDirectory.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);

//        for(auto file : currentEntryInfo){
//            currentEntryFiles.append(file.absoluteFilePath());
//        }

//        qDebug() << "EntryManager::setWatcher   currentEntryFiles:  " << currentEntryFiles;
        return true;
    }

    return false;
}

bool EntryManager::removeFile(QString fileName)
{
    QFileInfo fileInfo(rootFolder+"/"+fileName);
    // установим текущую рабочую директорию, где будет файл, без QFileInfo может не заработать
    QDir::setCurrent(fileInfo.path());
    // Создаём объект файла и открываем его на запись
    QFile fileToDelete(rootFolder+"/"+fileName);

    return fileToDelete.remove();
}

void EntryManager::slotEntryDirectoryChanged(const QString &folderName)
{
//    qDebug() << "EntryManager::slotEntryDirectoryChanged:        ";
//    processingManager = new ProcessingManager();
//    QFileInfoList list = processingManager->entryFiles(folderName);     //  получаем список файлов директории

//    for (int i = 0; i < list.size(); i++) {
//        QFileInfo fileInfo = list.at(i);
//        qDebug() << "EntryManager::slotEntryDirectoryChanged:        " << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.fileName(), 5));   //  выводим в формате "размер имя"

//    }
    //  определяем папку Entry
    QDir entryDirectory(rootFolder);
    //  получаем текущие файлы
    QFileInfoList nowEntryFiles = entryDirectory.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
    qDebug() << "EntryManager::slotEntryDirectoryChanged:   nowEntryFiles.count():" << nowEntryFiles.count();

    QList<QString> nowFilesList = {};

    for(auto file : nowEntryFiles){
        nowFilesList.append(file.absoluteFilePath());
    }

//    //  приводим предыдущие текущие файлы и те, что сейчас
//    QSet<QString> currentDirSet(currentEntryFiles.begin(), currentEntryFiles.end());
//    QSet<QString> nowDirSet(nowFilesList.begin(), nowFilesList.end());

//    //  получаем новые файлы
//    QSet<QString> newFiles = nowDirSet - currentDirSet;
//    //  приводим это к списку строк файлов
//    QList<QString> newFilesList(newFiles.begin(), newFiles.end());
//    qDebug() << "EntryManager::slotEntryDirectoryChanged:   newFilesList:" << newFilesList;

//    qDebug() << "EntryManager::slotEntryDirectoryChanged:        " << folderName;
//    qDebug() << "EntryManager::slotEntryDirectoryChanged:        " << "================";     // переводим строку

    if(nowEntryFiles.count() == 0){
        //  иначе будет дублирование сообщений
        return;
    }

    if(nowEntryFiles.count() != 1){
        QFileInfoList entryFiles = entryDirectory.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
        emit signalClearEntryFolder("В Entry несколько файлов! Они были пересланы в папку ожидания.", entryFiles);

        return;
    }

    emit signalEntryFiles(nowFilesList);

    //  пересохраняем текущие файлы
//    currentEntryFiles = entryDirectory.entryList(QDir::NoDotAndDotDot | QDir::Files);

//    if(processingManager != nullptr){
//        delete processingManager;
//        processingManager = nullptr;
//    }
}

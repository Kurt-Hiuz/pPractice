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

bool EntryManager::removeFile(QString fileName)
{
    QFileInfo fileInfo(rootFolder+"/"+fileName);
    // установим текущую рабочую директорию, где будет файл, без QFileInfo может не заработать
    QDir::setCurrent(fileInfo.path());
    // Создаём объект файла
    QFile fileToDelete(rootFolder+"/"+fileName);

    return fileToDelete.remove();
}

QString EntryManager::getFile(QString fileName)
{
    QFileInfo fileInfo(rootFolder+"/"+fileName);

    if(fileInfo.exists()){
        return rootFolder+"/"+fileName;
    }
    return QString("");
}

void EntryManager::slotEntryDirectoryChanged(const QString &folderName)
{
    //  определяем папку Entry
    QDir entryDirectory(rootFolder);
    //  получаем текущие файлы
    QFileInfoList nowEntryFiles = entryDirectory.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);

    QList<QString> nowFilesList = {};

    for(auto file : nowEntryFiles){
        if(file.fileName().startsWith("processed_")){

            continue;
        }

        nowFilesList.append(file.absoluteFilePath());
    }

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
}

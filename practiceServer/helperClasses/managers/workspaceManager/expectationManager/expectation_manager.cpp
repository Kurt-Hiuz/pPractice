#include "expectation_manager.h"

ExpectationManager::ExpectationManager(QString rootFolder)
{
    this->rootFolder = rootFolder;
}

bool ExpectationManager::createFile(QString filePath)
{
    QFileInfo fileInfo(filePath);
    // установим текущую рабочую директорию, где будет файл, без QFileInfo может не заработать
    QDir::setCurrent(fileInfo.path());
    // Создаём объект файла и открываем его на запись
    QFile newFile(filePath);

    return newFile.copy(filePath, rootFolder+"/"+fileInfo.fileName());
}

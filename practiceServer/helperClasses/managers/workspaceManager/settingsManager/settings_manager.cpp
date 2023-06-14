#include "settings_manager.h"

SettingsManager::SettingsManager(QString rootFolder)
{
    this->rootFolder = rootFolder;  //  путь/Settings

    serverSettingsFileName = rootFolder+"/"+"Настройки.json";
    possibleProcessingFileName = rootFolder+"/"+"Обработки.json";
}

QString SettingsManager::setSettings(QJsonObject currentJsonObject)
{
    QFileInfo fileProcessingInfo(possibleProcessingFileName);
    // установим текущую рабочую директорию, где будет файл, без QFileInfo может не заработать
    QDir::setCurrent(fileProcessingInfo.path());
    // Создаём объект файла
    QFile possibleProcessingFile(possibleProcessingFileName);

    //  на тот случай, если файл уже был создан и имеет какие-то данные
    //  24 - это размер всей ПУСТОЙ структуры
    //  в случае отсутствия данных или нарушения данных соберется новая структура
    if(possibleProcessingFile.size() < 24){
        if(!possibleProcessingFile.open(QIODevice::WriteOnly)){
            return QString("<br>Не удалось сохранить список обработок");
        }
        //  создание пустой json структуры внутри файла

        QJsonObject m_currentJsonObject;
        //  m_currentJsonObject - это просто { }
        m_currentJsonObject.insert("Data", m_currentJsonObject);
        possibleProcessingFile.write(QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented));
        possibleProcessingFile.close();
    }

    QFileInfo fileInfo(serverSettingsFileName);
    // установим текущую рабочую директорию, где будет файл, без QFileInfo может не заработать
    QDir::setCurrent(fileInfo.path());
    // Создаём объект файла и открываем его на запись
    QFile jsonFile(serverSettingsFileName);
    if (!jsonFile.open(QIODevice::WriteOnly))
    {
        return QString("<hr/>Не удалось сохранить настройки");
    }

    // Записываем текущий объект Json в файл
    jsonFile.write(QJsonDocument(currentJsonObject).toJson(QJsonDocument::Indented));
    jsonFile.close();

    settingsFilesWatcher = new QFileSystemWatcher();

    if(settingsFilesWatcher->addPath(possibleProcessingFileName) &&
       settingsFilesWatcher->addPath(serverSettingsFileName) &&
       settingsFilesWatcher->addPath(rootFolder)){
        connect(settingsFilesWatcher, &QFileSystemWatcher::fileChanged, this, &SettingsManager::slotProcessingFileChanged);
        connect(settingsFilesWatcher, &QFileSystemWatcher::directoryChanged, this, &SettingsManager::slotSettingsDirectoryChanged);
    }

    return QString("<hr/>Настройки сохранены");
}

bool SettingsManager::createSettingsFiles()
{
    QFileInfo fileProcessingInfo(possibleProcessingFileName);
    // установим текущую рабочую директорию, где будет файл, без QFileInfo может не заработать
    QDir::setCurrent(fileProcessingInfo.path());
    // Создаём объект файла и открываем его на запись
    QFile possibleProcessingFile(possibleProcessingFileName);

    QFileInfo fileInfo(serverSettingsFileName);
    // установим текущую рабочую директорию, где будет файл, без QFileInfo может не заработать
    QDir::setCurrent(fileInfo.path());
    // Создаём объект файла и открываем его на запись
    QFile jsonFile(serverSettingsFileName);

    //  вернёт true, если файлы успешно создались
    return possibleProcessingFile.exists() && jsonFile.exists();
}

void SettingsManager::slotProcessingFileChanged(const QString &filePath)
{
    if(filePath == possibleProcessingFileName){
        emit signalProcessingFileChanged(filePath);
        return;
    }

    if(filePath == serverSettingsFileName){
        emit signalSettingsFileChanged(filePath);
        return;
    }
}

void SettingsManager::slotSettingsDirectoryChanged(const QString &folderPath)
{

}

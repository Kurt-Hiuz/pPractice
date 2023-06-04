#include "settings_manager.h"

SettingsManager::SettingsManager(QString rootFolder)
{
    this->rootFolder = rootFolder;  //  путь/Settings

    serverSettingsFileName = rootFolder+"/"+"serverSettings.json";
    possibleProcessingFileName = rootFolder+"/"+"possibleProcessing.json";
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
            return QString("<hr/>Не удалось сохранить список обработок");
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

    if(settingsFilesWatcher->addPath(possibleProcessingFileName)){
        qDebug() << "SettingsManager::setSettings   flag true";
        connect(settingsFilesWatcher, &QFileSystemWatcher::fileChanged, this, &SettingsManager::processingFileChanged);
    } else {
        qDebug() << "SettingsManager::setSettings   flag false";
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

void SettingsManager::processingFileChanged(const QString &fileName)
{
    emit processingFileChangedSignal(fileName);
}

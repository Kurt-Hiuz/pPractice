#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

///     Класс StorageManager определяет зону ответственности за папку Storage
///     Переменные:
///         rootFolder - корневая папка менеджера
///         storageFilesWatcher - наблюдатель за папкой
///     Методы:
///         setWatcher() - устанавливаем наблюдателя
///         saveFiles() - сохраняет переданные файлы
///     Слоты:
///         slotSendedDirectoryChanged - папка SendedFiles изменилась

///  ========================    классы для работы
#include <QObject>              //  для создания объекта
///  ========================
///
///  ========================    для работы с файлами
#include <QFileInfo>            //  данные о файлах
#include <QDir>                 //  определитель директории
#include <QFile>                //  определитель файла
#include <QFileSystemWatcher>   //  наблюдатель
///  ========================

class StorageManager : public QObject
{
    Q_OBJECT
public:
    StorageManager(QString rootFolder);
    QString saveFiles(QString beforeFilePath, QString afterFilePath);
    bool setWatcher();

private:
    QString rootFolder;
    QFileSystemWatcher *storageFilesWatcher = nullptr;

private slots:
    void slotStorageDirectoryChanged(const QString &folderName);
};

#endif // STORAGEMANAGER_H

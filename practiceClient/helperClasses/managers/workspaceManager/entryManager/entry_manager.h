#ifndef ENTRYMANAGER_H
#define ENTRYMANAGER_H

///     Класс EntryManager определяет зону ответственности за папку Entry
///     Переменные:
///         rootFolder - корневая папка менеджера
///         entryFilesWatcher - наблюдатель за папкой
///         processingManager - определяет фильтр файлов
///     Методы:
///         setWatcher() - устанавливаем наблюдателя
///         removeFile() - удаляем файл
///     Сигналы:
///         signalNewEntryFile() - отправляем содержимое папки
///     Слоты:
///         slotEntryDirectoryChanged - папка Entry изменилась

///  ========================    классы для работы
#include <QObject>
///  ========================
///
///  ========================    классы для работы с файлами
#include <QFileSystemWatcher>   //  наблюдатель
#include <QFileInfoList>        //  данные о файлах
#include <QDir>                 //  определитель папки
///  ========================
///
///  ========================    классы проекта
//  менеджер фильтрации файлов
#include "helperClasses/managers/processingManager/processing_manager.h"
///  ========================

class EntryManager : public QObject
{
    Q_OBJECT
public:
    EntryManager(QString rootFolder);
    bool setWatcher();
    bool removeFile(QString filePath);

signals:
    void signalNewEntryFile(QFileInfo &fileInfo);

private:
    QString rootFolder;
    QFileSystemWatcher *entryFilesWatcher = nullptr;
    ProcessingManager *processingManager;

private slots:
    void slotEntryDirectoryChanged(const QString &folderName);
};

#endif // ENTRYMANAGER_H

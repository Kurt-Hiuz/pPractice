#ifndef ENTRYMANAGER_H
#define ENTRYMANAGER_H

///     Класс EntryManager определяет зону ответственности за папку Entry
///     Переменные:
///         rootFolder - корневая папка менеджера
///         entryFilesWatcher - наблюдатель за папкой
///         currentEntryInfo - файловые данные папки
///         currentEntryFiles - текущие файлы
///     Методы:
///         setWatcher() - устанавливаем наблюдателя
///         removeFile() - удаляем файл
///         getFile() - получаем путь до файла
///     Сигналы:
///         signalEntryFiles() - отправляем содержимое папки
///         signalClearEntryFolder() - очищаем папку по причине
///     Слоты:
///         slotEntryDirectoryChanged - папка Entry изменилась

///  ========================    классы для работы
#include <QObject>              //  для создания объекта
///  ========================
///
///  ========================    для работы с файлами
#include <QFileSystemWatcher>   //  наблюдатель
#include <QFileInfoList>        //  данные о файлах
#include <QDir>                 //  определитель директории
///  ========================

class EntryManager : public QObject
{
    Q_OBJECT
public:
    EntryManager(QString rootFolder);
    bool setWatcher();
    bool removeFile(QString fileName);
    QString getFile(QString fileName);

signals:
    void signalEntryFiles(QStringList &fileInfoList);
    void signalClearEntryFolder(QString message, QFileInfoList &fileInfoList);

private:
    QString rootFolder;
    QFileSystemWatcher *entryFilesWatcher = nullptr;
    QFileInfoList currentEntryInfo;
    QStringList currentEntryFiles;

private slots:
    void slotEntryDirectoryChanged(const QString &folderName);
};

#endif // ENTRYMANAGER_H

#ifndef PROCESSEDMANAGER_H
#define PROCESSEDMANAGER_H

///     Класс ProcessedManager определяет зону ответственности за папку Processed
///     Переменные:
///         rootFolder - корневая папка менеджера
///         processedFilesWatcher - наблюдатель за папкой
///     Методы:
///         setWatcher() - устанавливаем наблюдателя
///         removeFile() - удаляем файл
///     Сигналы:
///         signalProcessedFiles() - отправляем содержимое папки
///         signalFolderStatus() - статус папки
///     Слоты:
///         slotProcessedDirectoryChanged - папка Processed изменилась

///  ========================    классы для работы
#include <QObject>
///  ========================
///
///  ========================    для работы с файлами
#include <QFileSystemWatcher>   //  наблюдатель
#include <QFileInfoList>        //  данные о файлах
#include <QDir>                 //  определитель папок
///  ========================

class ProcessedManager : public QObject
{
    Q_OBJECT
public:
    ProcessedManager(QString rootFolder);
    bool setWatcher();
    bool removeFile(QString fileName);

signals:
    void signalProcessedFiles(QStringList &fileInfoList);
    void signalFolderStatus(QString status);

private:
    QString rootFolder;
    QFileSystemWatcher *processedFilesWatcher = nullptr;

private slots:
    void slotProcessedDirectoryChanged(const QString &folderName);
};

#endif // PROCESSEDMANAGER_H

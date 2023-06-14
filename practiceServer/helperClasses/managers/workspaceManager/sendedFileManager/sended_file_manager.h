#ifndef SENDEDFILEMANAGER_H
#define SENDEDFILEMANAGER_H

///     Класс SendedFileManager определяет зону ответственности за папку SendedFiles
///     Переменные:
///         rootFolder - корневая папка менеджера
///         sendedFilesWatcher - наблюдатель за папкой
///     Методы:
///         setWatcher() - устанавливаем наблюдателя
///         removeFile() - удаляем файл
///         createFile() - создаем файл
///         getFile() - получаем путь до файла
///     Слоты:
///         slotSendedDirectoryChanged - папка SendedFiles изменилась

///  ========================    классы для работы
#include <QObject>              //  для создания объекта
///  ========================
///
///  ========================    для работы с файлами
#include <QFile>                //  определитель файла
#include <QDir>                 //  определитель директории
#include <QFileInfo>            //  данные о файлах
#include <QFileSystemWatcher>   //  наблюдатель
///  ========================

class SendedFileManager : public QObject
{
    Q_OBJECT
public:
    SendedFileManager(QString rootFolder);

    bool createFile(QString filePath);
    QString getFile(QString fileName);
    bool removeFile(QString fileName);
    bool setWatcher();

private:
    QString rootFolder;
    QFileSystemWatcher *sendedFilesWatcher = nullptr;

private slots:
    void slotSendedDirectoryChanged(const QString &folderName);
};

#endif // SENDEDFILEMANAGER_H

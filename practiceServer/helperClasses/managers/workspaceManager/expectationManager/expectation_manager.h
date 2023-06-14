#ifndef EXPECTATIONMANAGER_H
#define EXPECTATIONMANAGER_H

///     Класс ExpectationManager определяет зону ответственности за папку Expectation
///     Переменные:
///         rootFolder - корневая папка менеджера
///         expectationFilesWatcher - наблюдатель за папкой
///     Методы:
///         setWatcher() - устанавливаем наблюдателя
///         removeFile() - удаляем файл
///         createFile() - создаем файл
///         getFile() - получаем путь до файла
///     Слоты:
///         slotExpectationDirectoryChanged - папка Expectation изменилась

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

class ExpectationManager : public QObject
{
    Q_OBJECT
public:
    ExpectationManager(QString rootFolder);

    bool createFile(QString filePath);
    QStringList getFiles();
    bool removeFile(QString fileName);
    bool setWatcher();

private:
    QString rootFolder;
    QFileSystemWatcher *expectationFilesWatcher = nullptr;

private slots:
    void slotExpectationDirectoryChanged(const QString &folderName);
};

#endif // EXPECTATIONMANAGER_H

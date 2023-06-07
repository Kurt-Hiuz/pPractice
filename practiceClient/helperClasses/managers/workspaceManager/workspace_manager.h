#ifndef WORKSPACEMANAGER_H
#define WORKSPACEMANAGER_H

///     Класс WorkspaceManager определяет зону ответственности за рабочую папку
///     Конструктор принимает абсолютный путь папки, над которой требуется менеджмент
///     Переменные:
///         rootFolder - абсолютный путь рабочей директории
///         processedFolder - путь до папки Processed
///         entryFolder - путь до папки Entry
///         parentUi - ссылка на родителя
///         workspaceWatcher - наблюдатель за состоянием рабочего пространства
///     Методы:
///         createWorkspaceFolders() - создаёт структуру папки и возвращает отчёт в консоль
///         setRootFolder() - устанавливает новые пути для папок
///     Слоты:
///         workspaceDirectoryChanged() - обработка сигнала от QFileSystemWatcher::directoryChanged
///     Сигналы:
///         signalStatusClient() - сигнал для отображения состояния клиента
///         signalSetClientFolders() - сигнал для установки папок на клиенте


///  ========================    классы проекта
#include "entryManager/entry_manager.h"         //  менеджер для папки /Entry
#include "processedManager/processed_manager.h" //  менеджер для папки /Processed
///  ========================
///
///  ========================    классы для работы с директориями
#include <QDir>                 //  для работы с директориями
#include <QFile>                //  для работы с файлами
#include <QFileSystemWatcher>   //  для наблюдения за состоянием папки
///  ========================
///
///  ========================    классы для работы с json
#include <QJsonDocument>        //  для создания json документов
#include <QJsonObject>          //  для использования переменных типа QJsonObject
///  ========================
///
///  ========================    классы для работы класса
#include <QObject>              //  класс для работы connect()
///  ========================


class WorkspaceManager : public QObject
{
    Q_OBJECT
public:
    WorkspaceManager();

    bool createWorkspaceFolders();
    void setRootFolder(QString incomingRootFolder);
    QString setEntryWatcher();
    QString setProcessedWatcher();
    void deleteFile(QString fileName);

signals:
    void signalStatusClient(QString status);
    void signalSetClientFolders(QMap<QString, QString> &subFolders);
    void signalSendProcessedFile(QString processedFilePath);

private:
    QString rootFolder;
    QString entryFolder;
    QString processedFolder;

    QFileSystemWatcher *workspaceWatcher;
    EntryManager *m_entryManager;
    ProcessedManager *m_processedManager;

private slots:
    void workspaceDirectoryChanged(const QString &fodlerName);
    void slotNewEntryFile(QFileInfo &fileInfo);
    void slotSendProcessedFile(QString processedFilePath);
};

#endif // WORKSPACEMANAGER_H

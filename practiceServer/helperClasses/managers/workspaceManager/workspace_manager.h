#ifndef WORKSPACEMANAGER_H
#define WORKSPACEMANAGER_H

///     Класс WorkspaceManager определяет зону ответственности за рабочую папку
///     Конструктор принимает абсолютный путь папки, над которой требуется менеджмент
///     Переменные:
///         rootFolder - абсолютный путь рабочей директории
///         settingsFolder - путь до папки Settings
///         dataFolder - путь до папки Data
///         entryFolder - путь до папки Entry
///         expectationFolder - путь до папки Excectation
///         storageFolder - путь до папки Storage
///         parentUi - ссылка на родителя
///         m_settingsManager - экземпляр менеджера папки Settings
///         workspaceWatcher - наблюдатель за состоянием рабочего пространства
///     Методы:
///         createWorkspaceFolders() - создаёт структуру папки и возвращает отчёт в консоль
///         saveSettings() - принимает json объект и создаёт из него файл.json, возвращая отчёт в консоль
///         setRootFolder() - устанавливает новые пути для папок
///     Слоты:
///         workspaceDirectoryChanged() - обработка сигнала от QFileSystemWatcher::directoryChanged
///         workspaceFileChanged() - обработка сигнала от QFileSystemWatcher::fileChanged
///     Сигналы:
///         signalUpdateUiComboBox() - сигнал для смены данных в ComboBox обработок
///         signalStatusServer() - сигнал для отображения статуса сервера
///         signalSetServerFolders() - сигнал для установки папок сервера

///  ========================    классы проекта
#include "settingsManager/settings_manager.h"   //  менеджер для папки /Settings
#include "entryManager/entry_manager.h"         //  менеджер для папки /Entry
#include "expectationManager/expectation_manager.h" //  менеджер для папки /ExpectationManager
#include "sendedFileManager/sended_file_manager.h"  //  менеджер для папки /SendedFileManager
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
    QString saveSettings(QJsonObject m_currentJsonObject);
    QString createSettingFiles();
    void setRootFolder(QString incomingRootFolder);
    QString setEntryWatcher();
    void copyToExpectation(QString filePath);
    void copyToSended(QString filePath);
    void deleteFile(QString fileName);

signals:
    void signalUpdateUiComboBox(const QString &fileName);
    void signalStatusServer(QString status);
    void signalSetServerFolders(QMap<QString, QString> &subFolders);
    void signalSiftFiles(QStringList &fileInfoList);
    void signalClearEntryFolder(QString message);

private:
    QString rootFolder;
    QString settingsFolder;
    QString dataFolder;
    QString entryFolder;
    QString expectationFolder;
    QString storageFolder;
    QString sendedFilesFolder;

    QFileSystemWatcher *workspaceWatcher;

    SettingsManager *m_settingsManager;
    EntryManager *m_entryManager;
    ExpectationManager *m_expectationManager;
    SendedFileManager *m_sendedFileManager;

private slots:
    void workspaceFileChanged(const QString &fileName);
    void workspaceDirectoryChanged(const QString &folderName);
    void slotEntryFiles(QStringList &fileInfoList);
    void slotClearEntryFolder(QString message, QFileInfoList &fileInfoList);
};

#endif // WORKSPACEMANAGER_H

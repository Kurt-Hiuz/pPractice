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
///         sendedFilesFolder - путь до папки SendedFiles
///         m_settingsManager - экземпляр менеджера папки Settings
///         m_entryManager - экземпляр менеджера папки Entry
///         m_expectationManager - экземпляр менеджера папки Excectation
///         m_sendedFileManager - экземпляр менеджера папки SendedFiles
///         m_storageManager - экземпляр менеджера папки Storage
///         workspaceWatcher - наблюдатель за состоянием рабочего пространства
///     Методы:
///         createWorkspaceFolders() - создаёт структуру папки и возвращает отчёт в консоль
///         saveSettings() - принимает json объект и создаёт из него файл.json, возвращая отчёт в консоль
///         setRootFolder() - устанавливает новые пути для папок
///         createSettingFiles() - создание файлов настроек
///         setFolderWatcher() - установка наблюдателя
///         copyToExpectation() - копирование файла в Expectation
///         copyToSended() - копирование файла в SendedFiles
///         deleteEntryFile() - удаление файла в Entry
///         deleteExpectationFile() - удаление файла в Expectation
///         saveProcessingFile() - сохранение обработанных файлов
///         getExpectationFolderFiles() - получение файлов из Expectation
///     Слоты:
///         slotWorkspaceDirectoryChanged() - обработка сигнала изменения рабочей папки
///         slotWorkspaceFileChanged() - обработка сигнала изменения файлов в рабочей папке
///         slotEntryFiles() - обработка входящих файлов
///         slotClearEntryFolder() - обработка сигнала очистить Entry
///     Сигналы:
///         signalUpdateUiComboBox() - сигнал для смены данных в ComboBox обработок
///         signalStatusServer() - сигнал для отображения статуса сервера
///         signalSetServerFolders() - сигнал для установки папок сервера
///         signalSettingsFileChanged() - сигнал изменения файла настроек
///         signalSetServerFolders() - установка папок в переменные сервера
///         signalSiftFiles() - сигнал отправить файлы
///         signalClearEntryFolder() - сигнал почистить папку Entry

///  ========================    классы для работы класса
#include <QObject>              //  класс для работы connect()
///  ========================
///
///  ========================    классы проекта
#include "settingsManager/settings_manager.h"       //  менеджер для папки /Settings
#include "entryManager/entry_manager.h"             //  менеджер для папки /Entry
#include "expectationManager/expectation_manager.h" //  менеджер для папки /ExpectationManager
#include "sendedFileManager/sended_file_manager.h"  //  менеджер для папки /SendedFileManager
#include "storageManager/storage_manager.h"         //  менеджер для папки /Data
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

class WorkspaceManager : public QObject
{
    Q_OBJECT
public:
    WorkspaceManager();

    bool createWorkspaceFolders();
    QString saveSettings(QJsonObject m_currentJsonObject);
    QString createSettingFiles();
    void setRootFolder(QString incomingRootFolder);
    QString setFolderWatcher();
    QString copyToExpectation(QString filePath);
    QString copyToSended(QString filePath);
    QString deleteEntryFile(QString fileName);
    QString deleteExpectationFile(QString fileName);
    QString saveProcessingFile(QString fileName);
    QStringList getExpectationFolderFiles();

signals:
    void signalUpdateUiComboBox(const QString &fileName);
    void signalSettingsFileChanged(const QString &fileName);
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
    StorageManager *m_storageManager;

private slots:
    void slotWorkspaceFileChanged(const QString &fileName);
    void slotWorkspaceDirectoryChanged(const QString &folderName);
    void slotEntryFiles(QStringList &fileInfoList);
    void slotClearEntryFolder(QString message, QFileInfoList &fileInfoList);
};

#endif // WORKSPACEMANAGER_H

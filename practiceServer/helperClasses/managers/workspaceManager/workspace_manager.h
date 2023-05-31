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
#include "helperClasses/managers/workspaceManager/settingsManager/settings_manager.h"   //  менеджер для папки /Settings
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
    void setRootFolder(QString incomingRootFolder);

signals:
    void signalUpdateUiComboBox(const QString &fileName);
    void signalStatusServer(QString status);
    void signalSetServerFolders(QMap<QString, QString> &subFolders);

private:
    QString rootFolder;
    QString settingsFolder;
    QString dataFolder;
    QString entryFolder;
    QString expectationFolder;
    QString storageFolder;

    QFileSystemWatcher *workspaceWatcher;

    SettingsManager *m_settingsManager;

private slots:
    void workspaceFileChanged(const QString &fileName);
    void workspaceDirectoryChanged(const QString &fodlerName);
};

#endif // WORKSPACEMANAGER_H

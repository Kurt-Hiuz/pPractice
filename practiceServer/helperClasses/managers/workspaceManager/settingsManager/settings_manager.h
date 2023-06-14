#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

///     Класс SettingsManager определяет зону ответственности за папку настроек
///     Конструктор принимает абсолютный путь папки настроек, над которой требуется менеджмент
///     Переменные:
///         rootFolder - абсолютный путь папки настроек
///         serverSettingsFileName - абсолютный путь файла настроек
///         possibleProcessingFileName - абсолютный путь файла обработок
///         settingsfilesWatcher - наблюдатель за файлами
///     Методы:
///         saveSettings() - принимает json объект и создаёт из него файл.json, возвращая отчёт в консоль
///         createSettingsFiles() - создает файлы настроек
///     Сигналы:
///         signalProcessingFileChanged - передает путь до измененного файла обработок
///         signalSettingsFileChanged - передает путь до измененного файла настроек
///     Слоты:
///         slotProcessingFileChanged - обрабатывает слот наблюдателя за possibleProcessing.json
///         slotSettingsDirectoryChanged - обрабатывает слот наблюдателя за папкой

///  ========================    классы для работы
#include <QObject>              //  работа с MainWindow
///  ========================
///
///  ========================    классы для работы с json
#include <QJsonObject>          //  работа с json объектами
#include <QJsonDocument>        //  работа с json документами
///  ========================
///
///  ========================    классы для работы с файлами
#include <QFileInfo>            //  информация о файле
#include <QDir>                 //  работа с директориями
#include <QFile>                //  работа с файлами
#include <QFileSystemWatcher>   //  наблюдатель за файлами
///  ========================

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    SettingsManager(QString rootFolder);

    QString setSettings(QJsonObject currentJsonObject);
    bool createSettingsFiles();

private:
    QString rootFolder;
    QString serverSettingsFileName;
    QString possibleProcessingFileName;

    QFileSystemWatcher *settingsFilesWatcher;

signals:
    void signalProcessingFileChanged(const QString &filePath);
    void signalSettingsFileChanged(const QString &filePath);

private slots:
    void slotProcessingFileChanged(const QString &filePath);
    void slotSettingsDirectoryChanged(const QString &folderPath);
};

#endif // SETTINGSMANAGER_H

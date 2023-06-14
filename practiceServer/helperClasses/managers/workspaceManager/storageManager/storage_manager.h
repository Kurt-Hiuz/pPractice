#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include <QObject>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>

class StorageManager : public QObject
{
    Q_OBJECT
public:
    StorageManager(QString rootFolder);
    QString saveFiles(QString beforeFilePath, QString afterFilePath);
    bool setWatcher();

private:
    QString rootFolder;
    QFileSystemWatcher *storageFilesWatcher = nullptr;

private slots:
    void slotStorageDirectoryChanged(const QString &folderName);
};

#endif // STORAGEMANAGER_H

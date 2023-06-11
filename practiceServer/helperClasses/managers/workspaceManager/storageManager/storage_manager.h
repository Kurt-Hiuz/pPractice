#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include <QObject>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QFile>

class StorageManager : public QObject
{
    Q_OBJECT
public:
    StorageManager(QString rootFolder);
    QString saveFiles(QString beforeFilePath, QString afterFilePath);

private:
    QString rootFolder;
};

#endif // STORAGEMANAGER_H

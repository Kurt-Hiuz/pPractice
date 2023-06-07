#ifndef PROCESSEDMANAGER_H
#define PROCESSEDMANAGER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QFileInfoList>
#include <QDir>
#include <QDebug>

class ProcessedManager : public QObject
{
    Q_OBJECT
public:
    ProcessedManager(QString rootFolder);
    bool setWatcher();
    bool removeFile(QString fileName);

signals:
    void signalSendProcessedFile(QString processedFilePath);

private:
    QString rootFolder;
    QFileSystemWatcher *processedFilesWatcher = nullptr;

private slots:
    void slotProcessedDirectoryChanged(const QString &folderName);
};

#endif // PROCESSEDMANAGER_H

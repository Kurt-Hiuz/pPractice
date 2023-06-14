#ifndef SENDEDFILEMANAGER_H
#define SENDEDFILEMANAGER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemWatcher>

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

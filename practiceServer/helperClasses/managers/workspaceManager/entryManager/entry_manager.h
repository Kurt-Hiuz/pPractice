#ifndef ENTRYMANAGER_H
#define ENTRYMANAGER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QFileInfoList>
#include <QDebug>
#include "helperClasses/managers/processingManager/processing_manager.h"

class EntryManager : public QObject
{
    Q_OBJECT
public:
    EntryManager(QString rootFolder);
    bool setWatcher();
    bool removeFile(QString fileName);

signals:
    void signalEntryFiles(QStringList &fileInfoList);
    void signalClearEntryFolder(QString message, QFileInfoList &fileInfoList);

private:
    QString rootFolder;
    QFileSystemWatcher *entryFilesWatcher = nullptr;
    ProcessingManager *processingManager;
    QFileInfoList currentEntryInfo;
    QStringList currentEntryFiles;

private slots:
    void slotEntryDirectoryChanged(const QString &folderName);
};

#endif // ENTRYMANAGER_H

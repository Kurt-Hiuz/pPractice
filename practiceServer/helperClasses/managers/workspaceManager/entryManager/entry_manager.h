#ifndef ENTRYMANAGER_H
#define ENTRYMANAGER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QDebug>
#include "helperClasses/managers/processingManager/processing_manager.h"

class EntryManager : public QObject
{
    Q_OBJECT
public:
    EntryManager(QString rootFolder);
    bool setWatcher();

private:
    QString rootFolder;
    QFileSystemWatcher *entryFilesWatcher = nullptr;
    ProcessingManager *processingManager;

private slots:
    void slotEntryDirectoryChanged(const QString &folderName);
};

#endif // ENTRYMANAGER_H

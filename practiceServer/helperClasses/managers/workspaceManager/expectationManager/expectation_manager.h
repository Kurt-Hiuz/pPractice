#ifndef EXPECTATIONMANAGER_H
#define EXPECTATIONMANAGER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemWatcher>

class ExpectationManager : public QObject
{
    Q_OBJECT
public:
    ExpectationManager(QString rootFolder);

    bool createFile(QString filePath);
    QStringList getFiles();
    bool removeFile(QString fileName);
    bool setWatcher();

private:
    QString rootFolder;
    QFileSystemWatcher *expectationFilesWatcher = nullptr;

private slots:
    void slotExpectationDirectoryChanged(const QString &folderName);
};

#endif // EXPECTATIONMANAGER_H

#ifndef SENDEDFILEMANAGER_H
#define SENDEDFILEMANAGER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QFileInfo>

class SendedFileManager : public QObject
{
    Q_OBJECT
public:
    SendedFileManager(QString rootFolder);

    bool createFile(QString filePath);
    QString getFile(QString fileName);
    bool removeFile(QString fileName);

private:
    QString rootFolder;
};

#endif // SENDEDFILEMANAGER_H

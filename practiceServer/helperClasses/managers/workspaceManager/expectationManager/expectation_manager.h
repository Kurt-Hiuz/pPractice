#ifndef EXPECTATIONMANAGER_H
#define EXPECTATIONMANAGER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QFileInfo>

class ExpectationManager : public QObject
{
    Q_OBJECT
public:
    ExpectationManager(QString rootFolder);

    bool createFile(QString filePath);
    QStringList getFiles();
    bool removeFile(QString fileName);

private:
    QString rootFolder;
};

#endif // EXPECTATIONMANAGER_H

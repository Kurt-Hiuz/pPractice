#ifndef CLIENTFILEREQUESTPARTMANAGER_H
#define CLIENTFILEREQUESTPARTMANAGER_H

#include "../I_message_manager.h"
#include <QFile>

class ClientFileRequestPartManager : public I_MessageManager
{
    Q_OBJECT
public:
    ClientFileRequestPartManager();

    void readDataFromStream(QDataStream &inStream) override;
    void writeDataToStream(QDataStream &outStream) override;
    void processData(QDataStream &inStream) override;
    QString typeOfMessage() override;

    void setFilePath(QString &filePath);

private:
    int fileSize;   //  размер файла
    QString fileName;   //  его название
    QFile *file;     //  сам файлик
    char *bytes = nullptr;     //  массив байт данных
    int blockData = 1000000;  //  размер данных
    QByteArray buffer;

signals:
    void signalStatusRRManager(QString status);
    void signalSendBufferRRManager(QByteArray &buffer);

public slots:
    void slotClearFileData();
};

#endif // CLIENTFILEREQUESTPARTMANAGER_H

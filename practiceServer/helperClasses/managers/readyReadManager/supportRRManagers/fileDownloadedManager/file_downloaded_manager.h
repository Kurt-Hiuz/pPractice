#ifndef FILEDOWNLOADEDMANAGER_H
#define FILEDOWNLOADEDMANAGER_H

#include "../I_message_manager.h"

class FileDownloadedManager : public I_MessageManager
{
    Q_OBJECT
private:
    QString string;
public:
    FileDownloadedManager();
    void readDataFromStream(QDataStream &inStream) override;
    void writeDataToStream(QDataStream &outStream) override;
    void processData(QDataStream &inStream, QTcpSocket *socket) override;
    QString typeOfMessage() override;

signals:
    void signalClearFileData();
    void signalStatusRRManager(QString status);
    void signalDeleteSendedFile(QString &fileName);
    void signalDeleteExpectationFile(QString &fileName);
};

#endif // FILEDOWNLOADEDMANAGER_H

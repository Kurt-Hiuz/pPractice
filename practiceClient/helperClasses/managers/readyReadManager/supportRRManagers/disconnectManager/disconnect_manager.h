#ifndef DISCONNECTMANAGER_H
#define DISCONNECTMANAGER_H

#include "../I_message_manager.h"

class DisconnectManager : public I_MessageManager
{
    Q_OBJECT
public:
    DisconnectManager();

    void readDataFromStream(QDataStream &inStream) override;
    void writeDataToStream(QDataStream &outStream) override;
    void processData(QDataStream &inStream) override;
    QString typeOfMessage() override;

private:
    QString string;

signals:
    void signalStatusRRManager(QString status);
    void signalEnableInterface(QString message);

//  переопределение операторов >> и <<
protected:
    friend QDataStream &operator >> (QDataStream &in, DisconnectManager &disconnectManager);
    friend QDataStream &operator << (QDataStream &out, DisconnectManager &disconnectManager);

};

#endif // DISCONNECTMANAGER_H

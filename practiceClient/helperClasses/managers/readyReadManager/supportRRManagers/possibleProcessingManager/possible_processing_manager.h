#ifndef POSSIBLEPROCESSINGMANAGER_H
#define POSSIBLEPROCESSINGMANAGER_H

#include "../I_message_manager.h"
#include <QDataStream>

class PossibleProcessingManager : public I_MessageManager
{
    Q_OBJECT
public:
    PossibleProcessingManager();
    void readDataFromStream(QDataStream &inStream) override;
    void writeDataToStream(QDataStream &outStream) override;
    void processData(QDataStream &inStream) override;
    QString typeOfMessage() override;

//  переопределение операторов >> и <<
protected:
    friend QDataStream &operator >> (QDataStream &in, PossibleProcessingManager &possibleProcessingManager);
    friend QDataStream &operator << (QDataStream &out, PossibleProcessingManager &possibleProcessingManager);

signals:
    void signalStatusRRManager(QString status);
    void signalSetCBDataRRManager(QMap<QString,QVariant> &possibleProcessingData);

private:
    QMap<QString,QVariant> possibleProcessingData;
};

#endif // POSSIBLEPROCESSINGMANAGER_H

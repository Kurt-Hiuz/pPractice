#include "disconnect_manager.h"

DisconnectManager::DisconnectManager()
{

}

void DisconnectManager::readDataFromStream(QDataStream &inStream)
{
    inStream >> this->string;
}

void DisconnectManager::writeDataToStream(QDataStream &outStream)
{
    outStream << this->string;
}

void DisconnectManager::processData(QDataStream &inStream)
{
    readDataFromStream(inStream);

    emit signalStatusRRManager(string);
    emit signalEnableInterface(string);
}

QString DisconnectManager::typeOfMessage()
{
    return QString("Disconnect");
}

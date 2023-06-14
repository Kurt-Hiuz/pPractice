#include "server_message_manager.h"

ServerMessageManager::ServerMessageManager()
{

}

void ServerMessageManager::readDataFromStream(QDataStream &inStream)
{
    inStream >> this->message;
}

void ServerMessageManager::writeDataToStream(QDataStream &outStream)
{
    outStream << this->message;
}

void ServerMessageManager::processData(QDataStream &inStream)
{
    readDataFromStream(inStream);
    emit signalMessageRRManager(message);     //  оформляем чат на стороне клиента
}

QString ServerMessageManager::typeOfMessage()
{
    return QString("Message");
}

#include "clients_processing_manager.h"

ClientsProcessingManager::ClientsProcessingManager()
{

}

void ClientsProcessingManager::readDataFromStream(QDataStream &inStream)
{
    inStream >> currentProcessing;
}

void ClientsProcessingManager::writeDataToStream(QDataStream &outStream)
{
    outStream << currentProcessing;
}

void ClientsProcessingManager::processData(QDataStream &inStream, QTcpSocket *socket)
{
    readDataFromStream(inStream);

    emit signalSetClientProcessing(socket, currentProcessing);
    emit signalCheckExpectationFolder(socket);
}

QString ClientsProcessingManager::typeOfMessage()
{
    return QString("Set processing on client");
}

#include "possible_processing_manager.h"

PossibleProcessingManager::PossibleProcessingManager()
{

}

void PossibleProcessingManager::readDataFromStream(QDataStream &inStream)
{
    inStream >> this->possibleProcessingData;
}

void PossibleProcessingManager::writeDataToStream(QDataStream &outStream)
{
    outStream << this->possibleProcessingData;
}

void PossibleProcessingManager::processData(QDataStream &inStream)
{
    readDataFromStream(inStream);

    emit signalStatusRRManager("Отправка "+QString::number(possibleProcessingData.size())+" обработок");     //  оформляем чат на стороне клиента
    emit signalSetCBDataRRManager(possibleProcessingData);
    qDebug() << "ServerMessageManager::processData:        possibleProcessingData.size():" << possibleProcessingData.size();
}

QString PossibleProcessingManager::typeOfMessage()
{
    return QString("Possible processing ComboBox data");
}

#include "null_manager.h"

NullManager::NullManager()
{

}

void NullManager::readDataFromStream(QDataStream &inStream)
{
    return;
}

void NullManager::writeDataToStream(QDataStream &outStream)
{
    return;
}

void NullManager::processData(QDataStream &inStream)
{
    return;
}

QString NullManager::typeOfMessage()
{
    return QString("No type");
}

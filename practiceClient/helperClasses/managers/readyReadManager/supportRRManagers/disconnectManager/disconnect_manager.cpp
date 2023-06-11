#include "disconnect_manager.h"

DisconnectManager::DisconnectManager()
{

}

void DisconnectManager::readDataFromStream(QDataStream &inStream)
{
    inStream >> this->str;
}

void DisconnectManager::writeDataToStream(QDataStream &outStream)
{
    outStream << this->str;
}

void DisconnectManager::processData(QDataStream &inStream)
{
    QString str;    //  определяем переменную, в которую сохраним уведомление от запроса
    inStream >> str;  //  выводим в переменную сообщение

    qDebug() << "DisconnectManager::processData:     str:" << str;  //  выводим в консоль
    emit signalStatusRRManager(str);
    emit signalEnableInterface(str);
}

QString DisconnectManager::typeOfMessage()
{
    return QString("Disconnect");
}

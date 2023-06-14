#include "clients_message_manager.h"

ClientsMessageManager::ClientsMessageManager()
{

}

void ClientsMessageManager::readDataFromStream(QDataStream &inStream)
{
    inStream >> this->message;
    inStream >> this->senderName;
}

void ClientsMessageManager::writeDataToStream(QDataStream &outStream)
{
    outStream << this->message;
    outStream << this->senderName;
}

void ClientsMessageManager::processData(QDataStream &inStream, QTcpSocket *socket)
{
    readDataFromStream(inStream);
    emit signalChatNewMessage("Клиент "+senderName+" "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+message);
    emit signalStatusRRManager("Клиент "+senderName+" "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+message);     //  оформляем чат на стороне Сервера
    emit signalSendToAllClientsRRManager("Message","<font color = black><\\font>User "+senderName+" "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": "+message);      //  мы просто избавляемся от префикса "MESS:" и пересылаем клиенту сообщение
    qDebug() << "ClientsMessageManager::processData:        " << message << "from" << senderName;
}

QString ClientsMessageManager::typeOfMessage()
{
    return QString("Message");
}

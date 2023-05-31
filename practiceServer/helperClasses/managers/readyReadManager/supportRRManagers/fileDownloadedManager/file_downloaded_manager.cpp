#include "file_downloaded_manager.h"

FileDownloadedManager::FileDownloadedManager()
{

}

void FileDownloadedManager::readDataFromStream(QDataStream &inStream)
{

}

void FileDownloadedManager::writeDataToStream(QDataStream &outStream)
{

}

void FileDownloadedManager::processData(QDataStream &inStream, QTcpSocket *socket)
{
    QString str;    //  определяем переменную, в которую сохраним данные
    inStream >> str;  //  выводим в переменную сообщение

    emit signalClearFileData();
    emit signalStatusRRManager(str);  //  и то же самое клиенту
}

QString FileDownloadedManager::typeOfMessage()
{
    return QString("File downloaded");
}

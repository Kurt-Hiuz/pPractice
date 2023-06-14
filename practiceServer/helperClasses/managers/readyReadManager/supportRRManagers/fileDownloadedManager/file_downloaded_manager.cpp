#include "file_downloaded_manager.h"

FileDownloadedManager::FileDownloadedManager()
{

}

void FileDownloadedManager::readDataFromStream(QDataStream &inStream)
{
    inStream >> string;
}

void FileDownloadedManager::writeDataToStream(QDataStream &outStream)
{
    outStream << string;
}

void FileDownloadedManager::processData(QDataStream &inStream, QTcpSocket *socket)
{
    readDataFromStream(inStream);
    emit signalClearFileData();
    emit signalDeleteSendedFile(string);
    emit signalDeleteExpectationFile(string);
    emit signalStatusRRManager("<font color = green><\\font>Файл от "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+" \""+string+"\" загружен");  //  и то же самое клиенту
}

QString FileDownloadedManager::typeOfMessage()
{
    return QString("File downloaded");
}

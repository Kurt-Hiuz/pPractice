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

void FileDownloadedManager::processData(QDataStream &inStream)
{
    QString str;    //  определяем переменную, в которую сохраним данные
    inStream >> str;  //  выводим в переменную сообщение

    emit signalClearFileData();
    emit signalDeleteSendedFile(str);
    emit signalStatusRRManager("<font color = green><\\font>file \""+str+"\" downloaded");  //  и то же самое клиенту
}

QString FileDownloadedManager::typeOfMessage()
{
    return QString("File downloaded");
}

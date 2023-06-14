#include "server_request_part_file_manager.h"

ServerRequestPartFileManager::ServerRequestPartFileManager()
{

}

void ServerRequestPartFileManager::readDataFromStream(QDataStream &inStream)
{
    inStream >> this->string;
}

void ServerRequestPartFileManager::writeDataToStream(QDataStream &outStream)
{
    outStream << this->string;
}

void ServerRequestPartFileManager::processData(QDataStream &inStream, QTcpSocket *socket)
{
    readDataFromStream(inStream);

    qDebug() << "ServerRequestPartFileManager::processData:     str = " << string;  //  выводим в консоль
    emit signalStatusRRManager(string); //  выводим клиенту

    qDebug() << "ServerRequestPartFileManager::processData:     file->pos():" << file->pos();
    if((fileSize - file->pos()) < blockData){   //  если остаток файла будет меньше блока байт
        blockData = fileSize - file->pos();     //  мы просто будем читать этот остаток
    }

    bytes = new char[blockData];   //  выделяем байты под файл, то есть передача пройдет в несколько этапов

    qDebug() << "ServerRequestPartFileManager::processData:     read: " << file->read(bytes, blockData);     //  читаем файл и записываем данные в байты
    qDebug() << "ServerRequestPartFileManager::processData:     blockData: " << blockData;   //  нужно, чтобы видеть текущий размер блоков

    buffer.clear();
    buffer.append(bytes, blockData);
    qDebug() << "ServerRequestPartFileManager::processData:     block size" << blockData << "buffer size" << buffer.size();

    if(bytes != nullptr){
        delete[] bytes;
        bytes = nullptr;
    }

    emit signalSendBufferRRManager(socket, buffer);
}

QString ServerRequestPartFileManager::typeOfMessage()
{
    return QString("Request part of processing file");
}

void ServerRequestPartFileManager::setFilePath(QString &filePath)
{
    file = new QFile(filePath);   //  определяем файл, чтобы поработать с его свойствами и данными
    fileSize = file->size();
    if(!file->open(QIODevice::ReadOnly)){
        emit signalStatusRRManager("Невозможно открыть файл "+filePath+" для дальнейшей отправки");
        delete file; //  удаляем файл
        file = nullptr; //  удаляем файл
        return;
    }
}

void ServerRequestPartFileManager::slotClearFileData()
{
    qDebug() << "File "+fileName+" downloaded";   //  выводим консоль, какой файл был загружен

    file->close();  //  закрываем файл
    delete file; //  удаляем файл
    file = nullptr; //  удаляем файл
    fileName.clear();   //  очищаем его название
    fileSize = 0;   //  очищаем его размер
    blockData = 1000000;  //  устанавливаем прежний размер байтов
    if(bytes != nullptr){
        delete[] bytes;
        bytes = nullptr;
    }
}

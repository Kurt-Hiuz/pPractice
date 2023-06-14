#include "client_file_request_part_manager.h"

ClientFileRequestPartManager::ClientFileRequestPartManager()
{

}

void ClientFileRequestPartManager::readDataFromStream(QDataStream &inStream)
{
    inStream >> this->string;
}

void ClientFileRequestPartManager::writeDataToStream(QDataStream &outStream)
{
    outStream << this->string;
}

void ClientFileRequestPartManager::processData(QDataStream &inStream)
{
    readDataFromStream(inStream);

    qDebug() << "ClientFileRequestPartManager::processData:     str:" << string;  //  выводим в консоль
    emit signalStatusRRManager(string);

    qDebug() << "ClientFileRequestPartManager::processData:     file->pos():" << file->pos();
    if((fileSize - file->pos()) < blockData){   //  если остаток файла будет меньше блока байт
        blockData = fileSize - file->pos();     //  мы просто будем читать этот остаток
    }

    bytes = new char[blockData];   //  выделяем байты под файл, то есть передача пройдет в несколько этапов

    qDebug() << "ClientFileRequestPartManager::processData:     read: " << file->read(bytes, blockData);     //  читаем файл и записываем данные в байты
    qDebug() << "ClientFileRequestPartManager::processData:     blockData: " << blockData;   //  нужно, чтобы видеть текущий размер блоков

    buffer.clear();
    buffer.append(bytes, blockData);
    qDebug() << "ClientFileRequestPartManager::processData:     block size" << blockData << "buffer size" << buffer.size();

    if(bytes != nullptr){
        delete[] bytes;
        bytes = nullptr;
    }

    emit signalSendBufferRRManager(buffer);
}

QString ClientFileRequestPartManager::typeOfMessage()
{
    return QString("Request part of file");
}

void ClientFileRequestPartManager::setFilePath(QString &filePath)
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

void ClientFileRequestPartManager::slotClearFileData()
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

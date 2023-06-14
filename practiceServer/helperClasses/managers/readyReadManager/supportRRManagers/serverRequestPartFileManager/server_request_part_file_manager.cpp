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

    emit signalStatusRRManager(string); //  выводим клиенту

    if((fileSize - file->pos()) < blockData){   //  если остаток файла будет меньше блока байт
        blockData = fileSize - file->pos();     //  мы просто будем читать этот остаток
    }

    bytes = new char[blockData];   //  выделяем байты под файл, то есть передача пройдет в несколько этапов

    buffer.clear();
    buffer.append(bytes, blockData);

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

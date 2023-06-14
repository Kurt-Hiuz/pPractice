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

    emit signalStatusRRManager(string);

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

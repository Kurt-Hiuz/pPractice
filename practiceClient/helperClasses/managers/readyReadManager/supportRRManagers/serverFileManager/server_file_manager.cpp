#include "server_file_manager.h"

ServerFileManager::ServerFileManager()
{

}

void ServerFileManager::readDataFromStream(QDataStream &inStream)
{
    inStream >> this->fileName;
    inStream >> this->fileSize;
}

void ServerFileManager::writeDataToStream(QDataStream &outStream)
{
    outStream << this->fileName;
    outStream << this->fileSize;
}

void ServerFileManager::processData(QDataStream &inStream)
{
    if(fileName.isEmpty()){    //  если файла не существует
        inStream >> fileName;  //  записываем из потока название файла
        inStream >> fileSize; //  считываем его размер

        if(fileSize < blockData){   //  если размер файла меньше выделенного блока
            blockData = fileSize;   //  устанавливаем размер блока ровно по файлу (передача произойдет в один этап)
        } else {
            blockData = 1000000;  //  устанавливаем по умолчанию (на случай последующей передачи, если размер файла будет куда больше)
        }

        file = new QFile;     //  определяем файл
        QDir::setCurrent(entryFolderName);  //  устанавливаем путь сохранения
        file->setFileName(fileName);    //  устанавливаем имя файла

        //  этой функции entryFolder определяется в Server::slotNewWorkspaceFolder
        //  поскольку этот слот всегда происходит раньше вызова Server::slotReadyRead

        emit signalStatusRRManager("Файл "+fileName+" создан на клиенте");  //  уведомляем

        emit signalSendToServer("Request part of file", "Downloading new part of file...");    //  запрашиваем первую часть файла
    } else {

        if((fileSize - file->size()) < blockData){  //  если разница между плановым и текущим размером файла меньше блока байтов
            blockData = fileSize - file->size();    //  мы устанавливаем такой размер для блока (разницу)
        }

        bytes = new char[blockData];   //  выделяем байты под файл, то есть передача пройдет в несколько этапов

        inStream >> bytes;    //  считываем байты

        if(file->open(QIODevice::Append)){  //  записываем в конец файла
            file->write(bytes, blockData);    //  записываем в файл
        } else {
            emit signalStatusRRManager("Не удается открыть файл "+fileName);
            return;
        }

        if(file->size() < fileSize){    //  если размер до сих пор не полон
            emit signalStatusRRManager("Текущий размер файла "+fileName+" от сервера = "+QString::number(file->size())+"\n"+"Ожидаемый размер = "+QString::number(fileSize));

            emit signalSendToServer("Request part of file", "<font color = black><\\font>Downloading new part of file...<font color = black><\\font>");    //  запрашиваем первую часть файла
        } else {
            //  оформляем чат на стороне Сервера
            emit signalStatusRRManager("Server: send file by name \""+fileName+"\"");

            emit signalSendToServer("File downloaded", "<font color = green><\\font>file \""+fileName+"\" downloaded");

            file->close();  //  закрываем файл
            file = nullptr; //  удаляем файл
            fileName.clear();   //  очищаем его название
            fileSize = 0;   //  очищаем его размер
            blockData = 1000000;  //  устанавливаем прежний размер байтов
            delete[] bytes; //  удаляем байты из кучи

            return; //  выходим
        }

        file->close();   //закрываем файл
        if(bytes != nullptr){   //  удаляем байты из кучи, делая проверку на случай двойного удаления
            delete[] bytes;
            bytes = nullptr;
        }
    }
}

QString ServerFileManager::typeOfMessage()
{
    return QString("File");
}

void ServerFileManager::setEntryFolderName(QString &entryFolder)
{
    this->entryFolderName = entryFolder;
    qDebug() << "ServerFileManager::setEntryFolderName:        установлена папка: " << entryFolder;
}


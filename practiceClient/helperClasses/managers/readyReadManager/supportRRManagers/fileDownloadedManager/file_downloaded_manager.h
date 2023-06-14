#ifndef FILEDOWNLOADEDMANAGER_H
#define FILEDOWNLOADEDMANAGER_H

///     Класс FileDownloadedManager реализует интерфейс I_MessageManager
///     обрабатывает всё необходимое, что связанно с загруженными файлами
///     Переменные:
///         string - строка из потока
///     Методы:
///         readDataFromStream() - чтение данных с потока
///         writeDataFromStream() - запись данных в поток
///         processData() - обрабатывает приходящие данные
///         typeOfMessage() - возвращает строку тип менеджера
///     Сигналы:
///         signalStatusRRManager() - отправка статуса ReadyReadManager'у
///         signalClearFileData() - очистка данных о файле
///         signalDeleteSendedFile() - удаление отправленных файлов

///  ========================       классы проекта
#include "../I_message_manager.h"   //  класс-родитель
///  ========================

class FileDownloadedManager : public I_MessageManager
{
    Q_OBJECT
private:
    QString string;

public:
    FileDownloadedManager();
    void readDataFromStream(QDataStream &inStream) override;
    void writeDataToStream(QDataStream &outStream) override;
    void processData(QDataStream &inStream) override;
    QString typeOfMessage() override;

signals:
    void signalClearFileData();
    void signalStatusRRManager(QString status);
    void signalDeleteSendedFile(QString &fileName);
};

#endif // FILEDOWNLOADEDMANAGER_H

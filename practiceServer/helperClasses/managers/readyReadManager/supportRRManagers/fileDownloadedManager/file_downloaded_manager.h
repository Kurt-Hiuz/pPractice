#ifndef FILEDOWNLOADEDMANAGER_H
#define FILEDOWNLOADEDMANAGER_H

///     Класс FileDownloadedManager реализует интерфейс I_MessageManager
///     обрабатывает всё необходимое, что связанное с окончанием загрузки
///     Методы:
///         readDataFromStream() - чтение данных с потока
///         writeDataFromStream() - запись данных в поток
///         processData() - обрабатывает приходящие данные
///         typeOfMessage() - возвращает строку тип менеджера
///     Сигналы:
///         signalStatusRRManager() - отправка статуса ReadyReadManager'у
///         signalClearFileData() - сигнал другому менеджеру очистить данные
///         signalDeleteSendedFile() - сигнал удалить файл из /Sended
///         signalDeleteExpectationFile() - сигнал удалить файл из /Expectation

///  ========================    классы проекта
#include "../I_message_manager.h"   //  реализуем интерфейс
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
    void processData(QDataStream &inStream, QTcpSocket *socket) override;
    QString typeOfMessage() override;

signals:
    void signalClearFileData();
    void signalStatusRRManager(QString status);
    void signalDeleteSendedFile(QString &fileName);
    void signalDeleteExpectationFile(QString &fileName);
};

#endif // FILEDOWNLOADEDMANAGER_H

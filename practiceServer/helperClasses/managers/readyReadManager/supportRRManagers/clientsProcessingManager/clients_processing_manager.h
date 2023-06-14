#ifndef CLIENTSPROCESSINGMANAGER_H
#define CLIENTSPROCESSINGMANAGER_H

///     Класс ClientsProcessingManager реализует интерфейс I_MessageManager
///     обрабатывает всё необходимое, что связанное с обработками
///     Методы:
///         readDataFromStream() - чтение данных с потока
///         writeDataFromStream() - запись данных в поток
///         processData() - обрабатывает приходящие данные
///         typeOfMessage() - возвращает строку тип менеджера
///     Сигналы:
///         signalStatusRRManager() - отправка статуса ReadyReadManager'у
///         signalSendToAllClientsRRManager() - отправка сообщения всем клиентам
///         signalSetClientProcessing() - закрепление обработки за клиентом
///         signalCheckExpectationFolder() - проверка папки ожидания

///  ========================    классы проекта
#include "../I_message_manager.h"   //  реализуем интерфейс
///  ========================

class ClientsProcessingManager : public I_MessageManager
{
    Q_OBJECT
private:
    QString currentProcessing;

public:
    ClientsProcessingManager();
    void readDataFromStream(QDataStream &inStream) override;
    void writeDataToStream(QDataStream &outStream) override;
    void processData(QDataStream &inStream, QTcpSocket *socket) override;
    QString typeOfMessage() override;
signals:
    void signalStatusRRManager(QString status);
    void signalSendToAllClientsRRManager(QString typeOfMsg, QString str);
    void signalSetClientProcessing(QTcpSocket *socket, QString currentProcessing);
    void signalCheckExpectationFolder(QTcpSocket *socket);
};

#endif // CLIENTSPROCESSINGMANAGER_H

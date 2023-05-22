#ifndef CLIENTSMESSAGEMANAGER_H
#define CLIENTSMESSAGEMANAGER_H

///     Класс ClientsMessageManager реализует интерфейс I_MessageManager
///     обрабатывает всё необходимое, что связанное с обычными сообщениями клиентов
///     Переменные:
///         message - сообщение от клиента
///         senderName - имя отправителя
///     Методы:
///         readDataFromStream() - чтение данных с потока
///         writeDataFromStream() - запись данных в поток
///         processData() - обрабатывает приходящие данные
///         typeOfMessage() - возвращает строку тип менеджера
///     Сигналы:
///         signalStatusRRManager() - отправка статуса ReadyReadManager'у
///         signalSendToOneRRManager() - отправка сообщения одному клиенту через ReadyReadManager

///  ========================    классы проекта
#include "../I_message_manager.h"   //  реализуем интерфейс
///  ========================

class ClientsMessageManager : public I_MessageManager
{
    Q_OBJECT
public:
    ClientsMessageManager();
    void readDataFromStream(QDataStream &inStream) override;
    void writeDataToStream(QDataStream &outStream) override;
    void processData(QDataStream &inStream, QTcpSocket *socket) override;
    QString typeOfMessage() override;

//  переопределение операторов >> и <<
protected:
    friend QDataStream &operator >> (QDataStream &in, ClientsMessageManager &clientsMessageManager);
    friend QDataStream &operator << (QDataStream &out, ClientsMessageManager &clientsMessageManager);

private:
    QString message;
    QString senderName;

signals:
    void signalStatusRRManager(QString status);
    void signalSendToAllClientsRRManager(QString typeOfMsg, QString str);
};

#endif // CLIENTSMESSAGEMANAGER_H

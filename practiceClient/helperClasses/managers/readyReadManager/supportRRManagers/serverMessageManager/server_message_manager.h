#ifndef SERVERMESSAGEMANAGER_H
#define SERVERMESSAGEMANAGER_H

///     Класс ServerMessageManager реализует интерфейс I_MessageManager
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
/// =========================

class ServerMessageManager : public I_MessageManager
{
    Q_OBJECT
public:
    ServerMessageManager();
    void readDataFromStream(QDataStream &inStream) override;
    void writeDataToStream(QDataStream &outStream) override;
    void processData(QDataStream &inStream) override;
    QString typeOfMessage() override;

//  переопределение операторов >> и <<
protected:
    friend QDataStream &operator >> (QDataStream &in, ServerMessageManager &serverMessageManager);
    friend QDataStream &operator << (QDataStream &out, ServerMessageManager &serverMessageManager);

private:
    QString message;

signals:
    void signalMessageRRManager(QString status);
};

#endif // SERVERMESSAGEMANAGER_H

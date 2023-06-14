#ifndef DISCONNECTMANAGER_H
#define DISCONNECTMANAGER_H

///     Класс DisconnectManager реализует интерфейс I_MessageManager
///     обрабатывает всё необходимое, что связанно с отключением
///     Переменные:
///         string - строка из потока
///     Методы:
///         readDataFromStream() - чтение данных с потока
///         writeDataFromStream() - запись данных в поток
///         processData() - обрабатывает приходящие данные
///         typeOfMessage() - возвращает строку тип менеджера
///     Сигналы:
///         signalStatusRRManager() - отправка статуса ReadyReadManager'у
///         signalEnableInterface() - вкл/выкл интерфейс

///  ========================       классы проекта
#include "../I_message_manager.h"   //  класс-родитель
///  ========================

class DisconnectManager : public I_MessageManager
{
    Q_OBJECT
public:
    DisconnectManager();

    void readDataFromStream(QDataStream &inStream) override;
    void writeDataToStream(QDataStream &outStream) override;
    void processData(QDataStream &inStream) override;
    QString typeOfMessage() override;

private:
    QString string;

signals:
    void signalStatusRRManager(QString status);
    void signalEnableInterface(QString message);

//  переопределение операторов >> и <<
protected:
    friend QDataStream &operator >> (QDataStream &in, DisconnectManager &disconnectManager);
    friend QDataStream &operator << (QDataStream &out, DisconnectManager &disconnectManager);

};

#endif // DISCONNECTMANAGER_H

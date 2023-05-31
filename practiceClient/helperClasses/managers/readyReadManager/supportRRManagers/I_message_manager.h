#ifndef I_MESSAGEMANAGER_H
#define I_MESSAGEMANAGER_H

///     Класс I_MessageManager является интерфейсом для обработчиков сообщений слота сервера ReadyRead
///     Методы:
///     readDataFromStream() - чтение данных с потока
///     writeDataFromStream() - запись данных в поток
///     processData() - обрабатывает приходящие данные
///     typeOfMessage() - возвращает строку тип менеджера

///  ========================    классы для работы
#include <QObject>              //  для создания объекта
#include <QTcpSocket>           //  для работы с сокетом
#include <QDebug>               //  для дебага
///  ========================

class I_MessageManager : public QObject
{
    Q_OBJECT
public:
    I_MessageManager();
    virtual void readDataFromStream(QDataStream &inStream) = 0;
    virtual void writeDataToStream(QDataStream &outStream) = 0;
    virtual void processData(QDataStream &inStream) = 0;
    virtual QString typeOfMessage() = 0;
};

#endif // I_MESSAGEMANAGER_H

#ifndef NULLMANAGER_H
#define NULLMANAGER_H

///     Класс NullManager реализует интерфейс I_MessageManager
///     обозначает отсутствие обработчика сообщений (заглушку)
///     Переменные:
///     Методы:
///     readDataFromStream() - чтение данных с потока
///     writeDataFromStream() - запись данных в поток
///     processData() - обрабатывает приходящие данные
///     typeOfMessage() - возвращает строку тип менеджера

///  ========================       классы проекта
#include "../I_message_manager.h"   //  реализуем интерфейс
///  ========================

class NullManager : public I_MessageManager
{
    Q_OBJECT
public:
    NullManager();
    void readDataFromStream(QDataStream &inStream) override;
    void writeDataToStream(QDataStream &outStream) override;
    void processData(QDataStream &inStream) override;
    QString typeOfMessage() override;
};

#endif // NULLMANAGER_H

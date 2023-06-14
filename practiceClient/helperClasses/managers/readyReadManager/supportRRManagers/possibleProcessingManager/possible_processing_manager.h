#ifndef POSSIBLEPROCESSINGMANAGER_H
#define POSSIBLEPROCESSINGMANAGER_H

///     Класс PossibleProcessingManager реализует интерфейс I_MessageManager
///     обрабатывает всё необходимое, что связанно с возможными обработками
///     Переменные:
///         possibleProcessingData - данные об обработках
///     Методы:
///         readDataFromStream() - чтение данных с потока
///         writeDataFromStream() - запись данных в поток
///         processData() - обрабатывает приходящие данные
///         typeOfMessage() - возвращает строку тип менеджера
///     Сигналы:
///         signalStatusRRManager() - отправка статуса ReadyReadManager'у
///         signalSetCBDataRRManager() - установка обработок

///  ========================       классы проекта
#include "../I_message_manager.h"   //  класс-родитель
///  ========================
///
///  ========================       для работы с потоком
#include <QDataStream>              //  поток данных
///  ========================

class PossibleProcessingManager : public I_MessageManager
{
    Q_OBJECT
public:
    PossibleProcessingManager();
    void readDataFromStream(QDataStream &inStream) override;
    void writeDataToStream(QDataStream &outStream) override;
    void processData(QDataStream &inStream) override;
    QString typeOfMessage() override;

//  переопределение операторов >> и <<
protected:
    friend QDataStream &operator >> (QDataStream &in, PossibleProcessingManager &possibleProcessingManager);
    friend QDataStream &operator << (QDataStream &out, PossibleProcessingManager &possibleProcessingManager);

signals:
    void signalStatusRRManager(QString status);
    void signalSetCBDataRRManager(QMap<QString,QVariant> &possibleProcessingData);

private:
    QMap<QString,QVariant> possibleProcessingData;
};

#endif // POSSIBLEPROCESSINGMANAGER_H

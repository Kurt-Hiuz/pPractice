#ifndef JSONPACKER_H
#define JSONPACKER_H

///     Класс JsonPacker определяет зону ответственности за упаковку данных в json вариант
///     Переменные:
///         m_settingsCardJsonValue - возвращаемое валидное json представление
///     Методы:
///         getJsonVersionValue() - превращает принимаемую карточку в json вариант

///  ========================    классы проекта
#include "../../components/frames/cardFrame/I_cardframe.h"  //  общий интерфейс карточек
///  ========================
///
///  ========================    классы для работы с json
#include <QJsonValue>           //  для создания json значений
#include <QJsonObject>          //  для создания json объектов
///  ========================
///
///  ========================   классы для работы
#include <QVariant>             //  для корректной работы метода .getValue()
///  ========================

class JsonPacker
{
public:
    JsonPacker();

    QJsonValue getJsonVersionValue(I_CardFrame *item);

private:
    QJsonValue m_settingsCardJsonValue;
};

#endif // JSONPACKER_H

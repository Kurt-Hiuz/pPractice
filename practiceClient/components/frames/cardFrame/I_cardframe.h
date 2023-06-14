#ifndef CARDFRAME_H
#define CARDFRAME_H

///     Класс I_CardFrame является интерфейсом для карточек виджетов
///     Каждая карточка является графическим элементом
///     Методы:
///     createInterface() - создание графического представления
///     getValue() - возвращает QMap из одного элемента {строка для сервера}:{данные}
///     setValue() - установка необходимых сообщений в графической части
///     enableInteface() - включить интефейс

///  ========================    классы для работы
#include <QFrame>               //  класс, от которого наследуемся
#include <QObject>              //  для создания объекта
///  ========================

class I_CardFrame : public QFrame
{
    Q_OBJECT
public:
    I_CardFrame();

    virtual void createInterface() = 0;
    virtual QMap<QString, QVariant> getValue() = 0;
    virtual void setValue(QVariant value) = 0;
    virtual void setEnabledInteface(bool value) = 0;
};

#endif // CARDFRAME_H

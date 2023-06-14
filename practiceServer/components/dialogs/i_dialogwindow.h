#ifndef I_DIALOGWINDOW_H
#define I_DIALOGWINDOW_H

///     Класс I_DialogWindow является интерфейсом для пользовательских диалоговых окон
///     Методы:
///         createInterface() - создание графического представления
///         getValue() - возвращает QVariant данных

///  ========================   классы для работы
#include <QDialog>              //  класс, от которого наследуемся
#include <QObject>              //  для создания объекта
///  ========================

class I_DialogWindow : public QDialog
{
    Q_OBJECT
public:
    I_DialogWindow();

    virtual void createInterface() = 0;
};

#endif // I_DIALOGWINDOW_H

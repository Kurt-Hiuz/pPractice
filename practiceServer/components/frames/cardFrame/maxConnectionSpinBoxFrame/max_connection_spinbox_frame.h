#ifndef MAXCONNECTIONSPINBOXFRAME_H
#define MAXCONNECTIONSPINBOXFRAME_H

///     Класс MaxConnectionSpinBoxFrame реализует интерфейс I_CardFrame
///     Переменные:
///         consoleMessage - сообщение в консоль сервера
///         maxConnectionSpinBox - поле ввода натуральных чисел
///         dataLabel - метка с подсказкой/данными для наглядности
///         parentUi - ссылка на родительский элемент ui
///     Методы:
///         createInterface() - создание графического представления
///         getValue() - возвращает QMap из одного элемента {строка для сервера}:{данные}
///         setValue() - установка необходимых сообщений в графической части
///         enableInteface() - включить интефейс

///  ========================    заголовочные файлы проекта
#include "../I_cardframe.h"     //  реализуемый интерфейс
#include "mainwindow.h"         //  родительский ui
///  ========================
///
///  ========================    классы для работы с виджетами
#include <QSpinBox>             //  виджет для ввода натуральных чисел
#include <QLabel>               //  виджет для подсказки
#include <QHBoxLayout>          //  горизонтальное выравнивание виджетов
///  ========================

class MaxConnectionSpinBoxFrame : public I_CardFrame
{
    Q_OBJECT
public:
    MaxConnectionSpinBoxFrame(MainWindow *parentUi);

    void createInterface() override;
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;
    void enableInteface(bool value) override;

private:
    QString consoleMessage;
    QSpinBox *maxConnectionSpinBox;
    QLabel *dataLabel;
    MainWindow *parentUi;
};

#endif // MAXCONNECTIONSPINBOXFRAME_H

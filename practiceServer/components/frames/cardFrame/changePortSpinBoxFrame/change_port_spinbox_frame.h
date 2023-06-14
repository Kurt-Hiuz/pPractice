#ifndef SPINBOXFRAME_H
#define SPINBOXFRAME_H

///     Класс ChangePortSpinBoxFrame реализует интерфейс I_CardFrame
///     используется для фрейма смены порта
///     Переменные:
///         consoleMessage - сообщение в консоль сервера
///         changePortSpinBox - поле ввода порта сервера
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
#include <QSpinBox>             //  поле ввода натуральных чисел
#include <QLabel>               //  метка для подсказки
///  ========================

class ChangePortSpinBoxFrame : public I_CardFrame
{
    Q_OBJECT
public:
    ChangePortSpinBoxFrame(MainWindow *parentUi);

    void createInterface() override;
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;
    void enableInteface(bool value) override;

private:
    QString consoleMessage;
    QSpinBox *changePortSpinBox;
    QLabel *dataLabel;
    MainWindow *parentUi; 
};

#endif // SPINBOXFRAME_H

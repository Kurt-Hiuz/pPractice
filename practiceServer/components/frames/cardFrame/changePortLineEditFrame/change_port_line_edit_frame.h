#ifndef LINEEDITFRAME_H
#define LINEEDITFRAME_H

///     Класс ChangeIPLineEditFrame реализует интерфейс I_CardFrame
///     Переменные:
///         consoleMessage - сообщение в консоль сервера
///         changeIPLineEdit - поле ввода IP сервера
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

class ChangePortLineEditFrame : public I_CardFrame
{
    Q_OBJECT
public:
    ChangePortLineEditFrame(MainWindow *parentUi);

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

#endif // LINEEDITFRAME_H

#ifndef POSSIBLEPROCESSINGFRAME_H
#define POSSIBLEPROCESSINGFRAME_H

///     Класс FileFrame реализует интерфейс I_CardFrame
///     используется для выбора файлов
///     Переменные:
///         chooseProcessingComboBox - метка пути файла
///         chooseProcessingPushButton - поле ввода пути файла
///         consoleMessage - кнопка открытия проводника
///         parentUi - ссылка на родительский элемент ui
///     Методы:
///         createInterface() - создание графического представления
///         getValue() - возвращает QMap из одного элемента {строка для сервера}:{данные}
///         setValue() - установка необходимых сообщений в графической части
///         setEnabledInteface() - включить интефейс
///         getCurrentData() - получение текущего значения

///  ========================    заголовочные файлы проекта
#include "../../I_cardframe.h"  //  класс-родитель
#include "mainwindow.h"         //  родительский ui
///  ========================
///
///  ========================    классы для работы с виджетами
#include <QComboBox>            //  класс комбобокс
#include <QPushButton>          //  класс кнопки
#include <QHBoxLayout>          //  горизонтальное выравнивание
///  ========================

class PossibleProcessingFrame : public I_CardFrame
{
    Q_OBJECT
public:
    PossibleProcessingFrame(MainWindow *ui);

    void createInterface() override;
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;
    void setEnabledInteface(bool value) override;
    QString getCurrentData();

private:
    QComboBox *chooseProcessingComboBox;
    QPushButton *chooseProcessingPushButton;
    MainWindow *parentUi;
    QString consoleMessage;
};

#endif // POSSIBLEPROCESSINGFRAME_H

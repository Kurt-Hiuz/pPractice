#ifndef CONNECTFRAME_H
#define CONNECTFRAME_H

///     Класс ConnectFrame реализует интерфейс I_CardFrame
///     используется для подключения клиента
///     Переменные:
///         IPLineEdit - поле ввода IP
///         portLineEdit - поле ввода порта
///         connectToServerPuchButton - кнопка подключения
///         parentUi - ссылка на родительский элемент ui
///     Методы:
///         createInterface() - создание графического представления
///         getValue() - возвращает QMap из одного элемента {строка для сервера}:{данные}
///         setValue() - установка необходимых сообщений в графической части
///         setEnabledInteface() - включить интефейс
///     Сигналы:
///         signalTurnOnInterface() - включить интерфейс
///     Слоты:
///         slotConnectToServerPushButtonClicked() - запрос на подключение

///  ========================    заголовочные файлы проекта
#include "../../I_cardframe.h"  //  класс-родитель
#include "mainwindow.h"         //  родительский ui
///  ========================
///
///  ========================    классы для работы с виджетами
#include <QLineEdit>            //  класс поля ввода
#include <QPushButton>          //  класс кнопки
#include <QHBoxLayout>          //  горизонтальное выравнивание
#include <QVBoxLayout>          //  вертикальное выравнивание
///  ========================

class ConnectFrame : public I_CardFrame
{
    Q_OBJECT
public:
    ConnectFrame(MainWindow *ui);

    void createInterface() override;
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;
    void setEnabledInteface(bool value) override;

signals:
    void signalTurnOnInterface();

private:
    QLineEdit *IPLineEdit;
    QLineEdit *portLineEdit;
    QPushButton *connectToServerPuchButton;
    MainWindow *parentUi;

private slots:
    void slotConnectToServerPushButtonClicked();
};

#endif // CONNECTFRAME_H

#ifndef SELECTWORKSPACEFRAME_H
#define SELECTWORKSPACEFRAME_H

///     Класс FileFrame реализует интерфейс I_CardFrame
///     используется для выбора файлов
///     Переменные:
///         chooseWorkspaceDirPushButton - кнопка выбора рабочей папки
///         dataLabel - метка данных
///         consoleMessage - консольное сообщение
///         parentUi - ссылка на родительский элемент ui
///     Методы:
///         createInterface() - создание графического представления
///         getValue() - возвращает QMap из одного элемента {строка для сервера}:{данные}
///         setValue() - установка необходимых сообщений в графической части
///         setEnabledInteface() - включить интефейс

///  ========================    заголовочные файлы проекта
#include "../../I_cardframe.h"  //  класс-родитель
#include "mainwindow.h"         //  класс формы
///  ========================
///
///  ========================    классы для работы с виджетами
#include <QLabel>               //  класс метки
#include <QPushButton>          //  класс кнопки
#include <QHBoxLayout>          //  горизонтальное выравнивание
///  ========================
///
///  ========================    классы для данных
#include <QVariant>             //  класс для общего класса
///  ========================

class SelectWorkspaceFrame : public I_CardFrame
{
    Q_OBJECT
public:
    SelectWorkspaceFrame(MainWindow *ui);

    void createInterface() override;
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;
    void setEnabledInteface(bool value) override;

private:
    QPushButton *chooseWorkspaceDirPushButton;
    QLabel *dataLabel;
    MainWindow *parentUi;
    QString consoleMessage;
};

#endif // SELECTWORKSPACEFRAME_H

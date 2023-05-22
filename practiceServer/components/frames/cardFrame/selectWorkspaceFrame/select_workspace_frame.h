#ifndef SELECTWORKSPACEFRAME_H
#define SELECTWORKSPACEFRAME_H

///     Класс SelectWorkspaceFrame реализует интерфейс I_CardFrame
///     Переменные:
///         consoleMessage - сообщение в консоль сервера
///         chooseWorkspaceDirPushButton - кнопка выбора папки для работы сервера
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
#include <QPushButton>          //  класс для работы с кнопками
#include <QLabel>               //  класс для меток
#include <QMainWindow>          //  для определения типа данных ui
#include <QHBoxLayout>          //  горизонтальное выравнивание виджетов
///  ========================
///
///  ========================    классы для работы с диалоговыми окнами
#include <QFileDialog>          //  диалоговые окна для файлов/папок
///  ========================

class SelectWorkspaceFrame : public I_CardFrame
{
    Q_OBJECT
public:
    SelectWorkspaceFrame(MainWindow *parentUi);

    void createInterface() override;
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;
    void enableInteface() override;

private:
    QString consoleMessage;
    QPushButton *chooseWorkspaceDirPushButton;
    QLabel *dataLabel;
    MainWindow *parentUi;

};

#endif // CHOOSESAVEDIRFRAME_H

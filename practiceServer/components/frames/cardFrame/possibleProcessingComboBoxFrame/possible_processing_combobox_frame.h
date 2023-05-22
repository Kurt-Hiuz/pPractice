#ifndef COMBOBOXFRAME_H
#define COMBOBOXFRAME_H

///     Класс PossibleProcessingComboBoxFrame реализует интерфейс I_CardFrame
///     Переменные:
///         consoleMessage - сообщение в консоль сервера
///         dataLabel - метка с подсказкой/данными для наглядности
///         parentUi - ссылка на родительский элемент ui
///         possibleProcessingComboBox - список обработок
///         updateProcessingPushButton - кнопка обновления списка
///         updateProcessingDialog - диалоговое окно для редактирования списка
///     Методы:
///         createInterface() - создание графического представления
///         getValue() - возвращает QMap из одного элемента {строка для сервера}:{данные}
///         setValue() - установка необходимых сообщений в графической части
///         enableInteface() - включить интефейс
///     Слоты:
///         on_updateProcessingPushButton_clicked() - открытие диалогового окна для редактирования списка обработок

///  ========================    заголовочные файлы проекта
#include "../I_cardframe.h"     //  реализуемый интерфейс
#include "mainwindow.h"         //  родительский ui
#include "components/dialogs/updateProcessingDialog/update_processing_dialog.h"   //  вызываемое диалоговое окно
///  ========================
///
///  ========================    классы для работы с виджетами
#include <QComboBox>            //  виджет выпадающего списка
#include <QLabel>               //  метка для подсказки
#include <QHBoxLayout>          //  горизонтальное выравнивание виджетов
#include <QPushButton>          //  класс кнопки
///  ========================

class PossibleProcessingComboBoxFrame : public I_CardFrame
{
    Q_OBJECT
public:
    PossibleProcessingComboBoxFrame(MainWindow *parentUi);

    void createInterface() override;
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;
    void enableInteface() override;

private:
    QString consoleMessage;
    QComboBox *possibleProcessingComboBox;
    QLabel *dataLabel;
    MainWindow *parentUi;
    QPushButton *updateProcessingPushButton;
    UpdateProcessingDialog *updateProcessingDialog;

private slots:
    void on_updateProcessingPushButton_clicked();
};

#endif // COMBOBOXFRAME_H

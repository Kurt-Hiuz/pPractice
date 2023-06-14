#ifndef UPDATEPROCESSINGDIALOG_H
#define UPDATEPROCESSINGDIALOG_H

///     Класс UpdateProcessingDialog реализует интерфейс I_DialogWindow
///     отвечает за обновление списка обработок
///     Переменные:
///         parentUi - ссылка на форму-родителя
///         jsonDataTextEdit - поле для отображения содержимого json файла
///         dataLabel - метка с подсказкой/данными для наглядности
///         openJsonFilePushButton - кнопка для открытия json файла
///         closeDialogPushButton - кнопка для закрытия диалогового окна
///         applyDataPushButton - кнопка для подтверждения новых данных
///         m_currentJsonObject - json объект для работы с файлом
///         m_jsonParser - сущность, определяющая ликвидность принимаемой json структуры
///         jsonFilePath - путь до json файла
///     Методы:
///         createInterface() - создание графического представления
///     Слоты:
///         on_openJsonFilePushButton_clicked() - устанавливает путь до файла с json данными
///         on_closeDialogPushButton_clicked() - закрывает текущее окно
///         on_applyDataPushButton_clicked() - возвращает сообщение о записи данных в ранее открытый файл

///  ========================    заголовочные файлы проекта
#include "components/dialogs/I_dialogwindow.h"  //  реализуемый интерфейс
#include "mainwindow.h"         //  родительский ui
#include "helperClasses/jsonParser/json_parser.h"   //  парсер для json данных
///  ========================
///
///  ========================    классы для работы с виджетами
#include <QTextEdit>            //  текстовое поле
#include <QLabel>               //  метка для подсказки
#include <QPushButton>          //  кнопки
#include <QVBoxLayout>          //  вертикальное выравнивание
///  ========================
///
///  ========================    классы для работы с файлами
#include <QFileDialog>          //  обращение к файлам через системное окно
#include <QFile>                //  работа с файлами
#include <QDir>                 //  работа с директориями
///  ========================
///
///  ========================    классы для работы с json
#include <QJsonObject>          //  работа с json объектами
#include <QJsonDocument>        //  работа с json документами
///  ========================

class UpdateProcessingDialog : public I_DialogWindow
{
public:
    UpdateProcessingDialog(MainWindow *parentUi);

    void createInterface() override;
private:
    MainWindow *parentUi;
    QTextEdit *jsonDataTextEdit;
    QLabel *dataLabel;
    QPushButton *openJsonFilePushButton;
    QPushButton *closeDialogPushButton;
    QPushButton *applyDataPushButton;

    QString jsonFilePath;

    QJsonObject m_currentJsonObject;
    JsonParser m_jsonParser;

private slots:
    void on_openJsonFilePushButton_clicked();
    void on_closeDialogPushButton_clicked();
    void on_applyDataPushButton_clicked();
};

#endif // UPDATEPROCESSINGDIALOG_H

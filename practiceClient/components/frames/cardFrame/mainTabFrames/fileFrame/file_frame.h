#ifndef FILEFRAME_H
#define FILEFRAME_H

///     Класс FileFrame реализует интерфейс I_CardFrame
///     используется для выбора файлов
///     Переменные:
///         filePathLabel - метка пути файла
///         filePathLineEdit - поле ввода пути файла
///         openFilePushButton - кнопка открытия проводника
///         sendFilePushButton - кнопка отправить файл
///         completer - дополняющий текст элемент
///         fileSystemModel - модель файловой структуры
///         parentUi - ссылка на родительский элемент ui
///     Методы:
///         createInterface() - создание графического представления
///         getValue() - возвращает QMap из одного элемента {строка для сервера}:{данные}
///         setValue() - установка необходимых сообщений в графической части
///         setEnabledInteface() - включить интефейс
///     Слоты:
///         sendFilePushButtonClicked() - обработка нажатия кнопки sendFilePushButton
///         openFilePushButtonClicked() - обработка нажатия кнопки openFilePushButton

///  ========================    заголовочные файлы проекта
#include "../../I_cardframe.h"  //  класс-родитель
#include "mainwindow.h"         //  родительский ui
///  ========================
///
///  ========================    классы для работы с виджетами
#include <QLabel>               //  класс текстовой метки
#include <QLineEdit>            //  класс поля ввода
#include <QPushButton>          //  класс кнопки
#include <QVBoxLayout>          //  вертикальное выравнивание
#include <QHBoxLayout>          //  горизонтальное выравнивание
#include <QFileSystemModel>     //  модель файловой системы
#include <QCompleter>           //  класс дополнения
///  ========================

class FileFrame : public I_CardFrame
{
    Q_OBJECT
public:
    FileFrame(MainWindow *ui);

    void createInterface() override;
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;
    void setEnabledInteface(bool value) override;

private:
    QLabel *filePathLabel;
    QLineEdit *filePathLineEdit;
    QPushButton *openFilePushButton;
    QPushButton *sendFilePushButton;
    MainWindow *parentUi;
    QCompleter *completer;
    QFileSystemModel *fileSystemModel;

private slots:
    void sendFilePushButtonClicked();
    void openFilePushButtonClicked();
};

#endif // FILEFRAME_H

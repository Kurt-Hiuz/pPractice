#ifndef CHATFRAME_H
#define CHATFRAME_H

///     Класс ChatFrame реализует интерфейс I_CardFrame
///     используется для фрейма чата
///     Переменные:
///         callMeLineEdit - поле для идентификации клиента
///         chatTextBrowser - поле чата
///         messageLineEdit - поле для сообщения
///         sendMessagePushButton - кнопка отправки сообщения
///         parentUi - ссылка на родительский элемент ui
///     Методы:
///         createInterface() - создание графического представления
///         getValue() - возвращает QMap из одного элемента {строка для сервера}:{данные}
///         setValue() - установка необходимых сообщений в графической части
///         setEnabledInteface() - включить интефейс
///         clearChat() - очистить чат
///     Сигналы:
///         signalSendMessageToServer() - отправить сообщение

///  ========================    заголовочные файлы проекта
#include "../../I_cardframe.h"  //  класс-родитель
#include "mainwindow.h"         //  родительский ui
///  ========================
///
///  ========================    классы для работы с виджетами
#include <QLineEdit>            //  класс поля ввода
#include <QTextBrowser>         //  класс чата
#include <QPushButton>          //  класс кнопки
#include <QVBoxLayout>          //  вертикальное выравнивание
#include <QHBoxLayout>          //  горизонтальное выравнивание
///  ========================

class ChatFrame : public I_CardFrame
{
    Q_OBJECT
public:
    ChatFrame(MainWindow *ui);

    void createInterface() override;
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;
    void setEnabledInteface(bool value) override;
    void clearChat();

signals:
    void signalSendMessageToServer(QString message, QString senderName);

private:
    QLineEdit *callMeLineEdit;
    QTextBrowser *chatTextBrowser;
    QLineEdit *messageLineEdit;
    QPushButton *sendMessagePushButton;
    MainWindow *parentUi;

private slots:
    void slotSendMessagePushButton();
};

#endif // CHATFRAME_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

///     Класс MainWindow определяет главную форму приложения
///     Переменные:
///         delimiter - строка разделитель для чата
///         * клиентские:
///             client - сам клиент
///             ui - сама форма
///             server - экземпляр класса сервера
///         * менеджеры:
///             workspaceManager - отвечает за работу с рабочей директорией
///         * графические компоненты:
///             mainContainer - контейнер главной вкладки
///             settingsContainer - контейнер настроек
///             connectFrame - фрейм подключения
///             possibleProcessingFrame - фрейм возможных обработок
///             chatFrame - фрейм чата
///             fileFrame - фрейм выбора файлов
///             selectWorkspaceFrame - фрейм выбора рабочей папки
///     Методы:
///         setEnabledInterface() - устанавливает видимость интерфейса
///         SendTextToServer() - отправка текста серверу
///         SendFileToServer() - отправка файла серверу
///         SendToServer() - отправка разнотипного сообщения
///     Сигналы:
///         signalSendTextToServer() - сигнал отправки текста
///         signalSendFileToServer() - сигнал отправки файла
///         signalSendToServer() - сигнал отправки разнотипного сообщения
///     Слоты:
///         slotStatusClient - отображение статуса сервера
///         slotMessageTextBrowser - отображение текста
///         slotSetCBDataForm - сигнал для установки обработок
///         slotSetFilePathLabel - отображение пути до файла
///         slotDisconnectClient - обработка отключения клиента
///         slotEnableInterface - отображение интерфейса
///         on_chooseWorkspaceDirPushButton_clicked - по нажатию на "Выбрать рабочую папку"
///         on_chooseProcessingPushButton_clicked - по нажатию на "Выбрать"

/// =========================   классы для работы формы
#include <QMainWindow>          //  класс-родитель
#include <QObject>              //  сигналы-слоты
/// =========================
///
/// =========================   для работы клиента
#include <QTcpSocket>           //  класс сокета
/// =========================
///
/// =========================   Для работы с файлами
#include <QFile>                //  определитель файла
#include <QFileInfo>            //  информация о файле
#include <QFileDialog>          //  класс работы с диалоговыми окнами (выбор файла)
/// =========================
///
/// =========================   Для работы автодополнения ввода
#include <QCompleter>           //  дополняет строку при вводе пути к файлу
#include <QFileSystemModel>     //  работа с файловой моделью
#include <QFileSystemWatcher>   //  наблюдатель за файловой системой
/// =========================
///
/// =========================   Разные дополнения
#include <QTime>                //  для отображения времени отправки
#include <QMap>                 //  определение глоссария для приходящих данных сокета
#include <QVBoxLayout>
/// =========================
///
/// =========================   Классы проекта
#include "client.h"             //  класс клиента
#include "components/frames/cardFrame/I_cardframe.h"    //  интерфейс виджета-карточки
#include "helperClasses/managers/workspaceManager/workspace_manager.h"  //  менеджер рабочей папки
/// =========================

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setEnableInteface();

signals:
    void signalSendTextToServer(QString &message, QString &senderName);
    void signalSendFileToServer(QString &filePath);
    void signalSendToServer(QString typeOfMsg, QString str);

private:
    Ui::MainWindow *ui;
    Client *client = nullptr;

    QString delimiter = "<font color = black><\\font><br>=======================";  //  создаем разделитель для сообщений

    void SendTextToServer(QString str); //  метод отправки на сервер сообщения
    void SendFileToServer(QString filePath);    //  метод отправки на сервер файла по его пути
    void SendToServer(QString typeOfMsg, QString str);  //  отправка служебных сообщений серверу

    void setEnabledInterface(bool flag);   //  функция блокировки интерфейса, пока снова не подключимся к хосту

    QVBoxLayout *mainContainer = new QVBoxLayout();
    QVBoxLayout *settingsContainer = new QVBoxLayout();
    I_CardFrame *connectFrame;
    I_CardFrame *possibleProcessingFrame;
    I_CardFrame *chatFrame;
    I_CardFrame *fileFrame;
    I_CardFrame *selectWorkspaceFrame;

    WorkspaceManager *workspaceManager;

private slots:
    void slotStatusClient(QString status);
    void slotMessageTextBrowser(QString message);
    void slotSetCBDataForm(QMap<QString,QVariant> possibleProcessingData);
    void slotSetFilePathLabel(QString text);
    void slotEnableInterface(QString message);

public slots:
    void on_chooseWorkspaceDirPushButton_clicked();
    void on_chooseProcessingPushButton_clicked();

};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>   //  класс сервера

/// =========================   Для работы с файлами
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>  //  класс работы с диалоговыми окнами (выбор файла)
/// =========================
///
/// =========================   Для работы автодополнения ввода
#include <QCompleter>   //  дополняет строку при вводе пути к файлу
#include <QFileSystemModel> //  работа с файловой моделью
#include <QFileSystemWatcher>   //  наблюдатель за файловой системой
/// =========================
///
/// =========================   Разные дополнения
#include <QTime>    //  для отображения времени отправки
#include <QMap>     //  определение глоссария для приходящих данных сокета
#include <QVBoxLayout>
/// =========================
///
/// =========================   Классы проекта
#include "client.h"             //  класс клиента
#include "components/frames/cardFrame/I_cardframe.h"    //  интерфейс виджета-карточки
#include "helperClasses/managers/workspaceManager/workspace_manager.h"  //  менеджер рабочей папки

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
    Client *client;

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

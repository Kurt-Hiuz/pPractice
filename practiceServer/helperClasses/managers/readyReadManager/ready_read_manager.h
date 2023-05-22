#ifndef READYREADMANAGER_H
#define READYREADMANAGER_H

///     Класс ReadyReadManager определяет зону ответственности за принимаемые сервером сообщения
///     Переменные:
///         messageManagers - хранит название обработчика и его указатель
///         clientsMessageManager - указатель на менеджер типа "Message"
///         clientsFileManager - указатель на менеджер типа "File"
///         nullManager - указатель на менеджер типа "No type"
///     Методы:
///         identifyMessage() - определяет тип сообщения и возвращает указатель на необходимого обработчика
///         setEntryFolder() - установки EntryFolder для ClientsFileManager
///     Сигналы:
///         signalStatusRRManagerServer() - отправляет серверу статус
///         signalSendToAllClientsServer() - отправляет сообщение всем клиентам, обращаясь к серверу
///         signalSendToOneRRManager() - отправляет сообщение указанному клиенту, обращаясь к серверу
///     Слоты:
///         slotStatusRRManager() - принимает статусы на сервер от дочерних менеджеров
///         slotSendToAllClientsRRManager() - принимает сообщения для всех клиентов от дочерних менеджеров
///         slotSendToOneRRManager() - принимает сообщения для указанного сокета от дочерних менеджеров

///  ========================    классы проекта
#include "supportRRManagers/I_message_manager.h"    //  для работы с ссылками обработчиков
#include "supportRRManagers/nullManager/null_manager.h"    //  для определения менеджера "заглушки"
#include "supportRRManagers/clientsMessageManager/clients_message_manager.h"    //  для определения менеджера "Message"
#include "supportRRManagers/clientsFileManager/clients_file_manager.h"  //  для определения менеджера "File"
///  ========================
///
///  ========================    классы для работы
#include <QObject>              //  для создания объекта
#include <QMap>                 //  для хранения менеджеров
///  ========================

class ReadyReadManager : public QObject
{
    Q_OBJECT
private:
    QMap<QString, I_MessageManager*> messageManagers;

    ClientsMessageManager *clientsMessageManager;
    ClientsFileManager *clientsFileManager;
    NullManager *nullManager;
public:
    ReadyReadManager();
    I_MessageManager* identifyMessage(QString typeOfMess);
    void setEntryFolder(QString &entryFolder);

signals:
    void signalStatusRRManagerServer(QString status);
    void signalSendToAllClientsServer(QString typeOfMsg, QString str);
    void signalSendToOneRRManager(QTcpSocket* socket, QString typeOfMsg, QString str);

private slots:
    void slotStatusRRManager(QString status);
    void slotSendToAllClientsRRManager(QString typeOfMsg, QString str);
    void slotSendToOneRRManager(QTcpSocket* socket, QString typeOfMsg, QString str);
};

#endif // READYREADMANAGER_H

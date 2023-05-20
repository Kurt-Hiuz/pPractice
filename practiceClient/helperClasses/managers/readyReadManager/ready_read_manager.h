#ifndef READYREADMANAGER_H
#define READYREADMANAGER_H

///     Класс ReadyReadManager определяет зону ответственности за принимаемые сервером сообщения
///     Переменные:
///     messageManagers - хранит название обработчика и его указатель
///     serverMessageManager - указатель на менеджер типа "Message"
///     clientsFileManager - указатель на менеджер типа "File"
///     nullManager - указатель на менеджер типа "No type"
///     Методы:
///     identifyMessage() - определяет тип сообщения и возвращает указатель на необходимого обработчика
///     setEntryFolder() - установки EntryFolder для ClientsFileManager
///     Сигналы:
///     signalStatusRRManagerServer() - отправляет серверу статус
///     signalSendToAllClientsServer() - отправляет сообщение всем клиентам, обращаясь к серверу
///     signalSendToOneRRManager() - отправляет сообщение указанному клиенту, обращаясь к серверу
///     Слоты:
///     slotStatusRRManager() - принимает статусы на сервер от дочерних менеджеров
///     slotSendToAllClientsRRManager() - принимает сообщения для всех клиентов от дочерних менеджеров
///     slotSendToOneRRManager() - принимает сообщения для указанного сокета от дочерних менеджеров

///  ========================    классы проекта
#include "supportRRManagers/I_message_manager.h"    //  для работы с ссылками обработчиков
//  для определения менеджера..
#include "supportRRManagers/nullManager/null_manager.h"     //.. "заглушки"
#include "supportRRManagers/serverMessageManager/server_message_manager.h"    //.. "Message"
#include "supportRRManagers/possibleProcessingManager/possible_processing_manager.h"    //.. списка обработок
#include "supportRRManagers/clientFileRequestPartManager/client_file_request_part_manager.h"    //..отправки файлов
#include "supportRRManagers/fileDownloadedManager/file_downloaded_manager.h"    //..остановки загрузки
///  ========================
///
///  ========================    классы для работы
#include <QObject>
#include <QMap>                 //  для хранения менеджеров

class ReadyReadManager : public QObject
{
    Q_OBJECT
private:
    QMap<QString, I_MessageManager*> messageManagers;

    ServerMessageManager *serverMessageManager;
    PossibleProcessingManager *possibleProcessingManager;
    ClientFileRequestPartManager *clientFileRequestPartManager;
    FileDownloadedManager *fileDownloadedManager;
    NullManager *nullManager;

public:
    ReadyReadManager();
    I_MessageManager* identifyMessage(QString typeOfMess);
    void setEntryFolder(QString &entryFolder);
    void setFileClientFileRequest(QString &filePath);

signals:
    void signalMessageRRManagerClient(QString message);
    void signalStatusRRManagerClient(QString status);
    void signalSetCBData(QMap<QString,QVariant> &possibleProcessingData);
    void signalSendToAllClientsServer(QString typeOfMsg, QString str);
    void signalSendToOneRRManager(QTcpSocket* socket, QString typeOfMsg, QString str);
    void signalSendBufferToServer(QByteArray &data);

private slots:
    void slotMessageRRManager(QString message);
    void slotStatusRRManager(QString status);
    void slotSetCBDataRRManager(QMap<QString,QVariant> &possibleProcessingData);
    void slotSendToAllClientsRRManager(QString typeOfMsg, QString str);
    void slotSendToOneRRManager(QTcpSocket* socket, QString typeOfMsg, QString str);
    void slotSendBufferRRManager(QByteArray &buffer);
};

#endif // READYREADMANAGER_H

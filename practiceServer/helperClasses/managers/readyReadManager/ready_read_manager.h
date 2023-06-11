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
                                                            //  для определения менеджера..
#include "supportRRManagers/nullManager/null_manager.h"     //   .."заглушки"
#include "supportRRManagers/clientsMessageManager/clients_message_manager.h"    //  .."Message"
#include "supportRRManagers/clientsFileManager/clients_file_manager.h"  //  .."File"
#include "supportRRManagers/fileDownloadedManager/file_downloaded_manager.h"    //  .."File Downloaded"
#include "supportRRManagers/serverRequestPartFileManager/server_request_part_file_manager.h"    //  .."Request part of processing file"
#include "supportRRManagers/clientsProcessingManager/clients_processing_manager.h"  //  .."Set processing on client"
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
    FileDownloadedManager *fileDownloadedManager;
    ServerRequestPartFileManager *serverRequestPartFileManager;
    ClientsProcessingManager *clientsProcessingManager;
    NullManager *nullManager;
public:
    ReadyReadManager();
    I_MessageManager* identifyMessage(QString typeOfMess);
    void setEntryFolder(QString &entryFolder);
    void setFileClientFileRequest(QString &filePath);

signals:
    void signalStatusRRManagerServer(QString status);
    void signalSendToAllClientsServer(QString typeOfMsg, QString str);
    void signalSendToOneRRManager(QTcpSocket* socket, QString typeOfMsg, QString str);
    void signalSendBufferToClient(QTcpSocket *socketToSend, QByteArray &buffer);
    void signalSetClientProcessing(QTcpSocket *socket, QString currentProcessing);
    void signalDeleteSendedFile(QString &fileName);
    void signalDeleteExpectationFile(QString &fileName);
    void signalSaveData(QString fileName);
    void signalCheckExpectationFolder(QTcpSocket *socket);
};

#endif // READYREADMANAGER_H

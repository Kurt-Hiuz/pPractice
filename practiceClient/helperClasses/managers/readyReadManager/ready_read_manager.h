#ifndef READYREADMANAGER_H
#define READYREADMANAGER_H

///     Класс ReadyReadManager определяет зону ответственности за принимаемые сервером сообщения
///     Переменные:
///         messageManagers - хранит название обработчика и его указатель
///         serverMessageManager - указатель на менеджер типа "Message"
///         serverFileManager - указатель на менеджер типа "File"
///         possibleProcessingManager - указатель на менеджер типа "Possible processing ComboBox data"
///         clientFileRequestPartManager - указатель на менеджер типа "Request part of file"
///         fileDownloadedManager - указатель на менеджер типа "File downloaded"
///         disconnectManager - указатель на менеджер типа "Disconnect"
///         nullManager - указатель на менеджер типа "No type"
///     Методы:
///         identifyMessage() - определяет тип сообщения и возвращает указатель на необходимого обработчика
///         setEntryFolder() - установки EntryFolder для ClientsFileManager
///         setFileClientFileRequest() - установка файлов
///     Сигналы:
///         signalStatusRRManagerClient() - отправляет клиенту статус
///         signalMessageRRManagerClient() - отправляет клиенту сообщение
///         signalSetCBData() - устанавливает данные в комбобокс
///         signalSendToAllClientsServer() - отправляет сообщение всем клиентам, обращаясь к серверу
///         signalSendToOneRRManager() - отправляет сообщение указанному клиенту, обращаясь к серверу
///         signalSendBufferToServer() - отправляет буфер данных серверу
///         signalSendToServer() - отправляет серверу некоторое сообщение
///         signalDeleteSendedFile() - удаляет отправленные файлы
///         signalEnableInterface() - вкл/выкл интерфейс

///  ========================    классы для работы
#include <QObject>              //  класс для сигналов и слотов
#include <QMap>                 //  для хранения менеджеров
///  ========================
///
///  ========================    классы проекта
#include "supportRRManagers/I_message_manager.h"    //  для работы с ссылками обработчиков
//  для определения менеджера..
#include "supportRRManagers/nullManager/null_manager.h"     //.. "заглушки"
#include "supportRRManagers/serverMessageManager/server_message_manager.h"    //.. "Message"
#include "supportRRManagers/possibleProcessingManager/possible_processing_manager.h"    //.. списка обработок
#include "supportRRManagers/clientFileRequestPartManager/client_file_request_part_manager.h"    //..отправки частей файлов
#include "supportRRManagers/fileDownloadedManager/file_downloaded_manager.h"    //..остановки загрузки
#include "supportRRManagers/serverFileManager/server_file_manager.h"    //..приёма файлов
#include "supportRRManagers/disconnectManager/disconnect_manager.h"     //..обработки отключения
///  ========================

class ReadyReadManager : public QObject
{
    Q_OBJECT
private:
    QMap<QString, I_MessageManager*> messageManagers;

    ServerMessageManager *serverMessageManager;
    PossibleProcessingManager *possibleProcessingManager;
    ClientFileRequestPartManager *clientFileRequestPartManager;
    FileDownloadedManager *fileDownloadedManager;
    ServerFileManager *serverFileManager;
    DisconnectManager *disconnectManager;
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
    void signalSendToServer(QString typeOfMsg, QString str);
    void signalDeleteSendedFile(QString &fileName);
    void signalEnableInterface(QString message);
};

#endif // READYREADMANAGER_H

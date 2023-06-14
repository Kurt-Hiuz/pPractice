#ifndef SERVER_H
#define SERVER_H

///     Класс Server определяет сам сервер
///     Переменные:
///         delimiter - создаем разделитель для сообщений
///         * серверные:
///             socket - сокет для подключения
///             generatedServerPort - сгенерированный порт
///             mapSockets - хранит сокет и закреплённую за ним обработку
///             Data - то, что отправляется между сервером и клиентом
///             mapRequest - определяем глоссарий запросов к сторонам
///             possibleProcessing - определяем возможные обработки с приставкой и её человеческим описанием
///             nextBlockSize - блок нового сообщения
///             maxConnections - макс. подключений
///         * для работы с файлами:
///             fileSystemWatcher - наблюдатель за файлами
///             file - определяем файл
///             bytes - массив байт данных
///             fileSize - размер файла
///             fileName - его название
///             blockData - размер данных
///             workspaceFolder - путь до рабочей директории
///             entryFolder -  путь до папки для файлов извне
///             storageFolder -  путь до папки с приходящей обработанной информацией от клиентов
///             expectationFolder - путь до папки ожидания
///             sendedFilesFolder - путь до папки отправленных файлов
///         * менеджеры:
///             processingManager - менеджер для обработок
///             readyReadManager - менеджер для чтения сообщений
///     Методы:
///         SendPossibleProcessing - отправка всех возможных обработок
///         SendToAllClients - отправка всем клиентам необходимых данных
///         SendToOneClient - отправка сообщений конкретному клиенту
///         SendFileToClient - отправка данных о файлах
///     Сигналы:
///         signalStatusServer() - отправляет серверу статус
///         signalChatNewMessage() - сигнал нового сообщений
///         signalAddSocketToListWidget() - сигнал для добавления сокета в clientsListWidget
///         signalDeleteSocketFromListWidget() - сигнал для удаления сокета из clientsListWidget при его отключении
///     Слоты:
///         slotSendBufferToClient() - отправляет буфер данных клиенту
///         slotSendToAllClientsRRManager() - принимает сообщения для всех клиентов от дочерних менеджеров
///         slotSendToOneRRManager() - принимает сообщения для указанного сокета от дочерних менеджеров
///         slotSetClientProcessing() - закрепление обработки за клиентом
///         slotSiftFiles() - отправка файлов по клиентам
///         slotDeleteSendedFile() - удаление отправленных файлов
///         slotDeleteExpectationFile() - удаление ожидающих файлов
///         slotSaveData() - сохранение данных
///         slotCheckExpectationFolder() - проверка папки ожидания
///         incomingConnection() - прием новых подключений
///         slotReadyRead() - слот чтения сообщений
///         slotDisconnect() - обработчик отключения клиента
///         slotSocketDisplayed() - обработчик отображаемого клиента
///         slotDisconnectSocket() - отключение клиента
///         slotDisconnectAll() - отключение всех клиентов
///         slotUpdatePossibleProcessing() - обновление возможных обработок
///         slotSetServerFolders() - установка папок в переменные
///         slotSendMessage() - отправка сообщений

///  ========================   классы для работы сервера
#include <QTcpServer>           //  сам сервер
#include <QTcpSocket>           //  работа с сокетами
#include <QMainWindow>          //  работа с формой
///  ========================
///
///  ========================   классы для работы с файлами
#include <QFile>                //  для работы с файлами
#include <QDir>                 //  для работы с директорией
#include <QFileSystemWatcher>   //  наблюдатель за файловой системой
///  ========================
///
///  ========================   дополнения
#include <QVector>              //  класс вектора для хранения созданных сокетов
#include <QTime>                //  время
#include <QMap>                 //  определение глоссария для приходящих данных сокета
#include <QRandomGenerator>     //  генератор случайных чисел
///  ========================
///
///  ========================   классы проекта
#include "helperClasses/managers/workspaceManager/workspace_manager.h"
#include "helperClasses/managers/readyReadManager/ready_read_manager.h"     //  класс для работы слота ReadyRead
#include "helperClasses/managers/readyReadManager/supportRRManagers/I_message_manager.h"    //  класс для работы с обработчиками сообщений
///  ========================

class Server : public QTcpServer{
    Q_OBJECT
public:
    Server(bool &server_started, int serverPort);
    QTcpSocket *socket;
    WorkspaceManager *workspaceManager = nullptr;
    void setWorkspaceManager(WorkspaceManager *newWorkspaceManager);
    void setMaxConnections(int count);

private:
    int generatedServerPort = 0;    //  дефолтное значение отсутствия порта. Порт сгенерируется позже
    QMap<QTcpSocket*, QString> mapSockets;
    int maxConnections = 30;    //  дефолтное значение
    QByteArray Data;

    QMap<QString,QString> mapRequest;
    QMap<QString, QVariant> possibleProcessing;

    qint64 nextBlockSize;

    QFile *file;
    char *bytes = {0};
    int fileSize;
    QString fileName;
    int blockData = 1000000;

    QString workspaceFolder = "";
    QString entryFolder = "";
    QString storageFolder = "";
    QString expectationFolder = "";
    QString sendedFilesFolder = "";
    QString delimiter = "<font color = black><\\font><br>=======================";

    QFileSystemWatcher *fileSystemWatcher;

    ReadyReadManager *readyReadManager;

    void SendPossibleProcessing(QTcpSocket* socket, QMap<QString, QVariant> possibleProcessingData);

    void SendToAllClients(QString typeOfMsg, QString str);
    void SendToOneClient(QTcpSocket* socket, QString typeOfMsg, QString str);

    void SendFileToClient(QTcpSocket *socket, QString filePath);

private slots:
    void slotSendToAllClients(QString typeOfMsg, QString str);
    void slotSendToOneClient(QTcpSocket* sendSocket, QString typeOfMsg, QString str);
    void slotSendBufferToClient(QTcpSocket *socketToSend, QByteArray &buffer);
    void slotSetClientProcessing(QTcpSocket* socket, QString currentProcessing);
    void slotSiftFiles(QStringList &fileInfoList);
    void slotDeleteSendedFile(QString &fileName);
    void slotDeleteExpectationFile(QString &fileName);
    void slotSaveData(QString fileName);
    void slotCheckExpectationFolder(QTcpSocket* checkingSocket);

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
    void slotDisconnect();
    void slotSocketDisplayed(QTcpSocket* displayedSocket);
    void slotDisconnectSocket(int socketDiscriptorToDelete);
    void slotDisconnectAll(QString reason);
    void slotUpdatePossibleProcessing(QVariant newPossibleProcessingData);
    void slotSetServerFolders(QMap<QString, QString> &subFolders);
    void slotSendMessage(QString message);

signals:
    void signalStatusServer(QString status);
    void signalChatNewMessage(QString message);
    void signalAddSocketToListWidget(QTcpSocket* socketToAdd);
    void signalDeleteSocketFromListWidget(QMap<QTcpSocket*, QString> mapSockets);
};

#endif // SERVER_H

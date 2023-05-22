#ifndef SERVER_H
#define SERVER_H

///     Класс Server определяет сам сервер
///     Переменные:
///         socket - сокет для подключения
///         generatedServerPort - сгенерированный порт
///         mapSockets - хранит сокет и закреплённую за ним обработку
///         Data - то, что отправляется между сервером и клиентом
///         mapRequest - определяем глоссарий запросов к сторонам
///         possibleProcessing - определяем возможные обработки с приставкой и её человеческим описанием
///         nextBlockSize - блок нового сообщения
///         delimiter - создаем разделитель для сообщений
///         fileSystemWatcher - наблюдатель за файлами
///         processingManager - менеджер для обработок
///         readyReadManager - менеджер для чтения сообщений
///     Методы:
///         SendPossibleProcessing - отправка всех возможных обработок
///         SendToAllClients - отправка всем клиентам необходимых данных
///         SendToOneClient - отправка сообщений конкретному клиенту
///     Сигналы:
///         signalStatusRRManagerServer() - отправляет серверу статус
///         signalSendToAllClientsServer() - отправляет сообщение всем клиентам, обращаясь к серверу
///         signalSendToOneRRManager() - отправляет сообщение указанному клиенту, обращаясь к серверу
///     Слоты:
///         slotStatusRRManager() - принимает статусы на сервер от дочерних менеджеров
///         slotSendToAllClientsRRManager() - принимает сообщения для всех клиентов от дочерних менеджеров
///         slotSendToOneRRManager() - принимает сообщения для указанного сокета от дочерних менеджеров

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
#include "helperClasses/managers/processingManager/processing_manager.h"    //  класс для распределения файлов на обработчиков
#include "helperClasses/managers/readyReadManager/ready_read_manager.h"     //  класс для работы слота ReadyRead
#include "helperClasses/managers/readyReadManager/supportRRManagers/I_message_manager.h"    //  класс для работы с обработчиками сообщений
///  ========================

class Server : public QTcpServer{
    Q_OBJECT
public:
    Server(bool &server_started);
    QTcpSocket *socket;
    int generatedServerPort = QRandomGenerator::global()->bounded(1024, 65535);

private:
    QMap<QTcpSocket*, QString> mapSockets;
    QByteArray Data;

    QMap<QString,QString> mapRequest;
    QMap<QString, QVariant> possibleProcessing;

    qint64 nextBlockSize;

    QFile *file;    //  определяем файл
    char *bytes = {0};     //  массив байт данных
    int fileSize;   //  размер файла
    QString fileName;   //  его название
    int blockData = 1000000;  //  размер данных

    QString workspaceFolder = "";   //  путь до рабочей директории
    QString entryFolder = ""; //  путь до папки для файлов извне
    QString storageFolder = "";  //  путь до папки с приходящей обработанной информацией от клиентов
    QString expectationFolder = "";
    QString delimiter = "<font color = black><\\font><br>=======================";

    QFileSystemWatcher *fileSystemWatcher;

    ProcessingManager *processingManager;
    ReadyReadManager *readyReadManager;

    void SendPossibleProcessing(QTcpSocket* socket, QMap<QString, QVariant> possibleProcessingData);  //  функция передачи возможных обработок

    void SendToAllClients(QString typeOfMsg, QString str);      //  функция для передачи данных всем клиентам
    void SendToOneClient(QTcpSocket* socket, QString typeOfMsg, QString str);       //  функция для передачи данных одному клиенту

    void SendFileToClient(QString filePath);    //  функция отправки файл (начало)
    void SendPartOfFile();      //  функция отправки части файла (продолжение)

private slots:
    void slotStatusServer(QString status);
    void slotSendToAllClients(QString typeOfMsg, QString str);
    void slotSendToOneClient(QTcpSocket* sendSocket, QString typeOfMsg, QString str);

public slots:
    void incomingConnection(qintptr socketDescriptor);  //  обработчик новых подключений
    void slotReadyRead();   //  обработчик полученных от клиента сообщений и файлов
    void slotDisconnect();  //  обработчик отключившихся клиентов
    void slotNewWorkspaceFolder(QString newFolderPath);  //  обработчик новой директории
    void slotEntryFolderChanged(const QString & fileName);  //  обработчик изменений в директории
    void slotSocketDisplayed(QTcpSocket* displayedSocket);  //  обработчик для размещенного сокета
    void slotDisconnectSocket(int socketDiscriptorToDelete);    //  обработчик для принудительного удаления сокета
    void slotUpdatePossibleProcessing(QVariant newPossibleProcessingData);

signals:
    void signalStatusServer(QString);   //  слот для обработки состояния сервера
    void signalAddSocketToListWidget(QTcpSocket* socketToAdd);     //  слот для добавления сокета в clientsListWidget
    void signalDeleteSocketFromListWidget(QMap<QTcpSocket*, QString> mapSockets);  //  слот для удаления сокета из clientsListWidget при его отключении
    //    void signalChatServer(QString);     //  слот для обработки чата сервераы
};

#endif // SERVER_H

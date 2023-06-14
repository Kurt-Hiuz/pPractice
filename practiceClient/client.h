#ifndef CLIENT_H
#define CLIENT_H

///     Класс Server определяет сам сервер
///     Переменные:
///         * серверные:
///             socket - сокет для подключения
///             Data - то, что отправляется между сервером и клиентом
///             nextBlockSize - блок нового сообщения
///         * для работы с файлами:
///             fileSystemWatcher - наблюдатель за файлами
///             file - определяем файл
///             fileSize - размер файла
///             fileName - его название
///             workspaceFolder - путь до рабочей директории
///             entryFolder -  путь до папки для файлов извне
///             processedFolder - путь до папки обработанных файлов
///         * менеджеры:
///             workspaceManager - менеджер для рабочей папки
///             readyReadManager - менеджер для чтения сообщений
///     Методы:
///         setWorkspaceManager() - установка workspaceManager
///     Сигналы:
///         signalStatusClient() - отправляет клиенту статус
///         signalMessageTextBrowser() - отображение сообщения в чате
///         signalSetCBDataForm() - установка обработок
///         signalSetFilePathLabel() - установка пути в метку
///         signalSetFileClientFileRequest() - установка файла на скачивание
///         signalEnableInterface() - вкл/выкл интерфейс
///     Слоты:
///         slotSendBufferToServer() - отправляет буфер данных серверу
///         slotSiftFiles() - отправка файлов по клиентам
///         slotReadyRead() - слот чтения сообщений
///         slotMessageServer - отправка сообщений
///         slotStatusClient - отображение статуса
///         slotSetCBData - установка обработок
///         slotSendTextToServer - отправка текста
///         slotSendFileToServer - отправка данных о файле
///         slotSetClientFolders - установка путей папок в переменные
///         slotSendToServer - отправка сообщения разного типа
///         slotDeleteSendedFile - удалить отправленные файлы

///  ========================   классы для работы клиента
#include <QTcpSocket>           //  определение сокета
#include <QDataStream>          //  работа с потоком данных
///  ========================
///
///  ========================   для работы класса
#include <QObject>              //  сигналы и слоты
///  ========================
///
///  ========================   для работы с файлами
#include <QFile>                //  определитель файла
#include <QFileInfo>            //  информация о файлах
#include <QFileSystemWatcher>   //  наблюдатель
///  ========================
///
///  ========================   классы проекта
#include "helperClasses/managers/readyReadManager/ready_read_manager.h" //  класс для распределения файлов на обработчиков
#include "helperClasses/managers/readyReadManager/supportRRManagers/I_message_manager.h"    //  класс для работы с обработчиками сообщений
#include "helperClasses/managers/workspaceManager/workspace_manager.h"
///  ========================

class Client : public QTcpSocket
{
    Q_OBJECT
public:
    Client();
    WorkspaceManager *workspaceManager = nullptr;
    void setWorkspaceManager(WorkspaceManager *newWorkspaceManager);

signals:
    void signalStatusClient(QString status);
    void signalMessageTextBrowser(QString message);
    void signalSetCBDataForm(QMap<QString,QVariant> &possibleProcessingData);
    void signalSetFilePathLabel(QString text);
    void signalSetFileClientFileRequest(QString &filePath);
    void signalEnableInterface(QString message);

private:
    QByteArray Data;    //  передаваемые файлы
    QTcpSocket *socket; //  сокет соединения
    ReadyReadManager *readyReadManager;
    qint64 nextBlockSize;

    int fileSize;   //  размер файла
    QString fileName;   //  его название
    QFile *file;     //  сам файлик

    QString workspaceFolder = "";   //  путь до рабочей директории
    QString entryFolder = ""; //  путь до папки для файлов извне
    QString processedFolder = "";  //  путь до папки с приходящей обработанной информацией от клиентов

    QFileSystemWatcher *fileSystemWatcher;

public slots:
    void slotSendTextToServer(QString &message, QString &senderName);
    void slotSendFileToServer(QString &filePath);
    void slotSetClientFolders(QMap<QString, QString> &subFolders);
    void slotSendToServer(QString typeOfMsg, QString str);
    void slotDeleteSendedFile(QString &fileName);

private slots:
    void slotReadyRead();
    void slotMessageServer(QString message);
    void slotStatusClient(QString status);
    void slotSetCBData(QMap<QString,QVariant> &possibleProcessingData);
    void slotSendBufferToServer(QByteArray &data);
    void slotEntryFolderChanged(const QString & fileName);  //  обработчик изменений в директории
    void slotSiftFiles(QStringList &fileInfoList);
};

#endif // CLIENT_H

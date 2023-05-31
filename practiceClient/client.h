#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QFileSystemWatcher>

#include "helperClasses/managers/readyReadManager/ready_read_manager.h" //  класс для распределения файлов на обработчиков
#include "helperClasses/managers/readyReadManager/supportRRManagers/I_message_manager.h"    //  класс для работы с обработчиками сообщений

class Client : public QTcpSocket
{
    Q_OBJECT
public:
    Client();

signals:
    void signalStatusClient(QString status);
    void signalMessageTextBrowser(QString message);
    void signalSetCBDataForm(QMap<QString,QVariant> &possibleProcessingData);
    void signalSetFilePathLabel(QString text);
    void signalSetFileClientFileRequest(QString &filePath);

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


private slots:
    void slotReadyRead();
    void slotMessageServer(QString message);
    void slotStatusClient(QString status);
    void slotSetCBData(QMap<QString,QVariant> &possibleProcessingData);
    void slotSendBufferToServer(QByteArray &data);
    void slotSendToServer(QString typeOfMsg, QString str);
    void slotEntryFolderChanged(const QString & fileName);  //  обработчик изменений в директории
};

#endif // CLIENT_H

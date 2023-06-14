#ifndef CLIENTSREQUESTPARTPROCESSINGFILE_H
#define CLIENTSREQUESTPARTPROCESSINGFILE_H

///     Класс ServerRequestPartFileManager реализует интерфейс I_MessageManager
///     обрабатывает всё необходимое, что связанно с запросами на части файлов
///     Переменные:
///         string - сообщение в консоль
///         file - определяем файл
///         bytes - массив байт данных
///         fileSize - размер файла
///         fileName - его название
///         blockData - размер данных
///         buffer - буфер байтов
///     Методы:
///         readDataFromStream() - чтение данных с потока
///         writeDataFromStream() - запись данных в поток
///         processData() - обрабатывает приходящие данные
///         typeOfMessage() - возвращает строку тип менеджера
///         setFilePath() - устанавливаем путь до файла
///     Сигналы:
///         signalStatusRRManager() - передача ReadyReadManager статуса сервера
///         signalSendBufferRRManager() - отправка данных конкретному сокету
///     Слоты:
///         slotClearFileData() - обработчик чистки данных о файле

///  ========================    классы проекта
#include "../I_message_manager.h"   //  реализуем интерфейс
///  ========================
///
///  ========================    для работы с файлами
#include <QFile>                //  определение файла
///  ========================

class ServerRequestPartFileManager : public I_MessageManager
{
    Q_OBJECT
public:
    ServerRequestPartFileManager();
    void readDataFromStream(QDataStream &inStream) override;
    void writeDataToStream(QDataStream &outStream) override;
    void processData(QDataStream &inStream, QTcpSocket *socket) override;
    QString typeOfMessage() override;

    void setFilePath(QString &filePath);

signals:
    void signalStatusRRManager(QString status);
    void signalSendBufferRRManager(QTcpSocket *socketToSend, QByteArray &buffer);

private:
    QString string;
    int fileSize;   //  размер файла
    QString fileName;   //  его название
    QFile *file;     //  сам файлик
    char *bytes = nullptr;     //  массив байт данных
    int blockData = 1000000;  //  размер данных
    QByteArray buffer;

//  переопределение операторов >> и <<
protected:
    friend QDataStream &operator >> (QDataStream &in, ServerRequestPartFileManager &serverRequestPartFileManager);
    friend QDataStream &operator << (QDataStream &out, ServerRequestPartFileManager &serverRequestPartFileManager);

public slots:
    void slotClearFileData();
};

#endif // CLIENTSREQUESTPARTPROCESSINGFILE_H

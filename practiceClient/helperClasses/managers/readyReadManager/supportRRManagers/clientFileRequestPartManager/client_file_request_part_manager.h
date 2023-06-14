#ifndef CLIENTFILEREQUESTPARTMANAGER_H
#define CLIENTFILEREQUESTPARTMANAGER_H

///     Класс ClientFileRequestPartManager реализует интерфейс I_MessageManager
///     обрабатывает всё необходимое, что связанно с частями файлов
///     Переменные:
///         string - строка из потока
///         file - определяем файл
///         bytes - массив байт данных
///         fileSize - размер файла
///         fileName - его название
///         blockData - размер данных
///         entryFolderName - папка для файлов извне
///         buffer - буфер данных
///     Методы:
///         readDataFromStream() - чтение данных с потока
///         writeDataFromStream() - запись данных в поток
///         processData() - обрабатывает приходящие данные
///         typeOfMessage() - возвращает строку тип менеджера
///         setFilePath() - установка пути файла
///     Сигналы:
///         signalStatusRRManager() - отправка статуса ReadyReadManager'у
///         signalSendBufferRRManager() - отправка буфера
///     Слоты:
///         slotClearFileData() - очистка данных

///  ========================    классы проекта
#include "../I_message_manager.h"
///  ========================
///
///  ========================    для работы с файлами
#include <QFile>                //  определитель файла
///  ========================

class ClientFileRequestPartManager : public I_MessageManager
{
    Q_OBJECT
public:
    ClientFileRequestPartManager();

    void readDataFromStream(QDataStream &inStream) override;
    void writeDataToStream(QDataStream &outStream) override;
    void processData(QDataStream &inStream) override;
    QString typeOfMessage() override;

    void setFilePath(QString &filePath);

private:
    QString string;
    int fileSize;   //  размер файла
    QString fileName;   //  его название
    QFile *file;     //  сам файлик
    char *bytes = nullptr;     //  массив байт данных
    int blockData = 1000000;  //  размер данных
    QByteArray buffer;

signals:
    void signalStatusRRManager(QString status);
    void signalSendBufferRRManager(QByteArray &buffer);

//  переопределение операторов >> и <<
protected:
    friend QDataStream &operator >> (QDataStream &in, ClientFileRequestPartManager &clientFileRequestPartManager);
    friend QDataStream &operator << (QDataStream &out, ClientFileRequestPartManager &clientFileRequestPartManager);


public slots:
    void slotClearFileData();
};

#endif // CLIENTFILEREQUESTPARTMANAGER_H

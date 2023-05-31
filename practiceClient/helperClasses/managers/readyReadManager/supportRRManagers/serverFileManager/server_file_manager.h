#ifndef SERVERFILEMANAGER_H
#define SERVERFILEMANAGER_H

///     Класс ServerFileManager реализует интерфейс I_MessageManager
///     обрабатывает всё необходимое, что связанное с присылаемыми файлами
///     Переменные:
///         file - определяем файл
///         bytes - массив байт данных
///         fileSize - размер файла
///         fileName - его название
///         blockData - размер данных
///         entryFolderName - папка для файлов извне
///     Методы:
///         readDataFromStream() - чтение данных с потока
///         writeDataFromStream() - запись данных в поток
///         processData() - обрабатывает приходящие данные
///         typeOfMessage() - возвращает строку тип менеджера
///         setEntryFolderName() - установка entryFolderName
///     Сигналы:
///         signalStatusRRManager() - отправка статуса ReadyReadManager'у
///         signalSendToOneRRManager() - отправка сообщения одному клиенту через ReadyReadManager

///  ========================    классы проекта
#include "../I_message_manager.h"   //  реализуем интерфейс
///  ========================
///
///  ========================    классы для работы
#include <QFile>                //  работа с файлами
#include <QDir>                 //  работа с директориями
///  ========================

class ServerFileManager : public I_MessageManager
{
    Q_OBJECT
public:
    ServerFileManager();
    void readDataFromStream(QDataStream &inStream) override;
    void writeDataToStream(QDataStream &outStream) override;
    void processData(QDataStream &inStream) override;
    QString typeOfMessage() override;
    void setEntryFolderName(QString &entryFolder);

protected:
    friend QDataStream &operator >> (QDataStream &in, ServerFileManager &serverFileManager);
    friend QDataStream &operator << (QDataStream &out, ServerFileManager &serverFileManager);

private:
    QFile *file;
    char *bytes = {0};
    int fileSize;
    QString fileName;
    int blockData = 1000000;
    QString entryFolderName;

signals:
    void signalStatusRRManager(QString status);
    void signalSendToServer(QString typeOfMsg, QString str);
};

#endif // SERVERFILEMANAGER_H

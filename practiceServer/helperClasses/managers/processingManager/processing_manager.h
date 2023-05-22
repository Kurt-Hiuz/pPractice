#ifndef PROCESSINGMANAGER_H
#define PROCESSINGMANAGER_H

///     Класс ProcessingManager определяет, какие файлы и какому клиенту нужно отправить
///     Если файл пока не может быть обработан, он отправится в папку ожидания (Data/Expectation)
///     Методы:
///         entryFiles() - принимает путь до папки, возвращая список файлов, упорядоченных по дате

///  ========================   классы для работы с файлами
#include <QFileInfoList>        //  для возврата списка файлов
#include <QDir>                 //  для работы с директориями
///  ========================
///
///  ========================   вспомогательные классы
#include <QDebug>               //  для вывода в консоль Qt
///  ========================

class ProcessingManager
{
public:
    ProcessingManager();

    QFileInfoList entryFiles(const QString &folderName);
};

#endif // PROCESSINGMANAGER_H

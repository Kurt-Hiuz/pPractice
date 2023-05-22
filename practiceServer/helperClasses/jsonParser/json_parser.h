#ifndef JSONPARSER_H
#define JSONPARSER_H

///     Класс JsonParser определяет, ликвидная ли json запись
///     Методы:
///         isJson() - возвращает булевое значение, парся данные в формате QByteArray

///  ========================    классы для работы с данными
#include <QJsonParseError>      //  класс, от которого наследуемся
#include <QJsonDocument>        //  работа с json документами
///  ========================

class JsonParser : public QJsonParseError
{
public:
    JsonParser();

    bool isJson(QByteArray &jsonData);
};

#endif // JSONPARSER_H

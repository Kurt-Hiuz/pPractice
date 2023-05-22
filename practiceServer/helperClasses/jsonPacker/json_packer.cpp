#include "json_packer.h"

JsonPacker::JsonPacker()
{

}

QJsonValue JsonPacker::getJsonVersionValue(I_CardFrame *item)
{
    //  метод item->getValue() возвращает QMap<QString, QVariant>, состоящий из одного элемента
    //  .first() возвращает первое значение (QVariant)

    //  переменная для исключения постоянного вызова методов при условии
    QString className = item->metaObject()->className();

    if(className.contains("ComboBox")){
        //  картеж из текста : данные
        m_settingsCardJsonValue = item->getValue().first().toJsonObject();

        return m_settingsCardJsonValue;
    }

    if(className.contains("SpinBox")){
        m_settingsCardJsonValue = item->getValue().first().toInt();

        return m_settingsCardJsonValue;
    }

    //  Если у нас ничего не подошло, то возвращаемое значение имеет формат строки
    m_settingsCardJsonValue = item->getValue().first().toString();

    return m_settingsCardJsonValue;
}

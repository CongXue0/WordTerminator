#include "json.h"
#include <QDebug>

JsonMap JsonMap::operator[](QString key)
{
    JsonMap map;
    map.m_value = this->m_value.toObject()[key];m_value.toArray();
    return map;
}

Json JsonMap::toJson()
{
    Json json;
    json.m_array = m_value.toArray();
    return json;
}

bool Json::read(QString path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly) == false)
        return false;
    QByteArray byte = file.readAll();
    file.close();
    m_array = QJsonDocument::fromJson(byte).array();
    return true;
}

Json Json::at(int i)
{
    Json json;
    json.m_array = this->m_array.at(i).toArray();
    return json;
}

JsonMap Json::operator[](QString key)
{
    JsonMap map;
    map.m_value = m_array.first().toObject()[key];
    return map;
}

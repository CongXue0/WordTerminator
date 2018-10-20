#ifndef JSON_H
#define JSON_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class Json;

class JsonMap
{
public:
    friend class Json;
    JsonMap operator[](QString key);
    bool toBool() const { return m_value.toBool(); }
    int toInt() const { return m_value.toInt(); }
    double toDouble() const { return m_value.toDouble(); }
    QString toString() const { return m_value.toString(); }
    Json toJson();

private:
    QJsonValue m_value;
};


class Json
{
public:
    friend class JsonMap;
    bool read(QString path);
    Json at(int i);
    JsonMap operator[](QString key);

signals:

public slots:

private:
    QJsonArray m_array;
};

#endif // JSON_H

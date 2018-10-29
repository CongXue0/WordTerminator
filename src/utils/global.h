#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <qglobal.h>
#include <QMutex>

class MemoryVar
{
public:
    MemoryVar() {}
    MemoryVar(QString value) : m_value(value) {}
    void setMemValue(const QString &value);
    void setMemValue(const quint32 &value);
    QString getMemValueStr();
    quint32 getMemValueInt();

private:
    QString m_value;
    QMutex m_mutex;

};

class XmlVar
{
public:
    friend class Global;
    XmlVar() {}
    XmlVar(QString xmlName, QString varValue) : m_xmlName(xmlName), m_varValue(varValue) {}
    void setValue(const QString &value);
    void setValue(const quint32 &value);
    QString getValueStr();
    quint32 getValueInt();

private:
    static quint32 m_flag;
    QString m_xmlName;
    QString m_varValue;

};

class Global
{
private:
    Global() {}

public:
    static void setXmlPath(const QString &path);
    static void load();
    static void reset();
    static void saveXML();

    static XmlVar m_curScript; //0-4
    static XmlVar m_script0;
    static XmlVar m_script1;
    static XmlVar m_script2;
    static XmlVar m_script3;
    static XmlVar m_script4;

private:


private:
    static QString m_path;

};

#endif // GLOBAL_H

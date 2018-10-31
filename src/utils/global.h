#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <qglobal.h>
#include <QMutex>

#define DECLINE_NUM 10
#define XMLVARNUM 18

class MemoryVar
{
public:
    MemoryVar() {}
    MemoryVar(QString value);
    void setMemValue(const QString &value);
    void setMemValue(const int &value);
    QString getMemValueStr();
    int getMemValueInt();

private:
    QString m_valueStr;
    int m_valueInt;
    QMutex m_mutex;
};

class XmlVar
{
public:
    friend class Global;
    XmlVar() {}
    XmlVar(QString xmlName, QString value);
    void setValue(const QString &value);
    void setValue(const int &value);
    QString getValueStr();
    int getValueInt();

private:
    static quint32 m_flag;
    QString m_xmlName;
    QString m_valueStr;
    int m_valueInt;

};

class Global
{
private:
    Global() {}

public:
    static void init(const QString &configPath);
    static void load();
    static void reset();
    static void saveXML();

    static XmlVar m_leastForeverTimes;//记忆时可以设为已记忆的最小次数
    static XmlVar m_memoryInterval;//刚记忆过的单词过多久可以再次记忆(分钟)
    static XmlVar m_decline_times[DECLINE_NUM];//单词过多久记忆次数下降一次的时间分布
    static XmlVar m_memoryLevel;//0 简单，1 困难
    static XmlVar m_range1Left;
    static XmlVar m_range1Right;
    static XmlVar m_range2Left;
    static XmlVar m_range2Right;
    static XmlVar m_range3Left;
    static XmlVar m_range3Right;
    static XmlVar m_range4Left;
    static XmlVar m_timesSet1;
    static XmlVar m_timesSet2;
    static XmlVar m_curScript; //0-4
    static XmlVar m_script0;
    static XmlVar m_script1;
    static XmlVar m_script2;
    static XmlVar m_script3;
    static XmlVar m_script4;

private:


private:
    static QString m_path;
    static QVector<XmlVar *> m_varList;

};

#endif // GLOBAL_H

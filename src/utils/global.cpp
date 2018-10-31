#include "global.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "wtool.h"

quint32 XmlVar::m_flag = 0;

XmlVar Global::m_leastForeverTimes;
XmlVar Global::m_memoryInterval;
XmlVar Global::m_decline_times[10];
XmlVar Global::m_memoryLevel;
XmlVar Global::m_range1Left;
XmlVar Global::m_range1Right;
XmlVar Global::m_range2Left;
XmlVar Global::m_range2Right;
XmlVar Global::m_range3Left;
XmlVar Global::m_range3Right;
XmlVar Global::m_range4Left;
XmlVar Global::m_timesSet1;
XmlVar Global::m_timesSet2;
XmlVar Global::m_curScript;
XmlVar Global::m_script0;
XmlVar Global::m_script1;
XmlVar Global::m_script2;
XmlVar Global::m_script3;
XmlVar Global::m_script4;

QString Global::m_path;
QVector<XmlVar *> Global::m_varList;

MemoryVar::MemoryVar(QString value)
{
    m_valueStr = value;
    m_valueInt = value.toInt();
}

void MemoryVar::setMemValue(const QString &value)
{
    m_mutex.lock();
    m_valueStr = value;
    m_valueInt = value.toInt();
    m_mutex.unlock();
}

void MemoryVar::setMemValue(const int &value)
{
    m_mutex.lock();
    m_valueStr = QString::number(value);
    m_valueInt = value;
    m_mutex.unlock();
}

QString MemoryVar::getMemValueStr()
{
    return m_valueStr;
}

int MemoryVar::getMemValueInt()
{
    return m_valueInt;
}

XmlVar::XmlVar(QString xmlName, QString value)
{
    m_xmlName = xmlName;
    m_valueStr = value;
    m_valueInt = value.toInt();
}

void XmlVar::setValue(const QString &value)
{
    if (m_valueStr != value)
    {
        m_flag++;
        m_valueStr = value;
        m_valueInt = value.toInt();
    }
}

void XmlVar::setValue(const int &value)
{
    if (m_valueStr != QString::number(value))
    {
        m_flag++;
        m_valueStr = QString::number(value);
        m_valueInt = value;
    }
}

QString XmlVar::getValueStr()
{
    return m_valueStr;
}

int XmlVar::getValueInt()
{
    return m_valueInt;
}

void Global::init(const QString &configPath)
{
    m_path = configPath;

    reset();

    m_varList.append(&m_leastForeverTimes);
    m_varList.append(&m_memoryInterval);
    m_varList.append(&m_decline_times[0]);
    m_varList.append(&m_decline_times[1]);
    m_varList.append(&m_decline_times[2]);
    m_varList.append(&m_decline_times[3]);
    m_varList.append(&m_decline_times[4]);
    m_varList.append(&m_decline_times[5]);
    m_varList.append(&m_decline_times[6]);
    m_varList.append(&m_decline_times[7]);
    m_varList.append(&m_decline_times[8]);
    m_varList.append(&m_decline_times[9]);
    m_varList.append(&m_memoryLevel);
    m_varList.append(&m_range1Left);
    m_varList.append(&m_range1Right);
    m_varList.append(&m_range2Left);
    m_varList.append(&m_range2Right);
    m_varList.append(&m_range3Left);
    m_varList.append(&m_range3Right);
    m_varList.append(&m_range4Left);
    m_varList.append(&m_timesSet1);
    m_varList.append(&m_timesSet2);
    m_varList.append(&m_curScript);
    m_varList.append(&m_script0);
    m_varList.append(&m_script1);
    m_varList.append(&m_script2);
    m_varList.append(&m_script3);
    m_varList.append(&m_script4);

    load();
}

void Global::load()
{
    QFile file(m_path);
    if (!file.exists())
    {
        DEBUG << m_path << " is not existed.";
        XmlVar::m_flag = 1;
        saveXML();
        return;
    }
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        DEBUG << "Error: Cannot read file " << m_path << ": " << file.errorString();
        return;
    }
    int count = 0;
    QXmlStreamReader xmlReader(&file);
    xmlReader.readNext();
    xmlReader.readNext();
    if (xmlReader.name() == "CONFIG")
    {
        xmlReader.readNext();
        xmlReader.readNext();
        while (!xmlReader.atEnd())
        {
            for (int i = 0; i < m_varList.count(); i++)
            {
                if (xmlReader.name() == m_varList[i]->m_xmlName)
                {
                    m_varList[i]->setValue(xmlReader.readElementText());
                    count++;
                    break;
                }
            }
            xmlReader.readNext();
            xmlReader.readNext();
        }
    }
    if (count != XMLVARNUM)
    {
        XmlVar::m_flag = 1;
        saveXML();
    }
    else
    {
        XmlVar::m_flag = 0;
    }
}

void Global::reset()
{
    m_leastForeverTimes = XmlVar("LEASTFOREVERTIMES", "1");
    m_memoryInterval = XmlVar("MEMORYINTERVAL", "360");
    m_decline_times[0] = XmlVar("DECLINETIMES0", "48");
    m_decline_times[1] = XmlVar("DECLINETIMES1", "52");
    m_decline_times[2] = XmlVar("DECLINETIMES2", "56");
    m_decline_times[3] = XmlVar("DECLINETIMES3", "60");
    m_decline_times[4] = XmlVar("DECLINETIMES4", "64");
    m_decline_times[5] = XmlVar("DECLINETIMES5", "72");
    m_decline_times[6] = XmlVar("DECLINETIMES6", "80");
    m_decline_times[7] = XmlVar("DECLINETIMES7", "88");
    m_decline_times[8] = XmlVar("DECLINETIMES8", "96");
    m_decline_times[9] = XmlVar("DECLINETIMES9", "104");
    m_memoryLevel = XmlVar("MEMORYLEVEL", "0");
    m_range1Left = XmlVar("RANGE1LEFT", "0");
    m_range1Right = XmlVar("RANGE1RIGHT", "5");
    m_range2Left = XmlVar("RANGE2LEFT", "6");
    m_range2Right = XmlVar("RANGE2RIGHT", "9");
    m_range3Left = XmlVar("RANGE3LEFT", "10");
    m_range3Right = XmlVar("RANGE3RIGHT", "99");
    m_range4Left = XmlVar("RANGE4LEFT", "100");
    m_timesSet1 = XmlVar("TIMESSET1", "10");
    m_timesSet2 = XmlVar("TIMESSET2", "100");
    m_curScript = XmlVar("CURSCRIPT", "0");
    m_script0 = XmlVar("SCRIPT0", "0,9,0,0;");
    m_script1 = XmlVar("SCRIPT1", "");
    m_script2 = XmlVar("SCRIPT2", "");
    m_script3 = XmlVar("SCRIPT3", "");
    m_script4 = XmlVar("SCRIPT4", "");
}

void Global::saveXML()
{
    if (XmlVar::m_flag == 0)
    {
        return;
    }

    QFile file(m_path);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("CONFIG");

    for (int i = 0; i < m_varList.count(); i++)
    {
        xmlWriter.writeTextElement(m_varList[i]->m_xmlName, m_varList[i]->m_valueStr);
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();

    XmlVar::m_flag = 0;
}

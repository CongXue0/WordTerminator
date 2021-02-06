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
XmlVar Global::m_timesSet3;
XmlVar Global::m_curScript;
XmlVar Global::m_script0;
XmlVar Global::m_script1;
XmlVar Global::m_script2;
XmlVar Global::m_script3;
XmlVar Global::m_script4;
XmlVar Global::m_groupName[21];
XmlVar Global::m_groupIndexLib;
XmlVar Global::m_groupIndexMemory;
XmlVar Global::m_singleMemoryNum;
XmlVar Global::m_autoSilderVolume;
XmlVar Global::m_sortType;
XmlVar Global::m_sortDescend;
XmlVar Global::m_pageVolume;

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
    m_varList.append(&m_timesSet3);
    m_varList.append(&m_curScript);
    m_varList.append(&m_script0);
    m_varList.append(&m_script1);
    m_varList.append(&m_script2);
    m_varList.append(&m_script3);
    m_varList.append(&m_script4);
    m_varList.append(&m_groupName[0]);
    m_varList.append(&m_groupName[1]);
    m_varList.append(&m_groupName[2]);
    m_varList.append(&m_groupName[3]);
    m_varList.append(&m_groupName[4]);
    m_varList.append(&m_groupName[5]);
    m_varList.append(&m_groupName[6]);
    m_varList.append(&m_groupName[7]);
    m_varList.append(&m_groupName[8]);
    m_varList.append(&m_groupName[9]);
    m_varList.append(&m_groupName[10]);
    m_varList.append(&m_groupName[11]);
    m_varList.append(&m_groupName[12]);
    m_varList.append(&m_groupName[13]);
    m_varList.append(&m_groupName[14]);
    m_varList.append(&m_groupName[15]);
    m_varList.append(&m_groupName[16]);
    m_varList.append(&m_groupName[17]);
    m_varList.append(&m_groupName[18]);
    m_varList.append(&m_groupName[19]);
    m_varList.append(&m_groupName[20]);
    m_varList.append(&m_groupIndexLib);
    m_varList.append(&m_groupIndexMemory);
    m_varList.append(&m_singleMemoryNum);
    m_varList.append(&m_autoSilderVolume);
    m_varList.append(&m_sortType);
    m_varList.append(&m_sortDescend);
    m_varList.append(&m_pageVolume);

    load();
}

void Global::load()
{
    QFile file(m_path);
    if (!file.exists())
    {
        DEBUG << m_path << " is not existed.";
        saveXML(true);
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
            for (int i = 0; i < m_varList.count(); ++i)
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
        saveXML(true);
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
    m_timesSet3 = XmlVar("TIMESSET3", "200");
    m_curScript = XmlVar("CURSCRIPT", "0");
    m_script0 = XmlVar("SCRIPT0", "0,9,0,0;");
    m_script1 = XmlVar("SCRIPT1", "");
    m_script2 = XmlVar("SCRIPT2", "");
    m_script3 = XmlVar("SCRIPT3", "");
    m_script4 = XmlVar("SCRIPT4", "");
    m_groupName[0] = XmlVar("GROUP0NAME", "default");
    m_groupName[1] = XmlVar("GROUP1NAME", "");
    m_groupName[2] = XmlVar("GROUP2NAME", "");
    m_groupName[3] = XmlVar("GROUP3NAME", "");
    m_groupName[4] = XmlVar("GROUP4NAME", "");
    m_groupName[5] = XmlVar("GROUP5NAME", "");
    m_groupName[6] = XmlVar("GROUP6NAME", "");
    m_groupName[7] = XmlVar("GROUP7NAME", "");
    m_groupName[8] = XmlVar("GROUP8NAME", "");
    m_groupName[9] = XmlVar("GROUP9NAME", "");
    m_groupName[10] = XmlVar("GROUP10NAME", "");
    m_groupName[11] = XmlVar("GROUP11NAME", "");
    m_groupName[12] = XmlVar("GROUP12NAME", "");
    m_groupName[13] = XmlVar("GROUP13NAME", "");
    m_groupName[14] = XmlVar("GROUP14NAME", "");
    m_groupName[15] = XmlVar("GROUP15NAME", "");
    m_groupName[16] = XmlVar("GROUP16NAME", "");
    m_groupName[17] = XmlVar("GROUP17NAME", "");
    m_groupName[18] = XmlVar("GROUP18NAME", "");
    m_groupName[19] = XmlVar("GROUP19NAME", "");
    m_groupName[20] = XmlVar("GROUP20NAME", "");
    m_groupIndexLib = XmlVar("GROUPINDEXLIB", "-1");
    m_groupIndexMemory = XmlVar("GROUPINDEXMEMORY", "-1");
    m_singleMemoryNum = XmlVar("SINGLEMEMORYNUM", "40");
    m_autoSilderVolume = XmlVar("AUTOSILDERVOLUME", "10");
    m_sortType = XmlVar("SORTTYPE", "0");
    m_sortDescend = XmlVar("SORTDESCEND", "0");
    m_pageVolume = XmlVar("PAGEVOLUME", "200");
}

bool Global::saveXML(bool force)
{
    if (XmlVar::m_flag == 0 && !force)
    {
        return false;
    }

    QFile file(m_path);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("CONFIG");

    for (int i = 0; i < m_varList.count(); ++i)
    {
        xmlWriter.writeTextElement(m_varList[i]->m_xmlName, m_varList[i]->m_valueStr);
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();

    XmlVar::m_flag = 0;
    return true;
}

#include "global.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "wtool.h"

quint32 XmlVar::m_flag = 0;

XmlVar Global::m_curScript;
XmlVar Global::m_script0;
XmlVar Global::m_script1;
XmlVar Global::m_script2;
XmlVar Global::m_script3;
XmlVar Global::m_script4;

QString Global::m_path;

void MemoryVar::setMemValue(const QString &value)
{
    m_mutex.lock();
    m_value = value;
    m_mutex.unlock();
}

void MemoryVar::setMemValue(const quint32 &value)
{
    m_mutex.lock();
    m_value = QString::number(value);
    m_mutex.unlock();
}

QString MemoryVar::getMemValueStr()
{
    return m_value;
}

quint32 MemoryVar::getMemValueInt()
{
    return m_value.toUInt();
}

void XmlVar::setValue(const QString &value)
{
    if (m_varValue != value)
    {
        m_flag++;
        m_varValue = value;
    }
}

void XmlVar::setValue(const quint32 &value)
{
    if (m_varValue != QString::number(value))
    {
        m_flag++;
        m_varValue = QString::number(value);
    }
}

QString XmlVar::getValueStr()
{
    return m_varValue;
}

quint32 XmlVar::getValueInt()
{
    return m_varValue.toUInt();
}

void Global::setXmlPath(const QString &path)
{
    m_path = path;
}

void Global::load()
{
    reset();

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
    DEBUG<<xmlReader.name();
    if (xmlReader.name() == "CONFIG")
    {
        xmlReader.readNext();
        xmlReader.readNext();
        while (!xmlReader.atEnd())
        {
            if (!xmlReader.name().isEmpty())
            {
                count++;
            }
            if (xmlReader.name() == "CURSCRIPT")
            {
                m_curScript.setValue(xmlReader.readElementText());
            }
            else if (xmlReader.name() == "SCRIPT0")
            {
                m_script0.setValue(xmlReader.readElementText());
            }
            else if (xmlReader.name() == "SCRIPT1")
            {
                m_script1.setValue(xmlReader.readElementText());
            }
            else if (xmlReader.name() == "SCRIPT2")
            {
                m_script2.setValue(xmlReader.readElementText());
            }
            else if (xmlReader.name() == "SCRIPT3")
            {
                m_script3.setValue(xmlReader.readElementText());
            }
            else if (xmlReader.name() == "SCRIPT4")
            {
                m_script4.setValue(xmlReader.readElementText());
            }
            xmlReader.readNext();
            xmlReader.readNext();
        }
    }
    if (count != 6)
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

    xmlWriter.writeTextElement(m_curScript.m_xmlName, m_curScript.m_varValue);
    xmlWriter.writeTextElement(m_script0.m_xmlName, m_script0.m_varValue);
    xmlWriter.writeTextElement(m_script1.m_xmlName, m_script1.m_varValue);
    xmlWriter.writeTextElement(m_script2.m_xmlName, m_script2.m_varValue);
    xmlWriter.writeTextElement(m_script3.m_xmlName, m_script3.m_varValue);
    xmlWriter.writeTextElement(m_script4.m_xmlName, m_script4.m_varValue);

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();

    XmlVar::m_flag = 0;
}

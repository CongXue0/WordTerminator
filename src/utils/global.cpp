#include "global.h"
#include <QCoreApplication>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "wtool.h"

quint32 XmlVar::m_flag = 0;

//XmlVar Global::m_var1;
//XmlVar Global::m_var2;
//XmlVar Global::m_var3;
//XmlVar Global::m_var4;
//XmlVar Global::m_var5;

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
//            if (xmlReader.name() == "VAR1")
//            {
//                m_var1.setValue(xmlReader.readElementText());
//            }
//            else if (xmlReader.name() == "VAR2")
//            {
//                m_var2.setValue(xmlReader.readElementText());
//            }
//            else if (xmlReader.name() == "VAR3")
//            {
//                m_var3.setValue(xmlReader.readElementText());
//            }
//            else if (xmlReader.name() == "VAR4")
//            {
//                m_var4.setValue(xmlReader.readElementText());
//            }
//            else if (xmlReader.name() == "VAR5")
//            {
//                m_var5.setValue(xmlReader.readElementText());
//            }
            xmlReader.readNext();
            xmlReader.readNext();
        }
    }
    XmlVar::m_flag = 0;
}

void Global::reset()
{
//    m_var1 = XmlVar("VAR1", "0");
//    m_var2 = XmlVar("VAR2", "0");
//    m_var3 = XmlVar("VAR3", "0");
//    m_var4 = XmlVar("VAR4", "0");
//    m_var5 = XmlVar("VAR5", "0");
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
//    xmlWriter.writeTextElement(m_var1.m_xmlName, m_var1.m_varValue);
//    xmlWriter.writeTextElement(m_var2.m_xmlName, m_var2.m_varValue);
//    xmlWriter.writeTextElement(m_var3.m_xmlName, m_var3.m_varValue);
//    xmlWriter.writeTextElement(m_var4.m_xmlName, m_var4.m_varValue);
//    xmlWriter.writeTextElement(m_var5.m_xmlName, m_var5.m_varValue);
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();

    XmlVar::m_flag = 0;
}

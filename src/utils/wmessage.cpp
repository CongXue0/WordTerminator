#include "wmessage.h"

WMessage::WMessage()
{
    m_list.clear();
}

WMessage::WMessage(QString info, QString value)
{
    m_list.clear();
    insertMessage(0, info, value);
}

WMessage::~WMessage()
{
    m_list.clear();
}

void WMessage::insertMessage(int index, QString info, QString value)
{
    m_list.insert(index * 2, info);
    m_list.insert(index * 2 + 1, value);
}

void WMessage::getMessage(int index, QString &info, QString &value)
{
    if (index >= getMessageNum())
    {
        info = "";
        value = "";
    }
    else
    {
        info = m_list.at(index * 2);
        value = m_list.at(index * 2 + 1);
    }
}

void WMessage::clearMessage()
{
    m_list.clear();
}

int WMessage::getMessageNum()
{
    return m_list.size() / 2;
}

#include "memorythread.h"
#include <QDebug>
#include "wtool.h"

extern WordAdmin *p_wordAdmin;

MemoryThread::MemoryThread(QObject *parent) : QThread(parent)
{
    m_isRun = true;
}

void MemoryThread::start(QThread::Priority priority)
{
    m_isRun = true;
    QThread::start(priority);
}

void MemoryThread::stop()
{
    m_isRun = false;
}

void MemoryThread::run()
{
    QDateTime startDateTime;
    while (m_isRun)
    {
        startDateTime = QDateTime::currentDateTime();
        m_infoList = p_wordAdmin->getWordCannotMemorizeWithoutTime(30);
        m_nameList.clear();
        for (int i = 0; i < m_infoList.size(); ++i)
            m_nameList.append(m_infoList.at(i).m_name);
        DEBUG << "memotyInfoListSize: " << m_infoList.size();
        while (m_isRun && startDateTime.secsTo(QDateTime::currentDateTime()) < 25 * 60)
        {
            if (m_infoList.size() > 0)
            {
                m_mutex.lock();
                for (int i = 0; m_isRun && i < m_infoList.size(); ++i)
                {
                    if (m_infoList.at(i).m_modifyTime.secsTo(QDateTime::currentDateTime()) >= WTool::getMemoryInterval())
                    {
                        emit wordCanMemorizeSignal(m_infoList.at(i).m_name);
                        m_nameList.removeAt(i);
                        m_infoList.removeAt(i);
                        i--;
                    }
                    msleep(10);
                }
                m_mutex.unlock();
            }
            WTool::sDelay(1, 100, &m_isRun);
        }
    }
}

void MemoryThread::slot_wordTimeIncrease(QString name)
{
    m_mutex.lock();
    if (m_nameList.contains(name))
    {
        int index = m_nameList.indexOf(name);
        m_nameList.removeAt(index);
        m_infoList.removeAt(index);
    }
    m_mutex.unlock();
}

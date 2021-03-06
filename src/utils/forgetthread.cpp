#include "forgetthread.h"
#include "wtool.h"
#include "dispatcher.h"
#include <QDateTime>
#include <QDebug>

extern WordAdmin *p_wordAdmin;

ForgetThread::ForgetThread(QObject *parent) : QThread(parent)
{
    m_isRun = true;
}

void ForgetThread::start(QThread::Priority priority)
{
    m_isRun = true;
    QThread::start(priority);
}

void ForgetThread::stop()
{
    m_isRun = false;
}

void ForgetThread::run()
{
    QDateTime startDateTime, curDateTime;
    while (m_isRun)
    {
        startDateTime = QDateTime::currentDateTime();
        m_infoList = p_wordAdmin->getWordListWithinTime(60 * 24, false);
        m_nameList.clear();
        for (int i = 0; i < m_infoList.size(); ++i)
            m_nameList.append(m_infoList.at(i).m_name);
        DEBUG << "forgetInfoListSize: " << m_infoList.size();
        while (m_isRun && startDateTime.secsTo(QDateTime::currentDateTime()) < 10 * 60)
        {
            if (m_infoList.size() > 0)
            {
                m_mutex.lock();
                curDateTime = QDateTime::currentDateTime();
                for (int i = 0; m_isRun && i < m_infoList.size(); ++i)
                {
                    if (WTool::timesCanDecline(m_infoList.at(i).m_times, m_infoList.at(i).m_modifyTime,
                        curDateTime))
                    {
                        WTool::memoryDecline(m_infoList[i].m_times, m_infoList[i].m_modifyTime, curDateTime);
                        m_infoList[i].m_remember = (m_infoList[i].m_remember > 0) ? 2 : -1;
                        if (p_wordAdmin->updateWord(m_infoList.at(i).m_name, "Times",
                            QString::number(m_infoList.at(i).m_times), "ModifyTime",
                            m_infoList.at(i).m_modifyTime.toString(TIMEFORMAT), "RememberState",
                            QString::number(m_infoList[i].m_remember)))
                        {
                            DEBUG << m_infoList.at(i).m_name << " forget times";
                            emit Dispatch(this).signal_wordTimeDecline(m_infoList.at(i).m_name);
                            m_nameList.removeAt(i);
                            m_infoList.removeAt(i);
                            i--;
                        }
                    }
                }
                m_mutex.unlock();
            }
            WTool::sDelay(60, 100, &m_isRun);
        }
    }
}

void ForgetThread::slot_wordTimeIncrease(QString name)
{
    DEBUG << name << " slot_wordTimeIncrease";
    m_mutex.lock();
    if (m_nameList.contains(name))
    {
        int index = m_nameList.indexOf(name);
        m_nameList.removeAt(index);
        m_infoList.removeAt(index);
    }
    m_mutex.unlock();
}

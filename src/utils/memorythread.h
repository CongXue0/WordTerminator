#ifndef MEMORYTHREAD_H
#define MEMORYTHREAD_H

#include <QThread>
#include "wordadmin.h"

class MemoryThread : public QThread
{
    Q_OBJECT
public:
    explicit MemoryThread(QObject *parent = nullptr);

public slots:
    void start(Priority priority = InheritPriority);
    void stop();

protected:
    void run();

protected slots:
    void slot_wordTimeIncrease(QString name);

protected:
    bool m_isRun;

    QList<BriefWordInfo> m_infoList;
    QStringList m_nameList;

    QMutex m_mutex;
};

#endif // MEMORYTHREAD_H

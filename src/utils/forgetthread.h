#ifndef FORGETTHREAD_H
#define FORGETTHREAD_H

#include <QThread>
#include "wordadmin.h"
#include <QMutex>

class ForgetThread : public QThread
{
    Q_OBJECT
public:
    explicit ForgetThread(QObject *parent = 0);

public slots:
    void start(Priority priority = InheritPriority);
    void stop();

private:
    void run();

signals:
    void wordTimeDeclineSignal(QString name);


private slots:
    void slot_wordTimeIncrease(QString name);


private:
    bool m_isRun;

    QList<BriefWordInfo> m_infoList;
    QStringList m_nameList;

    QMutex m_mutex;

};

#endif // FORGETTHREAD_H

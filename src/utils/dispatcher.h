#pragma once
#include <QObject>
#include "wmessage.h"

class Dispatcher : public QObject
{
	Q_OBJECT
public:
    static Dispatcher& getInstance();
    static void setSender(QObject *obj);
    static QObject *senderObj();

signals:
    void signal_sendMessage(WMessage message);
    void signal_wordTimeIncrease(QString name);
    void signal_wordTimeDecline(QString name);
    void signal_wordCanMemorize(QString name);
    void signal_stopWordMemorize(bool *ret);
    void signal_memorizeFinished();

protected:
	Dispatcher();
    static QObject *m_sender;
};
static auto Dispatch = [](QObject *obj) ->Dispatcher& { Dispatcher::setSender(obj); return Dispatcher::getInstance(); };

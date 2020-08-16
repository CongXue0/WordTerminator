#include "dispatcher.h"

QObject *Dispatcher::m_sender = nullptr;

Dispatcher& Dispatcher::getInstance()
{
	static Dispatcher global_instance;
    return global_instance;
}

void Dispatcher::setSender(QObject *obj)
{
    Dispatcher::m_sender = obj;
}

QObject *Dispatcher::senderObj()
{
    return Dispatcher::m_sender;
}

Dispatcher::Dispatcher() : QObject(nullptr)
{
    m_sender = nullptr;
}

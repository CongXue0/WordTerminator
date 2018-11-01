#ifndef WMESSAGE_H
#define WMESSAGE_H

#include <QString>
#include <QStringList>

class WMessage
{
public:
    WMessage();
    WMessage(QString info, QString value);
    ~WMessage();

    void insertMessage(const int & index, const QString &info, const QString &value);
    void getMessage(const int &index, QString &info, QString &value);
    void clearMessage();
    int getMessageNum();

private:
    QStringList m_list;
};

#endif // WMESSAGE_H

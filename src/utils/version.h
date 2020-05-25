#ifndef VERSION_H
#define VERSION_H

#include <QString>

class Version
{
public:
    Version() = delete;

    static QString getVersion();
};

#endif // VERSION_H

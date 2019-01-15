#include "version.h"

#define MAIN_NUM "1.0"

QString Version::getVersion()
{
    return QString("%1%2").arg("v").arg(MAIN_NUM);
}

#include "wordterminator.h"
#include <QApplication>
#include "wordadmin.h"
#include "wtool.h"
#include "forgetthread.h"
#include <QDebug>
#include "time.h"
#include "memorythread.h"

WordAdmin *p_wordAdmin;
ForgetThread *p_forgetThread;
MemoryThread *p_memThread;
WordTerminator *p_wordTerm;

int main(int argc, char *argv[])
{
    p_wordAdmin = NULL;
    p_forgetThread = NULL;
    p_memThread = NULL;
    p_wordTerm = NULL;

    qsrand(time(NULL));

    QApplication a(argc, argv);
    WordTerminator w;
    p_wordTerm = &w;
    w.show();

    return a.exec();
}

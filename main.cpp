#include "wordterminator.h"
#include "wordadmin.h"
#include "wtool.h"
#include "forgetthread.h"
#include "memorythread.h"
#include "global.h"
#include <QApplication>
#include <QDebug>
#include <time.h>
#include <QTextCodec>
#include <QDir>

WordAdmin *p_wordAdmin = nullptr;
ForgetThread *p_forgetThread = nullptr;
MemoryThread *p_memThread = nullptr;
WordTerminator *p_wordTerm = nullptr;

int main(int argc, char *argv[])
{
    p_wordAdmin = nullptr;
    p_forgetThread = nullptr;
    p_memThread = nullptr;
    p_wordTerm = nullptr;

    qsrand(time(nullptr));

    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    WTool::dirInit();
    Global::init(WTool::getConfigPath());

    p_wordAdmin = WordAdmin::getInstance();
    p_forgetThread = new ForgetThread();
    p_memThread = new MemoryThread();

    WordTerminator w;
    p_wordTerm = &w;
    w.show();

    return a.exec();
}

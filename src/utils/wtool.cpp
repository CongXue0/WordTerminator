#include "wtool.h"
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QDesktopWidget>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDateTime>
#include <QThread>
#include "json.h"
#include "wordadmin.h"

int WTool::leastForeverTimes = 0;
uint WTool::memoryInterval = 0;
uint WTool::declineArray[DECLINE_NUM] = {0};

extern WordAdmin *p_wordAdmin;

QRect WTool::getScreenGeometry()
{
    return QApplication::desktop()->screenGeometry();
}

QString WTool::getScreenSize()
{
//    return "15.6";
//    return "14";
    int height = getScreenGeometry().height();
    if (height >= 1000 && height <= 1100)
        return "15.6";
    else if (height > 1100)
        return "15.6";
    else if (height >= 700 && height <= 800)
        return "14";
    else
        return "14";
}

int WTool::divide(int a, int b)
{
    return a / b + ((a % b > 0) ? 1 : 0);
}

QString WTool::getWordDBFilePath()
{
    return APPPATH + QString("/user/db/word.db");
}

int WTool::rand(int a, int b)
{
    if (a == b)
        return a;
    return (qrand() % (b - a + 1)) + a;
}

QDateTime WTool::getCurDateTimeMinus(uint sec)
{
    return QDateTime::fromTime_t(QDateTime::currentDateTime().toTime_t() - sec);
}

bool WTool::readFileInfo(QString relPath, QString absPath, QString &info)
{
    QFile file(relPath);
    file.open(QFile::ReadOnly);
    if (file.isOpen() == true)
    {
        info = QLatin1String(file.readAll());
        file.close();
//        if (info.length() > 0 && QChar('\n') == info.at(info.length() - 1))
//        {
//            info = info.mid(0, info.length() - 1);//去除多余回车
//        }
        return true;
    }
    else
    {
        file.setFileName(absPath);
        file.open(QFile::ReadOnly);
        if (file.isOpen() == true)
        {
            info = QLatin1String(file.readAll());
            file.close();
//            if (info.length() > 0 && QChar('\n') == info.at(info.length() - 1))
//            {
//                info = info.mid(0, info.length() - 1);//去除多余回车
//            }
            return true;
        }
        else
        {
            info = "";
            return false;
        }
    }
}

int WTool::getFontLength(QFont font, QString txt)
{
    return QFontMetrics(font).width(txt);
}

int WTool::getTextLineNumber(QFont font, QString txt, int lineWidth)
{
    txt = txt.trimmed();
    int len = txt.length();
    if (len <= 1)
        return 1;
    int num = 1;
    int start = 0;
    for (int i = 1; i <= len; i++)
    {
        if (txt.at(i - 1) == '\n')
        {
            num++;
            start = i;
            continue;
        }
        int tmp = getFontLength(font, txt.mid(start, i - start));
        if (tmp > lineWidth)
        {
            num++;
            if (txt.at(i - 1) == ' ')
            {
                int j = skipChar(txt, i, ' ', true);
                if (j >= i)
                {
                    start = j;
                    i = start;
                }
            }
            else
            {
                int j = getWordBorder(txt, i, false);
                if (j >= 0)
                {
                    if (j == start)
                    {
                        start = i - 1;
                        i = start;
                    }
                    else
                    {
                        start = j;
                        i = start;
                    }
                }
            }
        }
    }
    return num;
}

int WTool::getWordBorder(QString txt, int start, bool leftToRight)
{
    int len = txt.length();
    if (len <= 0 || start < 0 || start > len || (start == 0 && leftToRight == false) ||
        (start == len && leftToRight == true))
        return -1;
    int k = -1;
    if (leftToRight == true)
    {
        for (int i = start; i < len; i++)
        {
            if (txt.at(i) == ' ')
            {
                k = i;
                break;
            }
            else if (i == len - 1)
                k = len;
        }
    }
    else
    {
        for (int i = start - 1; i >= 0; i--)
        {
            if (txt.at(i) == ' ')
            {
                k = i + 1;
                break;
            }
            else if (i == 0)
                k = 0;
        }
    }
    return k;
}

int WTool::skipChar(QString txt, int start, char ch, bool leftToRight)
{
    int len = txt.length();
    if (len <= 0 || start < 0 || start > len || (start == 0 && leftToRight == false) ||
        (start == len && leftToRight == true))
        return -1;
    int k = -1;
    if (leftToRight == true)
    {
        for (int i = start; i < len; i++)
        {
            if (txt.at(i) != ch)
            {
                k = i;
                break;
            }
            else if (i == len - 1)
            {
                k = -2;
            }
        }
    }
    else
    {
        for (int i = start - 1; i >= 0; i--)
        {
            if (txt.at(i) != ch)
            {
                k = i + 1;
                break;
            }
            else if (i == 0)
            {
                k = -2;
            }
        }
    }
    return k;
}

QString WTool::arrangeWord(QString word, char sp)
{
    QStringList list = word.split(sp);
    word.clear();
    for (int i = 0; i < list.size(); i++)
    {
        QString tmp = list.at(i);
        tmp = tmp.trimmed();
        if (tmp.isEmpty() == false)
        {
            word.append(tmp + "; ");
        }
    }
    if (word.isEmpty() == false && word.at(word.length() - 2) == ';')
        word = word.mid(0, word.length() - 2);
    return word;
}

bool WTool::isSpace(QString txt)
{
    if (txt.isEmpty() == true)
        return true;
    int len = txt.length();
    bool flag = true;
    for (int i = 0; i < len; i++)
    {
        if (txt.at(i) != ' ')
        {
            flag = false;
            break;
        }
    }
    return flag;
}

bool WTool::isLetter(QString txt, char fl)
{
    int len = txt.length();
    bool flag = true;
    QChar ch;
    for (int i = 0; i < len; i++)
    {
        ch = txt.at(i);
        if ( ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) == false  && ch != ' ' &&
            ch != '-' && ch != '\'' && ch != '/' && ch != fl)
        {
            flag = false;
            break;
        }
    }
    return flag;
}

bool WTool::isLanguage(QString txt, char fl)
{
    int len = txt.length();
    bool flag = true;
    QChar ch;
    for (int i = 0; i < len; i++)
    {
        ch = txt.at(i);
        if ( ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) == false  && isChineseChar(ch) == false &&
            ch != ' ' && ch != fl)
        {
            flag = false;
            break;
        }
    }
    return flag;
}

bool WTool::isEnglishSentence(QString txt, char fl)
{
    int len = txt.length();
    bool flag = true;
    QChar ch;
    for (int i = 0; i < len; i++)
    {
        ch = txt.at(i);
        if ( ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) == false  && isChineseChar(ch) == false &&
            ch != ' ' && ch != fl && ch != ',' && ch != '.' && ch != '!' && ch != '?' && ch != '\'' && ch != '(' &&
            ch != ')' && ch != '/' && ch != '-' && ch != '~' && ch != '|' && ch != '=' && ch != '+' && ch != '?' &&
            ch != '#' && ch != '$' && ch != '*' && ch != ';' && ch != '[' && ch != ']' && ch != '\\' && ch != '{' &&
            ch != '}' && ch != '&' && ch != '@' && ch != '^' && ch != '.' && ch != ':' && ch != '\"')
        {
            flag = false;
            break;
        }
    }
    return flag;
}

bool WTool::isWritting(QString txt)
{
    int len = txt.length();
    bool flag = true;
    QChar ch;
    for (int i = 0; i < len; i++)
    {
        ch = txt.at(i);
        if (((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) == false  && isChineseChar(ch) == false &&
            (ch >= '0' && ch <= '9') == false && ch != ' ' && ch != ';' && ch != '?' && ch != '.' && ch != ',' &&
            ch != '<' && ch != '>' && ch != ':' && ch != '\'' && ch != '[' && ch != ']' && ch != '{' && ch != '}' &&
            ch != '=' && ch != '+' && ch != '-' && ch != '*' && ch != '&' && ch != '%' && ch != '$' && ch != '#' &&
            ch != '@' && ch != '!' && ch != '？' && ch != '！' && ch != '。' && ch != '，' && ch != '：' && ch != '、' &&
            ch != '（' && ch != '）' && ch != '·' && ch != '~' && ch != '`' && ch != '\n')
        {
            flag = false;
            break;
        }
    }
    return flag;
}

bool WTool::isChineseChar(QChar ch)
{
    ushort uni = ch.unicode();
    if (uni >= 0x4E00 && uni <= 0x9FA5)
        return true;
    else
        return false;
}

//bool WTool::textFrontCompare(QString txt, QString front)
//{
//    if (front.isEmpty() == true)
//        return true;
//    if (txt.length() < front.length())
//        return false;
//    return txt.mid(0, front.length()) == front;
//}

//bool WTool::textEndCompare(QString txt, QString end)
//{
//    if (end.isEmpty() == true)
//        return true;
//    if (txt.length() < end.length())
//        return false;
//    return txt.mid(txt.length() - end.length(), end.length()) == end;
//}

QStringList WTool::filterWordFromList(QStringList list, QString txt, QString strategy)
{
    int count = list.size();
    if (count == 0 || txt.isEmpty() == true)
        return list;
    QStringList wordList;
    QString tmp;
    if (strategy == "prefix")
    {
        for (int i = 0; i < count; i++)
        {
            tmp = list.at(i);
//            if (textFrontCompare(tmp, txt) == true)
            if (tmp.startsWith(txt) == true)
                wordList.append(tmp);
        }
    }
    else if (strategy == "suffix")
    {
        for (int i = 0; i < count; i++)
        {
            tmp = list.at(i);
//            if (textEndCompare(tmp, txt) == true)
            if (tmp.endsWith(txt) == true)
                wordList.append(tmp);
        }
    }
    else if (strategy == "contain")
    {
        for (int i = 0; i < count; i++)
        {
            tmp = list.at(i);
            if (tmp.contains(txt) == true)
                wordList.append(tmp);
        }
    }
    else if (strategy == "interpretation")
    {
        WordInfo wordInfo;
        for (int i = 0; i < count; i++)
        {
            tmp = list.at(i);
            p_wordAdmin->getWordInfo(tmp, &wordInfo);
            if (wordInfo.contains(txt) == true)
                wordList.append(tmp);
        }
    }
    return wordList;
}

QString WTool::shieldWord(QString txt, QString word)
{
    if (txt.isEmpty() || word.isEmpty() || txt.length() < word.length())
        return txt;
    if (txt.contains(word) == false)
        return txt;
    QString tmp;
    for (int i = 0; i < word.length(); i++)
        tmp.append("_");
    txt.replace(word, tmp);
    return txt;
}

void WTool::memoryConfigInit()
{
    QString path = APPPATH + "/user/config/memory.json";
    Json json;
    if (json.read(path) == false)
    {
        qDebug() << "memoryConfig fail";
        return;
    }

    leastForeverTimes = json["leastForeverTimes"].toInt();

    memoryInterval = json["memoryInterval"].toInt();

    JsonMap tmp = json["decline"];
    for (int i = 0; i < 10; i++)
    {
        declineArray[i] = tmp[QString::number(i + 1)].toInt();
        qDebug() << declineArray[i];
    }
    qDebug() << "memoryConfig success";
}

uint WTool::getDeclinePeriod(int times)
{
    if (times <= 0)
        times = 1;
    if (times <= DECLINE_NUM)
        return declineArray[times - 1];
    else
        return declineArray[DECLINE_NUM - 1] + 24 * (times - DECLINE_NUM);
}

bool WTool::memoryDecline(int &times, QDateTime &start, QDateTime end)
{
    if (times == 0 || start > end)
        return false;
    uint sec = end.toTime_t() - start.toTime_t();
    uint tmp1 = times, tmp2 = sec;
    uint t = getDeclinePeriod(times) * 3600;
    while (sec >= t)
    {
        times--;
        sec -= t;
        if (times == 0)
            break;
        t = getDeclinePeriod(times) * 3600;
    }
    if (tmp1 != times)
    {
        start = QDateTime::fromTime_t(start.toTime_t() + tmp2 - sec);
        return true;
    }
    else
        return false;
}

bool WTool::timesCanDecline(int times, QDateTime start, QDateTime end)
{
    if (times == 0 || start > end)
        return false;
    uint sec = end.toTime_t() - start.toTime_t();
    uint period = getDeclinePeriod(times) * 3600;
    if (sec >= period)
        return true;
    else
        return false;
}

int WTool::getLeastForeverTimes()
{
    return leastForeverTimes;
}

uint WTool::getMemoryInterval()//second
{
    return memoryInterval * 60;
}

void WTool::mDelay(uint msec, uint period, bool *isRun)
{
    QDateTime start = QDateTime::currentDateTime();
    if (isRun != NULL)
    {
        while (*isRun == true && start.msecsTo(QDateTime::currentDateTime()) < msec)
            QThread::msleep(period);
    }
    else
    {
        while (start.msecsTo(QDateTime::currentDateTime()) < msec)
            QThread::msleep(period);
    }
}

void WTool::sDelay(uint sec, uint period, bool *isRun)
{
    QDateTime start = QDateTime::currentDateTime();
    if (isRun != NULL)
    {
        while (*isRun == true && start.msecsTo(QDateTime::currentDateTime()) < sec * 1000)
            QThread::msleep(period);
    }
    else
    {
        while (start.msecsTo(QDateTime::currentDateTime()) < sec * 1000)
            QThread::msleep(period);
    }
}

QString WTool::getWordTerminatorQss()
{
    QString info;
    QString screenSize = getScreenSize();
    QString relPath = QString("skin/qss/%1/WordTerminator.qss").arg(screenSize);
    QString absPath = APPPATH + QString("/skin/qss/%1/WordTerminator.qss").arg(screenSize);
    readFileInfo(relPath, absPath, info);
    return info;
}

QString WTool::getWordLibraryWidgetQss()
{
    QString info;
    QString screenSize = getScreenSize();
    QString relPath = QString("skin/qss/%1/WordLibraryWidget.qss").arg(screenSize);
    QString absPath = APPPATH + QString("/skin/qss/%1/WordLibraryWidget.qss").arg(screenSize);
    readFileInfo(relPath, absPath, info);
    return info;
}

QString WTool::getWordCreateWidgetQss()
{
    QString info;
    QString screenSize = getScreenSize();
    QString relPath = QString("skin/qss/%1/WordCreateWidget.qss").arg(screenSize);
    QString absPath = APPPATH + QString("/skin/qss/%1/WordCreateWidget.qss").arg(screenSize);
    readFileInfo(relPath, absPath, info);
    return info;
}

QString WTool::getWordShowWidgetQss()
{
    QString info;
    QString screenSize = getScreenSize();
    QString relPath = QString("skin/qss/%1/WordShowWidget.qss").arg(screenSize);
    QString absPath = APPPATH + QString("/skin/qss/%1/WordShowWidget.qss").arg(screenSize);
    readFileInfo(relPath, absPath, info);
    return info;
}

QString WTool::getWordMemorizeWidgetQss()
{
    QString info;
    QString screenSize = getScreenSize();
    QString relPath = QString("skin/qss/%1/WordMemorizeWidget.qss").arg(screenSize);
    QString absPath = APPPATH + QString("/skin/qss/%1/WordMemorizeWidget.qss").arg(screenSize);
    readFileInfo(relPath, absPath, info);
    return info;
}

QString WTool::getWordMemorizeWidgetJsonPath()
{
    QString screenSize = getScreenSize();
    QString path = APPPATH + QString("/skin/qss/%1/WordMemorizeWidget.json").arg(screenSize);
    return path;
}

QString WTool::getConfigPath()
{
    return APPPATH + "/user/config/config.xml";
}

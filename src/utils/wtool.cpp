#include "wtool.h"
#include "json.h"
#include "wordadmin.h"
#include "global.h"
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QDesktopWidget>
#include <QSqlDatabase>
#include <QDateTime>
#include <QThread>
#include <QTextCodec>
#include <QQueue>
#include <QScreen>
#include <QPropertyAnimation>
#include <QPushButton>

QWidget *WTool::topWidget_ = nullptr;
QString WTool::m_uniStr = QString(QString("？！。，：、（）·").unicode());

extern WordAdmin *p_wordAdmin;

void WTool::dirInit()
{
    WTool::makeDir("./user");
    WTool::makeDir("./user/config");
    WTool::makeDir("./user/db");
}

QRect WTool::getScreenGeometry()
{
    return QGuiApplication::screens().first()->geometry();
//    return QApplication::desktop()->screenGeometry();
}

int WTool::divide(int a, int b)
{
    return a / b + ((a % b > 0) ? 1 : 0);
}

QString WTool::getWordDBFilePath()
{
    return QString("./user/db/word.db");
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

QStringList WTool::getGroupList()
{
    QStringList list;
    for (int i = 0; i < 21; ++i)
    {
        if (!Global::m_groupName[i].getValueStr().isEmpty())
        {
            list.append(Global::m_groupName[i].getValueStr());
        }
    }
    return list;
}

int WTool::getGroupNo(QString groupName)
{
    if (groupName == ALL_GROUP)
    {
        return -1;
    }
    for (int i = 0; i < 21; ++i)
    {
        if (Global::m_groupName[i].getValueStr() == groupName)
        {
            return i;
        }
    }
    return -1;
}

QString WTool::getGroupName(int groupId)
{
    if (groupId == -1)
    {
        return ALL_GROUP;
    }
    if (groupId >= 0 && groupId < 21)
    {
        return Global::m_groupName[groupId].getValueStr();
    }
    return "";
}

QString WTool::readFileInfo(QString path)
{
    QString info;
    QFile file(path);
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        info = QLatin1String(file.readAll());
        file.close();
        if (info.length() > 0 && QChar('\n') == info.at(info.length() - 1))
        {
            info = info.mid(0, info.length() - 1);
        }
    }
    return info;
}

void WTool::appendFileInfo(QString path, QString info)
{
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        if (!info.isEmpty())
        {
            QTextStream in(&file);
            in.setCodec(QTextCodec::codecForName("utf8"));
            in << info;
        }
        file.close();
    }
}

void WTool::writeFileInfo(QString path, QString info)
{
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        if (!info.isEmpty())
        {
            QTextStream in(&file);
            in.setCodec(QTextCodec::codecForName("utf8"));
            in << info;
        }
        file.close();
    }
}

bool WTool::makePath(QString path)
{
    QDir dir;
    if (!dir.exists(path))
    {
        return dir.mkpath(path);
    }
    return true;
}

bool WTool::makeDir(QString path)
{
    QDir dir;
    if (!dir.exists(path))
    {
        return dir.mkdir(path);
    }
    return true;
}

void WTool::messageBox(const QString &text, const QString &title)
{
    QMessageBox mbox(QMessageBox::Information, title, text, QMessageBox::Yes, topWidget_);
    mbox.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    mbox.exec();
}

void WTool::opacityMessageBox(const QString &text, const QString &title, double opacity, int msecs)
{
    QMessageBox *mbox = new QMessageBox(QMessageBox::Information, title, text, QMessageBox::Yes, topWidget_);
    mbox->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    if (auto *btnYes = mbox->button(QMessageBox::Yes))
    {
        btnYes->hide();
    }
    mbox->show();

    QPropertyAnimation *pAnimation = new QPropertyAnimation(mbox, "windowOpacity");
    pAnimation->setDuration(msecs);
    pAnimation->setKeyValueAt(0, opacity);
    pAnimation->setKeyValueAt(1, 0);
    pAnimation->setEasingCurve(QEasingCurve::InCubic);
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QObject::connect(pAnimation, &QAbstractAnimation::finished, mbox, &QObject::deleteLater);
}

void WTool::warningBox(const QString &text, const QString &title)
{
    QMessageBox mbox(QMessageBox::Warning, title, text, QMessageBox::Yes, topWidget_);
    mbox.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    mbox.exec();
}

void WTool::opacityWarningBox(const QString &text, const QString &title, double opacity, int msecs)
{
    QMessageBox *mbox = new QMessageBox(QMessageBox::Warning, title, text, QMessageBox::Yes, topWidget_);
    mbox->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    if (auto *btnYes = mbox->button(QMessageBox::Yes))
    {
        btnYes->hide();
    }
    mbox->show();

    QPropertyAnimation *pAnimation = new QPropertyAnimation(mbox, "windowOpacity");
    pAnimation->setDuration(msecs);
    pAnimation->setKeyValueAt(0, opacity);
    pAnimation->setKeyValueAt(1, 0);
    pAnimation->setEasingCurve(QEasingCurve::InCubic);
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QObject::connect(pAnimation, &QAbstractAnimation::finished, mbox, &QObject::deleteLater);
}

void WTool::errorBox(const QString &text, const QString &title)
{
    QMessageBox mbox(QMessageBox::Critical, title, text, QMessageBox::Yes, topWidget_);
    mbox.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    mbox.exec();
}

void WTool::opacityErrorBox(const QString &text, const QString &title, double opacity, int msecs)
{
    QMessageBox *mbox = new QMessageBox(QMessageBox::Critical, title, text, QMessageBox::Yes, topWidget_);
    mbox->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    if (auto *btnYes = mbox->button(QMessageBox::Yes))
    {
        btnYes->hide();
    }
    mbox->show();

    QPropertyAnimation *pAnimation = new QPropertyAnimation(mbox, "windowOpacity");
    pAnimation->setDuration(msecs);
    pAnimation->setKeyValueAt(0, opacity);
    pAnimation->setKeyValueAt(1, 0);
    pAnimation->setEasingCurve(QEasingCurve::InCubic);
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QObject::connect(pAnimation, &QAbstractAnimation::finished, mbox, &QObject::deleteLater);
}

int WTool::questionBox(const QString &text, const QString &title)
{
    QMessageBox qbox(QMessageBox::Question, title, text, QMessageBox::Yes | QMessageBox::No, topWidget_);
    qbox.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    return qbox.exec();
}

int WTool::questionBoxAll(const QString &text, const QString &title)
{
    QMessageBox qbox(QMessageBox::Question, title, text, QMessageBox::YesToAll | QMessageBox::NoToAll |QMessageBox::Yes | QMessageBox::No, topWidget_);
    qbox.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    return qbox.exec();
}

QSize WTool::getFontSize(const QFont &font, const QString &txt)
{
    return QFontMetrics(font).boundingRect(txt).size();
}

int WTool::getFontHeight(const QFont &font)
{
    return QFontMetrics(font).height();
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
    for (int i = 1; i <= len; ++i)
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

void WTool::clearLayout(QBoxLayout *layout, bool delete_view)
{
    if (!layout)
                return;
    for (QLayoutItem *child; (child = layout->takeAt(0)) != nullptr; )
    {
        if (child->widget() && delete_view)
            delete child->widget();
        if (child->layout())
        {
            WTool::clearLayout(dynamic_cast<QBoxLayout *>(child->layout()), delete_view);
        }
        delete child;
    }
}

//void WTool::reLayout(QBoxLayout *layout)
//{
//    if (!layout) return;
//    QQueue<std::tuple<QLayoutItem *, int>> items;
//    while (layout->count() > 0)
//    {
//        int st = layout->stretch(0);
//        items.enqueue(std::make_tuple(layout->takeAt(0), st));
//    }

//    int i = 0;
//    while (!items.isEmpty())
//    {
//        auto t = items.dequeue();
//        auto item_t = std::get<0>(t);
//        int st = std::get<1>(t);
//        layout->addItem(item_t);
//        layout->setStretch(i++, st);
//    }

//    for (i = 0; i < layout->count(); ++i)
//    {
//        auto item = layout->itemAt(i);
//        if (item->layout())
//        {
//            WTool::reLayout(dynamic_cast<QBoxLayout *>(item->layout()));
//        }
//        else if (item->widget())
//        {
//            WTool::reLayout(item->widget());
//        }
//    }
//}

//void WTool::reLayout(QWidget *w)
//{
//    if (!w) return;
//    if (w->metaObject()->className() != QString("QWidget")) return;
//    if (auto layout = dynamic_cast<QVBoxLayout *>(w->layout()))
//    {
//        QQueue<std::tuple<QLayoutItem *, int>> items;
//        while (layout->count() > 0)
//        {
//            int st = layout->stretch(0);
//            items.enqueue(std::make_tuple(w->layout()->takeAt(0), st));
//        }
//        QString name = layout->objectName();
//        auto marg = layout->contentsMargins();
//        int sp = layout->spacing();

//        delete layout;
//        layout = new QVBoxLayout(w);
//        layout->setObjectName(name);
//        layout->setContentsMargins(marg);
//        layout->setSpacing(sp);

//        int i = 0;
//        while (!items.isEmpty())
//        {
//            auto t = items.dequeue();
//            auto item_t = std::get<0>(t);
//            int st = std::get<1>(t);
//            layout->addItem(item_t);
//            layout->setStretch(i++, st);
//        }

//        for (i = 0; i < layout->count(); ++i)
//        {
//            auto item = layout->itemAt(i);
//            if (item->layout())
//            {
//                WTool::reLayout(dynamic_cast<QBoxLayout *>(item->layout()));
//            }
//            else if (item->widget())
//            {
//                WTool::reLayout(item->widget());
//            }
//        }
//        return;
//    }

//    if (auto layout = dynamic_cast<QHBoxLayout *>(w->layout()))
//    {
//        QQueue<std::tuple<QLayoutItem *, int>> items;
//        while (layout->count() > 0)
//        {
//            int st = layout->stretch(0);
//            items.enqueue(std::make_tuple(w->layout()->takeAt(0), st));
//        }
//        QString name = layout->objectName();
//        auto marg = layout->contentsMargins();
//        int sp = layout->spacing();

//        delete layout;
//        layout = new QHBoxLayout(w);
//        layout->setObjectName(name);
//        layout->setContentsMargins(marg);
//        layout->setSpacing(sp);

//        int i = 0;
//        while (!items.isEmpty())
//        {
//            auto t = items.dequeue();
//            auto item_t = std::get<0>(t);
//            int st = std::get<1>(t);
//            layout->addItem(item_t);
//            layout->setStretch(i++, st);
//        }

//        for (i = 0; i < layout->count(); ++i)
//        {
//            auto item = layout->itemAt(i);
//            if (item->layout())
//            {
//                WTool::reLayout(dynamic_cast<QBoxLayout *>(item->layout()));
//            }
//            else if (item->widget())
//            {
//                WTool::reLayout(item->widget());
//            }
//        }
//        return;
//    }
//}

void WTool::LayoutHelper_init(QBoxLayout *layout)
{
    if (!layout) return;
    layout->setEnabled(false);
    for (int i = 0; i < layout->count(); ++i)
    {
        auto item = layout->itemAt(i);
        if (item->layout())
        {
            WTool::LayoutHelper_init(dynamic_cast<QBoxLayout *>(item->layout()));
        }
        else if (item->widget())
        {
            if (item->widget()->metaObject()->className() == QString("QWidget"))
                WTool::LayoutHelper_init(dynamic_cast<QBoxLayout *>(item->widget()->layout()));
        }
    }
}

//void WTool::LayoutHelper_reLayout(QBoxLayout *layout, int level)
//{
//}

//void WTool::LayoutHelper_reLayout(QWidget *w, int level)
//{
//    if (!w) return;
//    if (!w->isVisible()) return;
//    if (!w->layout()) return;
//    if (w->layout()->isEnabled()) return;
//    if (auto vly = dynamic_cast<QVBoxLayout *>(w->layout()))
//    {
//        auto m = vly->contentsMargins();
//        int sp = vly->spacing();
//        int width = w->size().width(), height = w->size().height();
//        int w = width - m.left() - m.right(), h = height - m.top() - m.bottom();
//        int x = m.left(), y = m.top();
//        for (int i = 0; i < vly->count(); ++i)
//        {
//        }
//        return;
//    }
//    if (auto hly = dynamic_cast<QHBoxLayout *>(w->layout()))
//    {
//        auto m = hly->contentsMargins();
//        int sp = hly->spacing();
//        int height = w->size().height();
//        int h = height - m.top() - m.bottom();
//        return;
//    }
//}

int WTool::getWordBorder(QString txt, int start, bool leftToRight)
{
    int len = txt.length();
    if (len <= 0 || start < 0 || start > len || (start == 0 && !leftToRight) || (start == len && leftToRight))
        return -1;
    int k = -1;
    if (leftToRight)
    {
        for (int i = start; i < len; ++i)
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
    if (len <= 0 || start < 0 || start > len || (start == 0 && !leftToRight) || (start == len && leftToRight))
        return -1;
    int k = -1;
    if (leftToRight)
    {
        for (int i = start; i < len; ++i)
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
    for (int i = 0; i < list.size(); ++i)
    {
        QString tmp = list.at(i);
        tmp = tmp.trimmed();
        if (!tmp.isEmpty())
        {
            word.append(tmp + "; ");
        }
    }
    if (!word.isEmpty() && word.at(word.length() - 2) == ';')
        word = word.mid(0, word.length() - 2);
    return word;
}

bool WTool::isSpace(QString txt)
{
    if (txt.isEmpty())
        return true;
    int len = txt.length();
    bool flag = true;
    for (int i = 0; i < len; ++i)
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
    for (int i = 0; i < len; ++i)
    {
        ch = txt.at(i);
        if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) && ch != ' ' && ch != '-' && ch != '\'' && ch != '/' && ch != fl)
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
    for (int i = 0; i < len; ++i)
    {
        ch = txt.at(i);
        if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) && !isChineseChar(ch) && ch != ' ' && ch != fl)
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
    for (int i = 0; i < len; ++i)
    {
        ch = txt.at(i);
        if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) && !isChineseChar(ch) &&
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
    QString txtTmp = QString(txt.unicode());
    int len = txtTmp.length();
    bool flag = true;
    QChar ch;
    for (int i = 0; i < len; ++i)
    {
        ch = txtTmp.at(i);
        if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))  && !isChineseChar(ch) && !(ch >= '0' && ch <= '9') && ch != ' ' && ch != ';' && ch != '?' && ch != '.' && ch != ',' && ch != '<' && ch != '>' && ch != ':' && ch != '\'' && ch != '[' && ch != ']' && ch != '{' && ch != '}' && ch != '=' && ch != '+' && ch != '-' && ch != '*' && ch != '&' && ch != '%' && ch != '$' && ch != '#' && ch != '@' && ch != '!'  && ch != '~' && ch != '`' && ch != '\n' &&
            ch != m_uniStr.at(0) && ch != m_uniStr.at(1) && ch != m_uniStr.at(2) && ch != m_uniStr.at(3) && ch != m_uniStr.at(4) && ch != m_uniStr.at(5) && ch != m_uniStr.at(6) && ch != m_uniStr.at(7) && ch != m_uniStr.at(8))
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

QStringList WTool::filterWordFromList(QStringList list, QString txt, QString strategy)
{
    int count = list.size();
    if (count == 0 || txt.isEmpty())
        return list;
    QStringList wordList;
    QString tmp;
    if (strategy == "prefix")
    {
        for (int i = 0; i < count; ++i)
        {
            tmp = list.at(i);
            if (tmp.startsWith(txt))
                wordList.append(tmp);
        }
    }
    else if (strategy == "suffix")
    {
        for (int i = 0; i < count; ++i)
        {
            tmp = list.at(i);
            if (tmp.endsWith(txt))
                wordList.append(tmp);
        }
    }
    else if (strategy == "contain")
    {
        for (int i = 0; i < count; ++i)
        {
            tmp = list.at(i);
            if (tmp.contains(txt))
                wordList.append(tmp);
        }
    }
    else if (strategy == "interpretation")
    {
        WordInfo wordInfo;
        for (int i = 0; i < count; ++i)
        {
            tmp = list.at(i);
            p_wordAdmin->getWordInfo(tmp, &wordInfo);
            if (wordInfo.contains(txt))
                wordList.append(tmp);
        }
    }
    return wordList;
}

QString WTool::shieldWord(QString txt, QString word)
{
    if (txt.isEmpty() || word.isEmpty() || txt.length() < word.length())
        return txt;
    if (!txt.contains(word))
        return txt;
    QString tmp;
    for (int i = 0; i < word.length(); ++i)
        tmp.append("_");
    txt.replace(word, tmp);
    return txt;
}

uint WTool::getDeclinePeriod(int times)
{
    if (times <= 0)
        times = 1;
    if (times <= DECLINE_NUM)
        return uint(Global::m_decline_times[times - 1].getValueInt());
    else
        return uint(Global::m_decline_times[DECLINE_NUM - 1].getValueInt() + 24 * (times - DECLINE_NUM));
}

bool WTool::memoryDecline(int &times, QDateTime &start, QDateTime end)
{
    if (times == 0 || start > end)
        return false;
    uint sec = end.toTime_t() - start.toTime_t();
    uint tmp1 = uint(times), tmp2 = sec;
    uint t = getDeclinePeriod(times) * 3600;
    while (sec >= t)
    {
        times--;
        sec -= t;
        if (times == 0)
            break;
        t = getDeclinePeriod(times) * 3600;
    }
    if (static_cast<int>(tmp1) != times)
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
uint WTool::getMemoryInterval()//second

{
    return uint(Global::m_memoryInterval.getValueInt() * 60);
}

void WTool::mDelay(uint msec, uint period, bool *isRun)
{
    QDateTime start = QDateTime::currentDateTime();
    if (isRun != nullptr)
    {
        while (*isRun && start.msecsTo(QDateTime::currentDateTime()) < msec)
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
    if (isRun != nullptr)
    {
        while (*isRun && start.msecsTo(QDateTime::currentDateTime()) < sec * 1000)
            QThread::msleep(period);
    }
    else
    {
        while (start.msecsTo(QDateTime::currentDateTime()) < sec * 1000)
            QThread::msleep(period);
    }
}

QString WTool::getStyleQss(QString className)
{
    return readFileInfo(QString(":/main/res/skin/qss/%1.qss").arg(className));
}

QString WTool::getConfigPath()
{
    return "./user/config/config.xml";
}

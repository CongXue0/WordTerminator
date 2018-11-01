#ifndef WTOOL_H
#define WTOOL_H
#include <QString>
#include <QRect>
#include <QFont>
#include <QDebug>

#define BOOLSTR(a) ((a == true) ? "true" : "false")
#define TIMEFORMAT ("yyyy-MM-dd hh:mm:ss")
#define DEBUG qDebug() << QString("%1 %2:%3 ").arg(QString(__FILE__).split('\\').last()).arg(__FUNCTION__).arg(__LINE__)

#define ALL_GROUP ("all group")

typedef struct
{
    int m_groupId;
    QString m_groupName;
} GROUPINFO;

class WTool
{
public:
    /* 其他 */
    static void dirInit();//初始化文件夹
    static QRect getScreenGeometry();
    static QString getScreenSize();
    static int divide(const int &a, const int &b);
    static QString getWordDBFilePath();
    static int rand(const int &a, const int &b);//得到一个 [a, b] 范围内的随机数
    static QDateTime getCurDateTimeMinus(uint sec);

    /* 组相关 */
    static QStringList getGroupList();
    static int getGroupNo(const QString &groupName);

    /* 文件操作 */
    static QString readFileInfo(const QString &path);
    static void appendFileInfo(const QString &path, const QString &info);
    static void writeFileInfo(const QString &path, const QString &info);
    static bool makePath(const QString &path);
    static bool makeDir(const QString &path);

    /* QString功能 */
    static int getFontLength(const QFont &font, const QString &txt);
    static int getTextLineNumber(QFont font, QString txt, int lineWidth);
    static int getWordBorder(const QString &txt, const int &start, const bool &leftToRight);//查找失败返回 -1
    static int skipChar(const QString &txt, const int &start, const char &ch, const bool &leftToRight);//跳过字符 返回 -1 跳过失败 -2 无法跳过
    static QString arrangeWord(QString word, char sp);//规整单词
    static bool isSpace(const QString &txt);
    static bool isLetter(const QString &txt, char fl = ' ');//fl 为过滤字符，默认过滤空格
    static bool isLanguage(const QString &txt, char fl = ' ');
    static bool isEnglishSentence(const QString &txt, char fl = ' ');
    static bool isWritting(const QString &txt);
    static bool isChineseChar(const QChar &ch);
    static QStringList filterWordFromList(QStringList list, QString txt, QString strategy = "prefix");
    static QString shieldWord(QString txt, QString word);

    /* 遗忘算法 */
    static uint getDeclinePeriod(int times);
    static bool memoryDecline(int &times, QDateTime &start, QDateTime end);
    static bool timesCanDecline(int times, QDateTime start, QDateTime end);
    static uint getMemoryInterval();//second

    /* 间断延时 */
    static void mDelay(uint msec, uint period = 20, bool *isRun = NULL);
    static void sDelay(uint sec, uint period = 20, bool *isRun = NULL);

    /* 界面外观数据 */
    static QString getWordTerminatorQss();
    static QString getWordLibraryWidgetQss();
    static QString getWordCreateWidgetQss();
    static QString getWordShowWidgetQss();
    static QString getWordMemorizeWidgetQss();
    static QString getWordFunctionWidgetQss();
    static QString getWordSettingWidgetQss();

    /* 重要路径 */
    static QString getWordMemorizeWidgetJsonPath();
    static QString getConfigPath();

private:
    static QString m_uniStr;
//    static int leastForeverTimes;
//    static uint memoryInterval;//单位 分钟
//    static uint declineArray[DECLINE_NUM];//单位 小时

};

#endif // WTOOL_H

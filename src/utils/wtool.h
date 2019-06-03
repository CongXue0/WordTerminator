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
    static int divide(int a, int b);
    static QString getWordDBFilePath();
    static int rand(int a, int b);//得到一个 [a, b] 范围内的随机数
    static QDateTime getCurDateTimeMinus(uint sec);

    /* 组相关 */
    static QStringList getGroupList();
    static int getGroupNo(QString groupName);
    static QString getGroupName(int groupId);

    /* 文件操作 */
    static QString readFileInfo(QString path);
    static void appendFileInfo(QString path, QString info);
    static void writeFileInfo(QString path, QString info);
    static bool makePath(QString path);
    static bool makeDir(QString path);

    /* QString功能 */
    static int getFontLength(QFont font, QString txt);
    static int getTextLineNumber(QFont font, QString txt, int lineWidth);
    static int getWordBorder(QString txt, int start, bool leftToRight);//查找失败返回 -1
    static int skipChar(QString txt, int start, char ch, bool leftToRight);//跳过字符 返回 -1 跳过失败 -2 无法跳过
    static QString arrangeWord(QString word, char sp);//规整单词
    static bool isSpace(QString txt);
    static bool isLetter(QString txt, char fl = ' ');//fl 为过滤字符，默认过滤空格
    static bool isLanguage(QString txt, char fl = ' ');
    static bool isEnglishSentence(QString txt, char fl = ' ');
    static bool isWritting(QString txt);
    static bool isChineseChar(QChar ch);
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

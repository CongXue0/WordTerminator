#ifndef WORDADMIN_H
#define WORDADMIN_H

#include <QObject>
#include <QMutex>
#include <QDateTime>
#include <QReadWriteLock>
#include <QVector>

#define MAX_WORD_NUM 200000
#define WORD_NAME_UNDEFINED "-1"
#define MAX_TIMES 99999
#define PROPERTY_NUM 23
#define EXAMPLE_NUM 8
#define RELATED_NUM 8

class BriefWordInfo
{
public:
    void init();
    QString toString();

    QString m_name;
    int m_times;
    QDateTime m_modifyTime;
    int m_groupId;
    int m_remember;
    bool m_isPhrase;
private:
    char m_pad[3] = { 0 };
};

class WordInfo
{
public:
    void init();
    void arrange();
    QString toText(int mode = 0, QString spaceStr = "");
    int toMsecs();
    bool contains(QString str) const;
    bool isExampleSentenceEmpty();
    static QString sqlStrEsc(QString str);
    WordInfo sqliteEscape() const;

    QString m_name;
    int m_times;
    QDateTime m_modifyTime;
    int m_groupid;
    int m_remember;
    bool m_isPhrase;
    QString m_phoneticSymbol;
    QString m_voiceFile;
    QString m_adj_Chinese;
    QString m_adj_English;
    QString m_adv_Chinese;
    QString m_adv_English;
    QString m_vt_Chinese;
    QString m_vt_English;
    QString m_vi_Chinese;
    QString m_vi_English;
    QString m_pastTense;
    QString m_pastParticiple;
    QString m_presentParticiple;
    QString m_thirdPersonSingular;
    QString m_noun_Chinese;
    QString m_noun_English;
    QString m_prep_Chinese;
    QString m_prep_English;
    QString m_conj_Chinese;
    QString m_conj_English;
    QString m_pron_Chinese;
    QString m_pron_English;
    QString m_art_Chinese;
    QString m_art_English;
    QString m_num_Chinese;
    QString m_num_English;
    QString m_int_Chinese;
    QString m_int_English;
    QString m_exampleSentence[EXAMPLE_NUM];
    QString m_synonym;
    QString m_antonym;
    QString m_derivative;
};

class WordTest
{
public:
    WordTest(int testSize = 1);
    bool isPass() const;
    bool isVisible() const;
    void setSpeed(int speed);
    int getSpeed() const;
    int notPassNum() const;
    int toMsecs() const;

    BriefWordInfo m_info;
    QVector<bool> m_isPass;
    bool m_visible;
    int m_loopNum;

private:
    int m_speed;
};

class WordAdmin : public QObject
{
    Q_OBJECT
public:
    static WordAdmin *getInstance();
    int getCurrentWordNum();
    bool getWordInfo(QString name, WordInfo *word, int *offset = nullptr);
    bool insertWord(WordInfo *word);
    bool updateWord(WordInfo *word);
    bool updateWord(QString name, QString field, QString value);
    bool updateWord(QString name, QString field1, QString value1, QString field2, QString value2);
    bool updateWord(QString name, QString field1, QString value1, QString field2, QString value2, QString field3, QString value3);
    bool deleteWord(QString name);
    int resetAllWordRemerber(QWidget *parent);
    bool isWordExist(QString name, int *offset = nullptr);
    bool getWordBriefInfo(QString name, BriefWordInfo *word, int *offset = nullptr);
    bool getMutexStatus();
    static bool wordCanMemorize(int remState, QDateTime modifyTime);


    /* 查找从 t1~t2 学习次数的单词 */
    int getWordNumFromTimes(int t1, int t2, int groupId, bool isRemember);
    QStringList getWordListFromTimes(int t1, int t2, int groupId, bool isRemember);
    QStringList getAllWordList(int groupId);
    QVector<BriefWordInfo> getBriefWordInfoListFromTimes(int t1, int t2, int groupId, bool isRemember);
    QVector<BriefWordInfo> getAllBriefWordInfoList(int groupId);
    QList<BriefWordInfo> getWordListWithinTime(int minutes);
    QList<BriefWordInfo> getWordListWithinTime(int minutes, bool isRemember);
    QList<WordTest> getAllWordCanMemorizeList(int groupId, int testSize);
    QList<WordTest> getWordCanMemorizeListFromTimes(int t1, int t2, int groupId, bool isRemember, int testSize);
    int getWordCanMemorizeNumFromTimes(int t1, int t2, int groupId, bool isRemember);
    QList<BriefWordInfo> getWordCannotMemorizeWithoutTime(uint minutes);

    /* 索引查找 */
    int searchWordTestList(QList<WordTest> list, QString name);

private:
    explicit WordAdmin(QObject *parent = nullptr);
    void initDB();
    bool loadAllWord();
    int checkAllWordTimesDecline();//返回受影响的单词数
    bool readWordInfo(QString name, WordInfo *word);

private:
    static BriefWordInfo m_wordLib[MAX_WORD_NUM];
    static WordAdmin *m_wordAdmin;

    int m_currentNum;
    int m_currentTop;

    QReadWriteLock m_mutex;
};

#endif // WORDADMIN_H

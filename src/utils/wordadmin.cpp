#include "wordadmin.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "wtool.h"
#include <QProgressDialog>
#include <QCoreApplication>
#include <QFile>

BriefWordInfo WordAdmin::m_wordLib[MAX_WORD_NUM];
WordAdmin *WordAdmin::m_wordAdmin = nullptr;

void BriefWordInfo::init()
{
    m_name = WORD_NAME_UNDEFINED;
    m_times = -1;
    m_groupId = 0;
    m_remember = 0;
    m_isPhrase = false;
}

QString BriefWordInfo::toString()
{
    return QString("name:%1, times:%2, modify:%3, groupid:%4, remember:%5").arg(m_name)
        .arg(m_times).arg(m_modifyTime.toString(TIMEFORMAT)).arg(m_groupId).arg(m_remember);
}

void WordInfo::init()
{
    m_name = WORD_NAME_UNDEFINED;
    m_times = -1;
    m_groupid = 0;
    m_remember = 0;
    m_isPhrase = false;
    m_phoneticSymbol = "";
    m_voiceFile = "";
    m_adj_Chinese = "";
    m_adj_English = "";
    m_adv_Chinese = "";
    m_adv_English = "";
    m_vt_Chinese = "";
    m_vt_English = "";
    m_vi_Chinese = "";
    m_vi_English = "";
    m_pastTense = "";
    m_pastParticiple = "";
    m_presentParticiple = "";
    m_thirdPersonSingular = "";
    m_noun_Chinese = "";
    m_noun_English = "";
    m_prep_Chinese = "";
    m_prep_English = "";
    m_conj_Chinese = "";
    m_conj_English = "";
    m_pron_Chinese = "";
    m_pron_English = "";
    m_art_Chinese = "";
    m_art_English = "";
    m_num_Chinese = "";
    m_num_English = "";
    m_int_Chinese = "";
    m_int_English = "";
    for (int i = 0; i < EXAMPLE_NUM; ++i)
    {
        m_exampleSentence[i] = "";
    }
    m_synonym = "";
    m_antonym = "";
    m_derivative = "";
}

void WordInfo::arrange()
{
    m_name = m_name.trimmed();
    m_phoneticSymbol = m_phoneticSymbol.trimmed();
    m_voiceFile = m_voiceFile.trimmed();
    m_adj_Chinese = WTool::arrangeWord(m_adj_Chinese, ';');
    m_adj_English = WTool::arrangeWord(m_adj_English, ';');
    m_adv_Chinese = WTool::arrangeWord(m_adv_Chinese, ';');
    m_adv_English = WTool::arrangeWord(m_adv_English, ';');
    m_vt_Chinese = WTool::arrangeWord(m_vt_Chinese, ';');
    m_vt_English = WTool::arrangeWord(m_vt_English, ';');
    m_vi_Chinese = WTool::arrangeWord(m_vi_Chinese, ';');
    m_vi_English = WTool::arrangeWord(m_vi_English, ';');
    m_pastTense = m_pastTense.trimmed();
    m_pastParticiple = m_pastParticiple.trimmed();
    m_presentParticiple = m_presentParticiple.trimmed();
    m_thirdPersonSingular = m_thirdPersonSingular.trimmed();
    m_noun_Chinese = WTool::arrangeWord(m_noun_Chinese, ';');
    m_noun_English = WTool::arrangeWord(m_noun_English, ';');
    m_prep_Chinese = WTool::arrangeWord(m_prep_Chinese, ';');
    m_prep_English = WTool::arrangeWord(m_prep_English, ';');
    m_conj_Chinese = WTool::arrangeWord(m_conj_Chinese, ';');
    m_conj_English = WTool::arrangeWord(m_conj_English, ';');
    m_pron_Chinese = WTool::arrangeWord(m_pron_Chinese, ';');
    m_pron_English = WTool::arrangeWord(m_pron_English, ';');
    m_art_Chinese = WTool::arrangeWord(m_art_Chinese, ';');
    m_art_English = WTool::arrangeWord(m_art_English, ';');
    m_num_Chinese = WTool::arrangeWord(m_num_Chinese, ';');
    m_num_English = WTool::arrangeWord(m_num_English, ';');
    m_int_Chinese = WTool::arrangeWord(m_int_Chinese, ';');
    m_int_English = WTool::arrangeWord(m_int_English, ';');
    for (int i = 0, j = 0; i < EXAMPLE_NUM; ++i)
    {
        m_exampleSentence[i] = m_exampleSentence[i].trimmed();
        if (!m_exampleSentence[i].isEmpty())
        {
            if (i > j)
            {
                m_exampleSentence[j] = m_exampleSentence[i];
                m_exampleSentence[i].clear();
            }
            j++;
        }
    }
    m_synonym = WTool::arrangeWord(m_synonym, ';');
    m_antonym = WTool::arrangeWord(m_antonym, ';');
    m_derivative = WTool::arrangeWord(m_derivative, ';');
}

QString WordInfo::toText(int mode, QString spaceStr)
{
    QString text;
    if (m_name.isEmpty())
        return "";
    else
        text.append(m_name + "\n");
    if (!m_phoneticSymbol.isEmpty())
        text.append(spaceStr + m_phoneticSymbol + "\n");
    if (!m_adj_Chinese.isEmpty())
        text.append(spaceStr + "adj. " + m_adj_Chinese + "\n");
    if (!m_adj_English.isEmpty())
        text.append(spaceStr + "adj. " + m_adj_English + "\n");
    if (!m_adv_Chinese.isEmpty())
        text.append(spaceStr + "adv. " + m_adv_Chinese + "\n");
    if (!m_adv_English.isEmpty())
        text.append(spaceStr + "adv. " + m_adv_English + "\n");
    if (!m_vt_Chinese.isEmpty())
        text.append(spaceStr + "vt. " + m_vt_Chinese + "\n");
    if (!m_vt_English.isEmpty())
        text.append(spaceStr + "vt. " + m_vt_English + "\n");
    if (!m_vi_Chinese.isEmpty())
        text.append(spaceStr + "vi. " + m_vi_Chinese + "\n");
    if (!m_vi_English.isEmpty())
        text.append(spaceStr + "vi. " + m_vi_English + "\n");
    {
        QStringList tmp;
        if (!m_pastTense.isEmpty())
            tmp.append("过去式: " + m_pastTense);
        if (!m_pastParticiple.isEmpty())
            tmp.append("过去分词: " + m_pastParticiple);
        if (!m_presentParticiple.isEmpty())
            tmp.append("现在分词: " + m_presentParticiple);
        if (!m_thirdPersonSingular.isEmpty())
            tmp.append("第三人称单数: " + m_thirdPersonSingular);
        for (int i = 0; i < tmp.size(); ++i)
        {
            if (i == 0 || i == 2)
                text.append(spaceStr);
            text.append(tmp[i] + " ");
            if ((i == 1 && i != tmp.size() - 1) || i == tmp.size() - 1)
                text.append("\n");
        }
    }
    if (!m_noun_Chinese.isEmpty())
        text.append(spaceStr + "n. " + m_noun_Chinese + "\n");
    if (!m_noun_English.isEmpty())
        text.append(spaceStr + "n. " + m_noun_English + "\n");
    if (!m_prep_Chinese.isEmpty())
        text.append(spaceStr + "prep. " + m_prep_Chinese + "\n");
    if (!m_prep_English.isEmpty())
        text.append(spaceStr + "prep. " + m_prep_English + "\n");
    if (!m_conj_Chinese.isEmpty())
        text.append(spaceStr + "conj. " + m_conj_Chinese + "\n");
    if (!m_conj_English.isEmpty())
        text.append(spaceStr + "conj. " + m_conj_English + "\n");
    if (!m_pron_Chinese.isEmpty())
        text.append(spaceStr + "pron. " + m_pron_Chinese + "\n");
    if (!m_pron_English.isEmpty())
        text.append(spaceStr + "pron. " + m_pron_English + "\n");
    if (!m_art_Chinese.isEmpty())
        text.append(spaceStr + "art. " + m_art_Chinese + "\n");
    if (!m_art_English.isEmpty())
        text.append(spaceStr + "art. " + m_art_English + "\n");
    if (!m_num_Chinese.isEmpty())
        text.append(spaceStr + "num. " + m_num_Chinese + "\n");
    if (!m_num_English.isEmpty())
        text.append(spaceStr + "num. " + m_num_English + "\n");
    if (!m_int_Chinese.isEmpty())
        text.append(spaceStr + "int. " + m_int_Chinese + "\n");
    if (!m_int_English.isEmpty())
        text.append(spaceStr + "int. " + m_int_English + "\n");
    if (!m_synonym.isEmpty())
    {
        text.append(spaceStr + "同义词: ");
        QStringList list = m_synonym.split(';');
        for (int i = 0; i < list.count(); ++i)
        {
            if (i ==  list.count() - 1)
                text.append(QString(list.at(i)).trimmed());
            else
                text.append(QString(list.at(i)).trimmed() + " ");
        }
        text.append("\n");
    }
    if (!m_antonym.isEmpty())
    {
        text.append(spaceStr + "反义词: ");
        QStringList list = m_antonym.split(';');
        for (int j = 0; j < list.count(); j++)
        {
            if (j ==  list.count() - 1)
                text.append(QString(list.at(j)).trimmed());
            else
                text.append(QString(list.at(j)).trimmed() + " ");
        }
        text.append("\n");
    }
    if (!m_derivative.isEmpty())
    {
        text.append(spaceStr + "派生词: ");
        QStringList list = m_derivative.split(';');
        for (int j = 0; j < list.count(); j++)
        {
            if (j ==  list.count() - 1)
                text.append(QString(list.at(j)).trimmed());
            else
                text.append(QString(list.at(j)).trimmed() + " ");
        }
        text.append("\n");
    }
    if (!m_exampleSentence[0].isEmpty())
    {
        if (mode == 1)
            text.append("\n");
        else if (mode == 2)
            text.append("\n\n");
    }
    for (int i = 0; i < EXAMPLE_NUM; ++i)
    {
        if (m_exampleSentence[i].isEmpty())
            break;
        text.append(QString::number(i + 1) + ". " + m_exampleSentence[i] + "\n");
    }
    if (text.length() > 0 && text.at(text.length() - 1) != '\n')
        text.append("\n");
    return text;
}

int WordInfo::toMsecs()
{
    int msecs = 0, tmp = 0;
    if (m_name.isEmpty())
        return msecs;
    int min = 1000, max = 2200, num = 0;
    auto calcFunc = [&](const QString &str) -> int {
        double len = 0, maxLen = 14;
        for (int i = 0; i < str.size(); ++i)
        {
            if (str[i] == ' ' || str[i] == ';' || str[i] == ',' ||
                str[i] == '(' || str[i] == ')')
            {
                continue;
            }
            else if (str[i] == '.')
            {
                len += 0.3;
            }
            else if (WTool::isChineseChar(str[i]))
            {
                len += 1;
            }
            else
                len += 0.5;
        }
        if (len > maxLen) len = maxLen;
        return int(max * ((maxLen * maxLen - (len - maxLen) * (len - maxLen)) / (maxLen * maxLen)));
    };
    if (!m_adj_Chinese.isEmpty() || !m_adj_English.isEmpty())
    {
        tmp = calcFunc(m_adj_Chinese + m_adj_English);
        if (tmp < min) tmp = min;
        if (tmp > max) tmp = max;
        msecs += tmp;
        num++;
    }
    if (!m_adv_Chinese.isEmpty() || !m_adv_English.isEmpty())
    {
        tmp = calcFunc(m_adv_Chinese + m_adv_English);
        if (tmp < min) tmp = min;
        if (tmp > max) tmp = max;
        msecs += tmp;
        num++;
    }
    if (!m_vt_Chinese.isEmpty() || !m_vt_English.isEmpty())
    {
        tmp = calcFunc(m_vt_Chinese + m_vt_English);
        if (tmp < min) tmp = min;
        if (tmp > max) tmp = max;
        msecs += tmp;
        num++;
    }
    if (!m_vi_Chinese.isEmpty() || !m_vi_English.isEmpty())
    {
        tmp = calcFunc(m_vi_Chinese + m_vi_English);
        if (tmp < min) tmp = min;
        if (tmp > max) tmp = max;
        msecs += tmp;
        num++;
    }
    if (!m_pastTense.isEmpty() || !m_pastParticiple.isEmpty() || !m_presentParticiple.isEmpty() || !m_thirdPersonSingular.isEmpty())
    {
        tmp = calcFunc(m_pastTense + m_pastParticiple + m_presentParticiple + m_thirdPersonSingular);
        if (tmp < min) tmp = min;
        if (tmp > max) tmp = max;
        msecs += tmp;
        num++;
    }
    if (!m_noun_Chinese.isEmpty() || !m_noun_English.isEmpty())
    {
        tmp = calcFunc(m_noun_Chinese + m_noun_English);
        if (tmp < min) tmp = min;
        if (tmp > max) tmp = max;
        msecs += tmp;
        num++;
    }
    if (!m_prep_Chinese.isEmpty() || !m_prep_English.isEmpty())
    {
        tmp = calcFunc(m_prep_Chinese + m_prep_English);
        if (tmp < min) tmp = min;
        if (tmp > max) tmp = max;
        msecs += tmp;
        num++;
    }
    if (!m_conj_Chinese.isEmpty() || !m_conj_English.isEmpty())
    {
        tmp = calcFunc(m_conj_Chinese + m_conj_English);
        if (tmp < min) tmp = min;
        if (tmp > max) tmp = max;
        msecs += tmp;
        num++;
    }
    if (!m_pron_Chinese.isEmpty() || !m_pron_English.isEmpty())
    {
        tmp = calcFunc(m_pron_Chinese + m_pron_English);
        if (tmp < min) tmp = min;
        if (tmp > max) tmp = max;
        msecs += tmp;
        num++;
    }
    if (!m_art_Chinese.isEmpty() || !m_art_English.isEmpty())
    {
        tmp = calcFunc(m_art_Chinese + m_art_English);
        if (tmp < min) tmp = min;
        if (tmp > max) tmp = max;
        msecs += tmp;
        num++;
    }
    if (!m_num_Chinese.isEmpty() || !m_num_English.isEmpty())
    {
        tmp = calcFunc(m_num_Chinese + m_num_English);
        if (tmp < min) tmp = min;
        if (tmp > max) tmp = max;
        msecs += tmp;
        num++;
    }
    if (!m_int_Chinese.isEmpty() || !m_int_English.isEmpty())
    {
        tmp = calcFunc(m_int_Chinese + m_int_English);
        if (tmp < min) tmp = min;
        if (tmp > max) tmp = max;
        msecs += tmp;
        num++;
    }
//    for (int i = 0; i < EXAMPLE_NUM; ++i)
//    {
//        if (m_exampleSentence[i].isEmpty())
//            break;
//        tmp = calcFunc(m_exampleSentence[i]);
//        if (tmp < 800) tmp = 800;
//        if (tmp > 1300) tmp = 1300;
//        msecs += tmp;
//    }
    if (!m_synonym.isEmpty() || !m_antonym.isEmpty())
    {
        tmp = calcFunc(m_synonym + m_antonym);
        if (tmp < min) tmp = min;
        if (tmp > max) tmp = max;
        msecs += tmp;
        num++;
    }
//    if (!m_derivative.isEmpty())
//    {
//        tmp = calcFunc(m_derivative);
//        if (tmp < min) tmp = min;
//        if (tmp > max) tmp = max;
//        msecs += tmp;
//        num++;
//    }
    if (msecs < 1500)
        msecs = 1500;
    if (num == 2)
    {
        msecs = int(msecs * 0.8);
    }
    else if (num == 3)
    {
        msecs = int(msecs * 0.7);
    }
    else if (num > 3)
    {
        msecs = int(msecs * 0.6);
    }
//    if (!m_phoneticSymbol.isEmpty())
//        msecs += 300;
    return msecs;
}

bool WordInfo::contains(QString str) const
{
    if (str.isEmpty())
        return true;
    return m_adj_Chinese.contains(str) || m_adj_English.contains(str) || m_adv_Chinese.contains(str) || m_adv_English.contains(str) ||
        m_vt_Chinese.contains(str) || m_vt_English.contains(str) || m_vi_Chinese.contains(str) || m_vi_English.contains(str) ||
        m_noun_Chinese.contains(str) || m_noun_English.contains(str) || m_prep_Chinese.contains(str) || m_prep_English.contains(str) ||
        m_conj_Chinese.contains(str) || m_conj_English.contains(str) || m_pron_Chinese.contains(str) || m_pron_English.contains(str) ||
        m_art_Chinese.contains(str) || m_art_English.contains(str) || m_num_Chinese.contains(str) || m_num_English.contains(str) ||
        m_int_Chinese.contains(str) || m_int_English.contains(str);
}

bool WordInfo::isExampleSentenceEmpty()
{
    for (int i = 0; i < EXAMPLE_NUM; ++i)
    {
        if (!m_exampleSentence[i].isEmpty())
            return false;
    }
    return true;
}

QString WordInfo::sqlStrEsc(QString str)
{
//    str = str.replace("/", "//");
    str = str.replace("'", "''");
//    str = str.replace("[", "/[");
//    str = str.replace("]", "/]");
//    str = str.replace("%", "/%");
//    str = str.replace("&", "/&");
//    str = str.replace("_", "/_");
//    str = str.replace("(", "/(");
//    str = str.replace(")", "/)");
    return str;
}

WordInfo WordInfo::sqliteEscape() const
{
    WordInfo tmp = *this;
    tmp.m_phoneticSymbol = sqlStrEsc(tmp.m_phoneticSymbol);
    tmp.m_voiceFile = sqlStrEsc(tmp.m_voiceFile);
    tmp.m_adj_Chinese = sqlStrEsc(tmp.m_adj_Chinese);
    tmp.m_adj_English = sqlStrEsc(tmp.m_adj_English);
    tmp.m_adv_Chinese = sqlStrEsc(tmp.m_adv_Chinese);
    tmp.m_adv_English = sqlStrEsc(tmp.m_adv_English);
    tmp.m_vt_Chinese = sqlStrEsc(tmp.m_vt_Chinese);
    tmp.m_vt_English = sqlStrEsc(tmp.m_vt_English);
    tmp.m_vi_Chinese = sqlStrEsc(tmp.m_vi_Chinese);
    tmp.m_vi_English = sqlStrEsc(tmp.m_vi_English);
    tmp.m_pastTense = sqlStrEsc(tmp.m_pastTense);
    tmp.m_pastParticiple = sqlStrEsc(tmp.m_pastParticiple);
    tmp.m_presentParticiple = sqlStrEsc(tmp.m_presentParticiple);
    tmp.m_thirdPersonSingular = sqlStrEsc(tmp.m_thirdPersonSingular);
    tmp.m_noun_Chinese = sqlStrEsc(tmp.m_noun_Chinese);
    tmp.m_noun_English = sqlStrEsc(tmp.m_noun_English);
    tmp.m_prep_Chinese = sqlStrEsc(tmp.m_prep_Chinese);
    tmp.m_prep_English = sqlStrEsc(tmp.m_prep_English);
    tmp.m_conj_Chinese = sqlStrEsc(tmp.m_conj_Chinese);
    tmp.m_conj_English = sqlStrEsc(tmp.m_conj_English);
    tmp.m_pron_Chinese = sqlStrEsc(tmp.m_pron_Chinese);
    tmp.m_pron_English = sqlStrEsc(tmp.m_pron_English);
    tmp.m_art_Chinese = sqlStrEsc(tmp.m_art_Chinese);
    tmp.m_art_English = sqlStrEsc(tmp.m_art_English);
    tmp.m_num_Chinese = sqlStrEsc(tmp.m_num_Chinese);
    tmp.m_num_English = sqlStrEsc(tmp.m_num_English);
    tmp.m_int_Chinese = sqlStrEsc(tmp.m_int_Chinese);
    tmp.m_int_English = sqlStrEsc(tmp.m_int_English);
    for (int i = 0; i < EXAMPLE_NUM; ++i)
        tmp.m_exampleSentence[i] = sqlStrEsc(tmp.m_exampleSentence[i]);
    tmp.m_synonym = sqlStrEsc(tmp.m_synonym);
    tmp.m_antonym = sqlStrEsc(tmp.m_antonym);
    tmp.m_derivative = sqlStrEsc(tmp.m_derivative);
    return tmp;
}

WordTest::WordTest(int testSize)
{
    if (testSize < 1) testSize = 1;
    m_isPass = QVector<bool>(testSize, true);
    m_visible = true;
    m_speed = 0;
    m_loopNum = 0;
}

bool WordTest::isPass() const
{
    bool pass = true;
    for (int i = 0; i < m_isPass.size(); ++i)
    {
        pass = pass && m_isPass[i];
    }
    return pass;
}

bool WordTest::isVisible() const
{
    return m_visible;
}

void WordTest::setSpeed(int speed)
{
    if (speed < -5)
        speed = -5;
    if (speed > 5)
        speed = 5;
    m_speed = speed;
}

int WordTest::getSpeed() const
{
    return m_speed;
}

int WordTest::notPassNum() const
{
    int num = 0;
    for (int i = 0; i < m_isPass.size(); ++i)
    {
        if (!m_isPass[i])
            num++;
    }
    return num;
}

int WordTest::toMsecs() const
{
    WordInfo wordInfo;
    if (WordAdmin::getInstance()->getWordInfo(m_info.m_name, &wordInfo))
    {
        int msecs = wordInfo.toMsecs();
        if (m_speed > 0)
        {
            double p = 1 - 0.12 * m_speed;
            msecs = int(msecs * p);
        }
        else if (m_speed < 0)
        {
            double p = 1 - 0.2 * m_speed;
            msecs = int(msecs * p);
        }
        return msecs;
    }
    return 4000;
}

WordAdmin *WordAdmin::getInstance()
{
    if (m_wordAdmin == nullptr)
    {
        m_wordAdmin = new WordAdmin();
    }
    return m_wordAdmin;
}

int WordAdmin::getCurrentWordNum()
{
    return m_currentNum;
}

bool WordAdmin::getWordInfo(QString name, WordInfo *word, int *offset)
{
    if (name == WORD_NAME_UNDEFINED)
        return false;
    m_mutex.lockForRead();
    for (int i = 0; i < m_currentTop; ++i)
    {
        if (m_wordLib[i].m_name == name)
        {
            bool ret = false;
            if (word != nullptr)
            {
                ret = readWordInfo(name, word);
            }
            if (offset != nullptr)
            {
                *offset = i;
            }
            m_mutex.unlock();
            return ret;
        }
    }
    m_mutex.unlock();
    return false;
}

bool WordAdmin::insertWord(WordInfo *word)
{
    if (word == nullptr || word->m_name == WORD_NAME_UNDEFINED)
        return false;
    m_mutex.lockForWrite();
    int j = -1;
    for (int i = 0; i < m_currentTop; ++i)
    {
        if (m_wordLib[i].m_name == word->m_name)
        {
            m_mutex.unlock();
            return false;
        }
        if (j == -1 && m_wordLib[i].m_name == WORD_NAME_UNDEFINED)
        {
            j = i;
            break;
        }
    }
    WordInfo tmp = word->sqliteEscape();
    QSqlQuery query;
    QString sql = QString::fromUtf8(u8R"(
insert into WordLibrary values('%1', '%2', '%3', '%4', '%5', '%6',
'%7', '%8', '%9', '%10', '%11', '%12', '%13', '%14', '%15', '%16', '%17', '%18', '%19',
'%20', '%21', '%22', '%23', '%24', '%25', '%26', '%27', '%28', '%29', '%30', '%31',
'%32', '%33', '%34', '%35', '%36', '%37', '%38', '%39', '%40', '%41', '%42', '%43', '%44', '%45')
)")
        .arg(tmp.m_name).arg(tmp.m_times).arg(tmp.m_modifyTime.toString(TIMEFORMAT)).arg(tmp.m_groupid)
        .arg(tmp.m_remember).arg(((tmp.m_isPhrase) ? 1 : 0)).arg(tmp.m_phoneticSymbol).arg(tmp.m_voiceFile)
        .arg(tmp.m_adj_Chinese).arg(tmp.m_adj_English).arg(tmp.m_adv_Chinese).arg(tmp.m_adv_English)
        .arg(tmp.m_vt_Chinese).arg(tmp.m_vt_English).arg(tmp.m_vi_Chinese).arg(tmp.m_vi_English)
        .arg(tmp.m_pastTense).arg(tmp.m_pastParticiple).arg(tmp.m_presentParticiple)
        .arg(tmp.m_thirdPersonSingular).arg(tmp.m_noun_Chinese).arg(tmp.m_noun_English)
        .arg(tmp.m_prep_Chinese).arg(tmp.m_prep_English).arg(tmp.m_conj_Chinese).arg(tmp.m_conj_English)
        .arg(tmp.m_pron_Chinese).arg(tmp.m_pron_English).arg(tmp.m_art_Chinese).arg(tmp.m_art_English)
        .arg(tmp.m_num_Chinese).arg(tmp.m_num_English).arg(tmp.m_int_Chinese).arg(tmp.m_int_English)
        .arg(tmp.m_exampleSentence[0]).arg(tmp.m_exampleSentence[1]).arg(tmp.m_exampleSentence[2])
        .arg(tmp.m_exampleSentence[3]).arg(tmp.m_exampleSentence[4]).arg(tmp.m_exampleSentence[5])
        .arg(tmp.m_exampleSentence[6]).arg(tmp.m_exampleSentence[7])
        .arg(tmp.m_synonym).arg(tmp.m_antonym).arg(tmp.m_derivative);
    if (query.exec(sql))
    {
        m_currentNum++;
        if (j == -1)
        {
            j = m_currentTop;
            m_currentTop++;
        }
        m_wordLib[j].m_name = word->m_name;
        m_wordLib[j].m_times = word->m_times;
        m_wordLib[j].m_modifyTime = word->m_modifyTime;
        m_wordLib[j].m_groupId = word->m_groupid;
        m_wordLib[j].m_remember = word->m_remember;
        m_wordLib[j].m_isPhrase = word->m_isPhrase;
        m_mutex.unlock();
        return true;
    }
    m_mutex.unlock();
    return false;
}

bool WordAdmin::updateWord(WordInfo *word)
{
    if (word == nullptr || word->m_name == WORD_NAME_UNDEFINED)
        return false;
    m_mutex.lockForWrite();
    for (int i = 0; i < m_currentTop; ++i)
    {
        if (m_wordLib[i].m_name == word->m_name)
        {
            WordInfo tmp = word->sqliteEscape();
            QSqlQuery query;
            QString sql = QString::fromUtf8(u8R"(
update WordLibrary set Times='%1', ModifyTime='%2', Groupid='%3', RememberState='%4',
IsPhrase='%5', PhoneticSymbol='%6', VoiceFile='%7', Adj_Chinese='%8', Adj_English='%9', Adv_Chinese='%10',
Adv_English='%11', Vt_Chinese='%12', Vt_English='%13', Vi_Chinese='%14', Vi_English='%15',
PastTense='%16', PastParticiple='%17', PresentParticiple='%18', ThirdPersonSingular='%19',
Noun_Chinese='%20', Noun_English='%21', Prep_Chinese='%22', Prep_English='%23', Conj_Chinese='%24',
Conj_English='%25', Pron_Chinese='%26', Pron_English='%27', Art_Chinese='%28', Art_English='%29',
Num_Chinese='%30', Num_English='%31', Int_Chinese='%32', Int_English='%33',
ExampleSentence1='%34', ExampleSentence2='%35', ExampleSentence3='%36', ExampleSentence4='%37',
ExampleSentence5='%38', ExampleSentence6='%39', ExampleSentence7='%40', ExampleSentence8='%41',
Synonym='%42', Antonym='%43', Derivative='%44' where Name='%45'
)")
                .arg(tmp.m_times).arg(tmp.m_modifyTime.toString(TIMEFORMAT)).arg(tmp.m_groupid).arg(tmp.m_remember)
                .arg(((tmp.m_isPhrase) ? 1 : 0)).arg(tmp.m_phoneticSymbol).arg(tmp.m_voiceFile)
                .arg(tmp.m_adj_Chinese).arg(tmp.m_adj_English).arg(tmp.m_adv_Chinese).arg(tmp.m_adv_English)
                .arg(tmp.m_vt_Chinese).arg(tmp.m_vt_English).arg(tmp.m_vi_Chinese).arg(tmp.m_vi_English)
                .arg(tmp.m_pastTense).arg(tmp.m_pastParticiple).arg(tmp.m_presentParticiple).arg(tmp.m_thirdPersonSingular)
                .arg(tmp.m_noun_Chinese).arg(tmp.m_noun_English).arg(tmp.m_prep_Chinese).arg(tmp.m_prep_English)
                .arg(tmp.m_conj_Chinese).arg(tmp.m_conj_English).arg(tmp.m_pron_Chinese).arg(tmp.m_pron_English)
                .arg(tmp.m_art_Chinese).arg(tmp.m_art_English).arg(tmp.m_num_Chinese).arg(tmp.m_num_English)
                .arg(tmp.m_int_Chinese).arg(tmp.m_int_English).arg(tmp.m_exampleSentence[0]).arg(tmp.m_exampleSentence[1])
                .arg(tmp.m_exampleSentence[2]).arg(tmp.m_exampleSentence[3]).arg(tmp.m_exampleSentence[4])
                .arg(tmp.m_exampleSentence[5]).arg(tmp.m_exampleSentence[6]).arg(tmp.m_exampleSentence[7])
                .arg(tmp.m_synonym).arg(tmp.m_antonym).arg(tmp.m_derivative).arg(tmp.m_name);
//            DEBUG << sql;
            if (query.exec(sql))
            {
                m_wordLib[i].m_times = word->m_times;
                m_wordLib[i].m_modifyTime = word->m_modifyTime;
                m_wordLib[i].m_groupId = word->m_groupid;
                m_wordLib[i].m_remember = word->m_remember;
                m_wordLib[i].m_isPhrase = word->m_isPhrase;
                m_mutex.unlock();
                return true;
            }
            else
                break;
        }
    }
    m_mutex.unlock();
    return false;
}

bool WordAdmin::updateWord(QString name, QString field, QString value)
{
    if (name == WORD_NAME_UNDEFINED || field == "Name")
        return false;
    m_mutex.lockForWrite();
    for (int i = 0; i < m_currentTop; ++i)
    {
        if (m_wordLib[i].m_name == name)
        {
            QSqlQuery query;
            QString sql = QString("update WordLibrary set %1='%2' where Name='%3'")
                .arg(field).arg(WordInfo::sqlStrEsc(value)).arg(name);
            if (query.exec(sql))
            {
                if (field == "Times")
                    m_wordLib[i].m_times = value.toInt();
                else if (field == "ModifyTime")
                    m_wordLib[i].m_modifyTime = QDateTime::fromString(value, TIMEFORMAT);
                else if (field == "Groupid")
                    m_wordLib[i].m_groupId = value.toInt();
                else if (field == "RememberState")
                    m_wordLib[i].m_remember = value.toInt();
                else if (field == "IsPhrase")
                    m_wordLib[i].m_isPhrase = value == "1";
                m_mutex.unlock();
                return true;
            }
        }
    }
    m_mutex.unlock();
    return false;
}

bool WordAdmin::updateWord(QString name, QString field1, QString value1, QString field2, QString value2)
{
    if (name == WORD_NAME_UNDEFINED || field1 == "Name" || field2 == "Name" || field1 == field2)
        return false;
    m_mutex.lockForWrite();
    for (int i = 0; i < m_currentTop; ++i)
    {
        if (m_wordLib[i].m_name == name)
        {
            QSqlQuery query;
            QString sql = QString("update WordLibrary set %1='%2', %3='%4' where Name='%5'")
                .arg(field1).arg(WordInfo::sqlStrEsc(value1))
                .arg(field2).arg(WordInfo::sqlStrEsc(value2)).arg(name);
            if (query.exec(sql))
            {
                if (field1 == "Times")
                    m_wordLib[i].m_times = value1.toInt();
                else if (field1 == "ModifyTime")
                    m_wordLib[i].m_modifyTime = QDateTime::fromString(value1, TIMEFORMAT);
                else if (field1 == "Groupid")
                    m_wordLib[i].m_groupId = value1.toInt();
                else if (field1 == "RememberState")
                    m_wordLib[i].m_remember = value1.toInt();
                else if (field1 == "IsPhrase")
                    m_wordLib[i].m_isPhrase = value1 == "1";

                if (field2 == "Times")
                    m_wordLib[i].m_times = value2.toInt();
                else if (field2 == "ModifyTime")
                    m_wordLib[i].m_modifyTime = QDateTime::fromString(value2, TIMEFORMAT);
                else if (field2 == "Groupid")
                    m_wordLib[i].m_groupId = value2.toInt();
                else if (field2 == "RememberState")
                    m_wordLib[i].m_remember = value2.toInt();
                else if (field2 == "IsPhrase")
                    m_wordLib[i].m_isPhrase = value2 == "1";
                m_mutex.unlock();
                return true;
            }
        }
    }
    m_mutex.unlock();
    return false;
}

bool WordAdmin::updateWord(QString name, QString field1, QString value1, QString field2, QString value2, QString field3, QString value3)
{
    if (name == WORD_NAME_UNDEFINED || field1 == "Name" || field2 == "Name" || field3 == "Name" ||
        field1 == field2 || field2 == field3 || field1 == field3)
        return false;
    m_mutex.lockForWrite();
    for (int i = 0; i < m_currentTop; ++i)
    {
        if (m_wordLib[i].m_name == name)
        {
            QSqlQuery query;
            QString sql = QString("update WordLibrary set %1='%2', %3='%4', %5='%6' where Name='%7'")
                .arg(field1).arg(WordInfo::sqlStrEsc(value1))
                .arg(field2).arg(WordInfo::sqlStrEsc(value2))
                .arg(field3).arg(WordInfo::sqlStrEsc(value3)).arg(name);
            if (query.exec(sql))
            {
                if (field1 == "Times")
                    m_wordLib[i].m_times = value1.toInt();
                else if (field1 == "ModifyTime")
                    m_wordLib[i].m_modifyTime = QDateTime::fromString(value1, TIMEFORMAT);
                else if (field1 == "Groupid")
                    m_wordLib[i].m_groupId = value1.toInt();
                else if (field1 == "RememberState")
                    m_wordLib[i].m_remember = value1.toInt();
                else if (field1 == "IsPhrase")
                    m_wordLib[i].m_isPhrase = value1 == "1";

                if (field2 == "Times")
                    m_wordLib[i].m_times = value2.toInt();
                else if (field2 == "ModifyTime")
                    m_wordLib[i].m_modifyTime = QDateTime::fromString(value2, TIMEFORMAT);
                else if (field2 == "Groupid")
                    m_wordLib[i].m_groupId = value2.toInt();
                else if (field2 == "RememberState")
                    m_wordLib[i].m_remember = value2.toInt();
                else if (field2 == "IsPhrase")
                    m_wordLib[i].m_isPhrase = value2 == "1";

                if (field3 == "Times")
                    m_wordLib[i].m_times = value3.toInt();
                else if (field3 == "ModifyTime")
                    m_wordLib[i].m_modifyTime = QDateTime::fromString(value3, TIMEFORMAT);
                else if (field3 == "Groupid")
                    m_wordLib[i].m_groupId = value3.toInt();
                else if (field3 == "RememberState")
                    m_wordLib[i].m_remember = value3.toInt();
                else if (field3 == "IsPhrase")
                    m_wordLib[i].m_isPhrase = value3 == "1";

                m_mutex.unlock();
                return true;
            }
        }
    }
    m_mutex.unlock();
    return false;
}

bool WordAdmin::deleteWord(QString name)
{
    if (name == WORD_NAME_UNDEFINED)
        return false;
    m_mutex.lockForWrite();
    for (int i = 0; i < m_currentTop; ++i)
    {
        if (m_wordLib[i].m_name == name)
        {
            QSqlQuery query;
            QString sql = QString("delete from WordLibrary where Name='%1'").arg(name);
            if (query.exec(sql))
            {
                DEBUG << "delete " + name + " success";
                m_currentNum--;
                m_wordLib[i].init();
                if (i == m_currentTop - 1)
                    m_currentTop--;
                m_mutex.unlock();
                return true;
            }
        }
    }
    m_mutex.unlock();
    return false;
}

int WordAdmin::resetAllWordRemerber(QWidget *parent)
{
    int num = 0;
    QSqlQuery query;
    QString sql;

    QProgressDialog dialog("单词重置进度", "取消", 0, m_currentTop, parent);
    dialog.setWindowTitle(tr("单词重置对话框"));
    dialog.setWindowModality(Qt::WindowModal);

    m_mutex.lockForWrite();
    dialog.show();
    for (int i = 0; i < m_currentTop; ++i)
    {
        dialog.setValue(i);
        QCoreApplication::processEvents();
        if (dialog.wasCanceled())
            break;
        if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED &&
            !WordAdmin::wordCanMemorize(m_wordLib[i].m_remember, m_wordLib[i].m_modifyTime))
        {
            m_wordLib[i].m_remember = (m_wordLib[i].m_remember > 0 ? 2 : -1);
            sql = QString("update WordLibrary set RememberState='%1' where Name='%2'")
                .arg(QString::number(m_wordLib[i].m_remember)).arg(m_wordLib[i].m_name);
            if (query.exec(sql))
            {
                num++;
            }
        }
    }
    m_mutex.unlock();
    dialog.setValue(m_currentTop);
    return num;
}

bool WordAdmin::isWordExist(QString name, int *offset)
{
    if (name == WORD_NAME_UNDEFINED)
        return false;
    m_mutex.lockForRead();
    for (int i = 0; i < m_currentTop; ++i)
    {
        if (m_wordLib[i].m_name == name)
        {
            if (offset != nullptr)
            {
                *offset = i;
            }
            m_mutex.unlock();
            return true;
        }
    }
    m_mutex.unlock();
    return false;
}

bool WordAdmin::getWordBriefInfo(QString name, BriefWordInfo *word, int *offset)
{
    if (name == WORD_NAME_UNDEFINED)
        return false;
    m_mutex.lockForRead();
    for (int i = 0; i < m_currentTop; ++i)
    {
        if (m_wordLib[i].m_name == name)
        {
            if (word != nullptr)
            {
                *word = m_wordLib[i];
            }
            if (offset != nullptr)
            {
                *offset = i;
            }
            m_mutex.unlock();
            return true;
        }
    }
    m_mutex.unlock();
    return false;
}

bool WordAdmin::getMutexStatus()
{
    bool ret = m_mutex.tryLockForRead() || m_mutex.tryLockForWrite();
    if (ret)
        m_mutex.unlock();
    return ret;
}

bool WordAdmin::wordCanMemorize(int remState, QDateTime modifyTime)
{
    if ((remState == -1 || remState == 2) || modifyTime.secsTo(QDateTime::currentDateTime()) >= WTool::getMemoryInterval())
        return true;
    else
        return false;
}

int WordAdmin::getWordNumFromTimes(int t1, int t2, int groupId, bool isRemember)
{
    int num = 0;
    if (t1 < 0 || t2 < 0 || t1 > t2)
        return num;
    m_mutex.lockForRead();
    if (groupId == -1)
    {
        for (int i = 0; i < m_currentTop; ++i)
        {
            if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && (m_wordLib[i].m_remember > 0) == isRemember &&
                (m_wordLib[i].m_times >= t1 && m_wordLib[i].m_times <= t2))
                num++;
        }
    }
    else
    {
        for (int i = 0; i < m_currentTop; ++i)
        {
            if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && m_wordLib[i].m_groupId == groupId &&
                (m_wordLib[i].m_remember > 0) == isRemember && (m_wordLib[i].m_times >= t1 && m_wordLib[i].m_times <= t2))
                num++;
        }
    }
    m_mutex.unlock();
    return num;
}

QStringList WordAdmin::getWordListFromTimes(int t1, int t2, int groupId, bool isRemember)
{
    QStringList list;
    if (t1 < 0 || t2 < 0 || t1 > t2)
        return list;
    m_mutex.lockForRead();
    if (groupId == -1)
    {
        for (int i = 0; i < m_currentTop; ++i)
        {
            if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && (m_wordLib[i].m_remember > 0) == isRemember &&
                (m_wordLib[i].m_times >= t1 && m_wordLib[i].m_times <= t2))
                list.append(m_wordLib[i].m_name);
        }
    }
    else
    {
        for (int i = 0; i < m_currentTop; ++i)
        {
            if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && m_wordLib[i].m_groupId == groupId &&
                (m_wordLib[i].m_remember > 0) == isRemember && (m_wordLib[i].m_times >= t1 && m_wordLib[i].m_times <= t2))
                list.append(m_wordLib[i].m_name);
        }
    }
    m_mutex.unlock();
    return list;
}

QStringList WordAdmin::getAllWordList(int groupId)
{
    QStringList list;
    m_mutex.lockForRead();
    if (m_currentNum <= 0)
    {
        m_mutex.unlock();
        return list;
    }
    if (groupId == -1)
    {
        for (int i = 0; i < m_currentTop; ++i)
        {
            if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED)
                list.append(m_wordLib[i].m_name);
        }
    }
    else
    {
        for (int i = 0; i < m_currentTop; ++i)
        {
            if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && m_wordLib[i].m_groupId == groupId)
                list.append(m_wordLib[i].m_name);
        }
    }
    m_mutex.unlock();
    return list;
}

QVector<BriefWordInfo> WordAdmin::getBriefWordInfoListFromTimes(int t1, int t2, int groupId, bool isRemember)
{
    QVector<BriefWordInfo> list;
    if (t1 < 0 || t2 < 0 || t1 > t2)
        return list;
    m_mutex.lockForRead();
    if (groupId == -1)
    {
        for (int i = 0; i < m_currentTop; ++i)
        {
            if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && (m_wordLib[i].m_remember > 0) == isRemember &&
                (m_wordLib[i].m_times >= t1 && m_wordLib[i].m_times <= t2))
                list.append(m_wordLib[i]);
        }
    }
    else
    {
        for (int i = 0; i < m_currentTop; ++i)
        {
            if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && m_wordLib[i].m_groupId == groupId &&
                (m_wordLib[i].m_remember > 0) == isRemember && (m_wordLib[i].m_times >= t1 && m_wordLib[i].m_times <= t2))
                list.append(m_wordLib[i]);
        }
    }
    m_mutex.unlock();
    return list;
}

QVector<BriefWordInfo> WordAdmin::getAllBriefWordInfoList(int groupId)
{
    QVector<BriefWordInfo> list;
    m_mutex.lockForRead();
    if (m_currentNum <= 0)
    {
        m_mutex.unlock();
        return list;
    }
    if (groupId == -1)
    {
        for (int i = 0; i < m_currentTop; ++i)
        {
            if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED)
                list.append(m_wordLib[i]);
        }
    }
    else
    {
        for (int i = 0; i < m_currentTop; ++i)
        {
            if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && m_wordLib[i].m_groupId == groupId)
                list.append(m_wordLib[i]);
        }
    }
    m_mutex.unlock();
    return list;
}

QList<BriefWordInfo> WordAdmin::getWordListWithinTime(int minutes)
{
    QList<BriefWordInfo> list;
    m_mutex.lockForRead();
    QDateTime end = QDateTime::fromTime_t(QDateTime::currentDateTime().toTime_t() + uint(minutes) * 60);
    for (int i = 0; i < m_currentTop; ++i)
    {
        if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED &&
            WTool::timesCanDecline(m_wordLib[i].m_times, m_wordLib[i].m_modifyTime, end))
        {
            list.append(m_wordLib[i]);
        }
    }
    m_mutex.unlock();
    return list;
}

QList<BriefWordInfo> WordAdmin::getWordListWithinTime(int minutes, bool isRemember)
{
    QList<BriefWordInfo> list;
    m_mutex.lockForRead();
    QDateTime end = QDateTime::fromTime_t(QDateTime::currentDateTime().toTime_t() + uint(minutes) * 60);
    for (int i = 0; i < m_currentTop; ++i)
    {
        if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && (m_wordLib[i].m_remember > 0) == isRemember &&
            (m_wordLib[i].m_remember != -1 && m_wordLib[i].m_remember != 2))
        {
            if (WTool::timesCanDecline(m_wordLib[i].m_times, m_wordLib[i].m_modifyTime, end))
            {
                list.append(m_wordLib[i]);
            }
        }
    }
    m_mutex.unlock();
    return list;
}

QList<WordTest> WordAdmin::getAllWordCanMemorizeList(int groupId, int testSize)
{
    QList<WordTest> list;
    m_mutex.lockForRead();
    QDateTime cur = QDateTime::currentDateTime();
    if (groupId == -1)
    {
        for (int i = 0; i < m_currentTop; ++i)
        {
            if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && ((m_wordLib[i].m_remember == -1 || m_wordLib[i].m_remember == 2) ||
                m_wordLib[i].m_modifyTime.secsTo(cur) >= WTool::getMemoryInterval()))
            {
                WordTest test(testSize);
                test.m_info = m_wordLib[i];
                list.append(test);
            }
        }
    }
    else
    {
        for (int i = 0; i < m_currentTop; ++i)
        {
            if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && m_wordLib[i].m_groupId == groupId &&
                ((m_wordLib[i].m_remember == -1 || m_wordLib[i].m_remember == 2) ||
                m_wordLib[i].m_modifyTime.secsTo(cur) >= WTool::getMemoryInterval()))
            {
                WordTest test(testSize);
                test.m_info = m_wordLib[i];
                list.append(test);
            }
        }
    }
    m_mutex.unlock();
    return list;
}

QList<WordTest> WordAdmin::getWordCanMemorizeListFromTimes(int t1, int t2, int groupId, bool isRemember, int testSize)
{
    QList<WordTest> list;
    if (t1 < 0 || t2 < 0 || t1 > t2)
        return list;
    m_mutex.lockForRead();
    QDateTime cur = QDateTime::currentDateTime();
    if (groupId == -1)
    {
        for (int i = 0; i < m_currentTop; ++i)
        {
            if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && (m_wordLib[i].m_remember > 0) == isRemember)
            {
                if (m_wordLib[i].m_times >= t1 && m_wordLib[i].m_times <= t2 &&
                    ((m_wordLib[i].m_remember == -1 || m_wordLib[i].m_remember == 2) ||
                    m_wordLib[i].m_modifyTime.secsTo(cur) >= WTool::getMemoryInterval()))
                {
                    WordTest test(testSize);
                    test.m_info = m_wordLib[i];
                    list.append(test);
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < m_currentTop; ++i)
        {
            if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && m_wordLib[i].m_groupId == groupId &&
                (m_wordLib[i].m_remember > 0) == isRemember)
            {
                if (m_wordLib[i].m_times >= t1 && m_wordLib[i].m_times <= t2 &&
                    ((m_wordLib[i].m_remember == -1 || m_wordLib[i].m_remember == 2) ||
                    m_wordLib[i].m_modifyTime.secsTo(cur) >= WTool::getMemoryInterval()))
                {
                    WordTest test(testSize);
                    test.m_info = m_wordLib[i];
                    list.append(test);
                }
            }
        }
    }
    m_mutex.unlock();
    return list;
}

int WordAdmin::getWordCanMemorizeNumFromTimes(int t1, int t2, int groupId, bool isRemember)
{
    int num = 0;
    if (t1 < 0 || t2 < 0 || t1 > t2)
        return num;
    m_mutex.lockForRead();
    QDateTime cur = QDateTime::currentDateTime();
    if (groupId == -1)
    {
        for (int i = 0; i < m_currentTop; ++i)
        {
            if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && (m_wordLib[i].m_remember > 0) == isRemember &&
                (m_wordLib[i].m_times >= t1 && m_wordLib[i].m_times <= t2))
            {
//                DEBUG << m_wordLib[i].toString();
//                QString info = m_wordLib[i].toString();
                if ((m_wordLib[i].m_remember == -1 || m_wordLib[i].m_remember == 2) ||
                    m_wordLib[i].m_modifyTime.secsTo(cur) >= WTool::getMemoryInterval())
                {
                    num++;
//                    info += " yes";
                }
//                else
//                {
//                    DEBUG << m_wordLib[i].toString();
//                }
//                DEBUG << info;
            }
        }
    }
    else
    {
        for (int i = 0; i < m_currentTop; ++i)
        {
            if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && m_wordLib[i].m_groupId == groupId &&
                (m_wordLib[i].m_remember > 0) == isRemember && (m_wordLib[i].m_times >= t1 && m_wordLib[i].m_times <= t2))
            {
                if ((m_wordLib[i].m_remember == -1 || m_wordLib[i].m_remember == 2) ||
                    m_wordLib[i].m_modifyTime.secsTo(cur) >= WTool::getMemoryInterval())
                {
                    num++;
                }
            }
        }
    }

    m_mutex.unlock();
    return num;
}

QList<BriefWordInfo> WordAdmin::getWordCannotMemorizeWithoutTime(uint minutes)
{
    QList<BriefWordInfo> list;
    m_mutex.lockForRead();
    QDateTime cur = QDateTime::currentDateTime();
    for (int i = 0; i < m_currentTop; ++i)
    {
        if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED)
        {
            uint sec = uint(m_wordLib[i].m_modifyTime.secsTo(cur));
            if (sec < WTool::getMemoryInterval() && sec >= (WTool::getMemoryInterval() - minutes * 60))
            {
                list.append(m_wordLib[i]);
            }
        }
    }
    m_mutex.unlock();
    return list;
}

int WordAdmin::searchWordTestList(QList<WordTest> list, QString name)
{
    int ret = -1;
    for (int i = 0; i < list.count(); ++i)
    {
        if (list.at(i).m_info.m_name == name)
        {
            ret = i;
            break;
        }
    }
    return ret;
}

WordAdmin::WordAdmin(QObject *parent) : QObject(parent)
{
    m_currentNum = 0;
    m_currentTop = 0;
    this->initDB();
    if (!this->loadAllWord())
    {
        DEBUG << "loadAllWord fail";
        m_mutex.lockForRead();
    }
    else
    {
        DEBUG << "loadAllWord success";
        checkAllWordTimesDecline();
    }
}

void WordAdmin::initDB()
{
    //Name,Times,ModifyTime,Groupid,RememberState,IsPhrase,PhoneticSymbol,VoiceFile,Adj_Chinese,Adj_English,Adv_Chinese,Adv_English,Vt_Chinese,Vt_English,Vi_Chinese,Vi_English,PastTense,PastParticiple,PresentParticiple,ThirdPersonSingular,Noun_Chinese,Noun_English,Prep_Chinese,Prep_English,Conj_Chinese,Conj_English,Pron_Chinese,Pron_English,Art_Chinese,Art_English,Num_Chinese,Num_English,Int_Chinese,Int_English,ExampleSentence1,ExampleSentence2,ExampleSentence3,ExampleSentence4,ExampleSentence5,ExampleSentence6,ExampleSentence7,ExampleSentence8,Synonym,Antonym,Derivative
    if (!QFile(WTool::getWordDBFilePath()).exists())
    {
        WTool::writeFileInfo(WTool::getWordDBFilePath(), "");
    }
    m_mutex.lockForWrite();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(WTool::getWordDBFilePath());
    if (db.open())
    {
        DEBUG << "db open success";
        QStringList existTables = db.tables();
        if (!existTables.contains("WordLibrary"))
        {
            QString sql = QString::fromUtf8(u8R"(
CREATE TABLE WordLibrary(%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, %13,
%14, %15, %16, %17, %18, %19, %20, %21, %22, %23, %24, %25, %26, %27, %28, %29,
%30, %31, %32, %33, %34, %35, %36, %37, %38, %39, %40, %41, %42, %43, %44, %45)
)")
                .arg("[Name] nvarchar")                 //1
                .arg("[Times] integer")                 //2
                .arg("[ModifyTime] nvarchar")           //3
                .arg("[Groupid] integer")               //4
                .arg("[RememberState] integer")         //5
                .arg("[IsPhrase] integer")              //6
                .arg("[PhoneticSymbol] nvarchar")       //7
                .arg("[VoiceFile] nvarchar")            //8
                .arg("[Adj_Chinese] nvarchar")          //9
                .arg("[Adj_English] nvarchar")          //10
                .arg("[Adv_Chinese] nvarchar")          //11
                .arg("[Adv_English] nvarchar")          //12
                .arg("[Vt_Chinese] nvarchar")           //13
                .arg("[Vt_English] nvarchar")           //14
                .arg("[Vi_Chinese] nvarchar")           //15
                .arg("[Vi_English] nvarchar")           //16
                .arg("[PastTense] nvarchar")            //17
                .arg("[PastParticiple] nvarchar")       //18
                .arg("[PresentParticiple] nvarchar")    //19
                .arg("[ThirdPersonSingular] nvarchar")  //20
                .arg("[Noun_Chinese] nvarchar")         //21
                .arg("[Noun_English] nvarchar")         //22
                .arg("[Prep_Chinese] nvarchar")         //23
                .arg("[Prep_English] nvarchar")         //24
                .arg("[Conj_Chinese] nvarchar")         //25
                .arg("[Conj_English] nvarchar")         //26
                .arg("[Pron_Chinese] nvarchar")         //27
                .arg("[Pron_English] nvarchar")         //28
                .arg("[Art_Chinese] nvarchar")          //29
                .arg("[Art_English] nvarchar")          //30
                .arg("[Num_Chinese] nvarchar")          //31
                .arg("[Num_English] nvarchar")          //32
                .arg("[Int_Chinese] nvarchar")          //33
                .arg("[Int_English] nvarchar")          //34
                .arg("[ExampleSentence1] nvarchar")     //35
                .arg("[ExampleSentence2] nvarchar")     //36
                .arg("[ExampleSentence3] nvarchar")     //37
                .arg("[ExampleSentence4] nvarchar")     //38
                .arg("[ExampleSentence5] nvarchar")     //39
                .arg("[ExampleSentence6] nvarchar")     //40
                .arg("[ExampleSentence7] nvarchar")     //41
                .arg("[ExampleSentence8] nvarchar")     //42
                .arg("[Synonym] nvarchar")              //43
                .arg("[Antonym] nvarchar")              //44
                .arg("[Derivative] nvarchar");          //45
            QSqlQuery query;
            if (query.exec(sql))
            {
                DEBUG << "DB init success";
                if (query.exec("create index index_word on WordLibrary(Name);"))
                {
                    DEBUG << "index init success";
                }
                else
                {
                    DEBUG << "index init fail";
                }
            }
            else
                DEBUG << "DB init fail";
        }
    }
    else
    {
        DEBUG << "db open fail";
    }
    m_mutex.unlock();
}

bool WordAdmin::loadAllWord()
{
    m_mutex.lockForWrite();
    m_currentNum = 0;
    m_currentTop = 0;
    QSqlQuery query;
    if (query.exec("select * from WordLibrary"))
    {
        int i = 0;
        while (query.next())
        {
            m_wordLib[i].m_name = query.value(0).toString();
            m_wordLib[i].m_times = query.value(1).toInt();
            m_wordLib[i].m_modifyTime = QDateTime::fromString(query.value(2).toString(), TIMEFORMAT);
            m_wordLib[i].m_groupId = query.value(3).toInt();
            m_wordLib[i].m_remember = query.value(4).toInt();
            m_wordLib[i].m_isPhrase = query.value(5).toInt() == 1;
            i++;
            if (i >= MAX_WORD_NUM)
                break;
        }
        m_currentNum = i;
        m_currentTop = i;
        m_mutex.unlock();
        return true;
    }
    m_mutex.unlock();
    return false;
}

int WordAdmin::checkAllWordTimesDecline()
{
    int num = 0;
    m_mutex.lockForWrite();
    QDateTime cur = QDateTime::currentDateTime();
    QSqlQuery query;
    QString sql;
    for (int i = 0; i < m_currentTop; ++i)
    {
        if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && m_wordLib[i].m_remember <= 0 &&
            WTool::timesCanDecline(m_wordLib[i].m_times, m_wordLib[i].m_modifyTime, cur))
        {
            WTool::memoryDecline(m_wordLib[i].m_times, m_wordLib[i].m_modifyTime, cur);
            m_wordLib[i].m_remember = -1;
            sql = QString("update WordLibrary set Times='%1', ModifyTime='%2', "
                "RememberState='%3' where Name='%4'")
                .arg(m_wordLib[i].m_times).arg(m_wordLib[i].m_modifyTime.toString(TIMEFORMAT))
                .arg(m_wordLib[i].m_remember).arg(m_wordLib[i].m_name);
            if (query.exec(sql))
            {
                num++;
//                DEBUG << m_wordLib[i].m_times << " checktimes change success";
            }
//            else
//                DEBUG << m_wordLib[i].m_times << " checktimes change fail";
        }
    }
    m_mutex.unlock();
    return num;
}

bool WordAdmin::readWordInfo(QString name, WordInfo *word)
{
    QSqlQuery query;
    QString sql = QString("select * from WordLibrary where Name='%1'").arg(name);
    if (query.exec(sql) && query.next())
    {
        word->m_name = query.value(0).toString();
        word->m_times = query.value(1).toInt();
        word->m_modifyTime = QDateTime::fromString(query.value(2).toString(), TIMEFORMAT);
        word->m_groupid = query.value(3).toInt();
        word->m_remember = query.value(4).toInt();
        word->m_isPhrase = query.value(5).toInt() == 1;
        word->m_phoneticSymbol = query.value(6).toString();
        word->m_voiceFile = query.value(7).toString();
        word->m_adj_Chinese = query.value(8).toString();
        word->m_adj_English = query.value(9).toString();
        word->m_adv_Chinese = query.value(10).toString();
        word->m_adv_English = query.value(11).toString();
        word->m_vt_Chinese = query.value(12).toString();
        word->m_vt_English = query.value(13).toString();
        word->m_vi_Chinese = query.value(14).toString();
        word->m_vi_English = query.value(15).toString();
        word->m_pastTense = query.value(16).toString();
        word->m_pastParticiple = query.value(17).toString();
        word->m_presentParticiple = query.value(18).toString();
        word->m_thirdPersonSingular = query.value(19).toString();
        word->m_noun_Chinese = query.value(20).toString();
        word->m_noun_English = query.value(21).toString();
        word->m_prep_Chinese = query.value(22).toString();
        word->m_prep_English = query.value(23).toString();
        word->m_conj_Chinese = query.value(24).toString();
        word->m_conj_English = query.value(25).toString();
        word->m_pron_Chinese = query.value(26).toString();
        word->m_pron_English = query.value(27).toString();
        word->m_art_Chinese = query.value(28).toString();
        word->m_art_English = query.value(29).toString();
        word->m_num_Chinese = query.value(30).toString();
        word->m_num_English = query.value(31).toString();
        word->m_int_Chinese = query.value(32).toString();
        word->m_int_English = query.value(33).toString();
        word->m_exampleSentence[0] = query.value(34).toString();
        word->m_exampleSentence[1] = query.value(35).toString();
        word->m_exampleSentence[2] = query.value(36).toString();
        word->m_exampleSentence[3] = query.value(37).toString();
        word->m_exampleSentence[4] = query.value(38).toString();
        word->m_exampleSentence[5] = query.value(39).toString();
        word->m_exampleSentence[6] = query.value(40).toString();
        word->m_exampleSentence[7] = query.value(41).toString();
        word->m_synonym = query.value(42).toString();
        word->m_antonym = query.value(43).toString();
        word->m_derivative = query.value(44).toString();
        return true;
    }
    return false;
}

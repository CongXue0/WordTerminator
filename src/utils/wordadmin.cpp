#include "wordadmin.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "wtool.h"
#include <QProgressDialog>
#include <QCoreApplication>

BriefWordInfo WordAdmin::m_wordLib[MAX_WORD_NUM] = {0};
WordAdmin *WordAdmin::m_wordAdmin = NULL;

void BriefWordInfo::init()
{
    m_name = WORD_NAME_UNDEFINED;
    m_times = -1;
    m_remember = 0;
    m_isPhrase = false;
}

QString BriefWordInfo::toString()
{
    return QString("name:%1, times:%2, modify:%3, remember:%4").arg(m_name)
        .arg(m_times).arg(m_modifyTime.toString(TIMEFORMAT)).arg(m_remember);
}

void WordInfo::init()
{
    m_name = WORD_NAME_UNDEFINED;
    m_times = -1;
    m_remember = 0;
    m_isPhrase = false;;
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
    m_exampleSentence[0] = "";
    m_exampleSentence[1] = "";
    m_exampleSentence[2] = "";
    m_exampleSentence[3] = "";
    m_exampleSentence[4] = "";
    m_exampleSentence[5] = "";
    m_synonym = "";
    m_antonym = "";
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
    for (int i = 0, j = 0; i < 6; i++)
    {
        m_exampleSentence[i] = m_exampleSentence[i].trimmed();
        if (m_exampleSentence[i].isEmpty() == false)
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
}

QString WordInfo::toText()
{
    QString text;
    if (m_name.isEmpty() == true)
        return "";
    else
        text.append(m_name + "\n");
    if (m_phoneticSymbol.isEmpty() == false)
        text.append(m_phoneticSymbol + "\n");
    if (m_adj_Chinese.isEmpty() == false)
        text.append("adj. " + m_adj_Chinese + "\n");
    if (m_adj_English.isEmpty() == false)
        text.append("adj. " + m_adj_English + "\n");
    if (m_adv_Chinese.isEmpty() == false)
        text.append("adv. " + m_adv_Chinese + "\n");
    if (m_adv_English.isEmpty() == false)
        text.append("adv. " + m_adv_English + "\n");
    if (m_vt_Chinese.isEmpty() == false)
        text.append("vt. " + m_vt_Chinese + "\n");
    if (m_vt_English.isEmpty() == false)
        text.append("vt. " + m_vt_English + "\n");
    if (m_vi_Chinese.isEmpty() == false)
        text.append("vi. " + m_vi_Chinese + "\n");
    if (m_vi_English.isEmpty() == false)
        text.append("vi. " + m_vi_English + "\n");
    if (m_pastTense.isEmpty() == false)
        text.append("过去式: " + m_pastTense + "  ");
    if (m_pastParticiple.isEmpty() == false)
        text.append("过去分词: " + m_pastParticiple + "\n");
    if (m_presentParticiple.isEmpty() == false)
        text.append("现在分词: " + m_presentParticiple + "  ");
    if (m_thirdPersonSingular.isEmpty() == false)
        text.append("第三人称单数: " + m_thirdPersonSingular + "\n");
    if (m_noun_Chinese.isEmpty() == false)
        text.append("n. " + m_noun_Chinese + "\n");
    if (m_noun_English.isEmpty() == false)
        text.append("n. " + m_noun_English + "\n");
    if (m_prep_Chinese.isEmpty() == false)
        text.append("prep. " + m_prep_Chinese + "\n");
    if (m_prep_English.isEmpty() == false)
        text.append("prep. " + m_prep_English + "\n");
    if (m_conj_Chinese.isEmpty() == false)
        text.append("conj. " + m_conj_Chinese + "\n");
    if (m_conj_English.isEmpty() == false)
        text.append("conj. " + m_conj_English + "\n");
    if (m_pron_Chinese.isEmpty() == false)
        text.append("pron. " + m_pron_Chinese + "\n");
    if (m_pron_English.isEmpty() == false)
        text.append("pron. " + m_pron_English + "\n");
    if (m_art_Chinese.isEmpty() == false)
        text.append("art. " + m_art_Chinese + "\n");
    if (m_art_English.isEmpty() == false)
        text.append("art. " + m_art_English + "\n");
    for (int i = 0; i < 6; i++)
    {
        if (m_exampleSentence[i].isEmpty() == true)
            break;
        text.append(QString::number(i + 1) + ". " + m_exampleSentence[i] + "\n");
    }
    if (m_synonym.isEmpty() == false)
    {
        text.append("同义词: ");
        QStringList list = m_synonym.split(';');
        for (int i = 0; i < list.count(); i++)
        {
            if (i ==  list.count() - 1)
                text.append(QString(list.at(i)).trimmed());
            else
                text.append(QString(list.at(i)).trimmed() + " ");
        }
        text.append("\n");
    }
    if (m_antonym.isEmpty() == false)
    {
        text.append("反义词: ");
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
    if (text.length() > 0 && text.at(text.length() - 1) != '\n')
        text.append("\n");
    return text;
}

bool WordInfo::contains(const QString &str) const
{
    if (str.isEmpty() == true)
        return true;
    return m_adj_Chinese.contains(str) || m_adj_English.contains(str) || m_adv_Chinese.contains(str) || m_adv_English.contains(str) ||
        m_vt_Chinese.contains(str) || m_vt_English.contains(str) || m_vi_Chinese.contains(str) || m_vi_English.contains(str) ||
        m_noun_Chinese.contains(str) || m_noun_English.contains(str) || m_prep_Chinese.contains(str) || m_prep_English.contains(str) ||
        m_conj_Chinese.contains(str) || m_conj_English.contains(str) || m_pron_Chinese.contains(str) || m_pron_English.contains(str) ||
        m_art_Chinese.contains(str) || m_art_English.contains(str);
}

WordAdmin *WordAdmin::getInstance()
{
    if (m_wordAdmin == NULL)
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
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name == name)
        {
            bool ret = false;
            if (word != NULL)
            {
                ret = readWordInfo(name, word);
            }
            if (offset != NULL)
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
    if (word == NULL || word->m_name == WORD_NAME_UNDEFINED)
        return false;
    m_mutex.lockForWrite();
    int j = -1;
    for (int i = 0; i < m_currentTop; i++)
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
    QSqlQuery query;
    QString sql = QString("insert into WordLibrary values(\"%1\", \"%2\", \"%3\", \"%4\", \"%5\", \"%6\", "
        "\"%7\", \"%8\", \"%9\", \"%10\", \"%11\", \"%12\", \"%13\", \"%14\", \"%15\", \"%16\", \"%17\", \"%18\", \"%19\", "
        "\"%20\", \"%21\", \"%22\", \"%23\", \"%24\", \"%25\", \"%26\", \"%27\", \"%28\", \"%29\", \"%30\", \"%31\", "
        "\"%32\", \"%33\", \"%34\", \"%35\", \"%36\", \"%37\")")
        .arg(word->m_name).arg(word->m_times).arg(word->m_modifyTime.toString(TIMEFORMAT)).arg(word->m_remember)
        .arg(((word->m_isPhrase == true) ? 1 : 0)).arg(word->m_phoneticSymbol).arg(word->m_voiceFile)
        .arg(word->m_adj_Chinese).arg(word->m_adj_English).arg(word->m_adv_Chinese).arg(word->m_adv_English)
        .arg(word->m_vt_Chinese).arg(word->m_vt_English).arg(word->m_vi_Chinese).arg(word->m_vi_English)
        .arg(word->m_pastTense).arg(word->m_pastParticiple).arg(word->m_presentParticiple)
        .arg(word->m_thirdPersonSingular).arg(word->m_noun_Chinese).arg(word->m_noun_English)
        .arg(word->m_prep_Chinese).arg(word->m_prep_English).arg(word->m_conj_Chinese).arg(word->m_conj_English)
        .arg(word->m_pron_Chinese).arg(word->m_pron_English).arg(word->m_art_Chinese).arg(word->m_art_English)
        .arg(word->m_exampleSentence[0]).arg(word->m_exampleSentence[1]).arg(word->m_exampleSentence[2])
        .arg(word->m_exampleSentence[3]).arg(word->m_exampleSentence[4]).arg(word->m_exampleSentence[5])
        .arg(word->m_synonym).arg(word->m_antonym);
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
    if (word == NULL || word->m_name == WORD_NAME_UNDEFINED)
        return false;
    m_mutex.lockForWrite();
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name == word->m_name)
        {
            QSqlQuery query;
            QString sql = QString("update WordLibrary set Times=\"%1\", ModifyTime=\"%2\", RememberState=\"%3\", IsPhrase=\"%4\", "
                "PhoneticSymbol=\"%5\", VoiceFile=\"%6\", Adj_Chinese=\"%7\", Adj_English=\"%8\", Adv_Chinese=\"%9\", "
                "Adv_English=\"%10\", Vt_Chinese=\"%11\", Vt_English=\"%12\", Vi_Chinese=\"%13\", Vi_English=\"%14\", "
                "PastTense=\"%15\", PastParticiple=\"%16\", PresentParticiple=\"%17\", ThirdPersonSingular=\"%18\", "
                "Noun_Chinese=\"%19\", Noun_English=\"%20\", Prep_Chinese=\"%21\", Prep_English=\"%22\", Conj_Chinese=\"%23\", "
                "Conj_English=\"%24\", Pron_Chinese=\"%25\", Pron_English=\"%26\", Art_Chinese=\"%27\", Art_English=\"%28\", "
                "ExampleSentence1=\"%29\", ExampleSentence2=\"%30\", ExampleSentence3=\"%31\", ExampleSentence4=\"%32\", "
                "ExampleSentence5=\"%33\", ExampleSentence6=\"%34\", Synonym=\"%35\", Antonym=\"%36\" where Name=\"%37\"")
                .arg(word->m_times).arg(word->m_modifyTime.toString(TIMEFORMAT)).arg(word->m_remember)
                .arg(((word->m_isPhrase == true) ? 1 : 0)).arg(word->m_phoneticSymbol).arg(word->m_voiceFile)
                .arg(word->m_adj_Chinese).arg(word->m_adj_English).arg(word->m_adv_Chinese).arg(word->m_adv_English)
                .arg(word->m_vt_Chinese).arg(word->m_vt_English).arg(word->m_vi_Chinese).arg(word->m_vi_English)
                .arg(word->m_pastTense).arg(word->m_pastParticiple).arg(word->m_presentParticiple).arg(word->m_thirdPersonSingular)
                .arg(word->m_noun_Chinese).arg(word->m_noun_English).arg(word->m_prep_Chinese).arg(word->m_prep_English)
                .arg(word->m_conj_Chinese).arg(word->m_conj_English).arg(word->m_pron_Chinese).arg(word->m_pron_English)
                .arg(word->m_art_Chinese).arg(word->m_art_English).arg(word->m_exampleSentence[0]).arg(word->m_exampleSentence[1])
                .arg(word->m_exampleSentence[2]).arg(word->m_exampleSentence[3]).arg(word->m_exampleSentence[4])
                .arg(word->m_exampleSentence[5]).arg(word->m_synonym).arg(word->m_antonym).arg(word->m_name);
//            qDebug() << sql;
            if (query.exec(sql) == true)
            {
                m_wordLib[i].m_times = word->m_times;
                m_wordLib[i].m_modifyTime = word->m_modifyTime;
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
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name == name)
        {
            QSqlQuery query;
            QString sql = QString("update WordLibrary set %1=\"%2\" where Name=\"%3\"")
                .arg(field).arg(value).arg(name);
            if (query.exec(sql) == true)
            {
                if (field == "Times")
                    m_wordLib[i].m_times = value.toInt();
                else if (field == "ModifyTime")
                    m_wordLib[i].m_modifyTime = QDateTime::fromString(value, TIMEFORMAT);
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
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name == name)
        {
            QSqlQuery query;
            QString sql = QString("update WordLibrary set %1=\"%2\", %3=\"%4\" where Name=\"%5\"")
                .arg(field1).arg(value1).arg(field2).arg(value2).arg(name);
            if (query.exec(sql) == true)
            {
                if (field1 == "Times")
                    m_wordLib[i].m_times = value1.toInt();
                else if (field1 == "ModifyTime")
                    m_wordLib[i].m_modifyTime = QDateTime::fromString(value1, TIMEFORMAT);
                else if (field1 == "RememberState")
                    m_wordLib[i].m_remember = value1.toInt();
                else if (field1 == "IsPhrase")
                    m_wordLib[i].m_isPhrase = value1 == "1";

                if (field2 == "Times")
                    m_wordLib[i].m_times = value2.toInt();
                else if (field2 == "ModifyTime")
                    m_wordLib[i].m_modifyTime = QDateTime::fromString(value2, TIMEFORMAT);
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
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name == name)
        {
            QSqlQuery query;
            QString sql = QString("update WordLibrary set %1=\"%2\", %3=\"%4\", %5=\"%6\" where Name=\"%7\"")
                .arg(field1).arg(value1).arg(field2).arg(value2).arg(field3).arg(value3).arg(name);
            if (query.exec(sql) == true)
            {
                if (field1 == "Times")
                    m_wordLib[i].m_times = value1.toInt();
                else if (field1 == "ModifyTime")
                    m_wordLib[i].m_modifyTime = QDateTime::fromString(value1, TIMEFORMAT);
                else if (field1 == "RememberState")
                    m_wordLib[i].m_remember = value1.toInt();
                else if (field1 == "IsPhrase")
                    m_wordLib[i].m_isPhrase = value1 == "1";

                if (field2 == "Times")
                    m_wordLib[i].m_times = value2.toInt();
                else if (field2 == "ModifyTime")
                    m_wordLib[i].m_modifyTime = QDateTime::fromString(value2, TIMEFORMAT);
                else if (field2 == "RememberState")
                    m_wordLib[i].m_remember = value2.toInt();
                else if (field2 == "IsPhrase")
                    m_wordLib[i].m_isPhrase = value2 == "1";

                if (field3 == "Times")
                    m_wordLib[i].m_times = value3.toInt();
                else if (field3 == "ModifyTime")
                    m_wordLib[i].m_modifyTime = QDateTime::fromString(value3, TIMEFORMAT);
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
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name == name)
        {
            QSqlQuery query;
            QString sql = QString("delete from WordLibrary where Name=\"%1\"").arg(name);
            if (query.exec(sql) == true)
            {
                qDebug() << "delete " + name + " success";
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
    for (int i = 0; i < m_currentTop; i++)
    {
        dialog.setValue(i);
        QCoreApplication::processEvents();
        if (dialog.wasCanceled() == true)
            break;
        if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED &&
            WordAdmin::wordCanMemorize(m_wordLib[i].m_remember, m_wordLib[i].m_modifyTime) == false)
        {
            m_wordLib[i].m_remember = (m_wordLib[i].m_remember > 0 ? 2 : -1);
            sql = QString("update WordLibrary set RememberState=\"%1\" where Name=\"%2\"")
                .arg(QString::number(m_wordLib[i].m_remember)).arg(m_wordLib[i].m_name);
            if (query.exec(sql) == true)
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
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name == name)
        {
            if (offset != NULL)
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
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name == name)
        {
            if (word != NULL)
            {
                *word = m_wordLib[i];
            }
            if (offset != NULL)
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
    if (ret == true)
        m_mutex.unlock();
    return ret;
}

bool WordAdmin::wordCanMemorize(int remState, QDateTime modifyTime)
{
    if ((remState == -1 || remState == 2) ||
        (modifyTime.secsTo(QDateTime::currentDateTime()) >= WTool::getMemoryInterval()))
        return true;
    else
        return false;
}

int WordAdmin::getWordNumFromTimes(int t1, int t2, bool isRemember)
{
    int num = 0;
    if (t1 < 0 || t2 < 0 || t1 > t2)
        return num;
    m_mutex.lockForRead();
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && (m_wordLib[i].m_remember > 0) == isRemember &&
            (m_wordLib[i].m_times >= t1 && m_wordLib[i].m_times <= t2))
            num++;
    }
    m_mutex.unlock();
    return num;
}

QStringList WordAdmin::getWordListFromTimes(int t1, int t2, bool isRemember)
{
    QStringList list;
    if (t1 < 0 || t2 < 0 || t1 > t2)
        return list;
    m_mutex.lockForRead();
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && (m_wordLib[i].m_remember > 0) == isRemember &&
            (m_wordLib[i].m_times >= t1 && m_wordLib[i].m_times <= t2))
            list.append(m_wordLib[i].m_name);
    }
    m_mutex.unlock();
    return list;
}

QStringList WordAdmin::getAllWordList()
{
    QStringList list;
    m_mutex.lockForRead();
    if (m_currentNum <= 0)
    {
        m_mutex.unlock();
        return list;
    }
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED)
            list.append(m_wordLib[i].m_name);
    }
    m_mutex.unlock();
    return list;
}

QList<BriefWordInfo> WordAdmin::getWordListWithinTime(int minutes)
{
    QList<BriefWordInfo> list;
    m_mutex.lockForRead();
    QDateTime end = QDateTime::fromTime_t(QDateTime::currentDateTime().toTime_t() + minutes * 60);
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED &&
            WTool::timesCanDecline(m_wordLib[i].m_times, m_wordLib[i].m_modifyTime, end) == true)
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
    QDateTime end = QDateTime::fromTime_t(QDateTime::currentDateTime().toTime_t() + minutes * 60);
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && (m_wordLib[i].m_remember > 0) == isRemember &&
            (m_wordLib[i].m_remember != -1 && m_wordLib[i].m_remember != 2))
        {
            if (WTool::timesCanDecline(m_wordLib[i].m_times, m_wordLib[i].m_modifyTime, end) == true)
            {
                list.append(m_wordLib[i]);
            }
        }
    }
    m_mutex.unlock();
    return list;
}

QList<WordTest> WordAdmin::getAllWordCanMemorizeList()
{
    QList<WordTest> list;
    m_mutex.lockForRead();
    QDateTime cur = QDateTime::currentDateTime();
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED &&
            ((m_wordLib[i].m_remember == -1 || m_wordLib[i].m_remember == 2) ||
            m_wordLib[i].m_modifyTime.secsTo(cur) >= WTool::getMemoryInterval()))
        {
            WordTest test;
            test.m_info = m_wordLib[i];
            list.append(test);
        }
    }
    m_mutex.unlock();
    return list;
}

QList<WordTest> WordAdmin::getWordCanMemorizeListFromTimes(int t1, int t2, bool isRemember)
{
    QList<WordTest> list;
    if (t1 < 0 || t2 < 0 || t1 > t2)
        return list;
    m_mutex.lockForRead();
    QDateTime cur = QDateTime::currentDateTime();
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && (m_wordLib[i].m_remember > 0) == isRemember)
        {
            if (m_wordLib[i].m_times >= t1 && m_wordLib[i].m_times <= t2 &&
                ((m_wordLib[i].m_remember == -1 || m_wordLib[i].m_remember == 2) ||
                m_wordLib[i].m_modifyTime.secsTo(cur) >= WTool::getMemoryInterval()))
            {
                WordTest test;
                test.m_info = m_wordLib[i];
                list.append(test);
            }
        }
    }
    m_mutex.unlock();
    return list;
}

int WordAdmin::getWordCanMemorizeNumFromTimes(int t1, int t2, bool isRemember)
{
    int num = 0;
    if (t1 < 0 || t2 < 0 || t1 > t2)
        return num;
    m_mutex.lockForRead();
    QDateTime cur = QDateTime::currentDateTime();
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && (m_wordLib[i].m_remember > 0) == isRemember &&
            (m_wordLib[i].m_times >= t1 && m_wordLib[i].m_times <= t2))
        {
//            qDebug() << m_wordLib[i].toString();
//            QString info = m_wordLib[i].toString();
            if ((m_wordLib[i].m_remember == -1 || m_wordLib[i].m_remember == 2) ||
                m_wordLib[i].m_modifyTime.secsTo(cur) >= WTool::getMemoryInterval())
            {
                num++;
//                info += " yes";
            }
            else
            {
//                qDebug() << m_wordLib[i].toString();
            }
//            qDebug() << info;
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
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED)
        {
            uint sec = m_wordLib[i].m_modifyTime.secsTo(cur);
            if (sec < WTool::getMemoryInterval() && sec >= (WTool::getMemoryInterval() - minutes * 60))
            {
                list.append(m_wordLib[i]);
            }
        }
    }
    m_mutex.unlock();
    return list;
}

int WordAdmin::searchWordTestList(QList<WordTest> &list, QString name)
{
    int ret = -1;
    for (int i = 0; i < list.count(); i++)
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
    if (this->loadAllWord() == false)
    {
        qDebug() << "loadAllWord fail";
        m_mutex.lockForRead();
    }
    else
    {
        qDebug() << "loadAllWord success";
        checkAllWordTimesDecline();
    }
}

void WordAdmin::initDB()
{
    m_mutex.lockForWrite();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(WTool::getWordDBFilePath());
    if (db.open() == true)
    {
        qDebug() << "db open success";
        QStringList existTables = db.tables();
        if (existTables.contains("WordLibrary") == false)
        {
            QString sql = QString("CREATE TABLE WordLibrary(%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, "
                "%11, %12, %13, %14, %15, %16, %17, %18, %19, %20, %21, %22, %23, %24, %25, %26, %27, "
                "%28, %29, %30, %31, %32, %33, %34, %35, %36, %37)")
                .arg("[Name] nvarchar")                 //1
                .arg("[Times] integer")                 //2
                .arg("[ModifyTime] nvarchar")           //3
                .arg("[RememberState] integer")         //4
                .arg("[IsPhrase] integer")              //5
                .arg("[PhoneticSymbol] nvarchar")       //6
                .arg("[VoiceFile] nvarchar")            //7
                .arg("[Adj_Chinese] nvarchar")          //8
                .arg("[Adj_English] nvarchar")          //9
                .arg("[Adv_Chinese] nvarchar")          //10
                .arg("[Adv_English] nvarchar")          //11
                .arg("[Vt_Chinese] nvarchar")           //12
                .arg("[Vt_English] nvarchar")           //13
                .arg("[Vi_Chinese] nvarchar")           //14
                .arg("[Vi_English] nvarchar")           //15
                .arg("[PastTense] nvarchar")            //16
                .arg("[PastParticiple] nvarchar")       //17
                .arg("[PresentParticiple] nvarchar")    //18
                .arg("[ThirdPersonSingular] nvarchar")  //19
                .arg("[Noun_Chinese] nvarchar")         //20
                .arg("[Noun_English] nvarchar")         //21
                .arg("[Prep_Chinese] nvarchar")         //22
                .arg("[Prep_English] nvarchar")         //23
                .arg("[Conj_Chinese] nvarchar")         //24
                .arg("[Conj_English] nvarchar")         //25
                .arg("[Pron_Chinese] nvarchar")         //26
                .arg("[Pron_English] nvarchar")         //27
                .arg("[Art_Chinese] nvarchar")          //28
                .arg("[Art_English] nvarchar")          //29
                .arg("[ExampleSentence1] nvarchar")     //30
                .arg("[ExampleSentence2] nvarchar")     //31
                .arg("[ExampleSentence3] nvarchar")     //32
                .arg("[ExampleSentence4] nvarchar")     //33
                .arg("[ExampleSentence5] nvarchar")     //34
                .arg("[ExampleSentence6] nvarchar")     //35
                .arg("[Synonym] nvarchar")              //36
                .arg("[Antonym] nvarchar");             //37
            QSqlQuery query;
            if (query.exec(sql) == true)
                qDebug() << "DB init success";
            else
                qDebug() << "DB init fail";
        }
    }
    else
    {
        qDebug() << "db open fail";
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
        while (query.next() == true)
        {
            m_wordLib[i].m_name = query.value(0).toString();
            m_wordLib[i].m_times = query.value(1).toInt();
            m_wordLib[i].m_modifyTime = QDateTime::fromString(query.value(2).toString(), TIMEFORMAT);
            m_wordLib[i].m_remember = query.value(3).toInt();
            m_wordLib[i].m_isPhrase = query.value(4).toInt() == 1;
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
    m_mutex.lockForWrite();
    QDateTime cur = QDateTime::currentDateTime();
    QSqlQuery query;
    QString sql;
    for (int i = 0; i < m_currentTop; i++)
    {
        if (m_wordLib[i].m_name != WORD_NAME_UNDEFINED && m_wordLib[i].m_remember <= 0 &&
            WTool::timesCanDecline(m_wordLib[i].m_times, m_wordLib[i].m_modifyTime, cur) == true)
        {
            WTool::memoryDecline(m_wordLib[i].m_times, m_wordLib[i].m_modifyTime, cur);
            m_wordLib[i].m_remember = -1;
            sql = QString("update WordLibrary set Times=\"%1\", ModifyTime=\"%2\", "
                "RememberState=\"%3\" where Name=\"%4\"")
                .arg(m_wordLib[i].m_times).arg(m_wordLib[i].m_modifyTime.toString(TIMEFORMAT))
                .arg(m_wordLib[i].m_remember).arg(m_wordLib[i].m_name);
            if (query.exec(sql) == true);
//                qDebug() << m_wordLib[i].m_times << " checktimes change success";
//            else
//                qDebug() << m_wordLib[i].m_times << " checktimes change fail";
        }
    }
    m_mutex.unlock();
}

bool WordAdmin::readWordInfo(QString name, WordInfo *word)
{
    QSqlQuery query;
    QString sql = QString("select * from WordLibrary where Name=\"%1\"").arg(name);
    if (query.exec(sql) == true)
    {
        if (query.next() == true)
        {
            word->m_name = query.value(0).toString();
            word->m_times = query.value(1).toInt();
            word->m_modifyTime = QDateTime::fromString(query.value(2).toString(), TIMEFORMAT);
            word->m_remember = query.value(3).toInt();
            word->m_isPhrase = query.value(4).toInt() == 1;
            word->m_phoneticSymbol = query.value(5).toString();
            word->m_voiceFile = query.value(6).toString();
            word->m_adj_Chinese = query.value(7).toString();
            word->m_adj_English = query.value(8).toString();
            word->m_adv_Chinese = query.value(9).toString();
            word->m_adv_English = query.value(10).toString();
            word->m_vt_Chinese = query.value(11).toString();
            word->m_vt_English = query.value(12).toString();
            word->m_vi_Chinese = query.value(13).toString();
            word->m_vi_English = query.value(14).toString();
            word->m_pastTense = query.value(15).toString();
            word->m_pastParticiple = query.value(16).toString();
            word->m_presentParticiple = query.value(17).toString();
            word->m_thirdPersonSingular = query.value(18).toString();
            word->m_noun_Chinese = query.value(19).toString();
            word->m_noun_English = query.value(20).toString();
            word->m_prep_Chinese = query.value(21).toString();
            word->m_prep_English = query.value(22).toString();
            word->m_conj_Chinese = query.value(23).toString();
            word->m_conj_English = query.value(24).toString();
            word->m_pron_Chinese = query.value(25).toString();
            word->m_pron_English = query.value(26).toString();
            word->m_art_Chinese = query.value(27).toString();
            word->m_art_English = query.value(28).toString();
            word->m_exampleSentence[0] = query.value(29).toString();
            word->m_exampleSentence[1] = query.value(30).toString();
            word->m_exampleSentence[2] = query.value(31).toString();
            word->m_exampleSentence[3] = query.value(32).toString();
            word->m_exampleSentence[4] = query.value(33).toString();
            word->m_exampleSentence[5] = query.value(34).toString();
            word->m_synonym = query.value(35).toString();
            word->m_antonym = query.value(36).toString();
            return true;
        }
    }
    return false;
}

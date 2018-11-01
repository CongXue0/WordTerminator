#include "wordcreatewidget.h"
#include "wtool.h"
#include <QDebug>
#include <QMessageBox>
#include <wordadmin.h>
#include <QKeyEvent>
#include <QScrollBar>
#include "global.h"

extern WordAdmin *p_wordAdmin;

WordCreateWidget::WordCreateWidget(QWidget *parent) : QWidget(parent)
{
    m_mode = CREATE;
    m_times = 0;
    m_remember = 0;

    scrollArea = new QScrollArea(this);
    scrollArea->setObjectName("scrollArea");
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    widget = new QWidget(this);
    widget->setObjectName("widget");
    scrollArea->setWidget(widget);

    //QLabel
    label_word = new QLabel(widget);
    label_word->setObjectName("label_word");
    label_word->setText("单词或词组");

    label_group = new QLabel(widget);
    label_group->setObjectName("label_group");
    label_group->setText("组：");

    label_phoneticSymbol = new QLabel(widget);
    label_phoneticSymbol->setObjectName("label_phoneticSymbol");
    label_phoneticSymbol->setText("音标");

    label_adj_Chinese = new QLabel(widget);
    label_adj_Chinese->setObjectName("label_adj_Chinese");
    label_adj_Chinese->setText("(中文)adj.");
    label_adj_English = new QLabel(widget);
    label_adj_English->setObjectName("label_adj_English");
    label_adj_English->setText("(英文)adj.");

    label_adv_Chinese = new QLabel(widget);
    label_adv_Chinese->setObjectName("label_adv_Chinese");
    label_adv_Chinese->setText("(中文)adv.");
    label_adv_English = new QLabel(widget);
    label_adv_English->setObjectName("label_adv_English");
    label_adv_English->setText("(英文)adv.");

    label_vt_Chinese = new QLabel(widget);
    label_vt_Chinese->setObjectName("label_vt_Chinese");
    label_vt_Chinese->setText("(中文)vt.");
    label_vt_English = new QLabel(widget);
    label_vt_English->setObjectName("label_vt_English");
    label_vt_English->setText("(英文)vt.");

    label_vi_Chinese = new QLabel(widget);
    label_vi_Chinese->setObjectName("label_vi_Chinese");
    label_vi_Chinese->setText("(中文)vi.");
    label_vi_English = new QLabel(widget);
    label_vi_English->setObjectName("label_vi_English");
    label_vi_English->setText("(英文)vi.");

    label_v_pastTense = new QLabel(widget);
    label_v_pastTense->setObjectName("label_v_pastTense");
    label_v_pastTense->setText("过去式");
    label_v_pastParticiple = new QLabel(widget);
    label_v_pastParticiple->setObjectName("label_v_pastParticiple");
    label_v_pastParticiple->setText("过去分词");
    label_v_presentParticiple = new QLabel(widget);
    label_v_presentParticiple->setObjectName("label_v_presentParticiple");
    label_v_presentParticiple->setText("现在分词");
    label_v_thirdPersonSingular = new QLabel(widget);
    label_v_thirdPersonSingular->setObjectName("label_v_thirdPersonSingular");
    label_v_thirdPersonSingular->setText("第三人称单数");

    label_noun_Chinese = new QLabel(widget);
    label_noun_Chinese->setObjectName("label_noun_Chinese");
    label_noun_Chinese->setText("(中文)n.");
    label_noun_English = new QLabel(widget);
    label_noun_English->setObjectName("label_noun_English");
    label_noun_English->setText("(英文)n.");

    label_prep_Chinese = new QLabel(widget);
    label_prep_Chinese->setObjectName("label_prep_Chinese");
    label_prep_Chinese->setText("(中文)prep.");
    label_prep_English = new QLabel(widget);
    label_prep_English->setObjectName("label_prep_English");
    label_prep_English->setText("(英文)prep.");

    label_conj_Chinese = new QLabel(widget);
    label_conj_Chinese->setObjectName("label_conj_Chinese");
    label_conj_Chinese->setText("(中文)conj.");
    label_conj_English = new QLabel(widget);
    label_conj_English->setObjectName("label_conj_English");
    label_conj_English->setText("(英文)conj.");

    label_pron_Chinese = new QLabel(widget);
    label_pron_Chinese->setObjectName("label_pron_Chinese");
    label_pron_Chinese->setText("(中文)pron.");
    label_pron_English = new QLabel(widget);
    label_pron_English->setObjectName("label_pron_English");
    label_pron_English->setText("(英文)pron.");

    label_art_Chinese = new QLabel(widget);
    label_art_Chinese->setObjectName("label_art_Chinese");
    label_art_Chinese->setText("(中文)art.");
    label_art_English = new QLabel(widget);
    label_art_English->setObjectName("label_art_English");
    label_art_English->setText("(英文)art.");

    label_exampleSentence = new QLabel(widget);
    label_exampleSentence->setObjectName("label_exampleSentence");
    label_exampleSentence->setText("例句：");

    label_synonym = new QLabel(widget);
    label_synonym->setObjectName("label_synonym");
    label_synonym->setText("同义词");
    label_antonym = new QLabel(widget);
    label_antonym->setObjectName("label_antonym");
    label_antonym->setText("反义词");

    //QLineEdit
    lineEdit_word = new QLineEdit(widget);
    lineEdit_word->setObjectName("lineEdit_word");

    lineEdit_phoneticSymbol = new QLineEdit(widget);
    lineEdit_phoneticSymbol->setObjectName("lineEdit_phoneticSymbol");

    lineEdit_adj_Chinese = new QLineEdit(widget);
    lineEdit_adj_Chinese->setObjectName("lineEdit_adj_Chinese");
    lineEdit_adj_English = new QLineEdit(widget);
    lineEdit_adj_English->setObjectName("lineEdit_adj_English");

    lineEdit_adv_Chinese = new QLineEdit(widget);
    lineEdit_adv_Chinese->setObjectName("lineEdit_adv_Chinese");
    lineEdit_adv_English = new QLineEdit(widget);
    lineEdit_adv_English->setObjectName("lineEdit_adv_English");

    lineEdit_vt_Chinese = new QLineEdit(widget);
    lineEdit_vt_Chinese->setObjectName("lineEdit_vt_Chinese");
    lineEdit_vt_English = new QLineEdit(widget);
    lineEdit_vt_English->setObjectName("lineEdit_vt_English");

    lineEdit_vi_Chinese = new QLineEdit(widget);
    lineEdit_vi_Chinese->setObjectName("lineEdit_vi_Chinese");
    lineEdit_vi_English = new QLineEdit(widget);
    lineEdit_vi_English->setObjectName("lineEdit_vi_English");

    lineEdit_v_pastTense = new QLineEdit(widget);
    lineEdit_v_pastTense->setObjectName("lineEdit_v_pastTense");
    lineEdit_v_pastParticiple = new QLineEdit(widget);
    lineEdit_v_pastParticiple->setObjectName("lineEdit_v_pastParticiple");
    lineEdit_v_presentParticiple = new QLineEdit(widget);
    lineEdit_v_presentParticiple->setObjectName("lineEdit_v_presentParticiple");
    lineEdit_v_thirdPersonSingular = new QLineEdit(widget);
    lineEdit_v_thirdPersonSingular->setObjectName("lineEdit_v_thirdPersonSingular");

    lineEdit_noun_Chinese = new QLineEdit(widget);
    lineEdit_noun_Chinese->setObjectName("lineEdit_noun_Chinese");
    lineEdit_noun_English = new QLineEdit(widget);
    lineEdit_noun_English->setObjectName("lineEdit_noun_English");

    lineEdit_prep_Chinese = new QLineEdit(widget);
    lineEdit_prep_Chinese->setObjectName("lineEdit_prep_Chinese");
    lineEdit_prep_English = new QLineEdit(widget);
    lineEdit_prep_English->setObjectName("lineEdit_prep_English");

    lineEdit_conj_Chinese = new QLineEdit(widget);
    lineEdit_conj_Chinese->setObjectName("lineEdit_conj_Chinese");
    lineEdit_conj_English = new QLineEdit(widget);
    lineEdit_conj_English->setObjectName("lineEdit_conj_English");

    lineEdit_pron_Chinese = new QLineEdit(widget);
    lineEdit_pron_Chinese->setObjectName("lineEdit_pron_Chinese");
    lineEdit_pron_English = new QLineEdit(widget);
    lineEdit_pron_English->setObjectName("lineEdit_pron_English");

    lineEdit_art_Chinese = new QLineEdit(widget);
    lineEdit_art_Chinese->setObjectName("lineEdit_art_Chinese");
    lineEdit_art_English = new QLineEdit(widget);
    lineEdit_art_English->setObjectName("lineEdit_art_English");

    lineEdit_synonym = new QLineEdit(widget);
    lineEdit_synonym->setObjectName("lineEdit_synonym");
    lineEdit_antonym = new QLineEdit(widget);
    lineEdit_antonym->setObjectName("lineEdit_antonym");

    //QCheckBox
    checkBox_phrase = new QCheckBox(widget);
    checkBox_phrase->setObjectName("checkBox_phrase");
    checkBox_phrase->setText("是否是词组");

    //QComboBox
    combox_group = new QComboBox(widget);
    combox_group->setObjectName("combox_group");

    //QTextEdit
    for(int i = 0; i < 6; i++)
    {
        textEdit_exampleSentence[i] = new QTextEdit(widget);
        textEdit_exampleSentence[i]->setObjectName(QString("textEdit_exampleSentence%1").arg(i + 1));
        textEdit_exampleSentence[i]->setPlaceholderText(QString("例句%1").arg(i + 1));
    }

    //QPushButton
    btn_cancel = new QPushButton(widget);
    btn_cancel->setObjectName("btn_cancel");
    connect(btn_cancel, SIGNAL(clicked()), this, SLOT(slot_btnCancel_clicked()));
    btn_confirm = new QPushButton(widget);
    btn_confirm->setObjectName("btn_confirm");
    connect(btn_confirm, SIGNAL(clicked()), this, SLOT(slot_btnConfirm_clicked()));

    m_reloadFlag = true;
    reloadGlobalValue();
    loadStyleSheet();
}

void WordCreateWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return && (event->modifiers() & Qt::ControlModifier))
    {
        slot_btnConfirm_clicked();
    }
    else if (event->key() == Qt::Key_Escape)
    {
        slot_btnCancel_clicked();
    }
}

void WordCreateWidget::recoveryInterface()
{
    scrollArea->verticalScrollBar()->setValue(0);
    lineEdit_word->clear();
    if (m_mode == CREATE)
        lineEdit_word->setReadOnly(false);
    else if (m_mode == MODIFY)
        lineEdit_word->setReadOnly(true);
    lineEdit_phoneticSymbol->clear();
    lineEdit_adj_Chinese->clear();
    lineEdit_adj_English->clear();
    lineEdit_adv_Chinese->clear();
    lineEdit_adv_English->clear();
    lineEdit_vt_Chinese->clear();
    lineEdit_vt_English->clear();
    lineEdit_vi_Chinese->clear();
    lineEdit_vi_English->clear();
    lineEdit_v_pastTense->clear();
    lineEdit_v_pastParticiple->clear();
    lineEdit_v_presentParticiple->clear();
    lineEdit_v_thirdPersonSingular->clear();
    lineEdit_noun_Chinese->clear();
    lineEdit_noun_English->clear();
    lineEdit_prep_Chinese->clear();
    lineEdit_prep_English->clear();
    lineEdit_conj_Chinese->clear();
    lineEdit_conj_English->clear();
    lineEdit_pron_Chinese->clear();
    lineEdit_pron_English->clear();
    lineEdit_art_Chinese->clear();
    lineEdit_art_English->clear();
    lineEdit_synonym->clear();
    lineEdit_antonym->clear();
    checkBox_phrase->setChecked(false);
    for(int i = 0; i < 6; i++)
        textEdit_exampleSentence[i]->clear();
    lineEdit_word->setFocus();
}

void WordCreateWidget::reloadGlobalValue()
{
    if (m_reloadFlag)
    {
        m_reloadFlag = false;
        combox_group->clear();
        m_groupList.clear();
        m_groupList = WTool::getGroupList();
        for (int i = 0; i < m_groupList.count(); i++)
        {
            combox_group->insertItem(i, m_groupList.at(i));
        }
        combox_group->setCurrentIndex(0);
    }
}

void WordCreateWidget::setReloadFlag(bool flag)
{
    m_reloadFlag = flag;
}

void WordCreateWidget::setCreateMode(WordCreateWidget::CREATE_MODE mode)
{
    m_mode = mode;
}

bool WordCreateWidget::loadWordInfo(QString name)
{
    WordInfo word;
    if (p_wordAdmin->getWordInfo(name, &word))
    {
        lineEdit_word->setText(word.m_name);
        m_times = word.m_times;
        m_remember = word.m_remember;
        lineEdit_phoneticSymbol->setText(word.m_phoneticSymbol);
        lineEdit_adj_Chinese->setText(word.m_adj_Chinese);
        lineEdit_adj_English->setText(word.m_adj_English);
        lineEdit_adv_Chinese->setText(word.m_adv_Chinese);
        lineEdit_adv_English->setText(word.m_adv_English);
        lineEdit_vt_Chinese->setText(word.m_vt_Chinese);
        lineEdit_vt_English->setText(word.m_vt_English);
        lineEdit_vi_Chinese->setText(word.m_vi_Chinese);
        lineEdit_vi_English->setText(word.m_vi_English);
        lineEdit_v_pastTense->setText(word.m_pastTense);
        lineEdit_v_pastParticiple->setText(word.m_pastParticiple);
        lineEdit_v_presentParticiple->setText(word.m_presentParticiple);
        lineEdit_v_thirdPersonSingular->setText(word.m_thirdPersonSingular);
        lineEdit_noun_Chinese->setText(word.m_noun_Chinese);
        lineEdit_noun_English->setText(word.m_noun_English);
        lineEdit_prep_Chinese->setText(word.m_prep_Chinese);
        lineEdit_prep_English->setText(word.m_prep_English);
        lineEdit_conj_Chinese->setText(word.m_conj_Chinese);
        lineEdit_conj_English->setText(word.m_conj_English);
        lineEdit_pron_Chinese->setText(word.m_pron_Chinese);
        lineEdit_pron_English->setText(word.m_pron_English);
        lineEdit_art_Chinese->setText(word.m_art_Chinese);
        lineEdit_art_English->setText(word.m_art_English);
        lineEdit_synonym->setText(word.m_synonym);
        lineEdit_antonym->setText(word.m_antonym);
        checkBox_phrase->setChecked(word.m_isPhrase);
        for (int i = 0; i < m_groupList.count(); i++)
        {
            if (Global::m_groupName[word.m_groupid].getValueStr() == m_groupList.at(i))
            {
                combox_group->setCurrentIndex(i);
                break;
            }
        }
        for(int i = 0; i < 6; i++)
            textEdit_exampleSentence[i]->setText(word.m_exampleSentence[i]);
        return true;
    }
    else
        return false;
}

void WordCreateWidget::loadStyleSheet()
{
    setStyleSheet(WTool::getWordCreateWidgetQss());
}

QString WordCreateWidget::inputCheck()
{
    QString info = "";
    if (!checkBox_phrase->isChecked() && lineEdit_word->text().isEmpty())
        info.append("单词不能为空\n");
    else if (checkBox_phrase->isChecked() && lineEdit_word->text().isEmpty())
        info.append("词组不能为空\n");
    if (lineEdit_adj_Chinese->text().trimmed().isEmpty() && lineEdit_adj_English->text().trimmed().isEmpty() &&
        lineEdit_adv_Chinese->text().trimmed().isEmpty() && lineEdit_adv_English->text().trimmed().isEmpty() &&
        lineEdit_vt_Chinese->text().trimmed().isEmpty() && lineEdit_vt_English->text().trimmed().isEmpty() &&
        lineEdit_vi_Chinese->text().trimmed().isEmpty() && lineEdit_vi_English->text().trimmed().isEmpty() &&
        lineEdit_v_pastTense->text().trimmed().isEmpty() && lineEdit_v_pastParticiple->text().trimmed().isEmpty() &&
        lineEdit_v_presentParticiple->text().trimmed().isEmpty() && lineEdit_v_thirdPersonSingular->text().trimmed().isEmpty() &&
        lineEdit_noun_Chinese->text().trimmed().isEmpty() && lineEdit_noun_English->text().trimmed().isEmpty() &&
        lineEdit_prep_Chinese->text().trimmed().isEmpty() && lineEdit_prep_English->text().trimmed().isEmpty() &&
        lineEdit_conj_Chinese->text().trimmed().isEmpty() && lineEdit_conj_English->text().trimmed().isEmpty() &&
        lineEdit_pron_Chinese->text().trimmed().isEmpty() && lineEdit_pron_English->text().trimmed().isEmpty() &&
        lineEdit_art_Chinese->text().trimmed().isEmpty() && lineEdit_art_English->text().trimmed().isEmpty())
        info.append("释义不能全为空\n");
    if (!checkBox_phrase->isChecked() && (!WTool::isLetter(lineEdit_word->text().trimmed()) ||
        lineEdit_word->text().trimmed().contains(' ')))
        info.append("单词输入错误\n");
    else if (checkBox_phrase->isChecked() && (!WTool::isLetter(lineEdit_word->text().trimmed()) ||
        !lineEdit_word->text().trimmed().contains(' ')))
        info.append("词组输入错误\n");
    if (!WTool::isEnglishSentence(lineEdit_adj_English->text(), ';'))
        info.append("adj. 输入错误\n");
    if (!WTool::isEnglishSentence(lineEdit_adv_English->text(), ';'))
        info.append("adv. 输入错误\n");
    if (!WTool::isEnglishSentence(lineEdit_vt_English->text(), ';'))
        info.append("vt. 输入错误\n");
    if (!WTool::isEnglishSentence(lineEdit_vi_English->text(), ';'))
        info.append("vi. 输入错误\n");
    if (!WTool::isLetter(lineEdit_v_pastTense->text()))
        info.append("过去式输入错误\n");
    if (!WTool::isLetter(lineEdit_v_pastParticiple->text()))
        info.append("过去分词输入错误\n");
    if (!WTool::isLetter(lineEdit_v_presentParticiple->text()))
        info.append("现在分词输入错误\n");
    if (!WTool::isLetter(lineEdit_v_thirdPersonSingular->text()))
        info.append("第三人称单数输入错误\n");
    if (!WTool::isEnglishSentence(lineEdit_noun_English->text(), ';'))
        info.append("noun. 输入错误\n");
    if (!WTool::isEnglishSentence(lineEdit_prep_English->text(), ';'))
        info.append("prep. 输入错误\n");
    if (!WTool::isEnglishSentence(lineEdit_conj_English->text(), ';'))
        info.append("conj. 输入错误\n");
    if (!WTool::isEnglishSentence(lineEdit_pron_English->text(), ';'))
        info.append("pron. 输入错误\n");
    if (!WTool::isEnglishSentence(lineEdit_art_English->text(), ';'))
        info.append("art. 输入错误\n");
    if (!WTool::isLetter(lineEdit_synonym->text(), ';'))
        info.append("同义词输入错误\n");
    if (!WTool::isLetter(lineEdit_antonym->text(), ';'))
        info.append("反义词输入错误\n");
    if (!info.isEmpty() && info.at(info.length() - 1) == '\n')
        info = info.mid(0, info.length() - 1);
    return info;
}

void WordCreateWidget::slot_btnCancel_clicked()
{
    emit sendMessageSignal(WMessage("cancel create", ""));
}

void WordCreateWidget::slot_btnConfirm_clicked()
{
    QString checkInfo = inputCheck();
    if (!checkInfo.isEmpty())
    {
        QMessageBox::about(this, "提示", checkInfo);
        return;
    }
    if (m_mode == CREATE)
    {
        if (p_wordAdmin->isWordExist(lineEdit_word->text().trimmed()))
        {
            QMessageBox::about(this, "提示", "无法创建已存在的单词");
            return;
        }
    }
    else if (m_mode == MODIFY)
    {
        if (!p_wordAdmin->isWordExist(lineEdit_word->text().trimmed()))
        {
            QMessageBox::about(this, "提示", "无法编辑不存在的单词");
            return;
        }
    }

    WordInfo wordInfo;
    wordInfo.m_name = lineEdit_word->text();
    if (m_mode == CREATE)
    {
        wordInfo.m_times = 0;
        wordInfo.m_modifyTime = QDateTime::currentDateTime();
        wordInfo.m_remember = -1;
    }
    else if (m_mode == MODIFY)
    {
        wordInfo.m_times = m_times;
        wordInfo.m_modifyTime = QDateTime::currentDateTime();
        wordInfo.m_remember = (m_remember > 0) ? 2 : -1;
    }
    wordInfo.m_isPhrase = checkBox_phrase->isChecked();
    wordInfo.m_phoneticSymbol = lineEdit_phoneticSymbol->text();
    wordInfo.m_voiceFile = "";
    wordInfo.m_adj_Chinese = lineEdit_adj_Chinese->text();
    wordInfo.m_adj_English = lineEdit_adj_English->text();
    wordInfo.m_adv_Chinese = lineEdit_adv_Chinese->text();
    wordInfo.m_adv_English = lineEdit_adv_English->text();
    wordInfo.m_vt_Chinese = lineEdit_vt_Chinese->text();
    wordInfo.m_vt_English = lineEdit_vt_English->text();
    wordInfo.m_vi_Chinese = lineEdit_vi_Chinese->text();
    wordInfo.m_vi_English = lineEdit_vi_English->text();
    wordInfo.m_pastTense = lineEdit_v_pastTense->text();
    wordInfo.m_pastParticiple = lineEdit_v_pastParticiple->text();
    wordInfo.m_presentParticiple = lineEdit_v_presentParticiple->text();
    wordInfo.m_thirdPersonSingular = lineEdit_v_thirdPersonSingular->text();
    wordInfo.m_noun_Chinese = lineEdit_noun_Chinese->text();
    wordInfo.m_noun_English = lineEdit_noun_English->text();
    wordInfo.m_prep_Chinese = lineEdit_prep_Chinese->text();
    wordInfo.m_prep_English = lineEdit_prep_English->text();
    wordInfo.m_conj_Chinese = lineEdit_conj_Chinese->text();
    wordInfo.m_conj_English = lineEdit_conj_English->text();
    wordInfo.m_pron_Chinese = lineEdit_pron_Chinese->text();
    wordInfo.m_pron_English = lineEdit_pron_English->text();
    wordInfo.m_art_Chinese = lineEdit_art_Chinese->text();
    wordInfo.m_art_English = lineEdit_art_English->text();

    for (int i = 0; i < 6; i++)
        wordInfo.m_exampleSentence[i] = textEdit_exampleSentence[i]->toPlainText();
    wordInfo.m_synonym = lineEdit_synonym->text();
    wordInfo.m_antonym = lineEdit_antonym->text();
    wordInfo.arrange();

    if (m_mode == CREATE)
    {
        if (p_wordAdmin->insertWord(&wordInfo))
        {
            QMessageBox::about(this, "提示", wordInfo.m_name + " 创建成功");
            emit sendMessageSignal(WMessage("create success", wordInfo.m_name));
        }
        else
        {
            QMessageBox::about(this, "提示", "单词创建失败");
        }
    }
    else if (m_mode == MODIFY)
    {
        if (p_wordAdmin->updateWord(&wordInfo))
        {
            QMessageBox::about(this, "提示", wordInfo.m_name + " 修改成功");
            emit sendMessageSignal(WMessage("modify success", wordInfo.m_name));
            emit wordTimeIncreaseSignal(wordInfo.m_name);
        }
        else
        {
            QMessageBox::about(this, "提示", "单词修改失败");
        }
    }
}

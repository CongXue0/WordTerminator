#include "wordcreatewidget.h"
#include "ui_wordcreatewidget.h"
#include "wtool.h"
#include "global.h"
#include "wordadmin.h"
#include "dispatcher.h"
#include <QDebug>
#include <QMessageBox>
#include <QKeyEvent>
#include <QScrollBar>

extern WordAdmin *p_wordAdmin;

WordCreateWidget::WordCreateWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordCreateWidget)
{
    setStyleSheet(WTool::getStyleQss("WordCreateWidget"));
    ui->setupUi(this);

    m_edit = false;
    m_times = 0;
    m_remember = 0;

    textEdit_exampleSentence.append(ui->textEdit_exampleSentence0);
    textEdit_exampleSentence.append(ui->textEdit_exampleSentence1);
    textEdit_exampleSentence.append(ui->textEdit_exampleSentence2);
    textEdit_exampleSentence.append(ui->textEdit_exampleSentence3);
    textEdit_exampleSentence.append(ui->textEdit_exampleSentence4);
    textEdit_exampleSentence.append(ui->textEdit_exampleSentence5);

    m_reloadFlag = true;
    reloadGlobalValue();

    connect(ui->btn_cancel, SIGNAL(clicked()), this, SLOT(slot_btnCancel_clicked()));
    connect(ui->btn_confirm, SIGNAL(clicked()), this, SLOT(slot_btnConfirm_clicked()));
}

WordCreateWidget::~WordCreateWidget()
{
    delete ui;
}

void WordCreateWidget::recoveryInterface()
{
    ui->scrollArea->verticalScrollBar()->setValue(0);
    ui->lineEdit_word->clear();
    ui->lineEdit_word->setReadOnly(m_edit);
    ui->lineEdit_phoneticSymbol->clear();
    ui->lineEdit_adj_Chinese->clear();
    ui->lineEdit_adj_English->clear();
    ui->lineEdit_adv_Chinese->clear();
    ui->lineEdit_adv_English->clear();
    ui->lineEdit_vt_Chinese->clear();
    ui->lineEdit_vt_English->clear();
    ui->lineEdit_vi_Chinese->clear();
    ui->lineEdit_vi_English->clear();
    ui->lineEdit_v_pastTense->clear();
    ui->lineEdit_v_pastParticiple->clear();
    ui->lineEdit_v_presentParticiple->clear();
    ui->lineEdit_v_thirdPersonSingular->clear();
    ui->lineEdit_noun_Chinese->clear();
    ui->lineEdit_noun_English->clear();
    ui->lineEdit_prep_Chinese->clear();
    ui->lineEdit_prep_English->clear();
    ui->lineEdit_conj_Chinese->clear();
    ui->lineEdit_conj_English->clear();
    ui->lineEdit_pron_Chinese->clear();
    ui->lineEdit_pron_English->clear();
    ui->lineEdit_art_Chinese->clear();
    ui->lineEdit_art_English->clear();
    ui->lineEdit_synonym->clear();
    ui->lineEdit_antonym->clear();
    ui->checkBox_phrase->setChecked(false);
    for(int i = 0; i < 6; ++i)
        textEdit_exampleSentence[i]->clear();
    ui->lineEdit_word->setFocus();
}

void WordCreateWidget::reloadGlobalValue()
{
    if (m_reloadFlag)
    {
        m_reloadFlag = false;
        ui->combox_group->clear();
        m_groupList.clear();
        m_groupList = WTool::getGroupList();
        for (int i = 0; i < m_groupList.count(); ++i)
        {
            ui->combox_group->insertItem(i, m_groupList.at(i));
        }
        ui->combox_group->setCurrentIndex(0);
    }
}

void WordCreateWidget::setReloadFlag(bool flag)
{
    m_reloadFlag = flag;
}

void WordCreateWidget::setEdit(bool edit)
{
    m_edit = edit;
}

bool WordCreateWidget::loadWordInfo(QString name)
{
    WordInfo word;
    if (p_wordAdmin->getWordInfo(name, &word))
    {
        ui->lineEdit_word->setText(word.m_name);
        m_times = word.m_times;
        m_remember = word.m_remember;
        ui->lineEdit_phoneticSymbol->setText(word.m_phoneticSymbol);
        ui->lineEdit_adj_Chinese->setText(word.m_adj_Chinese);
        ui->lineEdit_adj_English->setText(word.m_adj_English);
        ui->lineEdit_adv_Chinese->setText(word.m_adv_Chinese);
        ui->lineEdit_adv_English->setText(word.m_adv_English);
        ui->lineEdit_vt_Chinese->setText(word.m_vt_Chinese);
        ui->lineEdit_vt_English->setText(word.m_vt_English);
        ui->lineEdit_vi_Chinese->setText(word.m_vi_Chinese);
        ui->lineEdit_vi_English->setText(word.m_vi_English);
        ui->lineEdit_v_pastTense->setText(word.m_pastTense);
        ui->lineEdit_v_pastParticiple->setText(word.m_pastParticiple);
        ui->lineEdit_v_presentParticiple->setText(word.m_presentParticiple);
        ui->lineEdit_v_thirdPersonSingular->setText(word.m_thirdPersonSingular);
        ui->lineEdit_noun_Chinese->setText(word.m_noun_Chinese);
        ui->lineEdit_noun_English->setText(word.m_noun_English);
        ui->lineEdit_prep_Chinese->setText(word.m_prep_Chinese);
        ui->lineEdit_prep_English->setText(word.m_prep_English);
        ui->lineEdit_conj_Chinese->setText(word.m_conj_Chinese);
        ui->lineEdit_conj_English->setText(word.m_conj_English);
        ui->lineEdit_pron_Chinese->setText(word.m_pron_Chinese);
        ui->lineEdit_pron_English->setText(word.m_pron_English);
        ui->lineEdit_art_Chinese->setText(word.m_art_Chinese);
        ui->lineEdit_art_English->setText(word.m_art_English);
        ui->lineEdit_synonym->setText(word.m_synonym);
        ui->lineEdit_antonym->setText(word.m_antonym);
        ui->checkBox_phrase->setChecked(word.m_isPhrase);
        for (int i = 0; i < m_groupList.count(); ++i)
        {
            if (Global::m_groupName[word.m_groupid].getValueStr() == m_groupList.at(i))
            {
                ui->combox_group->setCurrentIndex(i);
                break;
            }
        }
        for(int i = 0; i < 6; ++i)
            textEdit_exampleSentence[i]->setText(word.m_exampleSentence[i]);
        return true;
    }
    else
        return false;
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

QString WordCreateWidget::inputCheck()
{
    QString info = "";
    if (!ui->checkBox_phrase->isChecked() && ui->lineEdit_word->text().isEmpty())
        info.append("单词不能为空\n");
    else if (ui->checkBox_phrase->isChecked() && ui->lineEdit_word->text().isEmpty())
        info.append("词组不能为空\n");
    if (ui->lineEdit_adj_Chinese->text().trimmed().isEmpty() && ui->lineEdit_adj_English->text().trimmed().isEmpty() &&
        ui->lineEdit_adv_Chinese->text().trimmed().isEmpty() && ui->lineEdit_adv_English->text().trimmed().isEmpty() &&
        ui->lineEdit_vt_Chinese->text().trimmed().isEmpty() && ui->lineEdit_vt_English->text().trimmed().isEmpty() &&
        ui->lineEdit_vi_Chinese->text().trimmed().isEmpty() && ui->lineEdit_vi_English->text().trimmed().isEmpty() &&
        ui->lineEdit_v_pastTense->text().trimmed().isEmpty() && ui->lineEdit_v_pastParticiple->text().trimmed().isEmpty() &&
        ui->lineEdit_v_presentParticiple->text().trimmed().isEmpty() && ui->lineEdit_v_thirdPersonSingular->text().trimmed().isEmpty() &&
        ui->lineEdit_noun_Chinese->text().trimmed().isEmpty() && ui->lineEdit_noun_English->text().trimmed().isEmpty() &&
        ui->lineEdit_prep_Chinese->text().trimmed().isEmpty() && ui->lineEdit_prep_English->text().trimmed().isEmpty() &&
        ui->lineEdit_conj_Chinese->text().trimmed().isEmpty() && ui->lineEdit_conj_English->text().trimmed().isEmpty() &&
        ui->lineEdit_pron_Chinese->text().trimmed().isEmpty() && ui->lineEdit_pron_English->text().trimmed().isEmpty() &&
        ui->lineEdit_art_Chinese->text().trimmed().isEmpty() && ui->lineEdit_art_English->text().trimmed().isEmpty())
        info.append("释义不能全为空\n");
    if (!ui->checkBox_phrase->isChecked() && (!WTool::isLetter(ui->lineEdit_word->text().trimmed()) ||
        ui->lineEdit_word->text().trimmed().contains(' ')))
        info.append("单词输入错误\n");
    else if (ui->checkBox_phrase->isChecked() && (!WTool::isLetter(ui->lineEdit_word->text().trimmed()) ||
        !ui->lineEdit_word->text().trimmed().contains(' ')))
        info.append("词组输入错误\n");
    if (!WTool::isEnglishSentence(ui->lineEdit_adj_English->text(), ';'))
        info.append("adj. 输入错误\n");
    if (!WTool::isEnglishSentence(ui->lineEdit_adv_English->text(), ';'))
        info.append("adv. 输入错误\n");
    if (!WTool::isEnglishSentence(ui->lineEdit_vt_English->text(), ';'))
        info.append("vt. 输入错误\n");
    if (!WTool::isEnglishSentence(ui->lineEdit_vi_English->text(), ';'))
        info.append("vi. 输入错误\n");
    if (!WTool::isLetter(ui->lineEdit_v_pastTense->text()))
        info.append("过去式输入错误\n");
    if (!WTool::isLetter(ui->lineEdit_v_pastParticiple->text()))
        info.append("过去分词输入错误\n");
    if (!WTool::isLetter(ui->lineEdit_v_presentParticiple->text()))
        info.append("现在分词输入错误\n");
    if (!WTool::isLetter(ui->lineEdit_v_thirdPersonSingular->text()))
        info.append("第三人称单数输入错误\n");
    if (!WTool::isEnglishSentence(ui->lineEdit_noun_English->text(), ';'))
        info.append("noun. 输入错误\n");
    if (!WTool::isEnglishSentence(ui->lineEdit_prep_English->text(), ';'))
        info.append("prep. 输入错误\n");
    if (!WTool::isEnglishSentence(ui->lineEdit_conj_English->text(), ';'))
        info.append("conj. 输入错误\n");
    if (!WTool::isEnglishSentence(ui->lineEdit_pron_English->text(), ';'))
        info.append("pron. 输入错误\n");
    if (!WTool::isEnglishSentence(ui->lineEdit_art_English->text(), ';'))
        info.append("art. 输入错误\n");
    if (!WTool::isLetter(ui->lineEdit_synonym->text(), ';'))
        info.append("同义词输入错误\n");
    if (!WTool::isLetter(ui->lineEdit_antonym->text(), ';'))
        info.append("反义词输入错误\n");
    if (!info.isEmpty() && info.at(info.length() - 1) == '\n')
        info = info.mid(0, info.length() - 1);
    return info;
}

void WordCreateWidget::slot_btnCancel_clicked()
{
    emit Dispatch(this).signal_sendMessage(WMessage("cancel create", ""));
}

void WordCreateWidget::slot_btnConfirm_clicked()
{
    QString checkInfo = inputCheck();
    if (!checkInfo.isEmpty())
    {
        QMessageBox::about(this, "提示", checkInfo);
        return;
    }
    if (!m_edit)
    {
        if (p_wordAdmin->isWordExist(ui->lineEdit_word->text().trimmed()))
        {
            QMessageBox::about(this, "提示", "无法创建已存在的单词");
            return;
        }
    }
    else
    {
        if (!p_wordAdmin->isWordExist(ui->lineEdit_word->text().trimmed()))
        {
            QMessageBox::about(this, "提示", "无法编辑不存在的单词");
            return;
        }
    }

    WordInfo wordInfo;
    wordInfo.m_name = ui->lineEdit_word->text();
    if (!m_edit)
    {
        wordInfo.m_times = 0;
        wordInfo.m_remember = -1;
    }
    else
    {
        wordInfo.m_times = m_times;
        wordInfo.m_remember = (m_remember > 0) ? 2 : -1;
    }
    wordInfo.m_modifyTime = QDateTime::currentDateTime();
    wordInfo.m_groupid = ui->combox_group->currentIndex();
    wordInfo.m_isPhrase = ui->checkBox_phrase->isChecked();
    wordInfo.m_phoneticSymbol = ui->lineEdit_phoneticSymbol->text();
    wordInfo.m_voiceFile = "";
    wordInfo.m_adj_Chinese = ui->lineEdit_adj_Chinese->text();
    wordInfo.m_adj_English = ui->lineEdit_adj_English->text();
    wordInfo.m_adv_Chinese = ui->lineEdit_adv_Chinese->text();
    wordInfo.m_adv_English = ui->lineEdit_adv_English->text();
    wordInfo.m_vt_Chinese = ui->lineEdit_vt_Chinese->text();
    wordInfo.m_vt_English = ui->lineEdit_vt_English->text();
    wordInfo.m_vi_Chinese = ui->lineEdit_vi_Chinese->text();
    wordInfo.m_vi_English = ui->lineEdit_vi_English->text();
    wordInfo.m_pastTense = ui->lineEdit_v_pastTense->text();
    wordInfo.m_pastParticiple = ui->lineEdit_v_pastParticiple->text();
    wordInfo.m_presentParticiple = ui->lineEdit_v_presentParticiple->text();
    wordInfo.m_thirdPersonSingular = ui->lineEdit_v_thirdPersonSingular->text();
    wordInfo.m_noun_Chinese = ui->lineEdit_noun_Chinese->text();
    wordInfo.m_noun_English = ui->lineEdit_noun_English->text();
    wordInfo.m_prep_Chinese = ui->lineEdit_prep_Chinese->text();
    wordInfo.m_prep_English = ui->lineEdit_prep_English->text();
    wordInfo.m_conj_Chinese = ui->lineEdit_conj_Chinese->text();
    wordInfo.m_conj_English = ui->lineEdit_conj_English->text();
    wordInfo.m_pron_Chinese = ui->lineEdit_pron_Chinese->text();
    wordInfo.m_pron_English = ui->lineEdit_pron_English->text();
    wordInfo.m_art_Chinese = ui->lineEdit_art_Chinese->text();
    wordInfo.m_art_English = ui->lineEdit_art_English->text();

    for (int i = 0; i < 6; ++i)
        wordInfo.m_exampleSentence[i] = textEdit_exampleSentence[i]->toPlainText();
    wordInfo.m_synonym = ui->lineEdit_synonym->text();
    wordInfo.m_antonym = ui->lineEdit_antonym->text();
    wordInfo.arrange();

    if (!m_edit)
    {
        if (p_wordAdmin->insertWord(&wordInfo))
        {
            QMessageBox::about(this, "提示", wordInfo.m_name + " 创建成功");
            emit Dispatch(this).signal_sendMessage(WMessage("create success", wordInfo.m_name));
        }
        else
        {
            QMessageBox::about(this, "提示", "单词创建失败");
        }
    }
    else
    {
        if (p_wordAdmin->updateWord(&wordInfo))
        {
            QMessageBox::about(this, "提示", wordInfo.m_name + " 修改成功");
            emit Dispatch(this).signal_sendMessage(WMessage("modify success", wordInfo.m_name));
            emit Dispatch(this).signal_wordTimeIncrease(wordInfo.m_name);
        }
        else
        {
            QMessageBox::about(this, "提示", "单词修改失败");
        }
    }
}

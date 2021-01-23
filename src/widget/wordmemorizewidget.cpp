#include "wordmemorizewidget.h"
#include "ui_wordmemorizewidget.h"
#include "json.h"
#include "memorythread.h"
#include "global.h"
#include "dispatcher.h"
#include "wordterminator.h"
#include <QDebug>
#include <QKeyEvent>
#include <QScrollBar>

extern WordAdmin *p_wordAdmin;
extern MemoryThread *p_memThread;

enum { SHOW_WORD, SHOW_EXPLAIN, SHOW_INFO };

WordMemorizeWidget::WordMemorizeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordMemorizeWidget)
{
    setStyleSheet(WTool::getStyleQss("WordMemorizeWidget"));
    ui->setupUi(this);

    m_mode = WELCOME;
    m_test = NONE;
    m_testNum = 0;
    m_passNum = 0;
    m_curIndex = -1;
    m_lastWord = "";

    ui->btn_right->setCheckable(true);
    ui->btn_right->setAutoCheck(false);

    ui->copyLabel_WORD->setAlignment(Qt::AlignCenter);

    for (int i = 0; i < PROPERTY_NUM; ++i)
    {
        copyLabel_property[i] = this->findChild<CopyLabel *>(QString("copyLabel_property%1").arg(i));
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        copyLabel_explain[i] = this->findChild<CopyLabel *>(QString("copyLabel_explain%1").arg(i));
        widget_explain[i] = this->findChild<QWidget *>(QString("widget_explain%1").arg(i));
    }
    for (int i = 0; i < EXAMPLE_NUM; ++i)
    {
        copyLabel_example[i] = this->findChild<CopyLabel *>(QString("copyLabel_example%1").arg(i));
    }

    for (int i = 0; i < RELATED_NUM; ++i)
    {
        linkLabel_synonym[i] = this->findChild<LinkLabel *>(QString("linkLabel_synonym%1").arg(i));
    }
    for (int i = 0; i < RELATED_NUM; ++i)
    {
        linkLabel_antonym[i] = this->findChild<LinkLabel *>(QString("linkLabel_antonym%1").arg(i));
    }

    m_reloadFlag = true;
    reloadGlobalValue();

    WTool::LayoutHelper_init(dynamic_cast<QBoxLayout *>(ui->widget_show->layout()));

    connect(ui->checkBox_range0, SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    connect(ui->checkBox_range1, SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    connect(ui->checkBox_range2, SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    connect(ui->checkBox_range3, SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    connect(ui->checkBox_forever, SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    connect(ui->combox_group, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboxGroup_currentIndexChanged(int)));
    connect(ui->btn_start, SIGNAL(clicked()), this, SLOT(slot_btnStart_Clicked()));
    connect(ui->btn_know, SIGNAL(clicked()), this, SLOT(slot_btnKnow_Clicked()));
    connect(ui->btn_notKnow, SIGNAL(clicked()), this, SLOT(slot_btnNotKnow_Clicked()));
    connect(ui->btn_forever, SIGNAL(clicked()), this, SLOT(slot_btnForever_Clicked()));
    connect(ui->btn_notforever, SIGNAL(clicked()), this, SLOT(slot_btnNotForever_Clicked()));
    connect(ui->btn_next, SIGNAL(clicked()), this, SLOT(slot_btnNext_Clicked()));
    connect(ui->btn_submit, SIGNAL(clicked()), this, SLOT(slot_btnSubmit_Clicked()));
}

WordMemorizeWidget::~WordMemorizeWidget()
{
    delete ui;
}

void WordMemorizeWidget::recoveryInterface()
{
    m_test = NONE;
    m_testNum = 0;
    m_passNum = 0;
    m_curIndex = -1;
    m_lastWord = "";
    m_testList.clear();
    m_nameList.clear();

    this->setMode(WELCOME);
    this->setFocus();
}

void WordMemorizeWidget::reloadGlobalValue()
{
    if (m_reloadFlag)
    {
        m_reloadFlag = false;
        QSignalBlocker sb(ui->combox_group);
        ui->combox_group->clear();
        ui->combox_group->addItem(ALL_GROUP);
        QStringList list = WTool::getGroupList();
        for (int i = 0; i < list.count(); ++i)
        {
            ui->combox_group->addItem(list.at(i));
        }
        ui->combox_group->setCurrentIndex(Global::m_groupIndexMemory.getValueInt() + 1);
        m_curGroupId = WTool::getGroupNo(ui->combox_group->currentText());

        ui->checkBox_range0->setText(QString("times %1~%2")
            .arg(Global::m_range1Left.getValueStr()).arg(Global::m_range1Right.getValueStr()));
        ui->checkBox_range1->setText(QString("times %1~%2")
            .arg(Global::m_range2Left.getValueStr()).arg(Global::m_range2Right.getValueStr()));
        ui->checkBox_range2->setText(QString("times %1~%2")
            .arg(Global::m_range3Left.getValueStr()).arg(Global::m_range3Right.getValueStr()));
        ui->checkBox_range3->setText(QString("times %1+").arg(Global::m_range4Left.getValueStr()));

        ui->spinBox_singleNum->setValue(Global::m_singleMemoryNum.getValueInt());
    }
}

void WordMemorizeWidget::setReloadFlag(bool flag)
{
    m_reloadFlag = flag;
}

void WordMemorizeWidget::updateWordStatistics()
{
    int num1 = 0, num2 = 0, num3 = 0;
    if (!ui->checkBox_range0->isChecked() && !ui->checkBox_range1->isChecked() && !ui->checkBox_range2->isChecked() &&
        !ui->checkBox_range3->isChecked() && !ui->checkBox_forever->isChecked())
    {
        num1 += p_wordAdmin->getWordCanMemorizeNumFromTimes(0, MAX_TIMES, m_curGroupId, false);
        num1 += p_wordAdmin->getWordCanMemorizeNumFromTimes(0, MAX_TIMES, m_curGroupId, true);
    }
    else
    {
        if (ui->checkBox_range0->isChecked())
            num1 += p_wordAdmin->getWordCanMemorizeNumFromTimes(Global::m_range1Left.getValueInt(), Global::m_range1Right.getValueInt(), m_curGroupId, ui->checkBox_forever->isChecked());
        if (ui->checkBox_range1->isChecked())
            num1 += p_wordAdmin->getWordCanMemorizeNumFromTimes(Global::m_range2Left.getValueInt(), Global::m_range2Right.getValueInt(), m_curGroupId, ui->checkBox_forever->isChecked());
        if (ui->checkBox_range2->isChecked())
            num1 += p_wordAdmin->getWordCanMemorizeNumFromTimes(Global::m_range3Left.getValueInt(), Global::m_range3Right.getValueInt(), m_curGroupId, ui->checkBox_forever->isChecked());
        if (ui->checkBox_range3->isChecked())
            num1 += p_wordAdmin->getWordCanMemorizeNumFromTimes(Global::m_range4Left.getValueInt(), MAX_TIMES, m_curGroupId, ui->checkBox_forever->isChecked());
    }
    num2 = p_wordAdmin->getWordNumFromTimes(0, MAX_TIMES, m_curGroupId, false);
    num3 = p_wordAdmin->getWordNumFromTimes(0, MAX_TIMES, m_curGroupId, true);

    ui->label_statistics->setText(QString("当前可记单词数：%1\n未记住的单词数：%2\n已记住的单词数：%3")
        .arg(num1).arg(num2).arg(num3));
}

int WordMemorizeWidget::getMode()
{
    return m_mode;
}

void WordMemorizeWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return && (event->modifiers() == Qt::ControlModifier))
    {
        if (m_test == NONE)
        {
            if (ui->btn_start->isVisible())
                slot_btnStart_Clicked();
        }
        else if (m_test == EXPLORATE)
        {
            if (ui->btn_know->isVisible())
                slot_btnKnow_Clicked();
            else if (ui->btn_next->isVisible())
                slot_btnNext_Clicked();
        }
        else if (m_test == RECALL)
        {
            if (ui->btn_submit->isVisible())
                slot_btnSubmit_Clicked();
            else if (ui->btn_next->isVisible())
                slot_btnNext_Clicked();
        }
    }
    else if (event->key() == Qt::Key_Return && (event->modifiers() == Qt::AltModifier))
    {
        if (m_test == EXPLORATE)
        {
            if (ui->btn_notKnow->isVisible())
                slot_btnNotKnow_Clicked();
            else if (ui->btn_forever->isVisible())
                slot_btnForever_Clicked();
        }
        else if (m_test == RECALL)
        {
            if (ui->btn_forever->isVisible())
                slot_btnForever_Clicked();
        }
    }
}

void WordMemorizeWidget::setMode(int mode)
{
    m_mode = mode;
    switch (m_mode)
    {
    case WELCOME:
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case MEMORY:
        ui->stackedWidget->setCurrentIndex(1);

        ui->copyLabel_WORD->clear();
        ui->copyLabel_WORD->hide();
        ui->copyLabel_word->clear();
        ui->copyLabel_word->hide();
        ui->copyLabel_phoneticSymbol->clear();
        ui->copyLabel_phoneticSymbol->hide();
        ui->widget_word->hide();
        for (int i = 0; i < PROPERTY_NUM; ++i)
        {
            copyLabel_property[i]->clear();
            copyLabel_explain[i]->clear();
            widget_explain[i]->hide();
        }
        ui->copyLabel_exampleSentence->clear();
        ui->copyLabel_exampleSentence->hide();
        for (int i = 0; i < EXAMPLE_NUM; ++i)
        {
            copyLabel_example[i]->clear();
            copyLabel_example[i]->hide();
        }
        ui->copyLabel_synonym->clear();
        ui->copyLabel_synonym->hide();
        ui->copyLabel_antonym->clear();
        ui->copyLabel_antonym->hide();
        for (int i = 0; i < RELATED_NUM; ++i)
        {
            linkLabel_synonym[i]->clear();
            linkLabel_synonym[i]->hide();
            linkLabel_antonym[i]->clear();
            linkLabel_antonym[i]->hide();
        }
        ui->widget_synonym0->hide();
        ui->widget_synonym1->hide();
        ui->widget_antonym0->hide();
        ui->widget_antonym1->hide();

        ui->btn_know->hide();
        ui->btn_notKnow->hide();
        ui->btn_next->hide();
        ui->btn_forever->hide();
        ui->btn_notforever->hide();
        ui->btn_right->hide();
        ui->btn_submit->hide();
        ui->label_input->clear();
        ui->label_input->hide();
        ui->lineEdit_input->clear();
        ui->lineEdit_input->hide();
        ui->hline_0->hide();
        break;
    }
}

void WordMemorizeWidget::testListInit()
{
    for (int i = 0; i < m_testList.size(); ++i)
    {
        if (ui->checkBox_test0->isChecked())
        {
            m_testList[i].m_isPass[0] = false;
        }
        if (ui->checkBox_test1->isChecked())
        {
            m_testList[i].m_isPass[1] = false;
        }
    }
}

void WordMemorizeWidget::loadTestInfo()
{
    chooseTestWord();
    if (p_wordAdmin->getWordInfo(m_testList.at(m_curIndex).m_info.m_name, &m_word))
    {
        setMode(MEMORY);

        int r = WTool::rand(0, m_testList.at(m_curIndex).notPassNum() - 1);
        m_test = -1;
        for (int i = 0; i < 3; ++i)
        {
            if (!m_testList.at(m_curIndex).m_isPass[i])
            {
                m_test++;
                if (m_test == r)
                {
                    m_test = i;
                    break;
                }
            }
        }

        switch (m_test)
        {
        case EXPLORATE:
            setWordInfo(false);
            reloadLayout();
            wordInfoShow(SHOW_WORD);
            bottomLayoutSet({{}, { ui->btn_know, ui->btn_notKnow }});
            this->setFocus();
            break;
        case RECALL:
            setWordInfo(true);
            reloadLayout();
            wordInfoShow(SHOW_EXPLAIN);
            bottomLayoutSet({{ ui->label_input, ui->lineEdit_input, ui->hline_0 }, { ui->btn_submit }});
            ui->lineEdit_input->setFocus();
            break;
        case DICTATE:
            break;
        }
    }
    else
        MESSAGE(QString("记忆模式加载 %1 失败").arg(m_testList.at(m_curIndex).m_info.m_name));
}

void WordMemorizeWidget::setWordInfo(bool isShield)
{
    QString tmp;
    if (m_word.m_isPhrase)
        ui->label_input->setText("请输入正确的词组：");
    else
        ui->label_input->setText("请输入正确的单词：");
    ui->copyLabel_WORD->setText(m_word.m_name);
    ui->copyLabel_WORD->show();

    m_lineNum = 1;
    ui->copyLabel_word->setText(m_word.m_name);
    ui->copyLabel_word->show();
    if (!m_word.m_phoneticSymbol.isEmpty())
    {
        ui->copyLabel_phoneticSymbol->setText(m_word.m_phoneticSymbol);
        ui->copyLabel_phoneticSymbol->show();
    }
    ui->widget_word->show();

    int i = 0;
    if (!m_word.m_adj_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("adj.");
        tmp = m_word.m_adj_Chinese;
        copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
        widget_explain[i++]->show();
    }
    if (!m_word.m_adj_English.isEmpty())
    {
        copyLabel_property[i]->setText("adj.");
        tmp = m_word.m_adj_English;
        copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
        i++;
    }
    if (!m_word.m_adv_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("adv.");
        tmp = m_word.m_adv_Chinese;
        copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
        widget_explain[i++]->show();
    }
    if (!m_word.m_adv_English.isEmpty())
    {
        copyLabel_property[i]->setText("adv.");
        tmp = m_word.m_adv_English;
        copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
        widget_explain[i++]->show();
    }
    if (m_word.m_vt_Chinese == m_word.m_vi_Chinese && m_word.m_vt_English == m_word.m_vi_English &&
        (!m_word.m_vt_Chinese.isEmpty() || !m_word.m_vt_English.isEmpty()))
    {
        if (!m_word.m_vt_Chinese.isEmpty())
        {
            copyLabel_property[i]->setText("vt.& vi.");
            tmp = m_word.m_vt_Chinese;
            copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
            widget_explain[i++]->show();
        }
        if (!m_word.m_vt_English.isEmpty())
        {
            copyLabel_property[i]->setText("vt.& vi.");
            tmp = m_word.m_vt_English;
            copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
            widget_explain[i++]->show();
        }
    }
    else
    {
        if (!m_word.m_vt_Chinese.isEmpty())
        {
            copyLabel_property[i]->setText("vt.");
            tmp = m_word.m_vt_Chinese;
            copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
            widget_explain[i++]->show();
        }
        if (!m_word.m_vt_English.isEmpty())
        {
            copyLabel_property[i]->setText("vt.");
            tmp = m_word.m_vt_English;
            copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
            widget_explain[i++]->show();
        }
        if (!m_word.m_vi_Chinese.isEmpty())
        {
            copyLabel_property[i]->setText("vi.");
            tmp = m_word.m_vi_Chinese;
            copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
            widget_explain[i++]->show();
        }
        if (!m_word.m_vi_English.isEmpty())
        {
            copyLabel_property[i]->setText("vi.");
            tmp = m_word.m_vi_English;
            copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
            widget_explain[i++]->show();
        }
    }
    if (!m_word.m_pastTense.isEmpty() || !m_word.m_pastParticiple.isEmpty() ||
        !m_word.m_presentParticiple.isEmpty() || !m_word.m_thirdPersonSingular.isEmpty())
    {
        copyLabel_property[i]->setText("变形");
        if (!m_word.m_pastTense.isEmpty())
            copyLabel_explain[i]->setText(copyLabel_explain[i]->text() + "  过去式: " + m_word.m_pastTense);
        if (!m_word.m_pastParticiple.isEmpty())
            copyLabel_explain[i]->setText(copyLabel_explain[i]->text() + "  过去分词: " + m_word.m_pastParticiple);
        if (!m_word.m_presentParticiple.isEmpty())
            copyLabel_explain[i]->setText(copyLabel_explain[i]->text() + "  现在分词: " + m_word.m_presentParticiple);
        if (!m_word.m_thirdPersonSingular.isEmpty())
            copyLabel_explain[i]->setText(copyLabel_explain[i]->text() + "  第三人称单数: " + m_word.m_thirdPersonSingular);
        copyLabel_explain[i]->setText(copyLabel_explain[i]->text().trimmed());
        widget_explain[i++]->show();
    }
    if (!m_word.m_noun_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("n.");
        tmp = m_word.m_noun_Chinese;
        copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
        widget_explain[i++]->show();
    }
    if (!m_word.m_noun_English.isEmpty())
    {
        copyLabel_property[i]->setText("n.");
        tmp = m_word.m_noun_English;
        copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
        widget_explain[i++]->show();
    }
    if (!m_word.m_prep_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("prep.");
        tmp = m_word.m_prep_Chinese;
        copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
        widget_explain[i++]->show();
    }
    if (!m_word.m_prep_English.isEmpty())
    {
        copyLabel_property[i]->setText("prep.");
        tmp = m_word.m_prep_English;
        copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
        widget_explain[i++]->show();
    }
    if (!m_word.m_conj_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("conj.");
        tmp = m_word.m_conj_Chinese;
        copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
        widget_explain[i++]->show();
    }
    if (!m_word.m_conj_English.isEmpty())
    {
        copyLabel_property[i]->setText("conj.");
        tmp = m_word.m_conj_English;
        copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
        widget_explain[i++]->show();
    }
    if (!m_word.m_pron_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("pron.");
        tmp = m_word.m_pron_Chinese;
        copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
        widget_explain[i++]->show();
    }
    if (!m_word.m_pron_English.isEmpty())
    {
        copyLabel_property[i]->setText("pron.");
        tmp = m_word.m_pron_English;
        copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
        widget_explain[i++]->show();
    }
    if (!m_word.m_art_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("art.");
        tmp = m_word.m_art_Chinese;
        copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
        widget_explain[i++]->show();
    }
    if (!m_word.m_art_English.isEmpty())
    {
        copyLabel_property[i]->setText("art.");
        tmp = m_word.m_art_English;
        copyLabel_explain[i]->setText((isShield ? WTool::shieldWord(tmp, m_word.m_name) : tmp));
        widget_explain[i++]->show();
    }
    m_lineNum += i;

    if (!m_word.m_exampleSentence[0].isEmpty() || !m_word.m_exampleSentence[1].isEmpty() ||
        !m_word.m_exampleSentence[2].isEmpty() || !m_word.m_exampleSentence[3].isEmpty() ||
        !m_word.m_exampleSentence[4].isEmpty() || !m_word.m_exampleSentence[5].isEmpty())
    {
        ui->copyLabel_exampleSentence->setText("例句：");
        ui->copyLabel_exampleSentence->show();
        for (i = 0; i < EXAMPLE_NUM; ++i)
        {
            if (!m_word.m_exampleSentence[i].isEmpty())
            {
                copyLabel_example[i]->setText(QString("%1. %2").arg(i + 1).arg(m_word.m_exampleSentence[i]));
                copyLabel_example[i]->show();
            }
            else
                break;
        }
        m_lineNum += i + 1;
    }
    if (!m_word.m_synonym.isEmpty())
    {
        ui->copyLabel_synonym->setText("同义词：");
        ui->copyLabel_synonym->show();
        QStringList list = m_word.m_synonym.split(';');
        for (i = 0; i < list.size() && i < RELATED_NUM; ++i)
        {
            if (list.at(i) != "")
            {
                linkLabel_synonym[i]->setText(list.at(i).trimmed());
                linkLabel_synonym[i]->show();
            }
        }
        if (i > 0) ui->widget_synonym0->show();
        if (i > 4) ui->widget_synonym1->show();

        if (i > 4)
            m_lineNum += 3;
        else if (i > 0)
            m_lineNum += 2;
    }
    if (!m_word.m_antonym.isEmpty())
    {
        ui->copyLabel_antonym->setText("反义词：");
        ui->copyLabel_antonym->show();
        QStringList list = m_word.m_antonym.split(';');
        for (i = 0; i < list.size() && i < RELATED_NUM; ++i)
        {
            if (list.at(i) != "")
            {
                linkLabel_antonym[i]->setText(list.at(i).trimmed());
                linkLabel_antonym[i]->show();
            }
        }
        if (i > 0) ui->widget_antonym0->show();
        if (i > 4) ui->widget_antonym1->show();

        if (i > 4)
            m_lineNum += 3;
        else if (i > 0)
            m_lineNum += 2;
    }
}

void WordMemorizeWidget::reloadLayout()
{
    auto ma = ui->widget_show->layout()->contentsMargins();
    int sp = ui->widget_show->layout()->spacing();
    int bar_width = m_lineNum > 10 ? ui->scrollArea->verticalScrollBar()->width() : 0;
    int width = ui->scrollArea->width() - bar_width;
    int l_width = width - ma.left() - ma.right();

    int x = ma.left(), y = ma.top(), h;
    QSize s;

    h = ui->copyLabel_WORD->minimumHeight();
    s = ui->scrollArea->size();
    ui->copyLabel_WORD->setGeometry(x, (s.height() - h) / 2, l_width, h);

    h = ui->copyLabel_word->minimumHeight();
    ui->widget_word->setGeometry(x, y, l_width, h);
    ui->copyLabel_word->setGeometry(0, 0, ui->copyLabel_word->sizeHint().width(), h);
    if (ui->copyLabel_phoneticSymbol->isVisible())
        ui->copyLabel_phoneticSymbol->setGeometry(ui->copyLabel_word->width() + ui->widget_word->layout()->spacing(), 0, ui->copyLabel_phoneticSymbol->sizeHint().width(), h);

    x = ma.left(); y += h + sp; h = ui->copyLabel_property0->minimumHeight();
    for (int i = 0; i < PROPERTY_NUM; ++i)
    {
        if (!widget_explain[i]->isVisible()) continue;
        copyLabel_property[i]->setGeometry(0, 0, copyLabel_property[i]->sizeHint().width(), h);
        int ex_x = copyLabel_property[i]->width() + widget_explain[i]->layout()->spacing();
        int ex_width = l_width - copyLabel_property[i]->width() - widget_explain[i]->layout()->spacing();
        copyLabel_explain[i]->setFixedWidth(ex_width);
        s = copyLabel_explain[i]->sizeHint();
        copyLabel_explain[i]->setGeometry(ex_x, 0, ex_width, s.height());

        widget_explain[i]->setGeometry(x, y, l_width, s.height());
        y += s.height() + sp;
    }

    if (ui->copyLabel_exampleSentence->isVisible())
    {
        ui->copyLabel_exampleSentence->setGeometry(x, y, l_width, h);
        y += h + sp;
        for (int i = 0; i < EXAMPLE_NUM; ++i)
        {
            if (!copyLabel_example[i]->isVisible()) continue;
            copyLabel_example[i]->setFixedWidth(l_width);
            s = copyLabel_example[i]->sizeHint();
            copyLabel_example[i]->setGeometry(x, y, l_width, s.height());
            y += s.height() + sp;
        }
    }

    if (ui->copyLabel_synonym->isVisible())
    {
        ui->copyLabel_synonym->setGeometry(x, y, l_width, h);
        y += h + sp;

        int t_x = 0;
        if (ui->widget_synonym0->isVisible())
        {
            ui->widget_synonym0->setGeometry(x, y, l_width, h);
            y += h + sp;
            for (int i = 0; i < 4; ++i)
            {
                if (!linkLabel_synonym[i]->isVisible()) continue;
                linkLabel_synonym[i]->setGeometry(t_x, 0, linkLabel_synonym[i]->sizeHint().width(), h);
                t_x += linkLabel_synonym[i]->width() + ui->widget_synonym0->layout()->spacing();
            }
            t_x = 0;
        }
        if (ui->widget_synonym1->isVisible())
        {
            ui->widget_synonym1->setGeometry(x, y, l_width, h);
            y += h + sp;
            for (int i = 4; i < RELATED_NUM; ++i)
            {
                if (!linkLabel_synonym[i]->isVisible()) continue;
                linkLabel_synonym[i]->setGeometry(t_x, 0, linkLabel_synonym[i]->sizeHint().width(), h);
                t_x += linkLabel_synonym[i]->width() + ui->widget_synonym1->layout()->spacing();
            }
        }
    }

    if (ui->copyLabel_antonym->isVisible())
    {

        ui->copyLabel_antonym->setGeometry(x, y, l_width, h);
        y += h + sp;

        int t_x = 0;
        if (ui->widget_antonym0->isVisible())
        {
            ui->widget_antonym0->setGeometry(x, y, l_width, h);
            y += h + sp;
            for (int i = 0; i < 4; ++i)
            {
                if (!linkLabel_antonym[i]->isVisible()) continue;
                linkLabel_antonym[i]->setGeometry(t_x, 0, linkLabel_antonym[i]->sizeHint().width(), h);
                t_x += linkLabel_antonym[i]->width() + ui->widget_antonym0->layout()->spacing();
            }
            t_x = 0;
        }
        if (ui->widget_antonym1->isVisible())
        {
            ui->widget_antonym1->setGeometry(x, y, l_width, h);
            y += h + sp;
            for (int i = 4; i < RELATED_NUM; ++i)
            {
                if (!linkLabel_antonym[i]->isVisible()) continue;
                linkLabel_antonym[i]->setGeometry(t_x, 0, linkLabel_antonym[i]->sizeHint().width(), h);
                t_x += linkLabel_antonym[i]->width() + ui->widget_antonym1->layout()->spacing();
            }
        }
    }

    y = y - sp + ma.bottom();
    if (y < ui->scrollArea->height())
        y = ui->scrollArea->height();
//    ui->widget_show->setFixedSize(width, y);
    m_container_height_ = y;
}

void WordMemorizeWidget::bottomLayoutSet(QVector<QVector<QWidget *>> w_set)
{
    if (w_set.size() != 2) return;
    QVector<QWidget *> views = { ui->label_input, ui->lineEdit_input, ui->hline_0, ui->btn_right, ui->btn_know, ui->btn_notKnow,
        ui->btn_forever, ui->btn_notforever, ui->btn_next, ui->btn_submit };

    QVector<QWidget *> views_2;
    for (int i = 0; i < w_set.size(); ++i)
    {
        for (int j = 0; j < w_set[i].size(); ++j)
        {
            views_2.append(w_set[i][j]);
        }
    }
    for (int i = 0; i < views.size(); ++i)
    {
        if (views_2.contains(views[i]))
            views[i]->show();
        else
            views[i]->hide();
    }

    QMargins ma(9, 9, 9, 9);
    QSize s1 = ui->widget_bottom->size(), s;
    int x = 0, y = 0, w = 0, h = 0, spacing = 6;
    int l_width = s1.width() - ma.left() - ma.right();
    QVector<QWidget *> set1 = w_set[0];
    QVector<QWidget *> set2 = w_set[1];
    if (!set1.isEmpty())
    {
        s = ui->label_input->sizeHint();
        w = s.width();
        h = s.height();
        x = ma.left();
        y = s1.height() / 2 - 9 - h - 50;
        ui->label_input->setGeometry(x, y, w, h);

        x += w;
        w = set1.size() == 4 ? (l_width - w - 50 - spacing) : (l_width - w);
        ui->lineEdit_input->setGeometry(x, y, w, h);
        ui->hline_0->setGeometry(x, y + 50, w, 1);
        if (set1.size() == 4)
        {
            x += w + spacing;
            ui->btn_right->setGeometry(x, y, h, h);
        }
    }
    if (!set2.isEmpty())
    {
        int num = set2.size();
        bool right_show = false;
        if (set2[set2.size() - 1] == ui->btn_right)
        {
            right_show = true;
            num--;
            if (num == 0) return;
        }
        if (num == 1)
        {
            s = set2[0]->minimumSize();
            w = s.width();
            h = s.height();
            x = (s1.width() - s.width()) / 2;
            y = s1.height() / 2 + 9 - 50;
            set2[0]->setGeometry(x, y, w, h);
        }
        else if (num == 2)
        {
            s = set2[0]->minimumSize();
            w = s.width();
            h = s.height();
            x = s1.width() / 2 - 27 - w;
            y = s1.height() / 2 + 9 - 50;
            set2[0]->setGeometry(x, y, w, h);

            x += w + 27;
            s = set2[1]->minimumSize();
            w = s.width();
            h = s.height();
            set2[1]->setGeometry(x, y, w, h);
        }
        if (right_show)
        {
            x += w + 18;
            ui->btn_right->setGeometry(x, y, h, h);
        }
    }
}

void WordMemorizeWidget::wordInfoShow(int mode)
{
    if (mode == SHOW_WORD)
    {
        ui->copyLabel_WORD->show();

        ui->widget_word->hide();
        for (int i = 0; i < PROPERTY_NUM; ++i)
        {
            widget_explain[i]->hide();
        }
        ui->copyLabel_exampleSentence->hide();
        for (int i = 0; i < EXAMPLE_NUM; ++i)
        {
            copyLabel_example[i]->hide();
        }
        ui->copyLabel_synonym->hide();
        ui->copyLabel_antonym->hide();
        ui->widget_synonym0->hide();
        ui->widget_synonym1->hide();
        ui->widget_antonym0->hide();
        ui->widget_antonym1->hide();

        ui->widget_show->setFixedHeight(ui->scrollArea->size().height());
    }
    else if (mode == SHOW_EXPLAIN)
    {
        ui->copyLabel_WORD->hide();

        ui->widget_word->hide();
        for (int i = 0; i < PROPERTY_NUM; ++i)
        {
            if (copyLabel_property[i]->text().isEmpty()) continue;
            if (copyLabel_property[i]->text() != "变形")
                widget_explain[i]->show();
            else
                widget_explain[i]->hide();
        }
        ui->copyLabel_exampleSentence->hide();
        for (int i = 0; i < EXAMPLE_NUM; ++i)
        {
            copyLabel_example[i]->hide();
        }
        ui->copyLabel_synonym->hide();
        ui->copyLabel_antonym->hide();
        ui->widget_synonym0->hide();
        ui->widget_synonym1->hide();
        ui->widget_antonym0->hide();
        ui->widget_antonym1->hide();

        ui->widget_show->setFixedHeight(m_container_height_);
    }
    else if (mode == SHOW_INFO)
    {
        ui->copyLabel_WORD->hide();

        ui->widget_word->show();
        if (!ui->copyLabel_phoneticSymbol->text().isEmpty())
            ui->copyLabel_phoneticSymbol->show();
        for (int i = 0; i < PROPERTY_NUM; ++i)
        {
            if (copyLabel_property[i]->text().isEmpty()) continue;
            widget_explain[i]->show();
        }
        if (!ui->copyLabel_exampleSentence->text().isEmpty())
            ui->copyLabel_exampleSentence->show();
        for (int i = 0; i < EXAMPLE_NUM; ++i)
        {
            if (copyLabel_example[i]->text().isEmpty()) continue;
            copyLabel_example[i]->show();
        }
        if (!ui->copyLabel_synonym->text().isEmpty())
            ui->copyLabel_synonym->show();
        if (!ui->copyLabel_antonym->text().isEmpty())
            ui->copyLabel_antonym->show();
        if (!linkLabel_synonym[0]->text().isEmpty())
            ui->widget_synonym0->show();
        if (!linkLabel_synonym[4]->text().isEmpty())
            ui->widget_synonym1->show();
        if (!linkLabel_antonym[0]->text().isEmpty())
            ui->widget_antonym0->show();
        if (!linkLabel_antonym[4]->text().isEmpty())
            ui->widget_antonym1->show();

        ui->widget_show->setFixedHeight(m_container_height_);
    }
}

void WordMemorizeWidget::chooseTestWord()
{
    switch (Global::m_memoryLevel.getValueInt())
    {
    case STRATEGY0:
        chooseStrategy_level0();
        break;
    case STRATEGY1:
        chooseStrategy_level1();
        break;
    default:
        chooseStrategy_level0();
        break;
    }
}

void WordMemorizeWidget::chooseStrategy_level0()
{
    if (m_nameList.count() == 0)
    {
        if (m_testNum > 15)
        {
            for (int i = 0; i < 10; ++i)
            {
                int r = WTool::rand(0, m_testNum - 1);
                while (m_nameList.contains(m_testList.at(r).m_info.m_name))
                {
                    r++;
                    if (r >= m_testNum)
                        r = 0;
                }
                m_nameList.append(m_testList.at(r).m_info.m_name);
            }
            DEBUG<<"NEW:"<<m_nameList;
            QString curWord;
            do
            {
                curWord = m_nameList.at(WTool::rand(0, m_nameList.count() - 1));
            } while (m_lastWord == curWord && m_nameList.count() > 1);
            m_curIndex = p_wordAdmin->searchWordTestList(m_testList, curWord);
        }
        else
        {
            int lastIndex = p_wordAdmin->searchWordTestList(m_testList, m_lastWord);
            do
            {
                m_curIndex = WTool::rand(0, m_testNum - 1);
            } while (lastIndex == m_curIndex && m_testNum > 1);
        }
    }
    else
    {
        QString curWord;
        do
        {
            curWord = m_nameList.at(WTool::rand(0, m_nameList.count() - 1));
        } while (m_lastWord == curWord && m_nameList.count() > 1);
        m_curIndex = p_wordAdmin->searchWordTestList(m_testList, curWord);
        DEBUG<<"curWord:"<<curWord;
    }
}

void WordMemorizeWidget::chooseStrategy_level1()
{
    int lastIndex = p_wordAdmin->searchWordTestList(m_testList, m_lastWord);
    do
    {
        m_curIndex = WTool::rand(0, m_testNum - 1);
    } while (lastIndex == m_curIndex && m_testNum > 1);
}

void WordMemorizeWidget::slot_radioButtonClicked()
{
    this->updateWordStatistics();
}

void WordMemorizeWidget::slot_comboxGroup_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    m_curGroupId = WTool::getGroupNo(ui->combox_group->currentText());
    this->updateWordStatistics();
    Global::m_groupIndexMemory.setValue(m_curGroupId);
    Global::saveXML();
}

void WordMemorizeWidget::slot_btnStart_Clicked()
{
    if (ui->spinBox_singleNum->value() != Global::m_singleMemoryNum.getValueInt())
    {
        Global::m_singleMemoryNum.setValue(ui->spinBox_singleNum->value());
        Global::saveXML();
    }

    m_testList.clear();
    m_nameList.clear();
    if (!ui->checkBox_range0->isChecked() && !ui->checkBox_range1->isChecked() && !ui->checkBox_range2->isChecked() &&
        !ui->checkBox_range3->isChecked() && !ui->checkBox_forever->isChecked())
    {
        m_testList = p_wordAdmin->getAllWordCanMemorizeList(m_curGroupId, 2);
    }
    else
    {
        if (ui->checkBox_range0->isChecked())
            m_testList += p_wordAdmin->getWordCanMemorizeListFromTimes(Global::m_range1Left.getValueInt(), Global::m_range1Right.getValueInt(), m_curGroupId, ui->checkBox_forever->isChecked(), 2);
        if (ui->checkBox_range1->isChecked())
            m_testList += p_wordAdmin->getWordCanMemorizeListFromTimes(Global::m_range2Left.getValueInt(), Global::m_range2Right.getValueInt(), m_curGroupId, ui->checkBox_forever->isChecked(), 2);
        if (ui->checkBox_range2->isChecked())
            m_testList += p_wordAdmin->getWordCanMemorizeListFromTimes(Global::m_range3Left.getValueInt(), Global::m_range3Right.getValueInt(), m_curGroupId, ui->checkBox_forever->isChecked(), 2);
        if (ui->checkBox_range3->isChecked())
            m_testList += p_wordAdmin->getWordCanMemorizeListFromTimes(Global::m_range4Left.getValueInt(), MAX_TIMES, m_curGroupId, ui->checkBox_forever->isChecked(), 2);
    }
    if (m_testList.size() > 0)
    {
        m_testList = m_testList.mid(0, Global::m_singleMemoryNum.getValueInt());

//        p_memThread->start();
        if (ui->checkBox_test0->isChecked() || ui->checkBox_test1->isChecked())
        {
            m_testNum = m_testList.size();
            m_passNum = 0;
            ui->label_info->setText(QString("已记忆：%1    未记忆：%2").arg(m_passNum).arg(m_testNum));
            this->testListInit();
            DEBUG << "m_testNum:" << m_testNum;
            this->loadTestInfo();
        }
        else
            MESSAGE("没有选择记忆模式");
    }
    else
        MESSAGE("没有要记忆的单词");
}

void WordMemorizeWidget::slot_btnKnow_Clicked()
{
    wordInfoShow(SHOW_INFO);

    m_testList[m_curIndex].m_isPass[m_test] = true;
    ui->btn_right->setChecked(true);
    if (m_testList[m_curIndex].isPass())
    {
        m_lastWord = "";
        m_word.m_times++;
        if (m_word.m_times >= Global::m_leastForeverTimes.getValueInt() && m_word.m_remember <= 0)
        {
            bottomLayoutSet({{}, { ui->btn_forever, ui->btn_next, ui->btn_right }});
        }
        else if (m_word.m_remember > 0)
        {
            bottomLayoutSet({{}, { ui->btn_notforever, ui->btn_next, ui->btn_right }});
        }
        else
        {
            bottomLayoutSet({{}, { ui->btn_next, ui->btn_right }});
        }

        m_testNum--;
        m_passNum++;
        ui->label_info->setText(QString("已记忆：%1    未记忆：%2").arg(m_passNum).arg(m_testNum));

        m_word.m_modifyTime = QDateTime::currentDateTime();
        if (m_word.m_remember == -1 || m_word.m_remember == 2)
        {
            m_word.m_remember = (m_word.m_remember > 0) ? 1 : 0;
            p_wordAdmin->updateWord(&m_word);
        }
        else
        {
            p_wordAdmin->updateWord(m_word.m_name, "Times", QString::number(m_word.m_times), "ModifyTime",
                m_word.m_modifyTime.toString(TIMEFORMAT));
        }
        m_nameList.removeAll(m_testList.at(m_curIndex).m_info.m_name);
        DEBUG<<"name:"<<m_testList.at(m_curIndex).m_info.m_name<<", remove:"<<m_nameList;
        m_testList.removeAt(m_curIndex);
        emit Dispatch(this).signal_wordTimeIncrease(m_word.m_name);
    }
    else
    {
        m_lastWord = m_testList.at(m_curIndex).m_info.m_name;
        bottomLayoutSet({{}, { ui->btn_next, ui->btn_right }});
    }
    this->setFocus();
}

void WordMemorizeWidget::slot_btnNotKnow_Clicked()
{
    m_lastWord = m_testList.at(m_curIndex).m_info.m_name;
    wordInfoShow(SHOW_INFO);

    ui->btn_right->setChecked(false);
    bottomLayoutSet({{}, { ui->btn_next, ui->btn_right }});

    this->setFocus();
}

void WordMemorizeWidget::slot_btnForever_Clicked()
{
    p_wordAdmin->updateWord(m_word.m_name, "RememberState", "1");
    if (m_testNum == 0)
    {
//        p_memThread->stop();
//        p_memThread->wait();
        MESSAGE("本次记忆单词数 " + QString::number(m_passNum));
        this->recoveryInterface();
        this->updateWordStatistics();
    }
    else
    {
        loadTestInfo();
    }
}

void WordMemorizeWidget::slot_btnNotForever_Clicked()
{
    p_wordAdmin->updateWord(m_word.m_name, "RememberState", "0");
    if (m_testNum == 0)
    {
//        p_memThread->stop();
//        p_memThread->wait();
        MESSAGE("本次记忆单词数 " + QString::number(m_passNum));
        this->recoveryInterface();
        this->updateWordStatistics();
    }
    else
    {
        loadTestInfo();
    }
}

void WordMemorizeWidget::slot_btnNext_Clicked()
{
    if (m_testNum == 0)
    {
//        p_memThread->stop();
//        p_memThread->wait();
        MESSAGE("本次记忆单词数 " + QString::number(m_passNum));
        this->recoveryInterface();
        this->updateWordStatistics();
    }
    else
    {
        loadTestInfo();
    }
}

void WordMemorizeWidget::slot_btnSubmit_Clicked()
{
    wordInfoShow(SHOW_INFO);
    if (ui->lineEdit_input->text() == m_word.m_name)
    {
        m_testList[m_curIndex].m_isPass[m_test] = true;
        if (m_testList[m_curIndex].isPass())
        {
            m_lastWord = "";
            m_word.m_times++;
            if (m_word.m_times >= Global::m_leastForeverTimes.getValueInt() && m_word.m_remember <= 0)
            {
                ui->btn_right->setChecked(true);
                bottomLayoutSet({{}, { ui->btn_forever, ui->btn_next, ui->btn_right }});
            }
            else if (m_word.m_remember > 0)
            {
                ui->btn_right->setChecked(true);
                bottomLayoutSet({{}, { ui->btn_notforever, ui->btn_next, ui->btn_right }});
            }
            else
            {
                ui->btn_right->setChecked(true);
                bottomLayoutSet({{}, { ui->btn_next, ui->btn_right }});
            }

            m_testNum--;
            m_passNum++;
            ui->label_info->setText(QString("已记忆：%1    未记忆：%2").arg(m_passNum).arg(m_testNum));

            m_word.m_modifyTime = QDateTime::currentDateTime();
            if (m_word.m_remember == -1 || m_word.m_remember == 2)
            {
                m_word.m_remember = (m_word.m_remember > 0) ? 1 : 0;
                p_wordAdmin->updateWord(&m_word);
            }
            else
            {
                p_wordAdmin->updateWord(m_word.m_name, "Times", QString::number(m_word.m_times), "ModifyTime",
                    m_word.m_modifyTime.toString(TIMEFORMAT));
            }
            m_nameList.removeAll(m_testList.at(m_curIndex).m_info.m_name);
            DEBUG<<"name:"<<m_testList.at(m_curIndex).m_info.m_name<<", remove:"<<m_nameList;
            m_testList.removeAt(m_curIndex);
            emit Dispatch(this).signal_wordTimeIncrease(m_word.m_name);
        }
        else
        {
            m_lastWord = m_testList.at(m_curIndex).m_info.m_name;
            ui->btn_right->setChecked(true);
            bottomLayoutSet({{}, { ui->btn_next, ui->btn_right }});
        }
    }
    else
    {
        m_lastWord = m_testList.at(m_curIndex).m_info.m_name;
        ui->btn_right->setChecked(false);
        bottomLayoutSet({{}, { ui->btn_next, ui->btn_right }});
    }
    this->setFocus();
}

//void WordMemorizeWidget::slot_wordCanMemorize(QString name)
//{
//    if (name.isEmpty() || name == WORD_NAME_UNDEFINED)
//        return;
//    for (int i = 0; i < m_testList.count(); ++i)
//    {
//        if (m_testList.at(i).m_info.m_name == name)
//            return;
//    }

//    WordTest test(2);
//    if (p_wordAdmin->getWordBriefInfo(name, &test.m_info))
//    {
//        int times = test.m_info.m_times;
//        if ((test.m_info.m_remember > 0) != ui->checkBox_forever->isChecked() ||
//            ((ui->checkBox_range0->isChecked() && (times < Global::m_range1Left.getValueInt() || times > Global::m_range1Right.getValueInt())) &&
//            (ui->checkBox_range1->isChecked() && (times < Global::m_range2Left.getValueInt() || times > Global::m_range2Right.getValueInt())) &&
//            (ui->checkBox_range2->isChecked() && (times < Global::m_range3Left.getValueInt() || times > Global::m_range3Right.getValueInt())) &&
//            (ui->checkBox_range3->isChecked() && (times < Global::m_range4Left.getValueInt() || times > MAX_TIMES))))
//        {
//            return;
//        }
//        if (ui->checkBox_test0->isChecked())
//        {
//            test.m_isPass[0] = false;
//        }
//        if (ui->checkBox_test1->isChecked())
//        {
//            test.m_isPass[1] = false;
//        }
//        m_testList.append(test);
//        m_testNum++;
//        ui->label_info->setText(QString("已记忆：%1    未记忆：%2").arg(m_passNum).arg(m_testNum));
//    }
//    else
//        DEBUG << "wordCanMemorize get word fail";
//}

//void WordMemorizeWidget::slot_wordTimeDecline(QString name)
//{
//    if (m_testList.at(m_curIndex).m_info.m_name != name)
//    {
//        for (int i = 0; i < m_testList.size(); ++i)
//        {
//            if (m_testList.at(i).m_info.m_name == name)
//            {
//                m_testList[i].m_info.m_times--;
//                break;
//            }
//        }
//    }
//}

void WordMemorizeWidget::slot_stopWordMemorize(bool *ret)
{
    if (WordTerminator::instance()->getCurrentWidgetIndex() != WordTerminator::Widget_WordMemorize) return;
    if (QUESTION_ISYES("是否停止记忆?"))
    {
//        p_memThread->stop();
//        p_memThread->wait();
        if (ret != nullptr)
        {
            *ret = true;
        }
    }
    else
    {
        if (ret != nullptr)
            *ret = false;
    }
}

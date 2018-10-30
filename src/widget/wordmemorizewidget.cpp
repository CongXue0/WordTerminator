#include "wordmemorizewidget.h"
#include "wtool.h"
#include <QMessageBox>
#include <QDebug>
#include "json.h"
#include "memorythread.h"
#include <QKeyEvent>
#include "global.h"

extern WordAdmin *p_wordAdmin;
extern MemoryThread *p_memThread;

WordMemorizeWidget::WordMemorizeWidget(QWidget *parent) : QWidget(parent)
{
    m_mode = WELCOME;
    m_test = NONE;
    m_propertyNum = 0;
    m_exampleNum = 0;
    m_synonymNum = 0;
    m_antonymNum = 0;
    m_testModeNum = 0;
    m_testNum = 0;
    m_pastNum = 0;
    m_curIndex = -1;
    m_lastWord = "";
    m_testStrategy = STRATEGY2;
    if (WTool::getScreenSize() == "14")
        m_spacing = 8;
    else if (WTool::getScreenSize() == "15.6")
        m_spacing = 10;

    label_bg_welcome = new QLabel(this);
    label_bg_welcome->setObjectName("label_bg_welcome");
    label_bg_start = new QLabel(this);
    label_bg_start->setObjectName("label_bg_start");
    label_statistics = new QLabel(label_bg_welcome);
    label_statistics->setObjectName("label_statistics");
    label_statistics->setText("当前可记单词数：2000\n"
                              "总共可记单词数：10000");
    label_info = new QLabel(label_bg_start);
    label_info->setObjectName("label_info");
    label_info->setText("已记忆：    未记忆：");
    label_cha = new QLabel(label_bg_start);
    label_cha->setObjectName("label_cha");
    label_gou = new QLabel(label_bg_start);
    label_gou->setObjectName("label_gou");
    label_input = new QLabel(label_bg_start);
    label_input->setObjectName("label_input");
    label_input->setText("请输入正确的单词：");
    label_line = new QLabel(label_bg_start);
    label_line->setObjectName("label_line");

    radioBtn_test[0] = new QRadioButton(label_bg_welcome);
    radioBtn_test[0]->setObjectName("radioBtn_test1");
    radioBtn_test[0]->setText("探索回忆");
    radioBtn_test[0]->setChecked(true);
    radioBtn_test[0]->setAutoExclusive(false);
    radioBtn_test[1] = new QRadioButton(label_bg_welcome);
    radioBtn_test[1]->setObjectName("radioBtn_test2");
    radioBtn_test[1]->setText("英文回想");
    radioBtn_test[1]->setChecked(true);
    radioBtn_test[1]->setAutoExclusive(false);

    radioBtn_range[0] = new QRadioButton(label_bg_welcome);
    radioBtn_range[0]->setObjectName("radioBtn_range1");
    radioBtn_range[0]->setText("times 0~5");
    radioBtn_range[0]->setChecked(true);
    radioBtn_range[0]->setAutoExclusive(false);
    connect(radioBtn_range[0], SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    radioBtn_range[1] = new QRadioButton(label_bg_welcome);
    radioBtn_range[1]->setObjectName("radioBtn_range2");
    radioBtn_range[1]->setText("times 6~9");
    radioBtn_range[1]->setChecked(true);
    radioBtn_range[1]->setAutoExclusive(false);
    connect(radioBtn_range[1], SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    radioBtn_range[2] = new QRadioButton(label_bg_welcome);
    radioBtn_range[2]->setObjectName("radioBtn_range3");
    radioBtn_range[2]->setText("times 10~99");
    radioBtn_range[2]->setChecked(false);
    radioBtn_range[2]->setAutoExclusive(false);
    connect(radioBtn_range[2], SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    radioBtn_range[3] = new QRadioButton(label_bg_welcome);
    radioBtn_range[3]->setObjectName("radioBtn_range4");
    radioBtn_range[3]->setText("times 100+");
    radioBtn_range[3]->setChecked(false);
    radioBtn_range[3]->setAutoExclusive(false);
    connect(radioBtn_range[3], SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    radioBtn_forever = new QRadioButton(label_bg_welcome);
    radioBtn_forever->setObjectName("radioBtn_forever");
    radioBtn_forever->setText("forever");
    radioBtn_forever->setChecked(false);
    radioBtn_forever->setAutoExclusive(false);
    connect(radioBtn_forever, SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));

    btn_start = new QPushButton(label_bg_welcome);
    btn_start->setObjectName("btn_start");
    btn_start->setText("开始记忆");
    connect(btn_start, SIGNAL(clicked()), this, SLOT(slot_btnStart_Clicked()));
    btn_know = new QPushButton(label_bg_start);
    btn_know->setObjectName("btn_know");
    btn_know->setText("认识");
    connect(btn_know, SIGNAL(clicked()), this, SLOT(slot_btnKnow_Clicked()));
    btn_notKnow = new QPushButton(label_bg_start);
    btn_notKnow->setObjectName("btn_notKnow");
    btn_notKnow->setText("不认识");
    connect(btn_notKnow, SIGNAL(clicked()), this, SLOT(slot_btnNotKnow_Clicked()));
    btn_forever = new QPushButton(label_bg_start);
    btn_forever->setObjectName("btn_forever");
    btn_forever->setText("设为forever组");
    connect(btn_forever, SIGNAL(clicked()), this, SLOT(slot_btnForever_Clicked()));
    btn_notforever = new QPushButton(label_bg_start);
    btn_notforever->setObjectName("btn_notforever");
    btn_notforever->setText("移出forever组");
    connect(btn_notforever, SIGNAL(clicked()), this, SLOT(slot_btnNotForever_Clicked()));
    btn_next = new QPushButton(label_bg_start);
    btn_next->setObjectName("btn_next");
    btn_next->setText("下一个");
    connect(btn_next, SIGNAL(clicked()), this, SLOT(slot_btnNext_Clicked()));
    btn_submit = new QPushButton(label_bg_start);
    btn_submit->setObjectName("btn_submit");
    btn_submit->setText("提交");
    connect(btn_submit, SIGNAL(clicked()), this, SLOT(slot_btnSubmit_Clicked()));

    scrollArea = new QScrollArea(label_bg_start);
    scrollArea->setObjectName("scrollArea");
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    widget_start = new QWidget(label_bg_start);
    widget_start->setObjectName("widget_start");
    scrollArea->setWidget(widget_start);

    copyLabel_WORD = new CopyLabel(label_bg_start);
    copyLabel_WORD->setObjectName("copyLabel_WORD");
    copyLabel_word = new CopyLabel(widget_start);
    copyLabel_word->setObjectName("copyLabel_word");
    copyLabel_phoneticSymbol = new CopyLabel(widget_start);
    copyLabel_phoneticSymbol->setObjectName("copyLabel_phoneticSymbol");
    for (int i = 0; i < 20; i++)
    {
        copyLabel_property[i] = new CopyLabel(widget_start);
        copyLabel_property[i]->setObjectName(QString("copyLabel_property%1").arg(i + 1));
    }
    for (int i = 0; i < 20; i++)
    {
        copyLabel_explain[i] = new CopyLabel(widget_start);
        copyLabel_explain[i]->setObjectName(QString("copyLabel_explain%1").arg(i + 1));
    }

    copyLabel_exampleSentence = new CopyLabel(widget_start);
    copyLabel_exampleSentence->setObjectName("copyLabel_exampleSentence");
    for (int i = 0; i < 6; i++)
    {
        copyLabel_example[i] = new CopyLabel(widget_start);
        copyLabel_example[i]->setObjectName(QString("copyLabel_example%1").arg(i + 1));
    }

    copyLabel_synonym = new CopyLabel(widget_start);
    copyLabel_synonym->setObjectName("copyLabel_synonym");
    copyLabel_antonym = new CopyLabel(widget_start);
    copyLabel_antonym->setObjectName("copyLabel_antonym");

    for (int i = 0; i < 8; i++)
    {
        linkLabel_synonym[i] = new LinkLabel(widget_start);
        linkLabel_synonym[i]->setObjectName(QString("linkLabel_synonym%1").arg(i + 1));
    }
    for (int i = 0; i < 8; i++)
    {
        linkLabel_antonym[i] = new LinkLabel(widget_start);
        linkLabel_antonym[i]->setObjectName(QString("linkLabel_antonym%1").arg(i + 1));
    }


    lineEdit_input = new QLineEdit(label_bg_start);
    lineEdit_input->setObjectName("lineEdit_input");

    loadStyleSheet();
    loadJsonRect();
}

void WordMemorizeWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return && (event->modifiers() == Qt::ControlModifier))
    {
        if (m_test == NONE)
        {
            if (btn_start->isVisible())
                slot_btnStart_Clicked();
        }
        else if (m_test == EXPLORATE)
        {
            if (btn_know->isVisible())
                slot_btnKnow_Clicked();
            else if (btn_next->isVisible())
                slot_btnNext_Clicked();
        }
        else if (m_test == RECALL)
        {
            if (btn_submit->isVisible())
                slot_btnSubmit_Clicked();
            else if (btn_next->isVisible())
                slot_btnNext_Clicked();
        }
    }
    else if (event->key() == Qt::Key_Return && (event->modifiers() == Qt::AltModifier))
    {
        if (m_test == EXPLORATE)
        {
            if (btn_notKnow->isVisible())
                slot_btnNotKnow_Clicked();
            else if (btn_forever->isVisible())
                slot_btnForever_Clicked();
        }
        else if (m_test == RECALL)
        {
            if (btn_forever->isVisible())
                slot_btnForever_Clicked();
        }
    }
}

void WordMemorizeWidget::recoveryInterface()
{
    m_test = NONE;
    m_propertyNum = 0;
    m_testModeNum = 0;
    m_testNum = 0;
    m_pastNum = 0;
    m_curIndex = -1;
    m_lastWord = "";
    m_testList.clear();
    m_nameList.clear();
    lineEdit_input->clear();

    this->setMode(WELCOME);
    this->setFocus();
}

void WordMemorizeWidget::updateWordStatistics()
{
    int num1 = 0, num2 = 0, num3 = 0;
    if (!radioBtn_range[0]->isChecked() && !radioBtn_range[1]->isChecked() && !radioBtn_range[2]->isChecked() &&
        !radioBtn_range[3]->isChecked() && !radioBtn_forever->isChecked())
    {
        num1 += p_wordAdmin->getWordCanMemorizeNumFromTimes(0, MAX_TIMES, false);
        num1 += p_wordAdmin->getWordCanMemorizeNumFromTimes(0, MAX_TIMES, true);
    }
    else
    {
        if (radioBtn_range[0]->isChecked())
            num1 += p_wordAdmin->getWordCanMemorizeNumFromTimes(0, 5, radioBtn_forever->isChecked());
        if (radioBtn_range[1]->isChecked())
            num1 += p_wordAdmin->getWordCanMemorizeNumFromTimes(6, 9, radioBtn_forever->isChecked());
        if (radioBtn_range[2]->isChecked())
            num1 += p_wordAdmin->getWordCanMemorizeNumFromTimes(10, 99, radioBtn_forever->isChecked());
        if (radioBtn_range[3]->isChecked())
            num1 += p_wordAdmin->getWordCanMemorizeNumFromTimes(100, MAX_TIMES, radioBtn_forever->isChecked());
    }
    num2 = p_wordAdmin->getWordNumFromTimes(0, MAX_TIMES, false);
    num3 = p_wordAdmin->getWordNumFromTimes(0, MAX_TIMES, true);

    label_statistics->setText(QString("当前可记单词数：%1\n未记住的单词数：%2\n已记住的单词数：%3")
        .arg(num1).arg(num2).arg(num3));
}

int WordMemorizeWidget::getMode()
{
    return m_mode;
}

void WordMemorizeWidget::loadStyleSheet()
{
    setStyleSheet(WTool::getWordMemorizeWidgetQss());
}

void WordMemorizeWidget::loadJsonRect()
{
    QString path = WTool::getWordMemorizeWidgetJsonPath();
    bool ok;
    QtJson::JsonObject result = QtJson::parse(WTool::readFileInfo(path), ok).toMap();
    if (!ok)
    {
        DEBUG << "open json file " + path + " failed";
        return;
    }

    QtJson::JsonObject tmp = result["explorate"].toMap()["left"].toMap();
    rect_explorate_left.setRect(tmp["x"].toInt(), tmp["y"].toInt(), tmp["w"].toInt(), tmp["h"].toInt());

    tmp = result["explorate"].toMap()["right"].toMap();
    rect_explorate_right.setRect(tmp["x"].toInt(), tmp["y"].toInt(), tmp["w"].toInt(), tmp["h"].toInt());

    tmp = result["explorate"].toMap()["center"].toMap();
    rect_explorate_center.setRect(tmp["x"].toInt(), tmp["y"].toInt(), tmp["w"].toInt(), tmp["h"].toInt());

    tmp = result["recall"].toMap()["left"].toMap();
    rect_recall_left.setRect(tmp["x"].toInt(), tmp["y"].toInt(), tmp["w"].toInt(), tmp["h"].toInt());

    tmp = result["recall"].toMap()["right"].toMap();
    rect_recall_right.setRect(tmp["x"].toInt(), tmp["y"].toInt(), tmp["w"].toInt(), tmp["h"].toInt());

    tmp = result["recall"].toMap()["center"].toMap();
    rect_recall_center.setRect(tmp["x"].toInt(), tmp["y"].toInt(), tmp["w"].toInt(), tmp["h"].toInt());
}

void WordMemorizeWidget::setMode(int mode)
{
    m_mode = mode;
    switch (m_mode)
    {
    case WELCOME:
        label_bg_start->hide();
        label_info->hide();
        scrollArea->hide();
        widget_start->hide();
        copyLabel_word->hide();
        copyLabel_phoneticSymbol->hide();
        for (int i = 0; i < 20; i++)
        {
            copyLabel_property[i]->hide();
            copyLabel_explain[i]->hide();
        }
        copyLabel_exampleSentence->hide();
        for (int i = 0; i < 6; i++)
            copyLabel_example[i]->hide();
        copyLabel_synonym->hide();
        copyLabel_antonym->hide();
        for (int i = 0; i < 8; i++)
        {
            linkLabel_synonym[i]->hide();
            linkLabel_antonym[i]->hide();
        }
        copyLabel_WORD->hide();
        btn_know->hide();
        btn_notKnow->hide();
        btn_next->hide();
        btn_forever->hide();
        btn_notforever->hide();
        label_cha->hide();
        label_gou->hide();
        label_input->hide();
        lineEdit_input->hide();
        label_line->hide();
        btn_submit->hide();

        label_bg_welcome->show();
        label_statistics->show();
        radioBtn_test[0]->show();
        radioBtn_test[1]->show();
        radioBtn_range[0]->show();
        radioBtn_range[1]->show();
        radioBtn_range[2]->show();
        radioBtn_range[3]->show();
        radioBtn_forever->show();
        btn_start->show();
        break;
    case MEMORY:
        label_bg_welcome->hide();
        label_statistics->hide();
        radioBtn_test[0]->hide();
        radioBtn_test[1]->hide();
        radioBtn_range[0]->hide();
        radioBtn_range[1]->hide();
        radioBtn_range[2]->hide();
        radioBtn_range[3]->hide();
        radioBtn_forever->hide();
        btn_start->hide();

        label_bg_start->show();
        label_info->show();
        scrollArea->hide();
        widget_start->hide();
        copyLabel_word->hide();
        copyLabel_phoneticSymbol->hide();
        for (int i = 0; i < 20; i++)
        {
            copyLabel_property[i]->hide();
            copyLabel_explain[i]->hide();
        }
        copyLabel_exampleSentence->hide();
        for (int i = 0; i < 6; i++)
            copyLabel_example[i]->hide();
        copyLabel_synonym->hide();
        copyLabel_antonym->hide();
        for (int i = 0; i < 8; i++)
        {
            linkLabel_synonym[i]->hide();
            linkLabel_antonym[i]->hide();
        }
        copyLabel_WORD->hide();
        btn_know->hide();
        btn_notKnow->hide();
        btn_next->hide();
        btn_forever->hide();
        btn_notforever->hide();
        label_cha->hide();
        label_gou->hide();
        label_input->hide();
        lineEdit_input->hide();
        label_line->hide();
        btn_submit->hide();
        break;
    }
}

void WordMemorizeWidget::testListInit()
{
    int size = m_testList.size();
    for (int i = 0; i < size; i++)
    {
        if (radioBtn_test[0]->isChecked())
        {
            m_testList[i].m_isPass[0] = false;
        }
        if (radioBtn_test[1]->isChecked())
        {
            m_testList[i].m_isPass[1] = false;
        }
    }
}

void WordMemorizeWidget::loadTestInfo()
{
    lineEdit_input->clear();
    chooseTestWord();
    if (p_wordAdmin->getWordInfo(m_testList.at(m_curIndex).m_info.m_name, &m_word))
    {
        setMode(MEMORY);

        int r = WTool::rand(0, m_testList.at(m_curIndex).notPassNum() - 1);
        m_test = -1;
        for (int i = 0; i < 3; i++)
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
            clearWordInfo();
            setWordInfo(false);
            setViewPosition();
            copyLabel_WORD->show();
            btn_know->show();
            btn_notKnow->show();
            this->setFocus();
            break;
        case RECALL:
            clearWordInfo();
            setWordInfo(true);
            setViewPosition();
            showWordInfo(false);
            label_input->show();
            lineEdit_input->show();
            label_line->show();
            btn_submit->show();
            lineEdit_input->setFocus();
            break;
        case DICTATE:
            break;
        }
    }
    else
        QMessageBox::about(this, "提示", QString("记忆模式加载 %1 失败").arg(m_testList.at(m_curIndex).m_info.m_name));
}

void WordMemorizeWidget::clearWordInfo()
{
    copyLabel_WORD->clear();
    copyLabel_word->clear();
    copyLabel_phoneticSymbol->clear();
    for (int i = 0; i < 20; i++)
    {
        copyLabel_property[i]->clear();
        copyLabel_explain[i]->clear();
    }
    copyLabel_exampleSentence->clear();
    for (int i = 0; i < 6; i++)
        copyLabel_example[i]->clear();
    copyLabel_synonym->clear();
    copyLabel_antonym->clear();
    for (int i = 0; i < 8; i++)
    {
        linkLabel_synonym[i]->clear();
        linkLabel_antonym[i]->clear();
    }
}

void WordMemorizeWidget::setWordInfo(bool isShield)
{
    QString name = m_word.m_name;
    QString tmp;
    if (m_word.m_isPhrase)
        label_input->setText("请输入正确的词组：");
    else
        label_input->setText("请输入正确的单词：");
    copyLabel_WORD->setText(name);
    copyLabel_WORD->setAlignment(Qt::AlignCenter);
    copyLabel_word->setText(name);
    if (m_word.m_phoneticSymbol.isEmpty() == false)
    {
        copyLabel_phoneticSymbol->setText(m_word.m_phoneticSymbol);
    }
    int i = 0;
    if (!m_word.m_adj_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("adj.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        tmp = m_word.m_adj_Chinese;
        copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
        i++;
    }
    if (!m_word.m_adj_English.isEmpty())
    {
        copyLabel_property[i]->setText("adj.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        tmp = m_word.m_adj_English;
        copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
        i++;
    }
    if (!m_word.m_adv_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("adv.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        tmp = m_word.m_adv_Chinese;
        copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
        i++;
    }
    if (!m_word.m_adv_English.isEmpty())
    {
        copyLabel_property[i]->setText("adv.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        tmp = m_word.m_adv_English;
        copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
        i++;
    }
    if (m_word.m_vt_Chinese == m_word.m_vi_Chinese && m_word.m_vt_English == m_word.m_vi_English &&
        (!m_word.m_vt_Chinese.isEmpty() || !m_word.m_vt_English.isEmpty()))
    {
        if (!m_word.m_vt_Chinese.isEmpty())
        {
            copyLabel_property[i]->setText("vt.& vi.");
            copyLabel_property[i]->setAlignment(Qt::AlignCenter);
            tmp = m_word.m_vt_Chinese;
            copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
            i++;
        }
        if (!m_word.m_vt_English.isEmpty())
        {
            copyLabel_property[i]->setText("vt.& vi.");
            copyLabel_property[i]->setAlignment(Qt::AlignCenter);
            tmp = m_word.m_vt_English;
            copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
            i++;
        }
    }
    else
    {
        if (!m_word.m_vt_Chinese.isEmpty())
        {
            copyLabel_property[i]->setText("vt.");
            copyLabel_property[i]->setAlignment(Qt::AlignCenter);
            tmp = m_word.m_vt_Chinese;
            copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
            i++;
        }
        if (!m_word.m_vt_English.isEmpty())
        {
            copyLabel_property[i]->setText("vt.");
            copyLabel_property[i]->setAlignment(Qt::AlignCenter);
            tmp = m_word.m_vt_English;
            copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
            i++;
        }
        if (!m_word.m_vi_Chinese.isEmpty())
        {
            copyLabel_property[i]->setText("vi.");
            copyLabel_property[i]->setAlignment(Qt::AlignCenter);
            tmp = m_word.m_vi_Chinese;
            copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
            i++;
        }
        if (!m_word.m_vi_English.isEmpty())
        {
            copyLabel_property[i]->setText("vi.");
            copyLabel_property[i]->setAlignment(Qt::AlignCenter);
            tmp = m_word.m_vi_English;
            copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
            i++;
        }
    }
    if (!m_word.m_pastTense.isEmpty() || !m_word.m_pastParticiple.isEmpty() ||
        !m_word.m_presentParticiple.isEmpty() || !m_word.m_thirdPersonSingular.isEmpty())
    {
        copyLabel_property[i]->setText("变形");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        if (!m_word.m_pastTense.isEmpty())
            copyLabel_explain[i]->setText(copyLabel_explain[i]->text() + "  过去式: " + m_word.m_pastTense);
        if (!m_word.m_pastParticiple.isEmpty())
            copyLabel_explain[i]->setText(copyLabel_explain[i]->text() + "  过去分词: " + m_word.m_pastParticiple);
        if (!m_word.m_presentParticiple.isEmpty())
            copyLabel_explain[i]->setText(copyLabel_explain[i]->text() + "  现在分词: " + m_word.m_presentParticiple);
        if (!m_word.m_thirdPersonSingular.isEmpty())
            copyLabel_explain[i]->setText(copyLabel_explain[i]->text() + "  第三人称单数: " + m_word.m_thirdPersonSingular);
        copyLabel_explain[i]->setText(copyLabel_explain[i]->text().trimmed());
        i++;
    }
    if (!m_word.m_noun_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("n.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        tmp = m_word.m_noun_Chinese;
        copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
        i++;
    }
    if (!m_word.m_noun_English.isEmpty())
    {
        copyLabel_property[i]->setText("n.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        tmp = m_word.m_noun_English;
        copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
        i++;
    }
    if (!m_word.m_prep_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("prep.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        tmp = m_word.m_prep_Chinese;
        copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
        i++;
    }
    if (!m_word.m_prep_English.isEmpty())
    {
        copyLabel_property[i]->setText("prep.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        tmp = m_word.m_prep_English;
        copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
        i++;
    }
    if (!m_word.m_conj_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("conj.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        tmp = m_word.m_conj_Chinese;
        copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
        i++;
    }
    if (!m_word.m_conj_English.isEmpty())
    {
        copyLabel_property[i]->setText("conj.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        tmp = m_word.m_conj_English;
        copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
        i++;
    }
    if (!m_word.m_pron_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("pron.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        tmp = m_word.m_pron_Chinese;
        copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
        i++;
    }
    if (!m_word.m_pron_English.isEmpty())
    {
        copyLabel_property[i]->setText("pron.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        tmp = m_word.m_pron_English;
        copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
        i++;
    }
    if (!m_word.m_art_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("art.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        tmp = m_word.m_art_Chinese;
        copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
        i++;
    }
    if (!m_word.m_art_English.isEmpty())
    {
        copyLabel_property[i]->setText("art.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        tmp = m_word.m_art_English;
        copyLabel_explain[i]->setText((isShield == true ? WTool::shieldWord(tmp, name) : tmp));
        i++;
    }
    m_propertyNum = i;
    if (!m_word.m_exampleSentence[0].isEmpty() || !m_word.m_exampleSentence[1].isEmpty() ||
        !m_word.m_exampleSentence[2].isEmpty() || !m_word.m_exampleSentence[3].isEmpty() ||
        !m_word.m_exampleSentence[4].isEmpty() || !m_word.m_exampleSentence[5].isEmpty())
    {
        copyLabel_exampleSentence->setText("例句：");
        for (i = 0; i < 6; i++)
        {
            if (!m_word.m_exampleSentence[i].isEmpty())
            {
                copyLabel_example[i]->setText(QString("%1. %2").arg(i + 1).arg(m_word.m_exampleSentence[i]));
            }
            else
            {
                m_exampleNum = i;
                break;
            }
        }
    }
    if (!m_word.m_synonym.isEmpty())
    {
        copyLabel_synonym->setText("同义词：");
        QStringList list = m_word.m_synonym.split(';');
        for (i = 0; i < list.size(); i++)
        {
            if (list.at(i) != "")
            {
                linkLabel_synonym[i]->setText(list.at(i));
            }
        }
        m_synonymNum = i;
    }
    if (!m_word.m_antonym.isEmpty())
    {
        copyLabel_antonym->setText("反义词：");
        QStringList list = m_word.m_antonym.split(';');
        for (i = 0; i < list.size(); i++)
        {
            if (list.at(i) != "")
            {
                linkLabel_antonym[i]->setText(list.at(i));
            }
        }
        m_antonymNum = i;
    }
}

void WordMemorizeWidget::setViewPosition()
{
    QString screen = WTool::getScreenSize();
    if (screen == "14")
        setViewPosition_14();
    else if (screen == "15.6")
        setViewPosition_15();
}

void WordMemorizeWidget::setViewPosition_14()
{
    int x = 8, y = 8, h = 47;
    int len = WTool::getFontLength(copyLabel_word->font(), copyLabel_word->text()) + 12;
    copyLabel_word->setGeometry(x, y, len, h);

    if (!copyLabel_phoneticSymbol->text().isEmpty())
    {
        len = WTool::getFontLength(copyLabel_phoneticSymbol->font(), copyLabel_phoneticSymbol->text()) + 12;
        x = copyLabel_word->x() + copyLabel_word->width() + 16;
        y = 12, h = 39;
        copyLabel_phoneticSymbol->setGeometry(x, y, len, h);
    }

    y = 8, h = 47;
    for (int i = 0; i < m_propertyNum; i++)
    {
        x = 8, y += h + 16, len = 80;
        h = 39;
        copyLabel_property[i]->setGeometry(x, y, len, h);
        len = WTool::getFontLength(copyLabel_explain[i]->font(), copyLabel_explain[i]->text()) + 8;
        x += copyLabel_property[i]->width() + 4;
        if (x + len + 8 > 635)
        {
            len = 635 - x;
            int num = WTool::getTextLineNumber(copyLabel_explain[i]->font(),
                copyLabel_explain[i]->text(), 635 - x - 8);
            h = 39 + 26 * (num - 1);
        }
        copyLabel_explain[i]->setGeometry(x, y, len, h);
    }

    if (!copyLabel_exampleSentence->text().isEmpty())
    {
        x = 8, y += h + 16;
        h = 39;
        len = WTool::getFontLength(copyLabel_exampleSentence->font(), copyLabel_exampleSentence->text()) + 8;
        copyLabel_exampleSentence->setGeometry(x, y, len, h);
        for (int i = 0; i < m_exampleNum; i++)
        {
            x = 8, y += h + 16;
            h = 39;
            len = WTool::getFontLength(copyLabel_example[i]->font(), copyLabel_example[i]->text()) + 8;
            if (x + len + 8 > 635)
            {
                len = 635 - x;
                int num = WTool::getTextLineNumber(copyLabel_example[i]->font(),
                    copyLabel_example[i]->text(), 635 - x - 8);
                h = 39 + 26 * (num - 1);
            }
            copyLabel_example[i]->setGeometry(x, y, len, h);
        }
    }

    if (!copyLabel_synonym->text().isEmpty())
    {
        x = 8, y += h + 16;
        h = 39;
        len = WTool::getFontLength(copyLabel_synonym->font(), copyLabel_synonym->text()) + 8;
        copyLabel_synonym->setGeometry(x, y, len, h);
        x = 8, y += h + 8;
        for (int i = 0; i < m_synonymNum; i++)
        {
            len = WTool::getFontLength(linkLabel_synonym[i]->font(), linkLabel_synonym[i]->text()) + 4;
            if (x + len > 635)
            {
                x = 8, y += h + 8;
            }
            linkLabel_synonym[i]->setGeometry(x, y, len, h);
            x += len + 16;
        }
    }

    if (!copyLabel_antonym->text().isEmpty())
    {
        x = 8, y += h + 16;
        h = 39;
        len = WTool::getFontLength(copyLabel_antonym->font(), copyLabel_antonym->text()) + 8;
        copyLabel_antonym->setGeometry(x, y, len, h);
        x = 8, y += h + 8;
        for (int i = 0; i < m_antonymNum; i++)
        {
            len = WTool::getFontLength(linkLabel_antonym[i]->font(), linkLabel_antonym[i]->text()) + 4;
            if (x + len > 635)
            {
                x = 8, y += h + 8;
            }
            linkLabel_antonym[i]->setGeometry(x, y, len, h);
            x += len + 16;
        }
    }

    y += h + 8;
    widget_start->setGeometry(0, 0, 660, y);
}

void WordMemorizeWidget::setViewPosition_15()
{
    int x = 10, y = 10, h = 55;
    int len = WTool::getFontLength(copyLabel_word->font(), copyLabel_word->text()) + 12;
    copyLabel_word->setGeometry(x, y, len, h);

    if (!copyLabel_phoneticSymbol->text().isEmpty())
    {
        len = WTool::getFontLength(copyLabel_phoneticSymbol->font(), copyLabel_phoneticSymbol->text()) + 12;
        x = copyLabel_word->x() + copyLabel_word->width() + 20;
        y = 15, h = 44;
        copyLabel_phoneticSymbol->setGeometry(x, y, len, h);
    }

    y = 10, h = 55;
    for (int i = 0; i < m_propertyNum; i++)
    {
        x = 10, y += h + 20, len = 100;
        h = 44;
        copyLabel_property[i]->setGeometry(x, y, len, h);
        len = WTool::getFontLength(copyLabel_explain[i]->font(), copyLabel_explain[i]->text()) + 9;
        x += copyLabel_property[i]->width() + 5;
        if (x + len + 10 > 790)
        {
            len = 790 - x;
            int num = WTool::getTextLineNumber(copyLabel_explain[i]->font(),
                copyLabel_explain[i]->text(), 790 - x - 10);
            h = 44 + 30 * (num - 1);
        }
        copyLabel_explain[i]->setGeometry(x, y, len, h);
    }

    if (!copyLabel_exampleSentence->text().isEmpty())
    {
        x = 10, y += h + 20;
        h = 44;
        len = WTool::getFontLength(copyLabel_exampleSentence->font(), copyLabel_exampleSentence->text()) + 8;
        copyLabel_exampleSentence->setGeometry(x, y, len, h);
        for (int i = 0; i < m_exampleNum; i++)
        {
            x = 10, y += h + 20;
            h = 44;
            len = WTool::getFontLength(copyLabel_example[i]->font(), copyLabel_example[i]->text()) + 8;
            if (x + len + 10 > 790)
            {
                len = 790 - x;
                int num = WTool::getTextLineNumber(copyLabel_example[i]->font(),
                    copyLabel_example[i]->text(), 790 - x - 10);
                h = 44 + 30 * (num - 1);
            }
            copyLabel_example[i]->setGeometry(x, y, len, h);
        }
    }

    if (!copyLabel_synonym->text().isEmpty())
    {
        x = 10, y += h + 20;
        h = 44;
        len = WTool::getFontLength(copyLabel_synonym->font(), copyLabel_synonym->text()) + 8;
        copyLabel_synonym->setGeometry(x, y, len, h);
        x = 10, y += h + 10;
        for (int i = 0; i < m_synonymNum; i++)
        {
            len = WTool::getFontLength(linkLabel_synonym[i]->font(), linkLabel_synonym[i]->text()) + 5;
            if (x + len > 790)
            {
                x = 10, y += h + 10;
            }
            linkLabel_synonym[i]->setGeometry(x, y, len, h);
            x += len + 20;
        }
    }

    if (!copyLabel_antonym->text().isEmpty())
    {
        x = 10, y += h + 20;
        h = 44;
        len = WTool::getFontLength(copyLabel_antonym->font(), copyLabel_antonym->text()) + 8;
        copyLabel_antonym->setGeometry(x, y, len, h);
        x = 10, y += h + 10;
        for (int i = 0; i < m_antonymNum; i++)
        {
            len = WTool::getFontLength(linkLabel_antonym[i]->font(), linkLabel_antonym[i]->text()) + 5;
            if (x + len > 790)
            {
                x = 10, y += h + 10;
            }
            linkLabel_antonym[i]->setGeometry(x, y, len, h);
            x += len + 20;
        }
    }

    y += h + 10;
    widget_start->setGeometry(0, 0, 820, y);
}

void WordMemorizeWidget::showWordInfo(bool showWord)
{
    scrollArea->show();
    widget_start->show();
    if (showWord)
    {
        copyLabel_word->show();
        if (!copyLabel_phoneticSymbol->text().isEmpty())
            copyLabel_phoneticSymbol->show();
        if (!copyLabel_exampleSentence->text().isEmpty())
            copyLabel_exampleSentence->show();
        for (int i = 0; i < m_exampleNum; i++)
            copyLabel_example[i]->show();

        if (!copyLabel_synonym->text().isEmpty())
            copyLabel_synonym->show();
        for (int i = 0; i < m_synonymNum; i++)
            linkLabel_synonym[i]->show();

        if (!copyLabel_antonym->text().isEmpty())
            copyLabel_antonym->show();
        for (int i = 0; i < m_antonymNum; i++)
            linkLabel_antonym[i]->show();
    }
    for (int i = 0; i < m_propertyNum; i++)
    {
        if (showWord || copyLabel_property[i]->text() != "变形")
        {
            copyLabel_property[i]->show();
            copyLabel_explain[i]->show();
        }
    }
}

void WordMemorizeWidget::chooseTestWord()
{
    switch (m_testStrategy)
    {
    case STRATEGY1:
        chooseStrategy1();
        break;
    case STRATEGY2:
        chooseStrategy2();
        break;
    default:
        chooseStrategy1();
        break;
    }
}

void WordMemorizeWidget::chooseStrategy1()
{
    int lastIndex = p_wordAdmin->searchWordTestList(m_testList, m_lastWord);
    do
    {
        m_curIndex = WTool::rand(0, m_testNum - 1);
    } while (lastIndex == m_curIndex && m_testNum > 1);
}

void WordMemorizeWidget::chooseStrategy2()
{
    if (m_nameList.count() == 0)
    {
        if (m_testNum > 15)
        {
            for (int i = 0; i < 10; i++)
            {
                int r = WTool::rand(0, m_testNum - 1);
                if (m_nameList.contains(m_testList.at(r).m_info.m_name))
                {
                    do
                    {
                        r++;
                        if (r >= m_testNum)
                            r = 0;
                    } while (m_nameList.contains(m_testList.at(r).m_info.m_name) == true);
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

void WordMemorizeWidget::slot_radioButtonClicked()
{
    this->updateWordStatistics();
}

void WordMemorizeWidget::slot_btnStart_Clicked()
{
    m_testList.clear();
    m_nameList.clear();
    if (!radioBtn_range[0]->isChecked() && !radioBtn_range[1]->isChecked() && !radioBtn_range[2]->isChecked() &&
        !radioBtn_range[3]->isChecked() && !radioBtn_forever->isChecked())
    {
        m_testList = p_wordAdmin->getAllWordCanMemorizeList();
    }
    else
    {
        if (radioBtn_range[0]->isChecked())
            m_testList += p_wordAdmin->getWordCanMemorizeListFromTimes(0, 5, radioBtn_forever->isChecked());
        if (radioBtn_range[1]->isChecked())
            m_testList += p_wordAdmin->getWordCanMemorizeListFromTimes(6, 9, radioBtn_forever->isChecked());
        if (radioBtn_range[2]->isChecked())
            m_testList += p_wordAdmin->getWordCanMemorizeListFromTimes(10, 99, radioBtn_forever->isChecked());
        if (radioBtn_range[3]->isChecked())
            m_testList += p_wordAdmin->getWordCanMemorizeListFromTimes(100, MAX_TIMES, radioBtn_forever->isChecked());
    }
    if (m_testList.size() > 0)
    {
        p_memThread->start();
        m_testModeNum = 0;
        for (int i = 0; i < 2; i++)
        {
            if (radioBtn_test[i]->isChecked())
                m_testModeNum++;
        }
        if (m_testModeNum > 0)
        {
            m_mutex.lock();
            m_testNum = m_testList.size();
            m_pastNum = 0;
            label_info->setText(QString("已记忆：%1    未记忆：%2").arg(m_pastNum).arg(m_testNum));
            this->testListInit();
            m_mutex.unlock();
            DEBUG << "m_testNum:" << m_testNum;
            if (m_testNum > 0)
                this->loadTestInfo();
            else
                QMessageBox::about(this, "提示", "没有要记忆的单词");
        }
        else
            QMessageBox::about(this, "提示", "没有选择记忆模式");
    }
    else
        QMessageBox::about(this, "提示", "没有要记忆的单词");
}

void WordMemorizeWidget::slot_btnKnow_Clicked()
{
    btn_know->hide();
    btn_notKnow->hide();
    copyLabel_WORD->hide();
    showWordInfo(true);

    m_testList[m_curIndex].m_isPass[m_test] = true;
    if (m_testList[m_curIndex].isPass())
    {
        m_lastWord = "";
        m_word.m_times++;
        if (m_word.m_times >= Global::m_leastForeverTimes.getValueInt() && m_word.m_remember <= 0)
        {
            btn_forever->setGeometry(rect_explorate_left);
            btn_forever->show();
            btn_next->setGeometry(rect_explorate_right);
            btn_next->show();

            label_gou->setGeometry(btn_next->x() + btn_next->width() + m_spacing, btn_next->y(),
                btn_next->height(), btn_next->height());
            label_gou->show();
        }
        else if (m_word.m_remember > 0)
        {
            btn_notforever->setGeometry(rect_explorate_left);
            btn_notforever->show();
            btn_next->setGeometry(rect_explorate_right);
            btn_next->show();

            label_gou->setGeometry(btn_next->x() + btn_next->width() + m_spacing, btn_next->y(),
                btn_next->height(), btn_next->height());
            label_gou->show();
        }
        else
        {
            btn_next->setGeometry(rect_explorate_center);
            btn_next->show();

            label_gou->setGeometry(btn_next->x() + btn_next->width() + m_spacing, btn_next->y(),
                btn_next->height(), btn_next->height());
            label_gou->show();
        }

        m_mutex.lock();
        m_testNum--;
        m_pastNum++;
        label_info->setText(QString("已记忆：%1    未记忆：%2").arg(m_pastNum).arg(m_testNum));

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
        m_mutex.unlock();
        emit wordTimeIncreaseSignal(m_word.m_name);
    }
    else
    {
        m_lastWord = m_testList.at(m_curIndex).m_info.m_name;
        btn_next->setGeometry(rect_explorate_center);
        btn_next->show();

        label_gou->setGeometry(btn_next->x() + btn_next->width() + m_spacing, btn_next->y(),
            btn_next->height(), btn_next->height());
        label_gou->show();
    }
    this->setFocus();
}

void WordMemorizeWidget::slot_btnNotKnow_Clicked()
{
    m_lastWord = m_testList.at(m_curIndex).m_info.m_name;
    btn_know->hide();
    btn_notKnow->hide();
    copyLabel_WORD->hide();
    showWordInfo(true);

    btn_next->setGeometry(rect_explorate_center);
    btn_next->show();

    label_cha->setGeometry(btn_next->x() + btn_next->width() + m_spacing, btn_next->y(),
        btn_next->height(), btn_next->height());
    label_cha->show();

    this->setFocus();
}

void WordMemorizeWidget::slot_btnForever_Clicked()
{
    p_wordAdmin->updateWord(m_word.m_name, "RememberState", "1");
    if (m_testNum == 0)
    {
        p_memThread->stop();
        p_memThread->wait();
        QMessageBox::about(this, "提示", "本次记忆单词数 " + QString::number(m_pastNum));
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
        p_memThread->stop();
        p_memThread->wait();
        QMessageBox::about(this, "提示", "本次记忆单词数 " + QString::number(m_pastNum));
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
        p_memThread->stop();
        p_memThread->wait();
        QMessageBox::about(this, "提示", "本次记忆单词数 " + QString::number(m_pastNum));
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
    btn_submit->hide();
    clearWordInfo();
    setWordInfo(true);
    setViewPosition();
    showWordInfo(true);
    for (int i = 0; i < m_propertyNum; i++)
    {
        if (copyLabel_property[i]->text() == "变形")
        {
            copyLabel_property[i]->show();
            copyLabel_explain[i]->show();
        }
    }
    if (lineEdit_input->text() == m_word.m_name)
    {
        m_testList[m_curIndex].m_isPass[m_test] = true;
        if (m_testList[m_curIndex].isPass())
        {
            m_lastWord = "";
            m_word.m_times++;
            if (m_word.m_times >= Global::m_leastForeverTimes.getValueInt() && m_word.m_remember <= 0)
            {
                btn_forever->setGeometry(rect_recall_left);
                btn_forever->show();
                btn_next->setGeometry(rect_recall_right);
                btn_next->show();

                label_gou->setGeometry(lineEdit_input->x() + lineEdit_input->width() + m_spacing,
                    lineEdit_input->y(), lineEdit_input->height(), lineEdit_input->height());
                label_gou->show();
            }
            else if (m_word.m_remember > 0)
            {
                btn_notforever->setGeometry(rect_recall_left);
                btn_notforever->show();
                btn_next->setGeometry(rect_recall_right);
                btn_next->show();

                label_gou->setGeometry(lineEdit_input->x() + lineEdit_input->width() + m_spacing,
                    lineEdit_input->y(), lineEdit_input->height(), lineEdit_input->height());
                label_gou->show();
            }
            else
            {
                btn_next->setGeometry(rect_recall_center);
                btn_next->show();

                label_gou->setGeometry(lineEdit_input->x() + lineEdit_input->width() + m_spacing,
                    lineEdit_input->y(), lineEdit_input->height(), lineEdit_input->height());
                label_gou->show();
            }

            m_mutex.lock();
            m_testNum--;
            m_pastNum++;
            label_info->setText(QString("已记忆：%1    未记忆：%2").arg(m_pastNum).arg(m_testNum));

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
            m_mutex.unlock();
            emit wordTimeIncreaseSignal(m_word.m_name);
        }
        else
        {
            m_lastWord = m_testList.at(m_curIndex).m_info.m_name;
            btn_next->setGeometry(rect_recall_center);
            btn_next->show();

            label_gou->setGeometry(lineEdit_input->x() + lineEdit_input->width() + m_spacing,
                lineEdit_input->y(), lineEdit_input->height(), lineEdit_input->height());
            label_gou->show();
        }
    }
    else
    {
        m_lastWord = m_testList.at(m_curIndex).m_info.m_name;
        btn_next->setGeometry(rect_recall_center);
        btn_next->show();

        label_cha->setGeometry(lineEdit_input->x() + lineEdit_input->width() + m_spacing,
            lineEdit_input->y(), lineEdit_input->height(), lineEdit_input->height());
        label_cha->show();
    }
    this->setFocus();
}

void WordMemorizeWidget::slot_wordCanMemorize(QString name)
{
    if (name.isEmpty() || name == WORD_NAME_UNDEFINED)
        return;
    for (int i = 0; i < m_testList.count(); i++)
    {
        if (m_testList.at(i).m_info.m_name == name)
            return;
    }

    WordTest test;
    if (p_wordAdmin->getWordBriefInfo(name, &test.m_info))
    {
        int times = test.m_info.m_times;
        if ((test.m_info.m_remember > 0) != radioBtn_forever->isChecked() ||
            ((radioBtn_range[0]->isChecked() == true && (times < 0 || times > 5)) &&
            (radioBtn_range[1]->isChecked() == true && (times < 6 || times > 9)) &&
            (radioBtn_range[2]->isChecked() == true && (times < 10 || times > 99)) &&
            (radioBtn_range[3]->isChecked() == true && (times < 100 || times > MAX_TIMES))))
        {
            return;
        }
        if (radioBtn_test[0]->isChecked())
        {
            test.m_isPass[0] = false;
        }
        if (radioBtn_test[1]->isChecked())
        {
            test.m_isPass[1] = false;
        }
        m_mutex.lock();
        m_testList.append(test);
        m_testNum++;
        label_info->setText(QString("已记忆：%1    未记忆：%2").arg(m_pastNum).arg(m_testNum));
        m_mutex.unlock();
    }
    else
        DEBUG << "wordCanMemorize get word fail";
}

void WordMemorizeWidget::slot_wordTimeDecline(QString name)
{
    m_mutex.lock();
    if (m_testList.at(m_curIndex).m_info.m_name != name)
    {
        for (int i = 0; i < m_testList.size(); i++)
        {
            if (m_testList.at(i).m_info.m_name == name)
            {
                m_testList[i].m_info.m_times--;
                break;
            }
        }
    }
    m_mutex.unlock();
}

void WordMemorizeWidget::slot_stopWordMemorize(bool *ret)
{
    if (QMessageBox::question(this, "queation", "是否停止记忆?", QMessageBox::Yes,
        QMessageBox::No) == QMessageBox::Yes)
    {
        p_memThread->stop();
        p_memThread->wait();
        if (ret != NULL)
        {
            *ret = true;
        }
    }
    else
    {
        if (ret != NULL)
            *ret = false;
    }
}

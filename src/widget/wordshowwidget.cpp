#include "wordshowwidget.h"
#include "wtool.h"
#include <QDebug>
#include <QThread>
#include <QCoreApplication>
#include <QScrollBar>
#include <QMessageBox>
#include <QKeyEvent>

extern WordAdmin *p_wordAdmin;

WordShowWidget::WordShowWidget(QWidget *parent) : QWidget(parent)
{
    m_screenSize = WTool::getScreenSize();
    m_propertyNum = 0;
    m_exampleNum = 0;
    m_synonymNum = 0;
    m_antonymNum = 0;

    widget_topBar = new QWidget(this);
    widget_topBar->setObjectName("widget_topBar");
    widget_show = new QWidget(this);
    widget_show->setObjectName("widget_show");

    btn_return = new QPushButton(widget_topBar);
    btn_return->setObjectName("btn_return");
    btn_return->setToolTip("Return");
    connect(btn_return, SIGNAL(clicked()), this, SLOT(slot_btnReturn_Clicked()));
    btn_mid = new QPushButton(widget_topBar);
    btn_mid->setObjectName("btn_mid");
    btn_mid->setToolTip("Set times 10");
    btn_mid->setText("10");
    connect(btn_mid, SIGNAL(clicked()), this, SLOT(slot_btnMid_Clicked()));
    btn_max = new QPushButton(widget_topBar);
    btn_max->setObjectName("btn_max");
    btn_max->setToolTip("Add times 100");
    btn_max->setText("100");
    connect(btn_max, SIGNAL(clicked()), this, SLOT(slot_btnMax_Clicked()));
    btn_add = new QPushButton(widget_topBar);
    btn_add->setObjectName("btn_add");
    btn_add->setToolTip("Add times");
    connect(btn_add, SIGNAL(clicked()), this, SLOT(slot_btnAdd_Clicked()));
    btn_delete = new QPushButton(widget_topBar);
    btn_delete->setObjectName("btn_delete");
    btn_delete->setToolTip("Delete");
    connect(btn_delete, SIGNAL(clicked()), this, SLOT(slot_btnDelete_Clicked()));
    btn_edit = new QPushButton(widget_topBar);
    btn_edit->setObjectName("btn_edit");
    btn_edit->setToolTip("Edit");
    connect(btn_edit, SIGNAL(clicked()), this, SLOT(slot_btnEdit_Clicked()));
    btn_reset = new QPushButton(widget_topBar);
    btn_reset->setObjectName("btn_reset");
    btn_reset->setToolTip("Reset memory times");
    connect(btn_reset, SIGNAL(clicked()), this, SLOT(slot_btnReset_Clicked()));

    wbtn_isRemember = new WTButton(widget_topBar);
    wbtn_isRemember->setObjectName("wbtn_isRemember");
    wbtn_isRemember->setText("                ");//为了使按钮能被按到
    wbtn_isRemember->setActiveTip("set to not forever");
    wbtn_isRemember->setInactiveTip("set to forever");
    connect(wbtn_isRemember, SIGNAL(clicked(bool)), this, SLOT(slot_wbtnRemember_Clicked(bool)));

    scrollArea = new QScrollArea(this);
    scrollArea->setObjectName("scrollArea");
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidget(widget_show);

    //CopyLabel
    copyLabel_word = new CopyLabel(widget_show);
    copyLabel_word->setObjectName("copyLabel_word");
    copyLabel_phoneticSymbol = new CopyLabel(widget_show);
    copyLabel_phoneticSymbol->setObjectName("copyLabel_phoneticSymbol");
    copyLabel_phoneticSymbol->hide();
    copyLabel_times = new CopyLabel(widget_show);
    copyLabel_times->setObjectName("copyLabel_times");
    copyLabel_times->setText("T:");

    for (int i = 0; i < 20; i++)
    {
        copyLabel_property[i] = new CopyLabel(widget_show);
        copyLabel_property[i]->setObjectName(QString("copyLabel_property%1").arg(i + 1));
        copyLabel_property[i]->hide();
    }
    for (int i = 0; i < 20; i++)
    {
        copyLabel_explain[i] = new CopyLabel(widget_show);
        copyLabel_explain[i]->setObjectName(QString("copyLabel_explain%1").arg(i + 1));
        copyLabel_explain[i]->hide();
    }

    copyLabel_exampleSentence = new CopyLabel(widget_show);
    copyLabel_exampleSentence->setObjectName("copyLabel_exampleSentence");
    copyLabel_exampleSentence->hide();
    for (int i = 0; i < 6; i++)
    {
        copyLabel_example[i] = new CopyLabel(widget_show);
        copyLabel_example[i]->setObjectName(QString("copyLabel_example%1").arg(i + 1));
        copyLabel_example[i]->hide();
    }

    copyLabel_synonym = new CopyLabel(widget_show);
    copyLabel_synonym->setObjectName("copyLabel_synonym");
    copyLabel_synonym->hide();
    copyLabel_antonym = new CopyLabel(widget_show);
    copyLabel_antonym->setObjectName("copyLabel_antonym");
    copyLabel_antonym->hide();

    //LinkLabel
    for (int i = 0; i < 8; i++)
    {
        linkLabel_synonym[i] = new LinkLabel(widget_show);
        linkLabel_synonym[i]->setObjectName(QString("linkLabel_synonym%1").arg(i + 1));
        connect(linkLabel_synonym[i], SIGNAL(released()), this, SLOT(slot_wordLinkPressed()));
        linkLabel_synonym[i]->hide();
    }
    for (int i = 0; i < 8; i++)
    {
        linkLabel_antonym[i] = new LinkLabel(widget_show);
        linkLabel_antonym[i]->setObjectName(QString("linkLabel_antonym%1").arg(i + 1));
        connect(linkLabel_antonym[i], SIGNAL(released()), this, SLOT(slot_wordLinkPressed()));
        linkLabel_antonym[i]->hide();
    }

    loadStyleSheet();
}

void WordShowWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        slot_btnReturn_Clicked();
    }
}

void WordShowWidget::recoveryInterface()
{
    m_propertyNum = 0;
    m_exampleNum = 0;
    m_synonymNum = 0;
    m_antonymNum = 0;
    scrollArea->verticalScrollBar()->setValue(0);
    copyLabel_word->clear();
    copyLabel_phoneticSymbol->clear();
    copyLabel_phoneticSymbol->hide();
    copyLabel_times->setText("T:");
    for (int i = 0; i < 20; i++)
    {
        copyLabel_property[i]->clear();
        copyLabel_property[i]->hide();
        copyLabel_explain[i]->clear();
        copyLabel_explain[i]->hide();
    }
    copyLabel_exampleSentence->clear();
    copyLabel_exampleSentence->hide();
    for (int i = 0; i < 6; i++)
    {
        copyLabel_example[i]->clear();
        copyLabel_example[i]->hide();
    }
    copyLabel_synonym->clear();
    copyLabel_synonym->hide();
    copyLabel_antonym->clear();
    copyLabel_antonym->hide();
    for (int i = 0; i < 8; i++)
    {
        linkLabel_synonym[i]->clear();
        linkLabel_synonym[i]->hide();
        linkLabel_antonym[i]->clear();
        linkLabel_antonym[i]->hide();
    }
}

bool WordShowWidget::loadWordInfo(QString name)
{
    if (p_wordAdmin->getWordInfo(name, &m_word) == true)
    {
        setWordInfo();
        setViewPosition();
        return true;
    }
    else
        return false;
}

void WordShowWidget::loadStyleSheet()
{
    setStyleSheet(WTool::getWordShowWidgetQss());
}

void WordShowWidget::setWordInfo()
{
    copyLabel_word->setText(m_word.m_name);
    if (m_word.m_phoneticSymbol.isEmpty() == false)
    {
        copyLabel_phoneticSymbol->setText(m_word.m_phoneticSymbol);
        copyLabel_phoneticSymbol->show();
    }
    copyLabel_times->setText(QString("T:%1").arg(m_word.m_times));
    wbtn_isRemember->setActive(m_word.m_remember > 0);
    int i = 0;
    if (m_word.m_adj_Chinese.isEmpty() == false)
    {
        copyLabel_property[i]->setText("adj.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        copyLabel_property[i]->show();
        copyLabel_explain[i]->setText(m_word.m_adj_Chinese);
        copyLabel_explain[i]->show();
        i++;
    }
    if (m_word.m_adj_English.isEmpty() == false)
    {
        copyLabel_property[i]->setText("adj.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        copyLabel_property[i]->show();
        copyLabel_explain[i]->setText(m_word.m_adj_English);
        copyLabel_explain[i]->show();
        i++;
    }
    if (m_word.m_adv_Chinese.isEmpty() == false)
    {
        copyLabel_property[i]->setText("adv.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        copyLabel_property[i]->show();
        copyLabel_explain[i]->setText(m_word.m_adv_Chinese);
        copyLabel_explain[i]->show();
        i++;
    }
    if (m_word.m_adv_English.isEmpty() == false)
    {
        copyLabel_property[i]->setText("adv.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        copyLabel_property[i]->show();
        copyLabel_explain[i]->setText(m_word.m_adv_English);
        copyLabel_explain[i]->show();
        i++;
    }
    if (m_word.m_vt_Chinese == m_word.m_vi_Chinese && m_word.m_vt_English == m_word.m_vi_English &&
        (m_word.m_vt_Chinese.isEmpty() == false || m_word.m_vt_English.isEmpty() == false))
    {
        if (m_word.m_vt_Chinese.isEmpty() == false)
        {
            copyLabel_property[i]->setText("vt.& vi.");
            copyLabel_property[i]->setAlignment(Qt::AlignCenter);
            copyLabel_property[i]->show();
            copyLabel_explain[i]->setText(m_word.m_vt_Chinese);
            copyLabel_explain[i]->show();
            i++;
        }
        if (m_word.m_vt_English.isEmpty() == false)
        {
            copyLabel_property[i]->setText("vt.& vi.");
            copyLabel_property[i]->setAlignment(Qt::AlignCenter);
            copyLabel_property[i]->show();
            copyLabel_explain[i]->setText(m_word.m_vt_English);
            copyLabel_explain[i]->show();
            i++;
        }
    }
    else
    {
        if (m_word.m_vt_Chinese.isEmpty() == false)
        {
            copyLabel_property[i]->setText("vt.");
            copyLabel_property[i]->setAlignment(Qt::AlignCenter);
            copyLabel_property[i]->show();
            copyLabel_explain[i]->setText(m_word.m_vt_Chinese);
            copyLabel_explain[i]->show();
            i++;
        }
        if (m_word.m_vt_English.isEmpty() == false)
        {
            copyLabel_property[i]->setText("vt.");
            copyLabel_property[i]->setAlignment(Qt::AlignCenter);
            copyLabel_property[i]->show();
            copyLabel_explain[i]->setText(m_word.m_vt_English);
            copyLabel_explain[i]->show();
            i++;
        }
        if (m_word.m_vi_Chinese.isEmpty() == false)
        {
            copyLabel_property[i]->setText("vi.");
            copyLabel_property[i]->setAlignment(Qt::AlignCenter);
            copyLabel_property[i]->show();
            copyLabel_explain[i]->setText(m_word.m_vi_Chinese);
            copyLabel_explain[i]->show();
            i++;
        }
        if (m_word.m_vi_English.isEmpty() == false)
        {
            copyLabel_property[i]->setText("vi.");
            copyLabel_property[i]->setAlignment(Qt::AlignCenter);
            copyLabel_property[i]->show();
            copyLabel_explain[i]->setText(m_word.m_vi_English);
            copyLabel_explain[i]->show();
            i++;
        }
    }
    if (m_word.m_pastTense.isEmpty() == false || m_word.m_pastParticiple.isEmpty() == false ||
        m_word.m_presentParticiple.isEmpty() == false || m_word.m_thirdPersonSingular.isEmpty() == false)
    {
        copyLabel_property[i]->setText("变形");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        copyLabel_property[i]->show();
        if (m_word.m_pastTense.isEmpty() == false)
            copyLabel_explain[i]->setText(copyLabel_explain[i]->text() + "  过去式: " + m_word.m_pastTense);
        if (m_word.m_pastParticiple.isEmpty() == false)
            copyLabel_explain[i]->setText(copyLabel_explain[i]->text() + "  过去分词: " + m_word.m_pastParticiple);
        if (m_word.m_presentParticiple.isEmpty() == false)
            copyLabel_explain[i]->setText(copyLabel_explain[i]->text() + "  现在分词: " + m_word.m_presentParticiple);
        if (m_word.m_thirdPersonSingular.isEmpty() == false)
            copyLabel_explain[i]->setText(copyLabel_explain[i]->text() + "  第三人称单数: " + m_word.m_thirdPersonSingular);
        copyLabel_explain[i]->setText(copyLabel_explain[i]->text().trimmed());
        copyLabel_explain[i]->show();
        i++;
    }
    if (m_word.m_noun_Chinese.isEmpty() == false)
    {
        copyLabel_property[i]->setText("n.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        copyLabel_property[i]->show();
        copyLabel_explain[i]->setText(m_word.m_noun_Chinese);
        copyLabel_explain[i]->show();
        i++;
    }
    if (m_word.m_noun_English.isEmpty() == false)
    {
        copyLabel_property[i]->setText("n.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        copyLabel_property[i]->show();
        copyLabel_explain[i]->setText(m_word.m_noun_English);
        copyLabel_explain[i]->show();
        i++;
    }
    if (m_word.m_prep_Chinese.isEmpty() == false)
    {
        copyLabel_property[i]->setText("prep.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        copyLabel_property[i]->show();
        copyLabel_explain[i]->setText(m_word.m_prep_Chinese);
        copyLabel_explain[i]->show();
        i++;
    }
    if (m_word.m_prep_English.isEmpty() == false)
    {
        copyLabel_property[i]->setText("prep.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        copyLabel_property[i]->show();
        copyLabel_explain[i]->setText(m_word.m_prep_English);
        copyLabel_explain[i]->show();
        i++;
    }
    if (m_word.m_conj_Chinese.isEmpty() == false)
    {
        copyLabel_property[i]->setText("conj.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        copyLabel_property[i]->show();
        copyLabel_explain[i]->setText(m_word.m_conj_Chinese);
        copyLabel_explain[i]->show();
        i++;
    }
    if (m_word.m_conj_English.isEmpty() == false)
    {
        copyLabel_property[i]->setText("conj.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        copyLabel_property[i]->show();
        copyLabel_explain[i]->setText(m_word.m_conj_English);
        copyLabel_explain[i]->show();
        i++;
    }
    if (m_word.m_pron_Chinese.isEmpty() == false)
    {
        copyLabel_property[i]->setText("pron.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        copyLabel_property[i]->show();
        copyLabel_explain[i]->setText(m_word.m_pron_Chinese);
        copyLabel_explain[i]->show();
        i++;
    }
    if (m_word.m_pron_English.isEmpty() == false)
    {
        copyLabel_property[i]->setText("pron.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        copyLabel_property[i]->show();
        copyLabel_explain[i]->setText(m_word.m_pron_English);
        copyLabel_explain[i]->show();
        i++;
    }
    if (m_word.m_art_Chinese.isEmpty() == false)
    {
        copyLabel_property[i]->setText("art.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        copyLabel_property[i]->show();
        copyLabel_explain[i]->setText(m_word.m_art_Chinese);
        copyLabel_explain[i]->show();
        i++;
    }
    if (m_word.m_art_English.isEmpty() == false)
    {
        copyLabel_property[i]->setText("art.");
        copyLabel_property[i]->setAlignment(Qt::AlignCenter);
        copyLabel_property[i]->show();
        copyLabel_explain[i]->setText(m_word.m_art_English);
        copyLabel_explain[i]->show();
        i++;
    }
    m_propertyNum = i;
    if (m_word.m_exampleSentence[0].isEmpty() == false || m_word.m_exampleSentence[1].isEmpty() == false ||
        m_word.m_exampleSentence[2].isEmpty() == false || m_word.m_exampleSentence[3].isEmpty() == false ||
        m_word.m_exampleSentence[4].isEmpty() == false || m_word.m_exampleSentence[5].isEmpty() == false)
    {
        copyLabel_exampleSentence->setText("例句：");
        copyLabel_exampleSentence->show();
        for (i = 0; i < 6; i++)
        {
            if (m_word.m_exampleSentence[i].isEmpty() == false)
            {
                copyLabel_example[i]->setText(QString("%1. %2").arg(i + 1).arg(m_word.m_exampleSentence[i]));
                copyLabel_example[i]->show();
            }
            else
            {
                m_exampleNum = i;
                break;
            }
        }
    }
    if (m_word.m_synonym.isEmpty() == false)
    {
        copyLabel_synonym->setText("同义词：");
        copyLabel_synonym->show();
        QStringList list = m_word.m_synonym.split(';');
        for (i = 0; i < list.size(); i++)
        {
            if (list.at(i) != "")
            {
                linkLabel_synonym[i]->setText(list.at(i));
                linkLabel_synonym[i]->show();
            }
        }
        m_synonymNum = i;
    }
    if (m_word.m_antonym.isEmpty() == false)
    {
        copyLabel_antonym->setText("反义词：");
        copyLabel_antonym->show();
        QStringList list = m_word.m_antonym.split(';');
        for (i = 0; i < list.size(); i++)
        {
            if (list.at(i) != "")
            {
                linkLabel_antonym[i]->setText(list.at(i));
                linkLabel_antonym[i]->show();
            }
        }
        m_antonymNum = i;
    }
}

void WordShowWidget::setViewPosition()
{
    if (m_screenSize == "14")
        setViewPosition_14();
    else if (m_screenSize == "15.6")
        setViewPosition_15();
}

void WordShowWidget::setViewPosition_14()// 643 635
{
    int x = 8, y = 8, h = 47;
    int len = WTool::getFontLength(copyLabel_word->font(), copyLabel_word->text()) + 12;
    copyLabel_word->setGeometry(x, y, len, h);

    if (copyLabel_phoneticSymbol->text().isEmpty() == false)
    {
        len = WTool::getFontLength(copyLabel_phoneticSymbol->font(), copyLabel_phoneticSymbol->text()) + 12;
        x = copyLabel_word->x() + copyLabel_word->width() + 16;
        y = 12, h = 39;
        copyLabel_phoneticSymbol->setGeometry(x, y, len, h);
    }

    len = WTool::getFontLength(copyLabel_times->font(), copyLabel_times->text()) + 8;
    x = 635 - len, y = 12, h = 39;
    copyLabel_times->setGeometry(x, y, len, h);

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

    if (copyLabel_exampleSentence->text().isEmpty() == false)
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

    if (copyLabel_synonym->text().isEmpty() == false)
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

    if (copyLabel_antonym->text().isEmpty() == false)
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
    widget_show->setGeometry(0, 0, 660, y);
}

void WordShowWidget::setViewPosition_15()
{
    int x = 10, y = 10, h = 55;
    int len = WTool::getFontLength(copyLabel_word->font(), copyLabel_word->text()) + 12;
    copyLabel_word->setGeometry(x, y, len, h);

    if (copyLabel_phoneticSymbol->text().isEmpty() == false)
    {
        len = WTool::getFontLength(copyLabel_phoneticSymbol->font(), copyLabel_phoneticSymbol->text()) + 12;
        x = copyLabel_word->x() + copyLabel_word->width() + 20;
        y = 15, h = 44;
        copyLabel_phoneticSymbol->setGeometry(x, y, len, h);
    }

    len = WTool::getFontLength(copyLabel_times->font(), copyLabel_times->text()) + 8;
    x = 790 - len, y = 15, h = 44;
    copyLabel_times->setGeometry(x, y, len, h);

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

    if (copyLabel_exampleSentence->text().isEmpty() == false)
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

    if (copyLabel_synonym->text().isEmpty() == false)
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

    if (copyLabel_antonym->text().isEmpty() == false)
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
    widget_show->setGeometry(0, 0, 820, y);
}

void WordShowWidget::slot_btnReturn_Clicked()
{
    emit sendMessageSignal(WMessage("return", ""));
}

void WordShowWidget::slot_btnMid_Clicked()
{
    if (m_word.m_times != 10)
    {
        m_word.m_times = 10;
        m_word.m_modifyTime = QDateTime::currentDateTime();
        m_word.m_remember = (m_word.m_remember > 0 ? 2 : -1);
        if (p_wordAdmin->updateWord(&m_word) == true)
        {
            copyLabel_times->setText(QString("T:%1").arg(m_word.m_times));
            if (m_screenSize == "14")
            {
                int len = WTool::getFontLength(copyLabel_times->font(), copyLabel_times->text()) + 8;
                copyLabel_times->setGeometry(635 - len, 12, len, 39);
            }
            else if (m_screenSize == "15.6")
            {
                int len = WTool::getFontLength(copyLabel_times->font(), copyLabel_times->text()) + 8;
                copyLabel_times->setGeometry(790 - len, 15, len, 44);
            }
            emit wordTimeIncreaseSignal(m_word.m_name);
        }
    }
}

void WordShowWidget::slot_btnMax_Clicked()
{
    if (m_word.m_times != 100)
    {
        m_word.m_times = 100;
        m_word.m_modifyTime = QDateTime::currentDateTime();
        m_word.m_remember = (m_word.m_remember > 0 ? 2 : -1);
        if (p_wordAdmin->updateWord(&m_word) == true)
        {
            copyLabel_times->setText(QString("T:%1").arg(m_word.m_times));
            if (m_screenSize == "14")
            {
                int len = WTool::getFontLength(copyLabel_times->font(), copyLabel_times->text()) + 8;
                copyLabel_times->setGeometry(635 - len, 12, len, 39);
            }
            else if (m_screenSize == "15.6")
            {
                int len = WTool::getFontLength(copyLabel_times->font(), copyLabel_times->text()) + 8;
                copyLabel_times->setGeometry(790 - len, 15, len, 44);
            }
            emit wordTimeIncreaseSignal(m_word.m_name);
        }
    }
}

void WordShowWidget::slot_btnAdd_Clicked()
{
    m_word.m_times++;
    m_word.m_modifyTime = QDateTime::currentDateTime();
    m_word.m_remember = (m_word.m_remember > 0 ? 2 : -1);
    if (p_wordAdmin->updateWord(&m_word) == true)
    {
        copyLabel_times->setText(QString("T:%1").arg(m_word.m_times));
        if (m_screenSize == "14")
        {
            int len = WTool::getFontLength(copyLabel_times->font(), copyLabel_times->text()) + 8;
            copyLabel_times->setGeometry(635 - len, 12, len, 39);
        }
        else if (m_screenSize == "15.6")
        {
            int len = WTool::getFontLength(copyLabel_times->font(), copyLabel_times->text()) + 8;
            copyLabel_times->setGeometry(790 - len, 15, len, 44);
        }
        emit wordTimeIncreaseSignal(m_word.m_name);
    }
}

void WordShowWidget::slot_btnDelete_Clicked()
{
    if (QMessageBox::question(this, "question", QString("是否删除 %1?").arg(m_word.m_name),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        if (p_wordAdmin->deleteWord(m_word.m_name) == true)
        {
            emit sendMessageSignal(WMessage("delete success", m_word.m_name));
            emit wordTimeIncreaseSignal(m_word.m_name);
        }
        else
            QMessageBox::about(this, "提示", QString("delete %1 fail").arg(m_word.m_name));
    }
}

void WordShowWidget::slot_btnEdit_Clicked()
{
    emit sendMessageSignal(WMessage("edit word", m_word.m_name));
}

void WordShowWidget::slot_btnReset_Clicked()
{
    m_word.m_times = 0;
    m_word.m_modifyTime = QDateTime::currentDateTime();
    m_word.m_remember = (m_word.m_remember > 0) ? 2 : -1;
    if (p_wordAdmin->updateWord(&m_word) == true)
    {
        copyLabel_times->setText("T:0");
        emit wordTimeIncreaseSignal(m_word.m_name);
    }
    else
        QMessageBox::about(this, "提示", "重置单词失败");
}

void WordShowWidget::slot_wbtnRemember_Clicked(bool active)
{
    m_word.m_remember = active ? 2 : -1;
    m_word.m_modifyTime = QDateTime::currentDateTime();
    if (p_wordAdmin->updateWord(m_word.m_name, "RememberState", QString::number(m_word.m_remember),
        "ModifyTime", m_word.m_modifyTime.toString(TIMEFORMAT)) == true)
    {
        emit wordTimeIncreaseSignal(m_word.m_name);
    }
    else
        QMessageBox::about(this, "提示", "设为记住失败");
}

void WordShowWidget::slot_wordLinkPressed()
{
    //    qDebug() << sender()->objectName() << " pressed";
}

void WordShowWidget::slot_wordTimeDecline(QString name)
{
    if (m_word.m_name == name)
    {
        p_wordAdmin->getWordInfo(name, &m_word);
        copyLabel_times->setText(QString("T:%1").arg(m_word.m_times));
    }
}

void WordShowWidget::slot_wordTimeIncrease(QString name)
{
    if (m_word.m_name == name)
    {
        p_wordAdmin->getWordInfo(name, &m_word);
        copyLabel_times->setText(QString("T:%1").arg(m_word.m_times));
    }
}

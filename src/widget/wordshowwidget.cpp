#include "wordshowwidget.h"
#include "ui_wordshowwidget.h"
#include "wtool.h"
#include "global.h"
#include "wordterminator.h"
#include <QDebug>
#include <QThread>
#include <QScrollBar>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTimer>

extern WordAdmin *p_wordAdmin;
extern WordTerminator *p_wordTerm;

WordShowWidget::WordShowWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordShowWidget)
{
    setStyleSheet(WTool::getStyleQss("WordShowWidget"));
    ui->setupUi(this);

    m_propertyNum = 0;
    m_exampleNum = 0;
    m_synonymNum = 0;
    m_antonymNum = 0;

    ui->wbtn_isRemember->setCheckedTip("set to not forever");
    ui->wbtn_isRemember->setUncheckedTip("set to forever");

    //CopyLabel
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

    //LinkLabel
    for (int i = 0; i < RELATED_NUM; ++i)
    {
        linkLabel_synonym[i] = this->findChild<LinkLabel *>(QString("linkLabel_synonym%1").arg(i));
        connect(linkLabel_synonym[i], SIGNAL(released()), this, SLOT(slot_wordLinkPressed()));
    }
    for (int i = 0; i < RELATED_NUM; ++i)
    {
        linkLabel_antonym[i] = this->findChild<LinkLabel *>(QString("linkLabel_antonym%1").arg(i));
        connect(linkLabel_antonym[i], SIGNAL(released()), this, SLOT(slot_wordLinkPressed()));
    }

    m_reloadFlag = true;
    reloadGlobalValue();

    WTool::LayoutHelper_init(dynamic_cast<QBoxLayout *>(ui->widget_show->layout()));

    connect(ui->btn_return, SIGNAL(clicked()), this, SLOT(slot_btnReturn_Clicked()));
    connect(ui->btn_min, SIGNAL(clicked()), this, SLOT(slot_btnMin_Clicked()));
    connect(ui->btn_mid, SIGNAL(clicked()), this, SLOT(slot_btnMid_Clicked()));
    connect(ui->btn_max, SIGNAL(clicked()), this, SLOT(slot_btnMax_Clicked()));
    connect(ui->btn_add, SIGNAL(clicked()), this, SLOT(slot_btnAdd_Clicked()));
    connect(ui->btn_delete, SIGNAL(clicked()), this, SLOT(slot_btnDelete_Clicked()));
    connect(ui->btn_edit, SIGNAL(clicked()), this, SLOT(slot_btnEdit_Clicked()));
    connect(ui->btn_reset, SIGNAL(clicked()), this, SLOT(slot_btnReset_Clicked()));
    connect(ui->wbtn_isRemember, SIGNAL(clicked(bool)), this, SLOT(slot_wbtnRemember_Clicked(bool)));
    connect(ui->combox_group, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboxGroup_currentIndexChanged(int)));
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
    ui->btn_min->setToolTip(QString("Set times %1").arg(Global::m_timesSet1.getValueStr()));
    ui->btn_min->setText(Global::m_timesSet1.getValueStr());
    ui->btn_mid->setToolTip(QString("Set times %1").arg(Global::m_timesSet2.getValueStr()));
    ui->btn_mid->setText(Global::m_timesSet2.getValueStr());
    ui->btn_max->setToolTip(QString("Set times %1").arg(Global::m_timesSet3.getValueStr()));
    ui->btn_max->setText(Global::m_timesSet3.getValueStr());

    m_propertyNum = 0;
    m_exampleNum = 0;
    m_synonymNum = 0;
    m_antonymNum = 0;
    ui->scrollArea->verticalScrollBar()->setValue(0);
    ui->copyLabel_word->clear();
    ui->copyLabel_phoneticSymbol->clear();
    ui->copyLabel_phoneticSymbol->hide();
    ui->copyLabel_times->setText("T:");
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
}

void WordShowWidget::reloadGlobalValue()
{
    if (m_reloadFlag)
    {
        m_reloadFlag = false;
        QSignalBlocker sb(ui->combox_group);
        ui->combox_group->clear();
        m_groupList = WTool::getGroupList();
        for (int i = 0; i < m_groupList.count(); ++i)
        {
            ui->combox_group->addItem(m_groupList.at(i));
        }
    }
}

void WordShowWidget::setReloadFlag(bool flag)
{
    m_reloadFlag = flag;
}

bool WordShowWidget::loadWordInfo(QString name)
{
    if (p_wordAdmin->getWordInfo(name, &m_word))
    {
        QTimer::singleShot(0, this, [this] { this->setWordInfo(); });
        return true;
    }
    else
        return false;
}

void WordShowWidget::setWordInfo()
{
    QSignalBlocker sb(ui->combox_group);
    m_lineNum = 1;

    ui->copyLabel_word->setText(m_word.m_name);
    for (int i = 0; i < m_groupList.count(); ++i)
    {
        if (Global::m_groupName[m_word.m_groupid].getValueStr() == m_groupList.at(i))
        {
            ui->combox_group->setCurrentIndex(i);
            break;
        }
    }
    if (!m_word.m_phoneticSymbol.isEmpty())
    {
        ui->copyLabel_phoneticSymbol->setText(m_word.m_phoneticSymbol);
        ui->copyLabel_phoneticSymbol->show();
    }
    ui->copyLabel_times->setText(QString("T:%1").arg(m_word.m_times));
    ui->wbtn_isRemember->setChecked(m_word.m_remember > 0);
    int i = 0;
    if (!m_word.m_adj_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("adj.");
        copyLabel_explain[i]->setText(m_word.m_adj_Chinese);
        widget_explain[i++]->show();
    }
    if (!m_word.m_adj_English.isEmpty())
    {
        copyLabel_property[i]->setText("adj.");
        copyLabel_explain[i]->setText(m_word.m_adj_English);
        widget_explain[i++]->show();
    }
    if (!m_word.m_adv_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("adv.");
        copyLabel_explain[i]->setText(m_word.m_adv_Chinese);
        widget_explain[i++]->show();
    }
    if (!m_word.m_adv_English.isEmpty())
    {
        copyLabel_property[i]->setText("adv.");
        copyLabel_explain[i]->setText(m_word.m_adv_English);
        widget_explain[i++]->show();
    }
    if (m_word.m_vt_Chinese == m_word.m_vi_Chinese && m_word.m_vt_English == m_word.m_vi_English &&
        (!m_word.m_vt_Chinese.isEmpty() || !m_word.m_vt_English.isEmpty()))
    {
        if (!m_word.m_vt_Chinese.isEmpty())
        {
            copyLabel_property[i]->setText("vt.& vi.");
            copyLabel_explain[i]->setText(m_word.m_vt_Chinese);
            widget_explain[i++]->show();
        }
        if (!m_word.m_vt_English.isEmpty())
        {
            copyLabel_property[i]->setText("vt.& vi.");
            copyLabel_explain[i]->setText(m_word.m_vt_English);
            widget_explain[i++]->show();
        }
    }
    else
    {
        if (!m_word.m_vt_Chinese.isEmpty())
        {
            copyLabel_property[i]->setText("vt.");
            copyLabel_explain[i]->setText(m_word.m_vt_Chinese);
            widget_explain[i++]->show();
        }
        if (!m_word.m_vt_English.isEmpty())
        {
            copyLabel_property[i]->setText("vt.");
            copyLabel_explain[i]->setText(m_word.m_vt_English);
            widget_explain[i++]->show();
        }
        if (!m_word.m_vi_Chinese.isEmpty())
        {
            copyLabel_property[i]->setText("vi.");
            copyLabel_explain[i]->setText(m_word.m_vi_Chinese);
            widget_explain[i++]->show();
        }
        if (!m_word.m_vi_English.isEmpty())
        {
            copyLabel_property[i]->setText("vi.");
            copyLabel_explain[i]->setText(m_word.m_vi_English);
            widget_explain[i++]->show();
        }
    }
    if (!m_word.m_pastTense.isEmpty() || !m_word.m_pastParticiple.isEmpty() ||
        !m_word.m_presentParticiple.isEmpty() || !m_word.m_thirdPersonSingular.isEmpty())
    {
        copyLabel_property[i]->setText(QString::fromUtf8("变形"));
        QString explain;
        if (!m_word.m_pastTense.isEmpty())
            explain.append("  过去式: " + m_word.m_pastTense);
        if (!m_word.m_pastParticiple.isEmpty())
            explain.append("  过去分词: " + m_word.m_pastParticiple);
        if (!m_word.m_presentParticiple.isEmpty())
            explain.append("  现在分词: " + m_word.m_presentParticiple);
        if (!m_word.m_thirdPersonSingular.isEmpty())
            explain.append("  第三人称单数: " + m_word.m_thirdPersonSingular);
        copyLabel_explain[i]->setText(explain.trimmed());
        widget_explain[i++]->show();
    }
    if (!m_word.m_noun_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("n.");
        copyLabel_explain[i]->setText(m_word.m_noun_Chinese);
        widget_explain[i++]->show();
    }
    if (!m_word.m_noun_English.isEmpty())
    {
        copyLabel_property[i]->setText("n.");
        copyLabel_explain[i]->setText(m_word.m_noun_English);
        widget_explain[i++]->show();
    }
    if (!m_word.m_prep_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("prep.");
        copyLabel_explain[i]->setText(m_word.m_prep_Chinese);
        widget_explain[i++]->show();
    }
    if (!m_word.m_prep_English.isEmpty())
    {
        copyLabel_property[i]->setText("prep.");
        copyLabel_explain[i]->setText(m_word.m_prep_English);
        widget_explain[i++]->show();
    }
    if (!m_word.m_conj_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("conj.");
        copyLabel_explain[i]->setText(m_word.m_conj_Chinese);
        widget_explain[i++]->show();
    }
    if (!m_word.m_conj_English.isEmpty())
    {
        copyLabel_property[i]->setText("conj.");
        copyLabel_explain[i]->setText(m_word.m_conj_English);
        widget_explain[i++]->show();
    }
    if (!m_word.m_pron_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("pron.");
        copyLabel_explain[i]->setText(m_word.m_pron_Chinese);
        widget_explain[i++]->show();
    }
    if (!m_word.m_pron_English.isEmpty())
    {
        copyLabel_property[i]->setText("pron.");
        copyLabel_explain[i]->setText(m_word.m_pron_English);
        widget_explain[i++]->show();
    }
    if (!m_word.m_art_Chinese.isEmpty())
    {
        copyLabel_property[i]->setText("art.");
        copyLabel_explain[i]->setText(m_word.m_art_Chinese);
        widget_explain[i++]->show();
    }
    if (!m_word.m_art_English.isEmpty())
    {
        copyLabel_property[i]->setText("art.");
        copyLabel_explain[i]->setText(m_word.m_art_English);
        widget_explain[i++]->show();
    }
    m_propertyNum = i;
    m_lineNum += m_propertyNum;

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
        m_exampleNum = i;
        m_lineNum += m_exampleNum + 1;
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
        m_synonymNum = i;
        if (m_synonymNum > 0) ui->widget_synonym0->show();
        if (m_synonymNum > 4) ui->widget_synonym1->show();

        if (m_synonymNum > 4)
            m_lineNum += 3;
        else if (m_synonymNum > 0)
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
        m_antonymNum = i;
        if (m_antonymNum > 0) ui->widget_antonym0->show();
        if (m_antonymNum > 4) ui->widget_antonym1->show();

        if (m_antonymNum > 4)
            m_lineNum += 3;
        else if (m_antonymNum > 0)
            m_lineNum += 2;
    }
    reloadLayout();
}

void WordShowWidget::reloadLayout()
{
    auto ma = ui->widget_show->layout()->contentsMargins();
    int sp = ui->widget_show->layout()->spacing();
    int bar_width = m_lineNum > 10 ? ui->scrollArea->verticalScrollBar()->width() : 0;
    int width = ui->scrollArea->width() - bar_width;
    int l_width = width - ma.left() - ma.right();

    int x = ma.left(), y = ma.top(), h = ui->copyLabel_word->minimumHeight();
    QSize s;

    ui->widget_word->setGeometry(x, y, l_width, h);
    ui->copyLabel_word->setGeometry(0, 0, ui->copyLabel_word->sizeHint().width(), h);
    if (ui->copyLabel_phoneticSymbol->isVisible())
        ui->copyLabel_phoneticSymbol->setGeometry(ui->copyLabel_word->width() + ui->widget_word->layout()->spacing(), 0, ui->copyLabel_phoneticSymbol->sizeHint().width(), h);
    s = ui->copyLabel_times->sizeHint();
    ui->copyLabel_times->setGeometry(l_width - s.width(), 0, s.width(), h);

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
    ui->widget_show->setFixedSize(width, y);
}

void WordShowWidget::setWordTimes(int times)
{
    if (m_word.m_times != times)
    {
        m_word.m_times = times;
        m_word.m_modifyTime = QDateTime::currentDateTime();
        m_word.m_remember = (m_word.m_remember > 0 ? 2 : -1);
        if (p_wordAdmin->updateWord(&m_word))
        {
            ui->copyLabel_times->setText(QString("T:%1").arg(m_word.m_times));
            int len = WTool::getFontLength(ui->copyLabel_times->font(), ui->copyLabel_times->text()) + 8;
            ui->copyLabel_times->setGeometry(790 - len, 15, len, 44);
            emit wordTimeIncreaseSignal(m_word.m_name);
        }
    }
}

void WordShowWidget::slot_btnReturn_Clicked()
{
    emit sendMessageSignal(WMessage("return", ""));
}

void WordShowWidget::slot_btnMin_Clicked()
{
    setWordTimes(Global::m_timesSet1.getValueInt());
}

void WordShowWidget::slot_btnMid_Clicked()
{
    setWordTimes(Global::m_timesSet2.getValueInt());
}

void WordShowWidget::slot_btnMax_Clicked()
{
    setWordTimes(Global::m_timesSet3.getValueInt());
}

void WordShowWidget::slot_btnAdd_Clicked()
{
    m_word.m_times++;
    m_word.m_modifyTime = QDateTime::currentDateTime();
    m_word.m_remember = (m_word.m_remember > 0 ? 2 : -1);
    if (p_wordAdmin->updateWord(&m_word))
    {
        ui->copyLabel_times->setText(QString("T:%1").arg(m_word.m_times));
        int len = WTool::getFontLength(ui->copyLabel_times->font(), ui->copyLabel_times->text()) + 8;
        ui->copyLabel_times->setGeometry(790 - len, 15, len, 44);
        emit wordTimeIncreaseSignal(m_word.m_name);
    }
}

void WordShowWidget::slot_btnDelete_Clicked()
{
    if (QMessageBox::question(this, "question", QString("是否删除 %1?").arg(m_word.m_name),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        if (p_wordAdmin->deleteWord(m_word.m_name))
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
    if (p_wordAdmin->updateWord(&m_word))
    {
        ui->copyLabel_times->setText("T:0");
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
        "ModifyTime", m_word.m_modifyTime.toString(TIMEFORMAT)))
    {
        emit wordTimeIncreaseSignal(m_word.m_name);
    }
    else
        QMessageBox::about(this, "提示", "设为记住失败");
}

void WordShowWidget::slot_comboxGroup_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    if (p_wordTerm != nullptr && p_wordTerm->getCurrentWidgetIndex() == WordTerminator::Widget_WordShow)
    {
        m_word.m_groupid = WTool::getGroupNo(ui->combox_group->currentText());
        m_word.m_remember = (m_word.m_remember > 0 ? 2 : -1);
        p_wordAdmin->updateWord(m_word.m_name, "Groupid", QString::number(m_word.m_groupid),
            "RememberState", QString::number(m_word.m_remember));
    }
}

void WordShowWidget::slot_wordLinkPressed()
{
//    DEBUG << sender()->objectName() << " pressed";
}

void WordShowWidget::slot_wordTimeDecline(QString name)
{
    if (m_word.m_name == name)
    {
        p_wordAdmin->getWordInfo(name, &m_word);
        ui->copyLabel_times->setText(QString("T:%1").arg(m_word.m_times));
    }
}

void WordShowWidget::slot_wordTimeIncrease(QString name)
{
    if (m_word.m_name == name)
    {
        p_wordAdmin->getWordInfo(name, &m_word);
        ui->copyLabel_times->setText(QString("T:%1").arg(m_word.m_times));
    }
}

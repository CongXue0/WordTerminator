#include "wordautomatedmemorizewidget.h"
#include "ui_wordautomatedmemorizewidget.h"
#include "wtool.h"
#include "memorythread.h"
#include "global.h"
#include "dispatcher.h"
#include "wordterminator.h"
#include <QMessageBox>

extern WordAdmin *p_wordAdmin;
extern MemoryThread *p_memThread;

WordAutomatedMemorizeWidget::WordAutomatedMemorizeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordAutomatedMemorizeWidget)
{
    setStyleSheet(WTool::getStyleQss("WordAutomatedMemorizeWidget"));
    ui->setupUi(this);

    m_mode = WELCOME;

    m_reloadFlag = true;
    reloadGlobalValue();

    connect(ui->checkBox_range0, SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    connect(ui->checkBox_range1, SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    connect(ui->checkBox_range2, SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    connect(ui->checkBox_range3, SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    connect(ui->checkBox_forever, SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    connect(ui->combox_group, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboxGroup_currentIndexChanged(int)));
    connect(ui->btn_start, SIGNAL(clicked()), this, SLOT(slot_btnStart_Clicked()));
}

WordAutomatedMemorizeWidget::~WordAutomatedMemorizeWidget()
{
    delete ui;
}

void WordAutomatedMemorizeWidget::recoveryInterface()
{
    setMode(WELCOME);
    setFocus();
}

void WordAutomatedMemorizeWidget::reloadGlobalValue()
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
        m_curGroupId = 0;

        ui->checkBox_range0->setText(QString("times %1~%2")
            .arg(Global::m_range1Left.getValueStr()).arg(Global::m_range1Right.getValueStr()));
        ui->checkBox_range1->setText(QString("times %1~%2")
            .arg(Global::m_range2Left.getValueStr()).arg(Global::m_range2Right.getValueStr()));
        ui->checkBox_range2->setText(QString("times %1~%2")
            .arg(Global::m_range3Left.getValueStr()).arg(Global::m_range3Right.getValueStr()));
        ui->checkBox_range3->setText(QString("times %1+").arg(Global::m_range4Left.getValueStr()));

        ui->spinBox_silderVolume->setValue(Global::m_autoSilderVolume.getValueInt());
        ui->spinBox_singleNum->setValue(Global::m_singleMemoryNum.getValueInt());
    }
}

void WordAutomatedMemorizeWidget::setReloadFlag(bool flag)
{
    m_reloadFlag = flag;
}

void WordAutomatedMemorizeWidget::updateWordStatistics()
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

int WordAutomatedMemorizeWidget::getMode()
{
    return m_mode;
}

void WordAutomatedMemorizeWidget::setMode(int mode)
{
    m_mode = mode;
    switch (m_mode)
    {
    case WELCOME:
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case MEMORY:
        ui->stackedWidget->setCurrentIndex(1);
        ui->auto_view->initiate(m_testList);
        m_testList.clear();
        ui->auto_view->start();
        break;
    }
}

void WordAutomatedMemorizeWidget::slot_radioButtonClicked()
{
    this->updateWordStatistics();
}

void WordAutomatedMemorizeWidget::slot_comboxGroup_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    m_curGroupId = WTool::getGroupNo(ui->combox_group->currentText());
    this->updateWordStatistics();
    Global::m_groupIndexMemory.setValue(m_curGroupId);
    Global::saveXML();
}

void WordAutomatedMemorizeWidget::slot_btnStart_Clicked()
{
    if (ui->spinBox_silderVolume->value() != Global::m_autoSilderVolume.getValueInt())
    {
        Global::m_autoSilderVolume.setValue(ui->spinBox_silderVolume->value());
        Global::saveXML();
    }
    if (ui->spinBox_singleNum->value() != Global::m_singleMemoryNum.getValueInt())
    {
        Global::m_singleMemoryNum.setValue(ui->spinBox_singleNum->value());
        Global::saveXML();
    }

    m_testList.clear();
    if (!ui->checkBox_range0->isChecked() && !ui->checkBox_range1->isChecked() && !ui->checkBox_range2->isChecked() &&
        !ui->checkBox_range3->isChecked() && !ui->checkBox_forever->isChecked())
    {
        m_testList = p_wordAdmin->getAllWordCanMemorizeList(m_curGroupId, 1);
    }
    else
    {
        if (ui->checkBox_range0->isChecked())
            m_testList += p_wordAdmin->getWordCanMemorizeListFromTimes(Global::m_range1Left.getValueInt(), Global::m_range1Right.getValueInt(), m_curGroupId, ui->checkBox_forever->isChecked(), 1);
        if (ui->checkBox_range1->isChecked())
            m_testList += p_wordAdmin->getWordCanMemorizeListFromTimes(Global::m_range2Left.getValueInt(), Global::m_range2Right.getValueInt(), m_curGroupId, ui->checkBox_forever->isChecked(), 1);
        if (ui->checkBox_range2->isChecked())
            m_testList += p_wordAdmin->getWordCanMemorizeListFromTimes(Global::m_range3Left.getValueInt(), Global::m_range3Right.getValueInt(), m_curGroupId, ui->checkBox_forever->isChecked(), 1);
        if (ui->checkBox_range3->isChecked())
            m_testList += p_wordAdmin->getWordCanMemorizeListFromTimes(Global::m_range4Left.getValueInt(), MAX_TIMES, m_curGroupId, ui->checkBox_forever->isChecked(), 1);
    }
    if (m_testList.size() > 0)
    {
        m_testList = m_testList.mid(0, Global::m_singleMemoryNum.getValueInt());
        DEBUG << "m_testNum:" << m_testList.size();

        setMode(MEMORY);
    }
    else
        QMessageBox::about(this, "提示", "没有要记忆的单词");
}

void WordAutomatedMemorizeWidget::slot_memorizeFinished()
{
    if (m_mode == MEMORY)
    {
        reloadGlobalValue();
        recoveryInterface();
        updateWordStatistics();
    }
}

void WordAutomatedMemorizeWidget::slot_stopWordMemorize(bool *ret)
{
    if (WordTerminator::instance()->getCurrentWidgetIndex() != WordTerminator::Widget_AutomatedMemorize) return;
    if (m_mode == MEMORY)
    {
        auto ani = ui->auto_view->currentAnimation();
        bool run = false;
        if (ani && ani->state() == QAbstractAnimation::Running)
        {
            run = true;
            ani->pause();
        }
        if (QMessageBox::question(this, "queation", "是否停止记忆?", QMessageBox::Yes,
            QMessageBox::No) == QMessageBox::Yes)
        {
            if (ret != nullptr)
            {
                *ret = true;
            }
            ui->auto_view->setState(WordAutomatedMemorizeSceneGraphView::State_Init);
        }
        else
        {
            if (ret != nullptr)
                *ret = false;
            if (ani && run)
            {
                ani->resume();
            }
        }
    }
    else
    {
        if (ret != nullptr)
        {
            *ret = true;
        }
    }
}

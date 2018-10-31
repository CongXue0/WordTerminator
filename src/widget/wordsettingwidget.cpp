#include "wordsettingwidget.h"
#include "wtool.h"
#include <QIntValidator>
#include "global.h"
#include <QMessageBox>

WordSettingWidget::WordSettingWidget(QWidget *parent) : QWidget(parent)
{
    tabWidget = new QTabWidget(this);
    tabWidget->setObjectName("tabWidget");

    widget_forgetSet = new QWidget(tabWidget);
    widget_forgetSet->setObjectName("widget_forgetSet");
    widget_libSet = new QWidget(tabWidget);
    widget_libSet->setObjectName("widget_libSet");
    widget_groupSet = new QWidget(tabWidget);
    widget_groupSet->setObjectName("widget_groupSet");
    widget_help = new QWidget(tabWidget);
    widget_help->setObjectName("widget_help");

    tabWidget->addTab(widget_forgetSet, "记忆设置");
    tabWidget->addTab(widget_libSet, "显示设置");
    tabWidget->addTab(widget_groupSet, "组设置");
    tabWidget->addTab(widget_help, "帮助");

    label_leastFoTi = new QLabel(widget_forgetSet);
    label_leastFoTi->setObjectName("label_leastFoTi");
    label_leastFoTi->setText("记忆时可以设为已记忆的最小次数：");
    label_memInterval = new QLabel(widget_forgetSet);
    label_memInterval->setObjectName("label_memInterval");
    label_memInterval->setText("刚记忆过的单词过多久可以再次记忆(分钟)：");
    label_declineTimes = new QLabel(widget_forgetSet);
    label_declineTimes->setObjectName("label_declineTimes");
    label_declineTimes->setWordWrap(true);
    label_declineTimes->setText("单词过多久记忆次数下降一次的时间分布， 单位是小时， 大于10次每增加一次下降时间增加24小时：");
    for (int i = 0; i < 10; i++)
    {
        label_times[i] = new QLabel(widget_forgetSet);
        label_times[i]->setObjectName(QString("label_times%1").arg(i));
        label_times[i]->setText(QString("%1次：").arg(i + 1));
    }
    label_memLevel = new QLabel(widget_forgetSet);
    label_memLevel->setObjectName("label_memLevel");
    label_memLevel->setText("记忆难度：");
    label_commonShow = new QLabel(widget_libSet);
    label_commonShow->setObjectName("label_commonShow");
    label_commonShow->setText("单词常用的显示区间，与单词库显示区所对应：");
    label_range1 = new QLabel(widget_libSet);
    label_range1->setObjectName("label_range1");
    label_range1->setText("区间1：");
    label_range2 = new QLabel(widget_libSet);
    label_range2->setObjectName("label_range2");
    label_range2->setText("区间2：");
    label_range3 = new QLabel(widget_libSet);
    label_range3->setObjectName("label_range3");
    label_range3->setText("区间3：");
    label_range4 = new QLabel(widget_libSet);
    label_range4->setObjectName("label_range4");
    label_range4->setText("区间4：");
    label_range1_f = new QLabel(widget_libSet);
    label_range1_f->setObjectName("label_range1_f");
    label_range1_f->setText("-");
    label_range2_f = new QLabel(widget_libSet);
    label_range2_f->setObjectName("label_range2_f");
    label_range2_f->setText("-");
    label_range3_f = new QLabel(widget_libSet);
    label_range3_f->setObjectName("label_range3_f");
    label_range3_f->setText("-");
    label_range4_plus = new QLabel(widget_libSet);
    label_range4_plus->setObjectName("label_range4_plus");
    label_range4_plus->setText("+");
    label_timesSet1 = new QLabel(widget_libSet);
    label_timesSet1->setObjectName("label_timesSet1");
    label_timesSet1->setText("次数设置1：");
    label_timesSet2 = new QLabel(widget_libSet);
    label_timesSet2->setObjectName("label_timesSet2");
    label_timesSet2->setText("次数设置2：");
    for (int i = 0; i < 21; i++)
    {
        label_group[i] = new QLabel(widget_groupSet);
        label_group[i]->setObjectName(QString("label_group%1").arg(i));
        label_group[i]->setText(QString("组%1名称：").arg(i));
    }

    QValidator *v2 = new QIntValidator(1, 10000, this);
    lineEdit_leastFoTi = new QLineEdit(widget_forgetSet);
    lineEdit_leastFoTi->setObjectName("lineEdit_leastFoTi");
    lineEdit_memInterval = new QLineEdit(widget_forgetSet);
    lineEdit_memInterval->setObjectName("lineEdit_memInterval");
    for (int i = 0; i < 10; i++)
    {
        lineEdit_times[i] = new QLineEdit(widget_forgetSet);
        lineEdit_times[i]->setObjectName(QString("lineEdit_times%1").arg(i));
        lineEdit_times[i]->setValidator(v2);
    }
    lineEdit_range1Left = new QLineEdit(widget_libSet);
    lineEdit_range1Left->setObjectName("lineEdit_range1Left");
    lineEdit_range1Right = new QLineEdit(widget_libSet);
    lineEdit_range1Right->setObjectName("lineEdit_range1Right");
    lineEdit_range2Left = new QLineEdit(widget_libSet);
    lineEdit_range2Left->setObjectName("lineEdit_range2Left");
    lineEdit_range2Right = new QLineEdit(widget_libSet);
    lineEdit_range2Right->setObjectName("lineEdit_range2Right");
    lineEdit_range3Left = new QLineEdit(widget_libSet);
    lineEdit_range3Left->setObjectName("lineEdit_range3Left");
    lineEdit_range3Right = new QLineEdit(widget_libSet);
    lineEdit_range3Right->setObjectName("lineEdit_range3Right");
    lineEdit_range4Left = new QLineEdit(widget_libSet);
    lineEdit_range4Left->setObjectName("lineEdit_range4Left");
    lineEdit_timesSet1 = new QLineEdit(widget_libSet);
    lineEdit_timesSet1->setObjectName("lineEdit_timesSet1");
    lineEdit_timesSet2 = new QLineEdit(widget_libSet);
    lineEdit_timesSet2->setObjectName("lineEdit_timesSet2");
    for (int i = 0; i < 21; i++)
    {
        lineEdit_groupName[i] = new QLineEdit(widget_groupSet);
        lineEdit_groupName[i]->setObjectName(QString("lineEdit_group%1Name").arg(i));
    }

    combox_memLevel = new QComboBox(widget_forgetSet);
    combox_memLevel->setObjectName("combox_memLevel");
    combox_memLevel->insertItem(0, "简单");
    combox_memLevel->insertItem(1, "困难");

    btn_reset = new QPushButton(widget_help);
    btn_reset->setObjectName("btn_reset");
    btn_reset->setText("恢复默认");
    connect(btn_reset, SIGNAL(clicked()), this, SLOT(slot_btnReset_clicked()));

    QValidator *v1 = new QIntValidator(1, 99, this);
    lineEdit_leastFoTi->setValidator(v1);
    lineEdit_memInterval->setValidator(v2);
    lineEdit_range1Left->setValidator(v2);
    lineEdit_range1Right->setValidator(v2);
    lineEdit_range2Left->setValidator(v2);
    lineEdit_range2Right->setValidator(v2);
    lineEdit_range3Left->setValidator(v2);
    lineEdit_range3Right->setValidator(v2);
    lineEdit_range4Left->setValidator(v2);
    lineEdit_timesSet1->setValidator(v2);
    lineEdit_timesSet2->setValidator(v2);

    reloadGlobalValue();
    loadStyleSheet();
}

void WordSettingWidget::recoveryInterface()
{
}

void WordSettingWidget::reloadGlobalValue()
{
    lineEdit_leastFoTi->setText(Global::m_leastForeverTimes.getValueStr());
    lineEdit_memInterval->setText(Global::m_memoryInterval.getValueStr());
    for (int i = 0; i < 10; i++)
    {
        lineEdit_times[i]->setText(Global::m_decline_times[i].getValueStr());
    }
    combox_memLevel->setCurrentIndex(Global::m_memoryLevel.getValueInt());
    lineEdit_range1Left->setText(Global::m_range1Left.getValueStr());
    lineEdit_range1Right->setText(Global::m_range1Right.getValueStr());
    lineEdit_range2Left->setText(Global::m_range2Left.getValueStr());
    lineEdit_range2Right->setText(Global::m_range2Right.getValueStr());
    lineEdit_range3Left->setText(Global::m_range3Left.getValueStr());
    lineEdit_range3Right->setText(Global::m_range3Right.getValueStr());
    lineEdit_range4Left->setText(Global::m_range4Left.getValueStr());
    lineEdit_timesSet1->setText(Global::m_timesSet1.getValueStr());
    lineEdit_timesSet2->setText(Global::m_timesSet2.getValueStr());
    for (int i = 0; i < 21; i++)
    {
        lineEdit_groupName[i]->setText(Global::m_groupName[i].getValueStr());
    }
}

void WordSettingWidget::saveGlobalValue()
{
    if (lineEdit_leastFoTi->text().toInt() < 1)
    {
        lineEdit_leastFoTi->setText("1");
    }
    if (lineEdit_memInterval->text().toInt() < 1)
    {
        lineEdit_memInterval->setText("1");
    }
    for (int i = 0; i < 10; i++)
    {
        if (lineEdit_times[i]->text().toInt() < 1)
        {
            lineEdit_times[i]->setText("1");
        }
    }
    if (lineEdit_range1Left->text().toInt() < 0)
    {
        lineEdit_range1Left->setText("0");
    }
    if (lineEdit_range1Right->text().toInt() < 1)
    {
        lineEdit_range1Right->setText("1");
    }
    if (lineEdit_range2Left->text().toInt() < 1)
    {
        lineEdit_range2Left->setText("1");
    }
    if (lineEdit_range2Right->text().toInt() < 1)
    {
        lineEdit_range2Right->setText("1");
    }
    if (lineEdit_range3Left->text().toInt() < 1)
    {
        lineEdit_range3Left->setText("1");
    }
    if (lineEdit_range3Right->text().toInt() < 1)
    {
        lineEdit_range3Right->setText("1");
    }
    if (lineEdit_range4Left->text().toInt() < 1)
    {
        lineEdit_range4Left->setText("1");
    }
    if (lineEdit_timesSet1->text().toInt() < 0)
    {
        lineEdit_timesSet1->setText("0");
    }
    if (lineEdit_timesSet2->text().toInt() < 0)
    {
        lineEdit_timesSet2->setText("0");
    }
    for (int i = 0; i < 21; i++)
    {
        lineEdit_groupName[i]->setText(lineEdit_groupName[i]->text().trimmed());
    }
    QString tmp;
    if (lineEdit_range1Left->text().toInt() > lineEdit_range1Right->text().toInt())
    {
        tmp = lineEdit_range1Left->text();
        lineEdit_range1Left->setText(lineEdit_range1Right->text());
        lineEdit_range1Right->setText(tmp);
    }
    if (lineEdit_range2Left->text().toInt() > lineEdit_range2Right->text().toInt())
    {
        tmp = lineEdit_range2Left->text();
        lineEdit_range2Left->setText(lineEdit_range2Right->text());
        lineEdit_range2Right->setText(tmp);
    }
    if (lineEdit_range3Left->text().toInt() > lineEdit_range3Right->text().toInt())
    {
        tmp = lineEdit_range3Left->text();
        lineEdit_range3Left->setText(lineEdit_range3Right->text());
        lineEdit_range3Right->setText(tmp);
    }
    if (lineEdit_range1Right->text().toInt() > lineEdit_range2Left->text().toInt() || lineEdit_range2Right->text().toInt() > lineEdit_range3Left->text().toInt() || lineEdit_range3Right->text().toInt() > lineEdit_range4Left->text().toInt())
    {
        QMessageBox::about(this, "error", "次数区间只能为递增");
        return;
    }

    Global::m_leastForeverTimes.setValue(lineEdit_leastFoTi->text());
    Global::m_memoryInterval.setValue(lineEdit_memInterval->text());
    for (int i = 0; i < 10; i++)
    {
        Global::m_decline_times[i].setValue(lineEdit_times[i]->text());
    }
    Global::m_memoryLevel.setValue(combox_memLevel->currentIndex());
    Global::m_range1Left.setValue(lineEdit_range1Left->text());
    Global::m_range1Right.setValue(lineEdit_range1Right->text());
    Global::m_range2Left.setValue(lineEdit_range2Left->text());
    Global::m_range2Right.setValue(lineEdit_range2Right->text());
    Global::m_range3Left.setValue(lineEdit_range3Left->text());
    Global::m_range3Right.setValue(lineEdit_range3Right->text());
    Global::m_range4Left.setValue(lineEdit_range4Left->text());
    Global::m_timesSet1.setValue(lineEdit_timesSet1->text());
    Global::m_timesSet2.setValue(lineEdit_timesSet2->text());
    for (int i = 0; i < 21; i++)
    {
        Global::m_groupName[i].setValue(lineEdit_groupName[i]->text());
    }

    Global::saveXML();
}

void WordSettingWidget::loadStyleSheet()
{
    setStyleSheet(WTool::getWordSettingWidgetQss());
}

void WordSettingWidget::slot_btnReset_clicked()
{
    if (QMessageBox::question(this, "question", "Are you sure to reset all configs?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        Global::reset();
        Global::saveXML();
    }
}

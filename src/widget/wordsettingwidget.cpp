#include "wordsettingwidget.h"
#include "wtool.h"
#include <QIntValidator>
#include "global.h"

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

    tabWidget->addTab(widget_forgetSet, "遗忘参数");
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

    QValidator *v1 = new QIntValidator(1, 99, this);
    lineEdit_leastFoTi->setValidator(v1);
    lineEdit_memInterval->setValidator(v2);


    reloadGlobalValue();
    loadStyleSheet();
}

void WordSettingWidget::recoveryInterface()
{
    DEBUG<<lineEdit_leastFoTi->text();
}

void WordSettingWidget::reloadGlobalValue()
{
    lineEdit_leastFoTi->setText(Global::m_leastForeverTimes.getValueStr());
    lineEdit_memInterval->setText(Global::m_memoryInterval.getValueStr());
    for (int i = 0; i < 10; i++)
    {
        lineEdit_times[i]->setText(Global::m_decline_times[i].getValueStr());
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

    Global::m_leastForeverTimes.setValue(lineEdit_leastFoTi->text());
    Global::m_memoryInterval.setValue(lineEdit_memInterval->text());
    for (int i = 0; i < 10; i++)
    {
        Global::m_decline_times[i].setValue(lineEdit_times[i]->text());
    }

    Global::saveXML();
}

void WordSettingWidget::loadStyleSheet()
{
     setStyleSheet(WTool::getWordSettingWidgetQss());
}

#include "wordsettingwidget.h"
#include "ui_wordsettingwidget.h"
#include "wtool.h"
#include "global.h"
#include "dispatcher.h"
#include <QIntValidator>

WordSettingWidget::WordSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordSettingWidget)
{
    setStyleSheet(WTool::getStyleQss("WordSettingWidget"));
    ui->setupUi(this);

    QValidator *v2 = new QIntValidator(1, 10000, this);
    for (int i = 0; i < 10; ++i)
    {
        lineEdit_times[i] = this->findChild<QLineEdit *>(QString("lineEdit_times%1").arg(i));
        lineEdit_times[i]->setValidator(v2);
    }
    for (int i = 0; i < 21; ++i)
    {
        lineEdit_groupName[i] = this->findChild<QLineEdit *>(QString("lineEdit_group%1Name").arg(i));
    }

    ui->combox_memLevel->addItem("简单");
    ui->combox_memLevel->addItem("困难");

    connect(ui->btn_reset, SIGNAL(clicked()), this, SLOT(slot_btnReset_clicked()));

    QValidator *v1 = new QIntValidator(1, 99, this);
    ui->lineEdit_leastFoTi->setValidator(v1);
    ui->lineEdit_memInterval->setValidator(v2);
    ui->lineEdit_range1Left->setValidator(v2);
    ui->lineEdit_range1Right->setValidator(v2);
    ui->lineEdit_range2Left->setValidator(v2);
    ui->lineEdit_range2Right->setValidator(v2);
    ui->lineEdit_range3Left->setValidator(v2);
    ui->lineEdit_range3Right->setValidator(v2);
    ui->lineEdit_range4Left->setValidator(v2);
    ui->lineEdit_timesSet1->setValidator(v2);
    ui->lineEdit_timesSet2->setValidator(v2);
    ui->lineEdit_timesSet3->setValidator(v2);

    QValidator *v3 = new QIntValidator(10, 10000000, this);
    ui->lineEdit_pageVolume->setValidator(v3);

    reloadGlobalValue();
}

WordSettingWidget::~WordSettingWidget()
{
    delete ui;
}

void WordSettingWidget::recoveryInterface()
{
}

void WordSettingWidget::reloadGlobalValue()
{
    ui->lineEdit_leastFoTi->setText(Global::m_leastForeverTimes.getValueStr());
    ui->lineEdit_memInterval->setText(Global::m_memoryInterval.getValueStr());
    for (int i = 0; i < 10; ++i)
    {
        lineEdit_times[i]->setText(Global::m_decline_times[i].getValueStr());
    }
    ui->combox_memLevel->setCurrentIndex(Global::m_memoryLevel.getValueInt());
    ui->lineEdit_range1Left->setText(Global::m_range1Left.getValueStr());
    ui->lineEdit_range1Right->setText(Global::m_range1Right.getValueStr());
    ui->lineEdit_range2Left->setText(Global::m_range2Left.getValueStr());
    ui->lineEdit_range2Right->setText(Global::m_range2Right.getValueStr());
    ui->lineEdit_range3Left->setText(Global::m_range3Left.getValueStr());
    ui->lineEdit_range3Right->setText(Global::m_range3Right.getValueStr());
    ui->lineEdit_range4Left->setText(Global::m_range4Left.getValueStr());
    ui->lineEdit_timesSet1->setText(Global::m_timesSet1.getValueStr());
    ui->lineEdit_timesSet2->setText(Global::m_timesSet2.getValueStr());
    ui->lineEdit_timesSet3->setText(Global::m_timesSet3.getValueStr());
    ui->lineEdit_pageVolume->setText(Global::m_pageVolume.getValueStr());
    for (int i = 0; i < 21; ++i)
    {
        lineEdit_groupName[i]->setText(Global::m_groupName[i].getValueStr());
    }
}

void WordSettingWidget::saveGlobalValue()
{
    if (ui->lineEdit_leastFoTi->text().toInt() < 1)
    {
        ui->lineEdit_leastFoTi->setText("1");
    }
    if (ui->lineEdit_memInterval->text().toInt() < 1)
    {
        ui->lineEdit_memInterval->setText("1");
    }
    for (int i = 0; i < 10; ++i)
    {
        if (lineEdit_times[i]->text().toInt() < 1)
        {
            lineEdit_times[i]->setText("1");
        }
    }
    if (ui->lineEdit_range1Left->text().toInt() < 0)
    {
        ui->lineEdit_range1Left->setText("0");
    }
    if (ui->lineEdit_range1Right->text().toInt() < 1)
    {
        ui->lineEdit_range1Right->setText("1");
    }
    if (ui->lineEdit_range2Left->text().toInt() < 1)
    {
        ui->lineEdit_range2Left->setText("1");
    }
    if (ui->lineEdit_range2Right->text().toInt() < 1)
    {
        ui->lineEdit_range2Right->setText("1");
    }
    if (ui->lineEdit_range3Left->text().toInt() < 1)
    {
       ui-> lineEdit_range3Left->setText("1");
    }
    if (ui->lineEdit_range3Right->text().toInt() < 1)
    {
        ui->lineEdit_range3Right->setText("1");
    }
    if (ui->lineEdit_range4Left->text().toInt() < 1)
    {
        ui->lineEdit_range4Left->setText("1");
    }
    if (ui->lineEdit_timesSet1->text().toInt() < 0)
    {
        ui->lineEdit_timesSet1->setText("0");
    }
    if (ui->lineEdit_timesSet2->text().toInt() < 0)
    {
        ui->lineEdit_timesSet2->setText("0");
    }
    if (ui->lineEdit_timesSet3->text().toInt() < 0)
    {
        ui->lineEdit_timesSet3->setText("0");
    }
    if (ui->lineEdit_pageVolume->text().toInt() < 0)
    {
        ui->lineEdit_pageVolume->setText("10");
    }
    for (int i = 0; i < 21; ++i)
    {
        lineEdit_groupName[i]->setText(lineEdit_groupName[i]->text().trimmed());
    }
    if (lineEdit_groupName[0]->text().isEmpty())
    {
        lineEdit_groupName[0]->setText("default");
    }
    QString tmp;
    if (ui->lineEdit_range1Left->text().toInt() > ui->lineEdit_range1Right->text().toInt())
    {
        tmp = ui->lineEdit_range1Left->text();
        ui->lineEdit_range1Left->setText(ui->lineEdit_range1Right->text());
        ui->lineEdit_range1Right->setText(tmp);
    }
    if (ui->lineEdit_range2Left->text().toInt() > ui->lineEdit_range2Right->text().toInt())
    {
        tmp = ui->lineEdit_range2Left->text();
        ui->lineEdit_range2Left->setText(ui->lineEdit_range2Right->text());
        ui->lineEdit_range2Right->setText(tmp);
    }
    if (ui->lineEdit_range3Left->text().toInt() > ui->lineEdit_range3Right->text().toInt())
    {
        tmp = ui->lineEdit_range3Left->text();
        ui->lineEdit_range3Left->setText(ui->lineEdit_range3Right->text());
        ui->lineEdit_range3Right->setText(tmp);
    }
    if (ui->lineEdit_range1Right->text().toInt() > ui->lineEdit_range2Left->text().toInt() || ui->lineEdit_range2Right->text().toInt() > ui->lineEdit_range3Left->text().toInt() ||
        ui->lineEdit_range3Right->text().toInt() > ui->lineEdit_range4Left->text().toInt())
    {
        ERRORBOX("次数区间只能为递增");
        return;
    }

    Global::m_leastForeverTimes.setValue(ui->lineEdit_leastFoTi->text());
    Global::m_memoryInterval.setValue(ui->lineEdit_memInterval->text());
    for (int i = 0; i < 10; ++i)
    {
        Global::m_decline_times[i].setValue(lineEdit_times[i]->text());
    }
    Global::m_memoryLevel.setValue(ui->combox_memLevel->currentIndex());
    Global::m_range1Left.setValue(ui->lineEdit_range1Left->text());
    Global::m_range1Right.setValue(ui->lineEdit_range1Right->text());
    Global::m_range2Left.setValue(ui->lineEdit_range2Left->text());
    Global::m_range2Right.setValue(ui->lineEdit_range2Right->text());
    Global::m_range3Left.setValue(ui->lineEdit_range3Left->text());
    Global::m_range3Right.setValue(ui->lineEdit_range3Right->text());
    Global::m_range4Left.setValue(ui->lineEdit_range4Left->text());
    Global::m_timesSet1.setValue(ui->lineEdit_timesSet1->text());
    Global::m_timesSet2.setValue(ui->lineEdit_timesSet2->text());
    Global::m_timesSet3.setValue(ui->lineEdit_timesSet3->text());
    Global::m_pageVolume.setValue(ui->lineEdit_pageVolume->text());
    for (int i = 0; i < 21; ++i)
    {
        Global::m_groupName[i].setValue(lineEdit_groupName[i]->text());
    }

    if (Global::saveXML())
    {
        emit Dispatch(this).signal_sendMessage(WMessage("set reload flag", "true"));
    }
}

void WordSettingWidget::slot_btnReset_clicked()
{
    if (QUESTION_ISYES("Are you sure to reset all configs?"))
    {
        Global::reset();
        Global::saveXML(true);
        emit Dispatch(this).signal_sendMessage(WMessage("set reload flag", "true"));
    }
}

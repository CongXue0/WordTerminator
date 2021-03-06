#include "wordfunctionwidget.h"
#include "ui_wordfunctionwidget.h"
#include "wtool.h"
#include "wordadmin.h"
#include "global.h"
#include <QInputDialog>
#include <QStandardPaths>
#include <QFile>
#include <QProgressDialog>
#include <QCoreApplication>
#include <QTextCodec>
#include <QStringList>

extern WordAdmin *p_wordAdmin;

WordFunctionWidget::WordFunctionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordFunctionWidget)
{
    setStyleSheet(WTool::getStyleQss("WordFunctionWidget"));
    ui->setupUi(this);

    ui->combox_script->addItem("方案1");
    ui->combox_script->addItem("方案2");
    ui->combox_script->addItem("方案3");
    ui->combox_script->addItem("方案4");
    ui->combox_script->addItem("方案5");

    reloadGlobalValue();

    connect(ui->btn_refresh, SIGNAL(clicked()), this, SLOT(slot_btnRefresh_clicked()));
    connect(ui->btn_export, SIGNAL(clicked()), this, SLOT(slot_btnExport_clicked()));
    connect(ui->combox_script, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboxScript_currentIndexChanged(int)));
}

WordFunctionWidget::~WordFunctionWidget()
{
    delete ui;
}

void WordFunctionWidget::recoveryInterface()
{
}

void WordFunctionWidget::reloadGlobalValue()
{
    ui->combox_script->setCurrentIndex(Global::m_curScript.getValueInt());
    slot_comboxScript_currentIndexChanged(ui->combox_script->currentIndex());

    ui->spinBox_spaceNum->setValue(Global::m_exportSpaceNum.getValueInt());
}

void WordFunctionWidget::saveGlobalValue()
{
    ui->lineEdit_script->setText(ui->lineEdit_script->text().trimmed());
    int index = ui->combox_script->currentIndex();
    switch (index)
    {
    case 0:
        Global::m_curScript.setValue(0);
        Global::m_script0.setValue(ui->lineEdit_script->text());
        break;
    case 1:
        Global::m_curScript.setValue(1);
        Global::m_script1.setValue(ui->lineEdit_script->text());
        break;
    case 2:
        Global::m_curScript.setValue(2);
        Global::m_script2.setValue(ui->lineEdit_script->text());
        break;
    case 3:
        Global::m_curScript.setValue(3);
        Global::m_script3.setValue(ui->lineEdit_script->text());
        break;
    case 4:
        Global::m_curScript.setValue(4);
        Global::m_script4.setValue(ui->lineEdit_script->text());
        break;
    }
    Global::m_exportSpaceNum.setValue(ui->spinBox_spaceNum->value());
    Global::saveXML();
}

void WordFunctionWidget::exportWord(int t1, int t2, int group, int remember)
{
    if (t1 == -1 && t2 == -1)
    {
        t1 = 0;
        t2 = MAX_TIMES;
    }
    int left = t1, right = t2;
    if (left > right)
    {
        int tmp = left;
        left = right;
        right = tmp;
    }
    QString rem;
    if (remember == 0)
        rem = "u";
    else if (remember == 1)
        rem = "r";
    else if (remember == 2)
        rem = "a";
    else
    {
        return;
    }
    QString fileName = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).at(0) +
        QString("/word(%1-%2)g%3_%4_%5.txt").arg(left).arg(right).arg((group == -1) ? "a" : QString::number(group)).arg(rem)
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        WARNING("file can't open");
    }
    QTextStream in(&file);
    QTextCodec *code = QTextCodec::codecForName("utf8");
    in.setCodec(code);

    QVector<BriefWordInfo> list;
    if (remember == 0)
        list = p_wordAdmin->getBriefWordInfoListFromTimes(left, right, group, false);
    else if (remember == 1)
        list = p_wordAdmin->getBriefWordInfoListFromTimes(left, right, group, true);
    else if (remember == -1)
        list = p_wordAdmin->getAllBriefWordInfoList(group);
    int type = Global::m_sortType.getValueInt();
    if (type > 0)//0 None, 1 Name, 2 Times, 3 ModifyTime
    {
        bool descend = Global::m_sortDescend.getValueInt() == 0;
        std::sort(list.begin(), list.end(), [&](const BriefWordInfo& t1, const BriefWordInfo& t2) {
            if (type == 1)
            {
                bool b1 = t1.m_name.compare(t2.m_name, Qt::CaseInsensitive) >= 0;
                return descend ? b1 : !b1;
            }
            else if (type == 2)
            {
                bool b1 = t1.m_times >= t2.m_times;
                return descend ? b1 : !b1;
            }
            else if (type == 3)
            {
                bool b1 = t1.m_modifyTime >= t2.m_modifyTime;
                return descend ? b1 : !b1;
            }
            else
                return true;
        });
    }

    int count = list.count();
    if (count > 0)
    {
        QString spaceStr;
        QString splitStr = "--------------------";
        for (int i = 0; i < ui->spinBox_spaceNum->value(); ++i)
        {
            spaceStr.append(" ");
            splitStr.append("-");
        }

        WordInfo wordInfo;
        QProgressDialog dialog("单词导出进度", "取消", 0, count, this);
        dialog.setWindowTitle(tr("单词导出对话框"));
        dialog.setWindowModality(Qt::WindowModal);
        dialog.show();
        int pageVolume = Global::m_pageVolume.getValueInt();
        for (int i = 0; i < count; ++i)
        {
            dialog.setValue(i);
            QCoreApplication::processEvents();
            if (dialog.wasCanceled())
                break;
            if (p_wordAdmin->getWordInfo(list[i].m_name, &wordInfo))
            {
                QString txt = wordInfo.toText(1, spaceStr);
                if (pageVolume > 0 && i != 0 && i % pageVolume == 0)
                    in << splitStr + "\n\n";
                if (i == count - 1)
                    in << txt;
                else
                    in << txt + "\n";
            }
            else
            {
                in << list[i].m_name + " " << "get fail\n";
            }
        }
        dialog.setValue(count);
    }
    else
    {
        in << "There are no words.";
    }
    MESSAGE("导出单词数 " + QString::number(count));
    file.close();
    file.setPermissions(QFile::ReadOwner | QFile::ReadUser | QFile::ReadGroup | QFile::ReadOther);
}

void WordFunctionWidget::slot_btnRefresh_clicked()
{
    int num = p_wordAdmin->resetAllWordRemerber(this);
    MESSAGE("ok " + QString::number(num));
}

void WordFunctionWidget::slot_btnExport_clicked()
{
    ui->lineEdit_script->setText(ui->lineEdit_script->text().trimmed());
    QString script = ui->lineEdit_script->text().trimmed();
    if (script.isEmpty())
    {
        ERRORBOX("输入不能为空");
        return;
    }
    QRegExp reg("((-1,-1|\\d+,\\d+),(-1|\\d+),(-1|\\d+);)+");
    if (reg.exactMatch(script))
    {
        saveGlobalValue();
        QStringList list = script.split(';');
        for (int i = 0; i < list.count(); ++i)
        {
            if (list.at(i).isEmpty())
            {
                continue;
            }
            QStringList tmpList = list.at(i).split(',');
            exportWord(tmpList.at(0).toInt(), tmpList.at(1).toInt(), tmpList.at(2).toInt(), tmpList.at(3).toInt());
        }
    }
    else
    {
        ERRORBOX("格式错误");
    }
}

void WordFunctionWidget::slot_comboxScript_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        ui->lineEdit_script->setText(Global::m_script0.getValueStr());
        break;
    case 1:
        ui->lineEdit_script->setText(Global::m_script1.getValueStr());
        break;
    case 2:
        ui->lineEdit_script->setText(Global::m_script2.getValueStr());
        break;
    case 3:
        ui->lineEdit_script->setText(Global::m_script3.getValueStr());
        break;
    case 4:
        ui->lineEdit_script->setText(Global::m_script4.getValueStr());
        break;
    }
}

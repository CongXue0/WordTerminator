#include "wordfunctionwidget.h"
#include "wtool.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QStandardPaths>
#include <QFile>
#include "wordadmin.h"
#include <QProgressDialog>
#include <QCoreApplication>
#include <QTextCodec>
#include "global.h"
#include <QStringList>

extern WordAdmin *p_wordAdmin;

WordFunctionWidget::WordFunctionWidget(QWidget *parent) : QWidget(parent)
{
    label_bg_func = new QLabel(this);
    label_bg_func->setObjectName("label_bg_func");
    label_refresh = new QLabel(label_bg_func);
    label_refresh->setObjectName("label_refresh");
    label_refresh->setText("使所有单词可以再次记忆");
    label_export = new QLabel(label_bg_func);
    label_export->setObjectName("label_export");
    label_export->setText("导出单词到桌面");

    line1 = new QLabel(label_bg_func);
    line1->setObjectName("line1");
    line2 = new QLabel(label_bg_func);
    line2->setObjectName("line2");

    btn_refresh = new QPushButton(label_bg_func);
    btn_refresh->setObjectName("btn_refresh");
    btn_refresh->setText("Refresh");
    connect(btn_refresh, SIGNAL(pressed()), this, SLOT(slot_btnRefresh_pressed()));

    btn_export = new QPushButton(label_bg_func);
    btn_export->setObjectName("btn_export");
    btn_export->setText("Export");
    connect(btn_export, SIGNAL(pressed()), this, SLOT(slot_btnExport_pressed()));

    combox_script = new QComboBox(label_bg_func);
    combox_script->setObjectName("combox_script");
    combox_script->insertItem(0, "方案1");
    combox_script->insertItem(1, "方案2");
    combox_script->insertItem(2, "方案3");
    combox_script->insertItem(3, "方案4");
    combox_script->insertItem(4, "方案5");
    connect(combox_script, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboxScript_currentIndexChanged(int)));

    lineEdit_script = new QLineEdit(label_bg_func);
    lineEdit_script->setObjectName("lineEdit_script");

    reloadGlobalValue();
    loadStyleSheet();
}

void WordFunctionWidget::recoveryInterface()
{
}

void WordFunctionWidget::reloadGlobalValue()
{
    combox_script->setCurrentIndex(Global::m_curScript.getValueInt());
    slot_comboxScript_currentIndexChanged(combox_script->currentIndex());
}

void WordFunctionWidget::saveGlobalValue()
{
    lineEdit_script->setText(lineEdit_script->text().trimmed());
    int index = combox_script->currentIndex();
    switch (index)
    {
    case 0:
        Global::m_curScript.setValue(0);
        Global::m_script0.setValue(lineEdit_script->text());
        break;
    case 1:
        Global::m_curScript.setValue(1);
        Global::m_script1.setValue(lineEdit_script->text());
        break;
    case 2:
        Global::m_curScript.setValue(2);
        Global::m_script2.setValue(lineEdit_script->text());
        break;
    case 3:
        Global::m_curScript.setValue(3);
        Global::m_script3.setValue(lineEdit_script->text());
        break;
    case 4:
        Global::m_curScript.setValue(4);
        Global::m_script4.setValue(lineEdit_script->text());
        break;
    }
    Global::saveXML();
}

void WordFunctionWidget::loadStyleSheet()
{
    setStyleSheet(WTool::getWordFunctionWidgetQss());
}

void WordFunctionWidget::exportWord(int t1, int t2, int group, int remember)
{
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
        QString("/word(%1-%2)%3_%4.txt").arg(left).arg(right).arg(rem)
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "file write", "can't open", QMessageBox::Yes);
    }
    QTextStream in(&file);
    QTextCodec *code = QTextCodec::codecForName("utf8");
    in.setCodec(code);

    QStringList list;
    if (remember == 0)
        list = p_wordAdmin->getWordListFromTimes(left, right, false);
    else if (remember == 1)
        list = p_wordAdmin->getWordListFromTimes(left, right, true);
    else if (remember == 2)
        list = p_wordAdmin->getAllWordList();
    int count = list.count();
    if (count > 0)
    {
        WordInfo wordInfo;
        QProgressDialog dialog("单词导出进度", "取消", 0, count, this);
        dialog.setWindowTitle(tr("单词导出对话框"));
        dialog.setWindowModality(Qt::WindowModal);
        dialog.show();
        for (int i = 0; i < count; i++)
        {
            dialog.setValue(i);
            QCoreApplication::processEvents();
            if (dialog.wasCanceled())
                break;
            if (p_wordAdmin->getWordInfo(list.at(i), &wordInfo))
            {
                if (i == count - 1)
                    in << wordInfo.toText();
                else
                    in << wordInfo.toText() + "\n";
            }
            else
            {
                in << list.at(i) + " " << "get fail\n";
            }
        }
        dialog.setValue(count);
    }
    else
    {
        in << "没有单词";
    }
    QMessageBox::about(this, "提示", "导出单词数 " + QString::number(count));
    file.close();
    file.setPermissions(QFile::ReadOwner | QFile::ReadUser | QFile::ReadGroup | QFile::ReadOther);
}

void WordFunctionWidget::slot_btnRefresh_pressed()
{
    int num = p_wordAdmin->resetAllWordRemerber(this);
    QMessageBox::about(this, "提示", "ok " + QString::number(num));
}

void WordFunctionWidget::slot_btnExport_pressed()
{
    lineEdit_script->setText(lineEdit_script->text().trimmed());
    QString script = lineEdit_script->text().trimmed();
    if (script.isEmpty())
    {
        QMessageBox::about(this, "error", "输入不能为空");
        return;
    }
    QRegExp reg("((-1,-1|\\d+,\\d+),(-1|\\d+),(-1|\\d+);)+");
    if (reg.exactMatch(script))
    {
        saveGlobalValue();
        QStringList list = script.split(';');
        for (int i = 0; i < list.count(); i++)
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
        QMessageBox::about(this, "error", "格式错误");
    }
}

void WordFunctionWidget::slot_comboxScript_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        lineEdit_script->setText(Global::m_script0.getValueStr());
        break;
    case 1:
        lineEdit_script->setText(Global::m_script1.getValueStr());
        break;
    case 2:
        lineEdit_script->setText(Global::m_script2.getValueStr());
        break;
    case 3:
        lineEdit_script->setText(Global::m_script3.getValueStr());
        break;
    case 4:
        lineEdit_script->setText(Global::m_script4.getValueStr());
        break;
    }
}

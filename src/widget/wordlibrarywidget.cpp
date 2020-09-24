#include "wordlibrarywidget.h"
#include "ui_wordlibrarywidget.h"
#include "wordadmin.h"
#include "global.h"
#include "dispatcher.h"
#include <QDebug>
#include <QKeyEvent>
#include <QApplication>
#include <QMessageBox>

extern WordAdmin *p_wordAdmin;

WordLibraryWidget::WordLibraryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordLibraryWidget)
{
    setStyleSheet(WTool::getStyleQss("WordLibraryWidget"));
    ui->setupUi(this);

    m_menu1 = new QMenu("操作", this);
    m_menu1->setObjectName("m_menu1");

    m_menu2 = new QMenu("set group", this);
    m_menu2->setObjectName("m_menu2");

    m_wordList.clear();
    m_model = new QStringListModel();

    ui->wordList->setModel(m_model);
    ui->wordList->viewport()->installEventFilter(this);

    ui->comboBox_search->addItem("prefix");
    ui->comboBox_search->addItem("suffix");
    ui->comboBox_search->addItem("contain");
    ui->comboBox_search->addItem("interpretation");

    m_reloadFlag = true;
    reloadGlobalValue();
    updateWordList();
    updateWordStatistics();

    connect(m_menu1, SIGNAL(triggered(QAction *)), this, SLOT(slot_menu1Triggered(QAction *)));
    connect(m_menu2, SIGNAL(triggered(QAction *)), this, SLOT(slot_menu2Triggered(QAction *)));

    connect(ui->btn_search, SIGNAL(clicked()), this, SLOT(slot_btnSearch_clicked()));
    connect(ui->btn_create, SIGNAL(clicked()), this, SLOT(slot_btnCreate_clicked()));

    connect(ui->wordList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slot_itemDoubleClicked(QModelIndex)));

    connect(ui->checkBox_range0, SIGNAL(clicked()), this, SLOT(slot_checkBoxClicked()));
    connect(ui->checkBox_range1, SIGNAL(clicked()), this, SLOT(slot_checkBoxClicked()));
    connect(ui->checkBox_range2, SIGNAL(clicked()), this, SLOT(slot_checkBoxClicked()));
    connect(ui->checkBox_range3, SIGNAL(clicked()), this, SLOT(slot_checkBoxClicked()));
    connect(ui->checkBox_forever, SIGNAL(clicked()), this, SLOT(slot_checkBoxClicked()));
    connect(ui->comboBox_group, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboxGroup_currentIndexChanged(int)));
}

WordLibraryWidget::~WordLibraryWidget()
{
    delete ui;
}

void WordLibraryWidget::recoveryInterface()
{
    clearSearch();
    m_wordList.clear();
    m_model->setStringList(m_wordList);
    ui->checkBox_range0->setChecked(false);
    ui->checkBox_range1->setChecked(false);
    ui->checkBox_range2->setChecked(false);
    ui->checkBox_range3->setChecked(false);
}

void WordLibraryWidget::reloadGlobalValue()
{
    if (m_reloadFlag)
    {
        m_reloadFlag = false;

        m_menu1->clear();
        m_menu2->clear();
        m_menu1->addAction(QString("Set times %1").arg(Global::m_timesSet1.getValueInt()))->setObjectName("menu1");
        m_menu1->addAction(QString("Set times %1").arg(Global::m_timesSet2.getValueInt()))->setObjectName("menu1");
        m_menu1->addAction(QString("Set times %1").arg(Global::m_timesSet3.getValueInt()))->setObjectName("menu1");
        QStringList list = WTool::getGroupList();
        for (int i = 0; i < list.count(); ++i)
        {
            m_menu2->addAction(list.at(i))->setObjectName("menu2");
        }
        m_menu1->addMenu(m_menu2);
        m_menu1->addAction(QString("Set to forever"))->setObjectName("menu1");
        m_menu1->addAction(QString("Set to not forever"))->setObjectName("menu1");
        m_menu1->addAction(QString("Delete"))->setObjectName("menu1");
        m_menu1->addAction(QString("Clear times"))->setObjectName("menu1");

        QSignalBlocker sb(ui->comboBox_group);
        ui->comboBox_group->clear();
        ui->comboBox_group->addItem(ALL_GROUP);
        for (int i = 0; i < list.count(); ++i)
        {
            ui->comboBox_group->addItem(list.at(i));
        }
        ui->comboBox_group->setCurrentIndex(Global::m_groupIndexLib.getValueInt() + 1);
        m_curGroupId = WTool::getGroupNo(ui->comboBox_group->currentText());

        ui->label_statistics->setText(QString("statistics:\n"
            "%1~%2 : \n"
            "%3~%4 : \n"
            "%5~%6 : \n"
            "%7+ : \n"
            "normal: \n"
            "forever: ").arg(Global::m_range1Left.getValueInt()).arg(Global::m_range1Right.getValueInt())
            .arg(Global::m_range2Left.getValueInt()).arg(Global::m_range2Right.getValueInt())
            .arg(Global::m_range3Left.getValueInt()).arg(Global::m_range3Right.getValueInt())
            .arg(Global::m_range4Left.getValueInt()));
        ui->checkBox_range0->setText(QString("times %1~%2")
            .arg(Global::m_range1Left.getValueStr()).arg(Global::m_range1Right.getValueStr()));
        ui->checkBox_range1->setText(QString("times %1~%2")
            .arg(Global::m_range2Left.getValueStr()).arg(Global::m_range2Right.getValueStr()));
        ui->checkBox_range2->setText(QString("times %1~%2")
            .arg(Global::m_range3Left.getValueStr()).arg(Global::m_range3Right.getValueStr()));
        ui->checkBox_range3->setText(QString("times %1+").arg(Global::m_range4Left.getValueStr()));
    }
}

void WordLibraryWidget::setReloadFlag(bool flag)
{
    m_reloadFlag = flag;
}

void WordLibraryWidget::clearSearch()
{
    ui->lineEdit_search->clear();
}

void WordLibraryWidget::updateWordList()
{
    slot_btnSearch_clicked();
}

void WordLibraryWidget::updateWordStatistics()
{
    bool rem = ui->checkBox_forever->isChecked();
    ui->label_statistics->setText(QString("statistics:\n%1~%2 : %8\n%3~%4 : %9\n%5~%6 : %10\n%7+ : %11\nnormal: %12\nforever: %13")
        .arg(Global::m_range1Left.getValueInt()).arg(Global::m_range1Right.getValueInt())
        .arg(Global::m_range2Left.getValueInt()).arg(Global::m_range2Right.getValueInt())
        .arg(Global::m_range3Left.getValueInt()).arg(Global::m_range3Right.getValueInt()).arg(Global::m_range4Left.getValueInt())
        .arg(p_wordAdmin->getWordNumFromTimes(Global::m_range1Left.getValueInt(), Global::m_range1Right.getValueInt(), m_curGroupId, rem))
        .arg(p_wordAdmin->getWordNumFromTimes(Global::m_range2Left.getValueInt(), Global::m_range2Right.getValueInt(), m_curGroupId, rem))
        .arg(p_wordAdmin->getWordNumFromTimes(Global::m_range3Left.getValueInt(), Global::m_range3Right.getValueInt(), m_curGroupId, rem))
        .arg(p_wordAdmin->getWordNumFromTimes(Global::m_range4Left.getValueInt(), MAX_TIMES, m_curGroupId, rem))
        .arg(p_wordAdmin->getWordNumFromTimes(0, MAX_TIMES, m_curGroupId, false)).arg(p_wordAdmin->getWordNumFromTimes(0, MAX_TIMES, m_curGroupId, true)));
}

void WordLibraryWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return && (event->modifiers() & Qt::ControlModifier))
    {
        slot_btnCreate_clicked();
    }
    else if (ui->lineEdit_search->hasFocus() && event->key() == Qt::Key_Return)
    {
        slot_btnSearch_clicked();
    }
}

bool WordLibraryWidget::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == ui->wordList->viewport() && e->type() == QEvent::MouseButtonPress && static_cast<QMouseEvent *>(e)->button() == Qt::RightButton)
    {
        m_modelList = ui->wordList->selectionModel()->selectedIndexes();
        m_menu1->exec(QCursor::pos());
    }
    return QWidget::eventFilter(obj, e);
}

void WordLibraryWidget::slot_menu1Triggered(QAction *act)
{
    if (act->objectName() != "menu1" || m_modelList.count() == 0)
        return;
    QString operation = act->text();
    if (operation.startsWith("Set times"))
    {
        int times = operation.mid(10, operation.length()).toInt();
        BriefWordInfo word;
        for (int i = 0; i < m_modelList.count(); ++i)
        {
            if (p_wordAdmin->getWordBriefInfo(m_modelList.at(i).data().toString(), &word))
            {
                word.m_times = times;
                word.m_modifyTime = QDateTime::currentDateTime();
                word.m_remember = (word.m_remember > 0 ? 2 : -1);
                if (p_wordAdmin->updateWord(word.m_name, "Times", QString::number(word.m_times), "ModifyTime",
                    word.m_modifyTime.toString(TIMEFORMAT), "RememberState", QString::number(word.m_remember)))
                {
                    emit Dispatch(this).signal_wordTimeIncrease(word.m_name);
                }
            }
        }
        this->updateWordList();
        this->updateWordStatistics();
    }
    else if (operation == "Set to forever" || operation == "Set to not forever")
    {
        int remember;
        if (operation == "Set to forever")
            remember = 2;
        else
            remember = -1;
        BriefWordInfo word;
        for (int i = 0; i < m_modelList.count(); ++i)
        {
            if (p_wordAdmin->getWordBriefInfo(m_modelList.at(i).data().toString(), &word))
            {
                word.m_modifyTime = QDateTime::currentDateTime();
                word.m_remember = remember;
                if (p_wordAdmin->updateWord(word.m_name, "ModifyTime", word.m_modifyTime.toString(TIMEFORMAT),
                    "RememberState", QString::number(word.m_remember)))
                {
                    emit Dispatch(this).signal_wordTimeIncrease(word.m_name);
                }
            }
        }
        this->updateWordList();
        this->updateWordStatistics();
    }
    else if (operation == "Delete")
    {
        if (QMessageBox::question(this, "question", QString("是否删除?"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
        {
            QString name;
            for (int i = 0; i < m_modelList.count(); ++i)
            {
                name = m_modelList.at(i).data().toString();
                if (p_wordAdmin->deleteWord(name))
                {
                    emit Dispatch(this).signal_wordTimeIncrease(name);
                }
            }
            this->updateWordList();
            this->updateWordStatistics();
        }
    }
    else if (operation == "Clear times")
    {
        BriefWordInfo word;
        for (int i = 0; i < m_modelList.count(); ++i)
        {
            if (p_wordAdmin->getWordBriefInfo(m_modelList.at(i).data().toString(), &word))
            {
                word.m_times = 0;
                word.m_modifyTime = QDateTime::currentDateTime();
                word.m_remember = (word.m_remember > 0 ? 2 : -1);
                if (p_wordAdmin->updateWord(word.m_name, "Times", QString::number(word.m_times), "ModifyTime",
                    word.m_modifyTime.toString(TIMEFORMAT), "RememberState", QString::number(word.m_remember)))
                {
                    emit Dispatch(this).signal_wordTimeIncrease(word.m_name);
                }
            }
        }
        this->updateWordList();
        this->updateWordStatistics();
    }
}

void WordLibraryWidget::slot_menu2Triggered(QAction *act)
{
    if (act->objectName() != "menu2" || m_modelList.count() == 0)
        return;
    int groupId = WTool::getGroupNo(act->text());
    BriefWordInfo word;
    for (int i = 0; i < m_modelList.count(); ++i)
    {
        if (p_wordAdmin->getWordBriefInfo(m_modelList.at(i).data().toString(), &word))
        {
            word.m_modifyTime = QDateTime::currentDateTime();
            word.m_groupId = groupId;
            word.m_remember = (word.m_remember > 0 ? 2 : -1);
            if (p_wordAdmin->updateWord(word.m_name, "ModifyTime", word.m_modifyTime.toString(TIMEFORMAT),
                "Groupid", QString::number(word.m_groupId), "RememberState", QString::number(word.m_remember)))
            {
                emit Dispatch(this).signal_wordTimeIncrease(word.m_name);
            }
        }
    }
    this->updateWordList();
    this->updateWordStatistics();
}

void WordLibraryWidget::slot_btnSearch_clicked()
{
    m_wordList.clear();
    if (!ui->checkBox_range0->isChecked() && !ui->checkBox_range1->isChecked() && !ui->checkBox_range2->isChecked() &&
        !ui->checkBox_range3->isChecked() && !ui->checkBox_forever->isChecked())
    {
        m_wordList = p_wordAdmin->getAllWordList(m_curGroupId);
    }
    else
    {
        if (ui->checkBox_range0->isChecked())
            m_wordList += p_wordAdmin->getWordListFromTimes(Global::m_range1Left.getValueInt(), Global::m_range1Right.getValueInt(), m_curGroupId, ui->checkBox_forever->isChecked());
        if (ui->checkBox_range1->isChecked())
            m_wordList += p_wordAdmin->getWordListFromTimes(Global::m_range2Left.getValueInt(), Global::m_range2Right.getValueInt(), m_curGroupId, ui->checkBox_forever->isChecked());
        if (ui->checkBox_range2->isChecked())
            m_wordList += p_wordAdmin->getWordListFromTimes(Global::m_range3Left.getValueInt(), Global::m_range3Right.getValueInt(), m_curGroupId, ui->checkBox_forever->isChecked());
        if (ui->checkBox_range3->isChecked())
            m_wordList += p_wordAdmin->getWordListFromTimes(Global::m_range4Left.getValueInt(), MAX_TIMES, m_curGroupId, ui->checkBox_forever->isChecked());
    }
    if (!ui->lineEdit_search->text().isEmpty())
    {
        m_wordList = WTool::filterWordFromList(m_wordList, ui->lineEdit_search->text(), ui->comboBox_search->currentText());
    }
    m_model->setStringList(m_wordList);
}

void WordLibraryWidget::slot_btnCreate_clicked()
{
    emit Dispatch(this).signal_sendMessage(WMessage("create word", ""));
}

void WordLibraryWidget::slot_itemDoubleClicked(QModelIndex index)
{
    QString name = index.data().toString();
    emit Dispatch(this).signal_sendMessage(WMessage("show word", name));
}

void WordLibraryWidget::slot_checkBoxClicked()
{
    updateWordList();
    updateWordStatistics();
}

void WordLibraryWidget::slot_comboxGroup_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    m_curGroupId = WTool::getGroupNo(ui->comboBox_group->currentText());
    updateWordList();
    updateWordStatistics();
    Global::m_groupIndexLib.setValue(m_curGroupId);
    Global::saveXML();
}

void WordLibraryWidget::slot_wordTimeDecline(QString name)
{
    if (m_wordList.contains(name))
    {
        updateWordList();
    }
    updateWordStatistics();
}

void WordLibraryWidget::slot_wordTimeIncrease(QString name)
{
    if (Dispatcher::senderObj() == this) return;
    if (m_wordList.contains(name))
    {
        updateWordList();
    }
    updateWordStatistics();
}

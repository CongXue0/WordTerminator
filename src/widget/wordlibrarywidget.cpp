#include "wordlibrarywidget.h"
#include <QDebug>
#include <QKeyEvent>
#include "wordadmin.h"
#include "global.h"
#include <QApplication>
#include <QMessageBox>

extern WordAdmin *p_wordAdmin;

WordLibraryWidget::WordLibraryWidget(QWidget *parent) : QWidget(parent)
{
    m_menu1 = new QMenu("操作", this);
    m_menu1->setObjectName("m_menu1");
    connect(m_menu1, SIGNAL(triggered(QAction *)), this, SLOT(slot_menu1Triggered(QAction *)));
    m_menu2 = new QMenu("set group", this);
    m_menu2->setObjectName("m_menu2");
    connect(m_menu2, SIGNAL(triggered(QAction *)), this, SLOT(slot_menu2Triggered(QAction *)));

    label_bg = new QLabel(this);
    label_bg->setObjectName("label_bg");
    label_statistics = new QLabel(this);
    label_statistics->setObjectName("label_statistics");

    lineEdit_search = new QLineEdit(this);
    lineEdit_search->setObjectName("lineEdit_search");

    btn_search = new QPushButton(this);
    btn_search->setObjectName("btn_search");
    connect(btn_search, SIGNAL(clicked()), this, SLOT(slot_btnSearch_clicked()));
    btn_create = new QPushButton(this);
    btn_create->setObjectName("btn_create");
    btn_create->setText("    New Word");
    connect(btn_create, SIGNAL(clicked()), this, SLOT(slot_btnCreate_clicked()));

    m_wordList.clear();
    m_model = new QStringListModel();

    wordList = new WTListView(this);
    wordList->setObjectName("wordList");
    wordList->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置只读
    wordList->setSelectionMode(QListView::ExtendedSelection);
    wordList->setModel(m_model);
    wordList->installEventFilter(this);
    connect(wordList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slot_itemDoubleClicked(QModelIndex)));

    radioBtn_range[0] = new QRadioButton(this);
    radioBtn_range[0]->setObjectName("radioBtn_range1");
    radioBtn_range[0]->setAutoExclusive(false);
    connect(radioBtn_range[0], SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    radioBtn_range[1] = new QRadioButton(this);
    radioBtn_range[1]->setObjectName("radioBtn_range2");
    radioBtn_range[1]->setAutoExclusive(false);
    connect(radioBtn_range[1], SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    radioBtn_range[2] = new QRadioButton(this);
    radioBtn_range[2]->setObjectName("radioBtn_range3");
    radioBtn_range[2]->setAutoExclusive(false);
    connect(radioBtn_range[2], SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    radioBtn_range[3] = new QRadioButton(this);
    radioBtn_range[3]->setObjectName("radioBtn_range4");
    radioBtn_range[3]->setAutoExclusive(false);
    connect(radioBtn_range[3], SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));
    radioBtn_forever = new QRadioButton(this);
    radioBtn_forever->setObjectName("radioBtn_forever");
    radioBtn_forever->setText("forever");
    radioBtn_forever->setAutoExclusive(false);
    connect(radioBtn_forever, SIGNAL(clicked()), this, SLOT(slot_radioButtonClicked()));

    combox_group = new QComboBox(this);
    combox_group->setObjectName("combox_group");
    combox_search = new QComboBox(this);
    combox_search->setObjectName("combox_search");
    combox_search->insertItem(0, "prefix");
    combox_search->insertItem(1, "suffix");
    combox_search->insertItem(2, "contain");
    combox_search->insertItem(3, "interpretation");

    m_reloadFlag = true;
    reloadGlobalValue();
    loadStyleSheet();
    updateWordList();
    updateWordStatistics();
}

void WordLibraryWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return && (event->modifiers() & Qt::ControlModifier))
    {
        slot_btnCreate_clicked();
    }
    else if (lineEdit_search->hasFocus() == true && event->key() == Qt::Key_Return)
    {
        slot_btnSearch_clicked();
    }
}

bool WordLibraryWidget::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == wordList && e->type() == QEvent::MouseButtonPress && static_cast<QMouseEvent *>(e)->button() == Qt::RightButton)
    {
        m_modelList = wordList->selectedIndexes();
        m_menu1->exec(QCursor::pos());
    }
    return QWidget::eventFilter(obj, e);
}

void WordLibraryWidget::recoveryInterface()
{
    clearSearch();
    m_wordList.clear();
    m_model->setStringList(m_wordList);
    radioBtn_range[0]->setChecked(false);
    radioBtn_range[1]->setChecked(false);
    radioBtn_range[2]->setChecked(false);
    radioBtn_range[3]->setChecked(false);
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
        for (int i = 0; i < list.count(); i++)
        {
            m_menu2->addAction(list.at(i))->setObjectName("menu2");
        }
        m_menu1->addMenu(m_menu2);
        m_menu1->addAction(QString("Set to forever"))->setObjectName("menu1");
        m_menu1->addAction(QString("Set to not forever"))->setObjectName("menu1");
        m_menu1->addAction(QString("Delete"))->setObjectName("menu1");
        m_menu1->addAction(QString("Clear times"))->setObjectName("menu1");

        disconnect(combox_group, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboxGroup_currentIndexChanged(int)));
        combox_group->clear();
        combox_group->addItem(ALL_GROUP);
        for (int i = 0; i < list.count(); i++)
        {
            combox_group->addItem(list.at(i));
        }
        combox_group->setCurrentIndex(1);
        connect(combox_group, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboxGroup_currentIndexChanged(int)));
        m_curGroupId = 0;

        label_statistics->setText(QString("statistics:\n"
            "%1~%2 : \n"
            "%3~%4 : \n"
            "%5~%6 : \n"
            "%7+ : \n"
            "normal: \n"
            "forever: ").arg(Global::m_range1Left.getValueInt()).arg(Global::m_range1Right.getValueInt())
            .arg(Global::m_range2Left.getValueInt()).arg(Global::m_range2Right.getValueInt())
            .arg(Global::m_range3Left.getValueInt()).arg(Global::m_range3Right.getValueInt())
            .arg(Global::m_range4Left.getValueInt()));
        radioBtn_range[0]->setText(QString("times %1~%2")
            .arg(Global::m_range1Left.getValueStr()).arg(Global::m_range1Right.getValueStr()));
        radioBtn_range[1]->setText(QString("times %1~%2")
            .arg(Global::m_range2Left.getValueStr()).arg(Global::m_range2Right.getValueStr()));
        radioBtn_range[2]->setText(QString("times %1~%2")
            .arg(Global::m_range3Left.getValueStr()).arg(Global::m_range3Right.getValueStr()));
        radioBtn_range[3]->setText(QString("times %1+").arg(Global::m_range4Left.getValueStr()));
    }
}

void WordLibraryWidget::setReloadFlag(bool flag)
{
    m_reloadFlag = flag;
}

void WordLibraryWidget::clearSearch()
{
    lineEdit_search->clear();
}

void WordLibraryWidget::updateWordList()
{
    slot_btnSearch_clicked();
}

void WordLibraryWidget::updateWordStatistics()
{
    bool rem = radioBtn_forever->isChecked();
    label_statistics->setText(QString("statistics:\n%1~%2 : %8\n%3~%4 : %9\n%5~%6 : %10\n%7+ : %11\nnormal: %12\nforever: %13")
        .arg(Global::m_range1Left.getValueInt()).arg(Global::m_range1Right.getValueInt())
        .arg(Global::m_range2Left.getValueInt()).arg(Global::m_range2Right.getValueInt())
        .arg(Global::m_range3Left.getValueInt()).arg(Global::m_range3Right.getValueInt()).arg(Global::m_range4Left.getValueInt())
        .arg(p_wordAdmin->getWordNumFromTimes(Global::m_range1Left.getValueInt(), Global::m_range1Right.getValueInt(), m_curGroupId, rem))
        .arg(p_wordAdmin->getWordNumFromTimes(Global::m_range2Left.getValueInt(), Global::m_range2Right.getValueInt(), m_curGroupId, rem))
        .arg(p_wordAdmin->getWordNumFromTimes(Global::m_range3Left.getValueInt(), Global::m_range3Right.getValueInt(), m_curGroupId, rem))
        .arg(p_wordAdmin->getWordNumFromTimes(Global::m_range4Left.getValueInt(), MAX_TIMES, m_curGroupId, rem))
        .arg(p_wordAdmin->getWordNumFromTimes(0, MAX_TIMES, m_curGroupId, false)).arg(p_wordAdmin->getWordNumFromTimes(0, MAX_TIMES, m_curGroupId, true)));
}

void WordLibraryWidget::loadStyleSheet()
{
    setStyleSheet(WTool::getWordLibraryWidgetQss());
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
        for (int i = 0; i < m_modelList.count(); i++)
        {
            if (p_wordAdmin->getWordBriefInfo(m_modelList.at(i).data().toString(), &word))
            {
                word.m_times = times;
                word.m_modifyTime = QDateTime::currentDateTime();
                word.m_remember = (word.m_remember > 0 ? 2 : -1);
                if (p_wordAdmin->updateWord(word.m_name, "Times", QString::number(word.m_times), "ModifyTime",
                    word.m_modifyTime.toString(TIMEFORMAT), "RememberState", QString::number(word.m_remember)))
                {
                    emit wordTimeIncreaseSignal(word.m_name);
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
        for (int i = 0; i < m_modelList.count(); i++)
        {
            if (p_wordAdmin->getWordBriefInfo(m_modelList.at(i).data().toString(), &word))
            {
                word.m_modifyTime = QDateTime::currentDateTime();
                word.m_remember = remember;
                if (p_wordAdmin->updateWord(word.m_name, "ModifyTime", word.m_modifyTime.toString(TIMEFORMAT),
                    "RememberState", QString::number(word.m_remember)))
                {
                    emit wordTimeIncreaseSignal(word.m_name);
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
            for (int i = 0; i < m_modelList.count(); i++)
            {
                name = m_modelList.at(i).data().toString();
                if (p_wordAdmin->deleteWord(name))
                {
                    emit wordTimeIncreaseSignal(name);
                }
            }
            this->updateWordList();
            this->updateWordStatistics();
        }
    }
    else if (operation == "Clear times")
    {
        BriefWordInfo word;
        for (int i = 0; i < m_modelList.count(); i++)
        {
            if (p_wordAdmin->getWordBriefInfo(m_modelList.at(i).data().toString(), &word))
            {
                word.m_times = 0;
                word.m_modifyTime = QDateTime::currentDateTime();
                word.m_remember = (word.m_remember > 0 ? 2 : -1);
                if (p_wordAdmin->updateWord(word.m_name, "Times", QString::number(word.m_times), "ModifyTime",
                    word.m_modifyTime.toString(TIMEFORMAT), "RememberState", QString::number(word.m_remember)))
                {
                    emit wordTimeIncreaseSignal(word.m_name);
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
    for (int i = 0; i < m_modelList.count(); i++)
    {
        if (p_wordAdmin->getWordBriefInfo(m_modelList.at(i).data().toString(), &word))
        {
            word.m_modifyTime = QDateTime::currentDateTime();
            word.m_groupId = groupId;
            word.m_remember = (word.m_remember > 0 ? 2 : -1);
            if (p_wordAdmin->updateWord(word.m_name, "ModifyTime", word.m_modifyTime.toString(TIMEFORMAT),
                "Groupid", QString::number(word.m_groupId), "RememberState", QString::number(word.m_remember)))
            {
                emit wordTimeIncreaseSignal(word.m_name);
            }
        }
    }
}

void WordLibraryWidget::slot_btnSearch_clicked()
{
    m_wordList.clear();
    if (!radioBtn_range[0]->isChecked() && !radioBtn_range[1]->isChecked() && !radioBtn_range[2]->isChecked() &&
        !radioBtn_range[3]->isChecked() && !radioBtn_forever->isChecked())
    {
        m_wordList = p_wordAdmin->getAllWordList(m_curGroupId);
    }
    else
    {
        if (radioBtn_range[0]->isChecked())
            m_wordList += p_wordAdmin->getWordListFromTimes(Global::m_range1Left.getValueInt(), Global::m_range1Right.getValueInt(), m_curGroupId, radioBtn_forever->isChecked());
        if (radioBtn_range[1]->isChecked())
            m_wordList += p_wordAdmin->getWordListFromTimes(Global::m_range2Left.getValueInt(), Global::m_range2Right.getValueInt(), m_curGroupId, radioBtn_forever->isChecked());
        if (radioBtn_range[2]->isChecked())
            m_wordList += p_wordAdmin->getWordListFromTimes(Global::m_range3Left.getValueInt(), Global::m_range3Right.getValueInt(), m_curGroupId, radioBtn_forever->isChecked());
        if (radioBtn_range[3]->isChecked())
            m_wordList += p_wordAdmin->getWordListFromTimes(Global::m_range4Left.getValueInt(), MAX_TIMES, m_curGroupId, radioBtn_forever->isChecked());
    }
    if (!lineEdit_search->text().isEmpty())
    {
        m_wordList = WTool::filterWordFromList(m_wordList, lineEdit_search->text(), combox_search->currentText());
    }
    m_model->setStringList(m_wordList);
}

void WordLibraryWidget::slot_btnCreate_clicked()
{
    emit sendMessageSignal(WMessage("create word", ""));
}

void WordLibraryWidget::slot_itemDoubleClicked(QModelIndex index)
{
    QString name = index.data().toString();
    emit sendMessageSignal(WMessage("show word", name));
}

void WordLibraryWidget::slot_radioButtonClicked()
{
    updateWordList();
    updateWordStatistics();
}

void WordLibraryWidget::slot_comboxGroup_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    m_curGroupId = WTool::getGroupNo(combox_group->currentText());
    updateWordList();
    updateWordStatistics();
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
    if (m_wordList.contains(name))
    {
        updateWordList();
    }
    updateWordStatistics();
}

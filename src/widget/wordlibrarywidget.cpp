#include "wordlibrarywidget.h"
#include "ui_wordlibrarywidget.h"
#include "wordadmin.h"
#include "global.h"
#include "dispatcher.h"
#include "dtcp_input_dialog.h"
#include "dtcp_tool_tip.h"
#include <QDebug>
#include <QKeyEvent>
#include <QApplication>
#include <cmath>
#include <QHelpEvent>
#include <QToolTip>
#include <QCursor>
#include <QClipboard>

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

    m_model = new QStringListModel();

    m_hideTipTimer = new QTimer(this);
    m_hideTipTimer->setSingleShot(true);
    m_showTipTimer = new QTimer(this);
    m_showTipTimer->setSingleShot(true);

    ui->wordList->setModel(m_model);
    ui->wordList->viewport()->setMouseTracking(true);
    ui->wordList->installEventFilter(this);
    ui->wordList->viewport()->installEventFilter(this);

    ui->comboBox_search->addItem("prefix");
    ui->comboBox_search->addItem("suffix");
    ui->comboBox_search->addItem("contain");
    ui->comboBox_search->addItem("interpretation");

    ui->comboBox_sort->addItem("None");
    ui->comboBox_sort->addItem("Name");
    ui->comboBox_sort->addItem("Times");
    ui->comboBox_sort->addItem("ModifyTime");

    ui->btn_descend->setChecked(false);

    m_reloadFlag = true;
    reloadGlobalValue();
    updateWordList();
    updateWordStatistics();

    connect(m_hideTipTimer, SIGNAL(timeout()), this, SLOT(slot_hideTipTimeout()));
    connect(m_showTipTimer, SIGNAL(timeout()), this, SLOT(slot_showTipTimeout()));
    connect(m_menu1, SIGNAL(triggered(QAction *)), this, SLOT(slot_menu1Triggered(QAction *)));
    connect(m_menu2, SIGNAL(triggered(QAction *)), this, SLOT(slot_menu2Triggered(QAction *)));

    connect(ui->lineEdit_search, SIGNAL(returnPressed()), this, SLOT(slot_lineEditSearch_editingFinished()));
    connect(ui->btn_locate, SIGNAL(clicked()), this, SLOT(slot_btnLocate_clicked()));
    connect(ui->btn_create, SIGNAL(clicked()), this, SLOT(slot_btnCreate_clicked()));
    connect(ui->btn_descend, SIGNAL(clicked()), this, SLOT(slot_btnDescend_clicked()));
    connect(ui->btn_prev, SIGNAL(clicked()), this, SLOT(slot_btnPrev_clicked()));
    connect(ui->btn_next, SIGNAL(clicked()), this, SLOT(slot_btnNext_clicked()));

    connect(ui->wordList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slot_itemDoubleClicked(QModelIndex)));

    connect(ui->checkBox_range0, SIGNAL(clicked()), this, SLOT(slot_checkBoxClicked()));
    connect(ui->checkBox_range1, SIGNAL(clicked()), this, SLOT(slot_checkBoxClicked()));
    connect(ui->checkBox_range2, SIGNAL(clicked()), this, SLOT(slot_checkBoxClicked()));
    connect(ui->checkBox_range3, SIGNAL(clicked()), this, SLOT(slot_checkBoxClicked()));
    connect(ui->checkBox_forever, SIGNAL(clicked()), this, SLOT(slot_checkBoxClicked()));
    connect(ui->comboBox_group, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboxGroup_currentIndexChanged(int)));
    connect(ui->comboBox_sort, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboxSort_currentIndexChanged(int)));
    connect(ui->comboBox_page, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboxPage_currentIndexChanged(int)));
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

        {
            QSignalBlocker sb(ui->comboBox_group);
            ui->comboBox_group->clear();
            ui->comboBox_group->addItem(ALL_GROUP);
            for (int i = 0; i < list.count(); ++i)
            {
                ui->comboBox_group->addItem(list.at(i));
            }
            ui->comboBox_group->setCurrentIndex(Global::m_groupIndexLib.getValueInt() + 1);
            m_curGroupId = WTool::getGroupNo(ui->comboBox_group->currentText());
        }

        {
            QSignalBlocker sb(ui->comboBox_sort);
            ui->comboBox_sort->setCurrentIndex(Global::m_sortType.getValueInt());
            ui->btn_descend->setChecked(Global::m_sortDescend.getValueInt() == 1);
        }

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
    slot_lineEditSearch_editingFinished();
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
    if (event->modifiers() & Qt::ControlModifier)
    {
        if (event->key() == Qt::Key_Return)
        {
            slot_btnCreate_clicked();
        }
        else if (event->key() == Qt::Key_F)
        {
            slot_btnLocate_clicked();
        }
    }

}

bool WordLibraryWidget::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == ui->wordList->viewport())
    {
        if (e->type() == QEvent::MouseButtonPress && static_cast<QMouseEvent *>(e)->button() == Qt::RightButton)
        {
            m_modelList = ui->wordList->selectionModel()->selectedIndexes();
            m_menu1->exec(QCursor::pos());
        }
        else if (e->type() == QEvent::MouseMove)
        {
            m_showTipTimer->start(20);
        }
        else if (e->type() == QEvent::Leave)
        {
            if (m_showTipTimer->isActive())
                m_showTipTimer->stop();
            if (DtcpToolTip::isVisible())
                slot_hideTipTimeout();
        }
    }
    else if (obj == ui->wordList)
    {
        if (e->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(e);
            if ((keyEvent->modifiers() & Qt::ControlModifier) && keyEvent->key() == Qt::Key_C)
            {
                auto list = ui->wordList->selectionModel()->selectedIndexes();
                if (!list.isEmpty())
                {
                    QClipboard *clip= QApplication::clipboard();
                    clip->setText(list.at(0).data().toString());
                    WTool::opacityMessageBox("Copy", "", 0.1, 100);
                }
            }
        }
    }
    return QWidget::eventFilter(obj, e);
}

void WordLibraryWidget::slot_hideTipTimeout()
{
    DtcpToolTip::hideText();
//    QCoreApplication::processEvents();
}

void WordLibraryWidget::slot_showTipTimeout()
{
    auto pos = ui->wordList->mapFromGlobal(QCursor::pos());
    if (pos.x() >= int(ui->wordList->width() * (1.0 / 3)) && pos.x() < int(ui->wordList->width() * (2.0 / 3)))
    {
        auto index = ui->wordList->indexAt(pos);
        QString name = index.data().toString();
        if (!name.isEmpty())
        {
            WordInfo wordInfo;
            if (p_wordAdmin->getWordInfo(name, &wordInfo))
            {
                QString txt;
                for (int i = 0; i < EXAMPLE_NUM; ++i)
                {
                    if (wordInfo.m_exampleSentence[i].isEmpty())
                        break;
                    txt.append(QString::number(i + 1) + ". " + wordInfo.m_exampleSentence[i] + "\n");
                }
                if (!txt.isEmpty())
                {
                    if (txt[txt.size() - 1] == QChar('\n'))
                        txt = txt.mid(0, txt.size() - 1);
                    DtcpToolTip::showText(QCursor::pos(), txt);
                }
                else
                    DtcpToolTip::hideText();
            }
        }
    }
    else if (pos.x() >= int(ui->wordList->width() * (2.0 / 3)))
    {
        auto index = ui->wordList->indexAt(pos);
        QString name = index.data().toString();
        if (!name.isEmpty())
        {
            WordInfo wordInfo;
            if (p_wordAdmin->getWordInfo(name, &wordInfo))
            {
                QString txt = wordInfo.toText();
                txt = txt.mid(txt.indexOf("\n") + 1);
                if (txt[txt.size() - 1] == QChar('\n'))
                    txt = txt.mid(0, txt.size() - 1);
                DtcpToolTip::showText(QCursor::pos(), txt);
            }
        }
    }
    else
    {
        DtcpToolTip::hideText();
    }
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
        if (QUESTION_ISYES("是否删除?"))
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

void WordLibraryWidget::slot_lineEditSearch_editingFinished()
{
    m_wordList.clear();
    QVector<BriefWordInfo> tmpList;
    if (!ui->checkBox_range0->isChecked() && !ui->checkBox_range1->isChecked() && !ui->checkBox_range2->isChecked() &&
        !ui->checkBox_range3->isChecked() && !ui->checkBox_forever->isChecked())
    {
        tmpList = p_wordAdmin->getAllBriefWordInfoList(m_curGroupId);
    }
    else
    {
        if (ui->checkBox_range0->isChecked())
            tmpList += p_wordAdmin->getBriefWordInfoListFromTimes(Global::m_range1Left.getValueInt(), Global::m_range1Right.getValueInt(), m_curGroupId, ui->checkBox_forever->isChecked());
        if (ui->checkBox_range1->isChecked())
            tmpList += p_wordAdmin->getBriefWordInfoListFromTimes(Global::m_range2Left.getValueInt(), Global::m_range2Right.getValueInt(), m_curGroupId, ui->checkBox_forever->isChecked());
        if (ui->checkBox_range2->isChecked())
            tmpList += p_wordAdmin->getBriefWordInfoListFromTimes(Global::m_range3Left.getValueInt(), Global::m_range3Right.getValueInt(), m_curGroupId, ui->checkBox_forever->isChecked());
        if (ui->checkBox_range3->isChecked())
            tmpList += p_wordAdmin->getBriefWordInfoListFromTimes(Global::m_range4Left.getValueInt(), MAX_TIMES, m_curGroupId, ui->checkBox_forever->isChecked());
    }
    if (!ui->lineEdit_search->text().isEmpty())
    {
        tmpList = WTool::filterWordFromList(tmpList, ui->lineEdit_search->text(), ui->comboBox_search->currentText());
    }

    if (ui->comboBox_sort->currentIndex() > 0)//0 None, 1 Name, 2 Times, 3 ModifyTime
    {
        bool descend = !ui->btn_descend->isChecked();
        std::sort(tmpList.begin(), tmpList.end(), [&](const BriefWordInfo& t1, const BriefWordInfo& t2) {
            if (ui->comboBox_sort->currentIndex() == 1)
            {
                bool b1 = t1.m_name.compare(t2.m_name, Qt::CaseInsensitive) >= 0;
                return descend ? b1 : !b1;
            }
            else if (ui->comboBox_sort->currentIndex() == 2)
            {
                bool b1 = t1.m_times >= t2.m_times;
                return descend ? b1 : !b1;
            }
            else if (ui->comboBox_sort->currentIndex() == 3)
            {
                bool b1 = t1.m_modifyTime >= t2.m_modifyTime;
                return descend ? b1 : !b1;
            }
            else
                return true;
        });
    }
    for (int i = 0; i < tmpList.size(); ++i)
        m_wordList.append(tmpList[i].m_name);

    {
        QSignalBlocker sb(ui->comboBox_page);
        int cur_index = ui->comboBox_page->currentIndex();
        if (cur_index < 0) cur_index = 0;
        ui->comboBox_page->clear();
        int volume = Global::m_pageVolume.getValueInt();
        int pageCount = int(std::ceil(1.0 * m_wordList.size() / volume));
        if (pageCount == 0)
            pageCount = 1;
        for (int i = 0; i < pageCount; ++i)
            ui->comboBox_page->addItem(QString::number(i + 1));
        if (cur_index < ui->comboBox_page->count())
            ui->comboBox_page->setCurrentIndex(cur_index);
        ui->label_record->setText(QString("Search %1 record, total %2 page").arg(m_wordList.size()).arg(pageCount));
    }
    slot_comboxPage_currentIndexChanged(ui->comboBox_page->currentIndex());
}

void WordLibraryWidget::slot_btnLocate_clicked()
{
    QString name = dtcp::InputDialog::getText("Locate", "Word name:");
    if (name.isEmpty() || !m_wordList.contains(name)) return;
    int index = m_wordList.indexOf(name);
    int volume = Global::m_pageVolume.getValueInt();
    int page_index = index / volume;
    QSignalBlocker sb(ui->comboBox_page);
    ui->comboBox_page->setCurrentIndex(page_index);
    int left = page_index * volume, right = (page_index + 1) * volume - 1;
    if (right > m_wordList.size() - 1)
        right = m_wordList.size() - 1;
    index = index - left;
    m_model->setStringList(m_wordList.mid(left, right - left + 1));
    ui->wordList->setCurrentIndex(m_model->index(index));
}

void WordLibraryWidget::slot_btnCreate_clicked()
{
    emit Dispatch(this).signal_sendMessage(WMessage("create word", ""));
}

void WordLibraryWidget::slot_btnDescend_clicked()
{
    updateWordList();
    Global::m_sortDescend.setValue(ui->btn_descend->isChecked() ? 1 : 0);
    Global::saveXML();
}

void WordLibraryWidget::slot_btnPrev_clicked()
{
    int index = ui->comboBox_page->currentIndex();
    if (index > 0)
        ui->comboBox_page->setCurrentIndex(index - 1);
}

void WordLibraryWidget::slot_btnNext_clicked()
{
    int index = ui->comboBox_page->currentIndex();
    if (index < ui->comboBox_page->count() - 1)
        ui->comboBox_page->setCurrentIndex(index + 1);
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

void WordLibraryWidget::slot_comboxSort_currentIndexChanged(int index)
{
    updateWordList();
    Global::m_sortType.setValue(index);
    Global::saveXML();
}

void WordLibraryWidget::slot_comboxPage_currentIndexChanged(int index)
{
    if (m_wordList.empty())
    {
        m_model->setStringList(m_wordList);
    }
    else
    {
        int volume = Global::m_pageVolume.getValueInt();
        int left = index * volume, right = (index + 1) * volume - 1;
        if (right > m_wordList.size() - 1)
            right = m_wordList.size() - 1;
        m_model->setStringList(m_wordList.mid(left, right - left + 1));
    }
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

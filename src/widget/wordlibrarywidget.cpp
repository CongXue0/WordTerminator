#include "wordlibrarywidget.h"
#include "wtool.h"
#include <QDebug>
#include <QKeyEvent>
#include "wordadmin.h"
#include "global.h"

extern WordAdmin *p_wordAdmin;

WordLibraryWidget::WordLibraryWidget(QWidget *parent) : QWidget(parent)
{
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

    wordList = new QListView(this);
    wordList->setObjectName("wordList");
    wordList->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置只读
    wordList->setModel(m_model);
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

    combox_search = new QComboBox(this);
    combox_search->setObjectName("combox_search");
    combox_search->insertItem(0, "prefix");
    combox_search->insertItem(1, "suffix");
    combox_search->insertItem(2, "contain");
    combox_search->insertItem(3, "interpretation");

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
    label_statistics->setText(QString("statistics:\n"
        "%1~%2 : \n"
        "%3~%4 : \n"
        "%5~%6 : \n"
        "%7+ : \n"
        "normal: \n"
        "forever: ").arg(Global::m_range1Left.getValueInt()).arg(Global::m_range1Right.getValueInt())
        .arg(Global::m_range2Left.getValueInt()).arg(Global::m_range2Right.getValueInt())
        .arg(Global::m_range3Left.getValueInt()).arg(Global::m_range3Right.getValueInt()).arg(Global::m_range4Left.getValueInt()));
    radioBtn_range[0]->setText(QString("times %1~%2").arg(Global::m_range1Left.getValueStr()).arg(Global::m_range1Right.getValueStr()));
    radioBtn_range[1]->setText(QString("times %1~%2").arg(Global::m_range2Left.getValueStr()).arg(Global::m_range2Right.getValueStr()));
    radioBtn_range[2]->setText(QString("times %1~%2").arg(Global::m_range3Left.getValueStr()).arg(Global::m_range3Right.getValueStr()));
    radioBtn_range[3]->setText(QString("times %1+").arg(Global::m_range4Left.getValueStr()));
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
        .arg(p_wordAdmin->getWordNumFromTimes(Global::m_range1Left.getValueInt(), Global::m_range1Right.getValueInt(), rem))
        .arg(p_wordAdmin->getWordNumFromTimes(Global::m_range2Left.getValueInt(), Global::m_range2Right.getValueInt(), rem))
        .arg(p_wordAdmin->getWordNumFromTimes(Global::m_range3Left.getValueInt(), Global::m_range3Right.getValueInt(), rem))
        .arg(p_wordAdmin->getWordNumFromTimes(Global::m_range4Left.getValueInt(), MAX_TIMES, rem))
        .arg(p_wordAdmin->getWordNumFromTimes(0, MAX_TIMES, false)).arg(p_wordAdmin->getWordNumFromTimes(0, MAX_TIMES, true)));
}

void WordLibraryWidget::loadStyleSheet()
{
    setStyleSheet(WTool::getWordLibraryWidgetQss());
}

void WordLibraryWidget::slot_btnSearch_clicked()
{
    m_wordList.clear();
    if (!radioBtn_range[0]->isChecked() && !radioBtn_range[1]->isChecked() && !radioBtn_range[2]->isChecked() &&
        !radioBtn_range[3]->isChecked() && !radioBtn_forever->isChecked())
    {
        m_wordList = p_wordAdmin->getAllWordList();
    }
    else
    {
        if (radioBtn_range[0]->isChecked())
            m_wordList += p_wordAdmin->getWordListFromTimes(Global::m_range1Left.getValueInt(), Global::m_range1Right.getValueInt(), radioBtn_forever->isChecked());
        if (radioBtn_range[1]->isChecked())
            m_wordList += p_wordAdmin->getWordListFromTimes(Global::m_range2Left.getValueInt(), Global::m_range2Right.getValueInt(), radioBtn_forever->isChecked());
        if (radioBtn_range[2]->isChecked())
            m_wordList += p_wordAdmin->getWordListFromTimes(Global::m_range3Left.getValueInt(), Global::m_range3Right.getValueInt(), radioBtn_forever->isChecked());
        if (radioBtn_range[3]->isChecked())
            m_wordList += p_wordAdmin->getWordListFromTimes(Global::m_range4Left.getValueInt(), MAX_TIMES, radioBtn_forever->isChecked());
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

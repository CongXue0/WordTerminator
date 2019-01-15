#include "wordterminator.h"
#include "ui_wordterminator.h"
#include <QLayout>
#include "wtool.h"
#include <QDebug>
#include <QDir>
#include <QDesktopWidget>
#include "wordadmin.h"
#include <QMessageBox>
#include "forgetthread.h"
#include <QCloseEvent>
#include "memorythread.h"
#include <QInputDialog>
#include <QProgressDialog>
#include <QTextCodec>
#include <QStandardPaths>
#include "global.h"
#include <QCoreApplication>
#include "src/utils/version.h"

extern WordAdmin *p_wordAdmin;
extern ForgetThread *p_forgetThread;
extern MemoryThread *p_memThread;

WordTerminator::WordTerminator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WordTerminator)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::Dialog);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    WTool::dirInit();
    Global::init(WTool::getConfigPath());

    setWindowTitle("WordTerminator " + Version::getVersion());

    if (p_wordAdmin == NULL)
        p_wordAdmin = WordAdmin::getInstance();
    if (p_forgetThread == NULL)
        p_forgetThread = new ForgetThread(this);
    if (p_memThread == NULL)
        p_memThread = new MemoryThread(this);

    m_first = true;
    m_widgets.clear();

    label_menubg = new QLabel(this);
    label_menubg->setObjectName("label_menubg");

    btn_save = new QPushButton(label_menubg);
    btn_save->setObjectName("btn_save");
    connect(btn_save, SIGNAL(clicked()), this, SLOT(slot_saveBtn_clicked()));

    btn_lib = new WTButton(label_menubg);
    btn_lib->setObjectName("btn_lib");
    btn_lib->setText("词库管理");
    btn_lib->setActive(true);
    btn_lib->setAutoExclusive(true);
    connect(btn_lib, SIGNAL(pressed()), this, SLOT(slot_wtbuttonPressed()));

    btn_mem = new WTButton(label_menubg);
    btn_mem->setObjectName("btn_mem");
    btn_mem->setText("单词记忆");
    btn_mem->setAutoExclusive(true);
    connect(btn_mem, SIGNAL(pressed()), this, SLOT(slot_wtbuttonPressed()));

    btn_fun = new WTButton(label_menubg);
    btn_fun->setObjectName("btn_fun");
    btn_fun->setText("其他功能");
    btn_fun->setAutoExclusive(true);
    connect(btn_fun, SIGNAL(pressed()), this, SLOT(slot_wtbuttonPressed()));

    btn_set = new WTButton(label_menubg);
    btn_set->setObjectName("btn_set");
    btn_set->setText("设置选项");
    btn_set->setAutoExclusive(true);
    connect(btn_set, SIGNAL(pressed()), this, SLOT(slot_wtbuttonPressed()));

    stackedWidget = new QStackedWidget(this);
    stackedWidget->setObjectName("stackedWidget");

    loadStyleSheet();

    wordLibrary = new WordLibraryWidget(this);
    wordLibrary->setObjectName("WordLibraryWidget");
    connect(wordLibrary, SIGNAL(sendMessageSignal(WMessage)), this, SLOT(slot_handleMessage(WMessage)));
    connect(wordLibrary, SIGNAL(wordTimeIncreaseSignal(QString)), p_forgetThread, SLOT(slot_wordTimeIncrease(QString)));
    connect(p_forgetThread, SIGNAL(wordTimeDeclineSignal(QString)), wordLibrary, SLOT(slot_wordTimeDecline(QString)));
    stackedWidget->addWidget(wordLibrary);
    pushWidgetIndex(Widget_WordLibrary);

    wordCreate = new WordCreateWidget(this);
    wordCreate->setObjectName("WordCreateWidget");
    connect(wordCreate, SIGNAL(sendMessageSignal(WMessage)), this, SLOT(slot_handleMessage(WMessage)));
    connect(wordCreate, SIGNAL(wordTimeIncreaseSignal(QString)), p_forgetThread, SLOT(slot_wordTimeIncrease(QString)));
    stackedWidget->addWidget(wordCreate);

    wordShow = new WordShowWidget(this);
    wordShow->setObjectName("WordShowWidget");
    connect(wordShow, SIGNAL(sendMessageSignal(WMessage)), this, SLOT(slot_handleMessage(WMessage)));
    connect(wordShow, SIGNAL(wordTimeIncreaseSignal(QString)), p_forgetThread, SLOT(slot_wordTimeIncrease(QString)));
    connect(p_forgetThread, SIGNAL(wordTimeDeclineSignal(QString)), wordShow, SLOT(slot_wordTimeDecline(QString)));
    stackedWidget->addWidget(wordShow);

    wordMemorize = new WordMemorizeWidget(this);
    wordMemorize->setObjectName("WordMemorizeWidget");
    connect(p_memThread, SIGNAL(wordCanMemorizeSignal(QString)), wordMemorize, SLOT(slot_wordCanMemorize(QString)));
    connect(wordMemorize, SIGNAL(wordTimeIncreaseSignal(QString)), p_memThread, SLOT(slot_wordTimeIncrease(QString)));
    connect(wordMemorize, SIGNAL(wordTimeIncreaseSignal(QString)), p_forgetThread, SLOT(slot_wordTimeIncrease(QString)));
    connect(wordMemorize, SIGNAL(wordTimeIncreaseSignal(QString)), wordLibrary, SLOT(slot_wordTimeIncrease(QString)));
    connect(wordMemorize, SIGNAL(wordTimeIncreaseSignal(QString)), wordShow, SLOT(slot_wordTimeIncrease(QString)));
    connect(this, SIGNAL(stopWordMemorizeSignal(bool*)), wordMemorize, SLOT(slot_stopWordMemorize(bool*)));
    stackedWidget->addWidget(wordMemorize);

    wordFunc = new WordFunctionWidget(this);
    wordFunc->setObjectName("WordFunctionWidget");
    stackedWidget->addWidget(wordFunc);

    wordSetting = new WordSettingWidget(this);
    wordSetting->setObjectName("WordSettingWidget");
    connect(wordSetting, SIGNAL(sendMessageSignal(WMessage)), this, SLOT(slot_handleMessage(WMessage)));
    stackedWidget->addWidget(wordSetting);

    p_forgetThread->start();
}

WordTerminator::~WordTerminator()
{
    delete ui;
}

void WordTerminator::showEvent(QShowEvent *)
{
    if (m_first)
    {
        QSize size = this->size();
        this->setFixedSize(size);//固定窗口大小

        QPoint point;
        QRect screen = WTool::getScreenGeometry();
        point.setX(screen.width() / 2 - size.width() / 2);
        if (WTool::getScreenSize() == "14")
            point.setY(8 * 4);
        else if (WTool::getScreenSize() == "15.6")
            point.setY(10 * 4);
        this->move(point);

        m_first = false;
    }
}

void WordTerminator::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::question(this, "question", "is close WT?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        p_forgetThread->stop();
        p_forgetThread->wait();
        p_memThread->stop();
        p_memThread->wait();
        event->accept();
    }
    else
        event->ignore();
}

int WordTerminator::getCurrentWidgetIndex()
{
    return stackedWidget->currentIndex();
}

void WordTerminator::loadStyleSheet()
{
    setStyleSheet(WTool::getWordTerminatorQss());
}

int WordTerminator::topWidgetIndex()
{
    if (m_widgets.size() > 0)
        return m_widgets.last();
    else
        return -1;
}

void WordTerminator::pushWidgetIndex(int index)
{
    m_widgets.append(index);
}

void WordTerminator::popWidgetIndex()
{
    if (m_widgets.size() > 0)
    {
        m_widgets.remove(m_widgets.size() - 1);
    }
    else
        DEBUG << "popWidgetIndex error";
}

void WordTerminator::clearWidgetIndex()
{
    m_widgets.clear();
}

void WordTerminator::slot_saveBtn_clicked()
{
    int index = stackedWidget->currentIndex();
    switch (index)
    {
    case Widget_WordLibrary:
        break;
    case Widget_WordCreate:
        break;
    case Widget_WordShow:
        break;
    case Widget_WordMemorize:
        break;
    case Widget_Function:
        wordFunc->saveGlobalValue();
        break;
    case Widget_Setting:
        wordSetting->saveGlobalValue();
        break;
    }
}

void WordTerminator::slot_wtbuttonPressed()
{
    QString name = sender()->objectName();
    int index = stackedWidget->currentIndex();
    if ((name == "btn_lib" && index == Widget_WordLibrary) || (name == "btn_mem" && index == Widget_WordMemorize) ||
        (name == "btn_fun" && index == Widget_Function) || (name == "btn_set" && index == Widget_Setting))
        return;

    switch (index)//先处理
    {
    case Widget_WordLibrary:
        break;
    case Widget_WordCreate:
        break;
    case Widget_WordShow:
        break;
    case Widget_WordMemorize:
        if (wordMemorize->getMode() == WordMemorizeWidget::MEMORY)
        {
            bool ret = false;
            emit stopWordMemorizeSignal(&ret);
            if (!ret)
                return;
        }
        break;
    case Widget_Function:
        break;
    case Widget_Setting:
        break;
    }

    clearWidgetIndex();
    if (name == "btn_lib")
        index = Widget_WordLibrary;
    else if (name == "btn_mem")
        index = Widget_WordMemorize;
    else if (name == "btn_fun")
        index = Widget_Function;
    else if (name == "btn_set")
        index = Widget_Setting;
    switch (index)//后处理
    {
    case Widget_WordLibrary:
        wordLibrary->reloadGlobalValue();
        wordLibrary->clearSearch();
        wordLibrary->updateWordList();
        wordLibrary->updateWordStatistics();
        break;
    case Widget_WordMemorize:
        wordMemorize->reloadGlobalValue();
        wordMemorize->recoveryInterface();
        wordMemorize->updateWordStatistics();
        break;
    case Widget_Function:
        wordFunc->reloadGlobalValue();
        break;
    case Widget_Setting:
        wordSetting->reloadGlobalValue();
        break;
    }
    pushWidgetIndex(index);
    stackedWidget->setCurrentIndex(index);

    if (name == "btn_lib")
    {
        btn_lib->setActive(true);
        btn_mem->setActive(false);
        btn_fun->setActive(false);
        btn_set->setActive(false);
    }
    else if (name == "btn_mem")
    {
        btn_lib->setActive(false);
        btn_mem->setActive(true);
        btn_fun->setActive(false);
        btn_set->setActive(false);
    }
    else if (name == "btn_fun")
    {
        btn_lib->setActive(false);
        btn_mem->setActive(false);
        btn_fun->setActive(true);
        btn_set->setActive(false);
    }
    else if (name == "btn_set")
    {
        btn_lib->setActive(false);
        btn_mem->setActive(false);
        btn_fun->setActive(false);
        btn_set->setActive(true);
    }
}

void WordTerminator::slot_handleMessage(WMessage message)
{
    int msgNum = message.getMessageNum();
    if (msgNum <= 0)
        return;
    QString name = sender()->objectName();
    if (name == "WordLibraryWidget")
    {
        if (msgNum == 1)
        {
            QString info, value;
            message.getMessage(0, info, value);
            if (info == "create word")
            {
                wordCreate->setCreateMode(WordCreateWidget::CREATE);
                wordCreate->recoveryInterface();
                pushWidgetIndex(Widget_WordCreate);
                stackedWidget->setCurrentIndex(Widget_WordCreate);
            }
            else if (info == "show word")
            {
                wordShow->recoveryInterface();
                wordShow->reloadGlobalValue();
                if (wordShow->loadWordInfo(value))
                {
                    pushWidgetIndex(Widget_WordShow);
                    stackedWidget->setCurrentIndex(Widget_WordShow);
                }
                else
                {
                    QMessageBox::about(this, "提示", "获取单词信息失败");
                }
            }
        }
//        else if (msgNum > 1) {}
    }
    else if (name == "WordCreateWidget")
    {
        if (msgNum == 1)
        {
            QString info, value;
            message.getMessage(0, info, value);
            if (info == "cancel create")
            {
                popWidgetIndex();
                stackedWidget->setCurrentIndex(topWidgetIndex());
            }
            else if (info == "create success")
            {
                popWidgetIndex();
                if (topWidgetIndex() == Widget_WordLibrary)
                {
                    wordLibrary->clearSearch();
                    wordLibrary->updateWordList();
                    wordLibrary->updateWordStatistics();
                }
                stackedWidget->setCurrentIndex(topWidgetIndex());
            }
            else if (info == "modify success")
            {
                popWidgetIndex();
                if (topWidgetIndex() == Widget_WordShow)
                {
                    wordShow->recoveryInterface();
                    if (wordShow->loadWordInfo(value))
                    {
                        stackedWidget->setCurrentIndex(Widget_WordShow);
                    }
                    else
                    {
                        QMessageBox::about(this, "提示", "获取单词信息失败");
                        popWidgetIndex();
                        if (topWidgetIndex() == Widget_WordLibrary)
                        {
                            wordLibrary->clearSearch();
                            wordLibrary->updateWordList();
                            wordLibrary->updateWordStatistics();
                        }
                        stackedWidget->setCurrentIndex(topWidgetIndex());
                    }
                }
            }
        }
//        else if (msgNum > 1) {}
    }
    else if (name == "WordShowWidget")
    {
        if (msgNum == 1)
        {
            QString info, value;
            message.getMessage(0, info, value);
            if (info == "return")
            {
                popWidgetIndex();
                if (topWidgetIndex() == Widget_WordLibrary)
                {
                    wordLibrary->updateWordList();
                    wordLibrary->updateWordStatistics();
                }
                stackedWidget->setCurrentIndex(topWidgetIndex());
            }
            else if (info == "delete success")
            {
                wordLibrary->clearSearch();
                wordLibrary->updateWordList();
                wordLibrary->updateWordStatistics();
                popWidgetIndex();
                stackedWidget->setCurrentIndex(topWidgetIndex());
            }
            else if (info == "edit word")
            {
                wordCreate->setCreateMode(WordCreateWidget::MODIFY);
                wordCreate->recoveryInterface();
                if (wordCreate->loadWordInfo(value))
                {
                    pushWidgetIndex(Widget_WordCreate);
                    stackedWidget->setCurrentIndex(Widget_WordCreate);
                }
                else
                    QMessageBox::about(this, "提示", "获取单词信息失败");
            }
        }
//        else if (msgNum > 1) {}
    }
    else if (name == "WordSettingWidget")
    {
        if (msgNum == 1)
        {
            QString info, value;
            message.getMessage(0, info, value);
            if (info == "set reload flag")
            {
                bool flag = (value == "true" ? true : false);
                wordCreate->setReloadFlag(flag);
                wordLibrary->setReloadFlag(flag);
                wordShow->setReloadFlag(flag);
                wordMemorize->setReloadFlag(flag);
            }
        }
    }
}

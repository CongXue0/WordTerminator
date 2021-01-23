#include "wordterminator.h"
#include "ui_wordterminator.h"
#include "wtool.h"
#include "wordadmin.h"
#include "forgetthread.h"
#include "memorythread.h"
#include "global.h"
#include "src/utils/version.h"
#include "dispatcher.h"
#include <QLayout>
#include <QDir>
#include <QCloseEvent>
#include <QProgressDialog>

extern WordAdmin *p_wordAdmin;
extern ForgetThread *p_forgetThread;
extern MemoryThread *p_memThread;

WordTerminator *WordTerminator::m_self = nullptr;

WordTerminator::WordTerminator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WordTerminator)
{
    WordTerminator::m_self = this;
    setStyleSheet(WTool::getStyleQss("WordTerminator"));
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::Dialog);
    setWindowTitle("WordTerminator " + Version::getVersion());

    ui->btn_lib->setProperty("index", Widget_WordLibrary);
    ui->btn_autoMem->setProperty("index", Widget_AutomatedMemorize);
    ui->btn_mem->setProperty("index", Widget_WordMemorize);
    ui->btn_fun->setProperty("index", Widget_Function);
    ui->btn_set->setProperty("index", Widget_Setting);

    ui->btn_lib->setAutoCheck(false);
    ui->btn_autoMem->setAutoCheck(false);
    ui->btn_mem->setAutoCheck(false);
    ui->btn_fun->setAutoCheck(false);
    ui->btn_set->setAutoCheck(false);
    m_btn_group[0] = ui->btn_lib;
    m_btn_group[1] = ui->btn_autoMem;
    m_btn_group[2] = ui->btn_mem;
    m_btn_group[3] = ui->btn_fun;
    m_btn_group[4] = ui->btn_set;

    m_first = true;

    ui->stackedWidget->setCurrentIndex(Widget_WordLibrary);
    pushWidgetIndex(Widget_WordLibrary);

    p_forgetThread->start();

    installEventFilter(this);

    connect(ui->btn_save, SIGNAL(clicked()), this, SLOT(slot_saveBtn_clicked()));
    connect(ui->btn_lib, SIGNAL(clicked()), this, SLOT(slot_switchButtonClicked()));
    connect(ui->btn_autoMem, SIGNAL(clicked()), this, SLOT(slot_switchButtonClicked()));
    connect(ui->btn_mem, SIGNAL(clicked()), this, SLOT(slot_switchButtonClicked()));
    connect(ui->btn_fun, SIGNAL(clicked()), this, SLOT(slot_switchButtonClicked()));
    connect(ui->btn_set, SIGNAL(clicked()), this, SLOT(slot_switchButtonClicked()));

    connect(&Dispatcher::getInstance(), SIGNAL(signal_sendMessage(WMessage)), this, SLOT(slot_handleMessage(WMessage)));

    connect(&Dispatcher::getInstance(), SIGNAL(signal_wordTimeIncrease(QString)), p_forgetThread, SLOT(slot_wordTimeIncrease(QString)));
//    connect(&Dispatcher::getInstance(), SIGNAL(signal_wordTimeIncrease(QString)), p_memThread, SLOT(slot_wordTimeIncrease(QString)));
    connect(&Dispatcher::getInstance(), SIGNAL(signal_wordTimeIncrease(QString)), ui->widget_wordLibrary, SLOT(slot_wordTimeIncrease(QString)));
    connect(&Dispatcher::getInstance(), SIGNAL(signal_wordTimeIncrease(QString)), ui->widget_wordShow, SLOT(slot_wordTimeIncrease(QString)));

    connect(&Dispatcher::getInstance(), SIGNAL(signal_wordTimeDecline(QString)), ui->widget_wordLibrary, SLOT(slot_wordTimeDecline(QString)));
    connect(&Dispatcher::getInstance(), SIGNAL(signal_wordTimeDecline(QString)), ui->widget_wordShow, SLOT(slot_wordTimeDecline(QString)));

//    connect(&Dispatcher::getInstance(), SIGNAL(signal_wordCanMemorize(QString)), ui->widget_wordMemorize, SLOT(slot_wordCanMemorize(QString)));
    connect(&Dispatcher::getInstance(), SIGNAL(signal_stopWordMemorize(bool*)), ui->widget_wordMemorize, SLOT(slot_stopWordMemorize(bool*)));

    connect(&Dispatcher::getInstance(), SIGNAL(signal_stopWordMemorize(bool*)), ui->widget_autoMemorize, SLOT(slot_stopWordMemorize(bool*)));
    connect(&Dispatcher::getInstance(), SIGNAL(signal_memorizeFinished()), ui->widget_autoMemorize, SLOT(slot_memorizeFinished()));
}

WordTerminator::~WordTerminator()
{
    delete ui;
}

int WordTerminator::getCurrentWidgetIndex()
{
    return ui->stackedWidget->currentIndex();
}

void WordTerminator::showEvent(QShowEvent *)
{
    if (m_first)
    {
        QSize size = this->size();
        this->setFixedSize(size);

        QPoint point;
        QRect screen = WTool::getScreenGeometry();
        point.setX(screen.width() / 2 - size.width() / 2);
        point.setY(screen.height() / 2 - size.height() / 2);
        this->move(point);

        m_first = false;
    }
}

void WordTerminator::closeEvent(QCloseEvent *event)
{
    if (QUESTION_ISYES("is close WT?"))
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

bool WordTerminator::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Escape)
        {
            return true;
        }
    }
    return QDialog::eventFilter(obj, event);
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
    int index = ui->stackedWidget->currentIndex();
    switch (index)
    {
    case Widget_WordLibrary:
        break;
    case Widget_WordCreate:
        break;
    case Widget_WordShow:
        break;
    case Widget_AutomatedMemorize:
        break;
    case Widget_WordMemorize:
        break;
    case Widget_Function:
        ui->widget_wordFunc->saveGlobalValue();
        break;
    case Widget_Setting:
        ui->widget_wordSetting->saveGlobalValue();
        break;
    }
}

void WordTerminator::slot_switchButtonClicked()
{
    auto btn = dynamic_cast<QPushButton *>(this->sender());
    QString name = btn->objectName();
    int index = ui->stackedWidget->currentIndex();
    if (btn->property("index").toInt() == index) return;

    switch (index)//先处理
    {
    case Widget_WordLibrary:
        break;
    case Widget_WordCreate:
        break;
    case Widget_WordShow:
        break;
    case Widget_AutomatedMemorize:
        if (ui->widget_autoMemorize->getMode() == WordAutomatedMemorizeWidget::MEMORY)
        {
            bool ret = false;
            emit Dispatch(this).signal_stopWordMemorize(&ret);
            if (!ret)
                return;
        }
        break;
    case Widget_WordMemorize:
        if (ui->widget_wordMemorize->getMode() == WordMemorizeWidget::MEMORY)
        {
            bool ret = false;
            emit Dispatch(this).signal_stopWordMemorize(&ret);
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
    else if (name == "btn_autoMem")
        index = Widget_AutomatedMemorize;
    else if (name == "btn_mem")
        index = Widget_WordMemorize;
    else if (name == "btn_fun")
        index = Widget_Function;
    else if (name == "btn_set")
        index = Widget_Setting;
    switch (index)//后处理
    {
    case Widget_WordLibrary:
        ui->widget_wordLibrary->reloadGlobalValue();
        ui->widget_wordLibrary->clearSearch();
        ui->widget_wordLibrary->updateWordList();
        ui->widget_wordLibrary->updateWordStatistics();
        break;
    case Widget_AutomatedMemorize:
        ui->widget_autoMemorize->reloadGlobalValue();
        ui->widget_autoMemorize->recoveryInterface();
        ui->widget_autoMemorize->updateWordStatistics();
        break;
    case Widget_WordMemorize:
        ui->widget_wordMemorize->reloadGlobalValue();
        ui->widget_wordMemorize->recoveryInterface();
        ui->widget_wordMemorize->updateWordStatistics();
        break;
    case Widget_Function:
        ui->widget_wordFunc->reloadGlobalValue();
        break;
    case Widget_Setting:
        ui->widget_wordSetting->reloadGlobalValue();
        break;
    }
    pushWidgetIndex(index);
    ui->stackedWidget->setCurrentIndex(index);

    for (int i = 0; i < 5; ++i)
    {
        m_btn_group[i]->setChecked(m_btn_group[i] == btn);
    }
}

void WordTerminator::slot_handleMessage(WMessage message)
{
    int msgNum = message.getMessageNum();
    if (msgNum <= 0)
        return;
    auto obj = Dispatcher::senderObj();
    if (obj == ui->widget_wordLibrary)
    {
        if (msgNum == 1)
        {
            QString info, value;
            message.getMessage(0, info, value);
            if (info == "create word")
            {
                ui->widget_wordCreate->setEdit(false);
                ui->widget_wordCreate->recoveryInterface();
                pushWidgetIndex(Widget_WordCreate);
                ui->stackedWidget->setCurrentIndex(Widget_WordCreate);
            }
            else if (info == "show word")
            {
                ui->widget_wordShow->recoveryInterface();
                ui->widget_wordShow->reloadGlobalValue();
                if (ui->widget_wordShow->loadWordInfo(value))
                {
                    pushWidgetIndex(Widget_WordShow);
                    ui->stackedWidget->setCurrentIndex(Widget_WordShow);
                }
                else
                {
                    MESSAGE("获取单词信息失败");
                }
            }
        }
//        else if (msgNum > 1) {}
    }
    else if (obj == ui->widget_wordCreate)
    {
        if (msgNum == 1)
        {
            QString info, value;
            message.getMessage(0, info, value);
            if (info == "cancel create")
            {
                popWidgetIndex();
                ui->stackedWidget->setCurrentIndex(topWidgetIndex());
            }
            else if (info == "create success")
            {
                popWidgetIndex();
                if (topWidgetIndex() == Widget_WordLibrary)
                {
                    ui->widget_wordLibrary->clearSearch();
                    ui->widget_wordLibrary->updateWordList();
                    ui->widget_wordLibrary->updateWordStatistics();
                }
                ui->stackedWidget->setCurrentIndex(topWidgetIndex());
            }
            else if (info == "modify success")
            {
                popWidgetIndex();
                if (topWidgetIndex() == Widget_WordShow)
                {
                    ui->widget_wordShow->recoveryInterface();
                    if (ui->widget_wordShow->loadWordInfo(value))
                    {
                        ui->stackedWidget->setCurrentIndex(Widget_WordShow);
                    }
                    else
                    {
                        MESSAGE("获取单词信息失败");
                        popWidgetIndex();
                        if (topWidgetIndex() == Widget_WordLibrary)
                        {
                            ui->widget_wordLibrary->clearSearch();
                            ui->widget_wordLibrary->updateWordList();
                            ui->widget_wordLibrary->updateWordStatistics();
                        }
                        ui->stackedWidget->setCurrentIndex(topWidgetIndex());
                    }
                }
            }
        }
//        else if (msgNum > 1) {}
    }
    else if (obj == ui->widget_wordShow)
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
                    ui->widget_wordLibrary->updateWordList();
                    ui->widget_wordLibrary->updateWordStatistics();
                }
                ui->stackedWidget->setCurrentIndex(topWidgetIndex());
            }
            else if (info == "delete success")
            {
                ui->widget_wordLibrary->clearSearch();
                ui->widget_wordLibrary->updateWordList();
                ui->widget_wordLibrary->updateWordStatistics();
                popWidgetIndex();
                ui->stackedWidget->setCurrentIndex(topWidgetIndex());
            }
            else if (info == "edit word")
            {
                ui->widget_wordCreate->setEdit(true);
                ui->widget_wordCreate->recoveryInterface();
                if (ui->widget_wordCreate->loadWordInfo(value))
                {
                    pushWidgetIndex(Widget_WordCreate);
                    ui->stackedWidget->setCurrentIndex(Widget_WordCreate);
                }
                else
                    MESSAGE("获取单词信息失败");
            }
        }
//        else if (msgNum > 1) {}
    }
    else if (obj == ui->widget_wordSetting)
    {
        if (msgNum == 1)
        {
            QString info, value;
            message.getMessage(0, info, value);
            if (info == "set reload flag")
            {
                bool flag = (value == "true" ? true : false);
                ui->widget_wordCreate->setReloadFlag(flag);
                ui->widget_wordLibrary->setReloadFlag(flag);
                ui->widget_wordShow->setReloadFlag(flag);
                ui->widget_wordMemorize->setReloadFlag(flag);
            }
        }
    }
}

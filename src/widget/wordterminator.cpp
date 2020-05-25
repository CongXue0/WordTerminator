#include "wordterminator.h"
#include "ui_wordterminator.h"
#include "wtool.h"
#include "wordadmin.h"
#include "forgetthread.h"
#include "memorythread.h"
#include "global.h"
#include "src/utils/version.h"
#include <QLayout>
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>
#include <QProgressDialog>

extern WordAdmin *p_wordAdmin;
extern ForgetThread *p_forgetThread;
extern MemoryThread *p_memThread;

WordTerminator::WordTerminator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WordTerminator)
{
    setStyleSheet(WTool::getStyleQss("WordTerminator"));
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::Dialog);

    setWindowTitle("WordTerminator " + Version::getVersion());

    m_first = true;

    pushWidgetIndex(Widget_WordLibrary);

    p_forgetThread->start();

    connect(ui->btn_save, SIGNAL(clicked()), this, SLOT(slot_saveBtn_clicked()));
    connect(ui->btn_lib, SIGNAL(pressed()), this, SLOT(slot_wtbuttonPressed()));
    connect(ui->btn_mem, SIGNAL(pressed()), this, SLOT(slot_wtbuttonPressed()));
    connect(ui->btn_fun, SIGNAL(pressed()), this, SLOT(slot_wtbuttonPressed()));
    connect(ui->btn_set, SIGNAL(pressed()), this, SLOT(slot_wtbuttonPressed()));

    connect(ui->widget_wordLibrary, SIGNAL(sendMessageSignal(WMessage)), this, SLOT(slot_handleMessage(WMessage)));
    connect(ui->widget_wordLibrary, SIGNAL(wordTimeIncreaseSignal(QString)), p_forgetThread, SLOT(slot_wordTimeIncrease(QString)));
    connect(p_forgetThread, SIGNAL(wordTimeDeclineSignal(QString)), ui->widget_wordLibrary, SLOT(slot_wordTimeDecline(QString)));

    connect(ui->widget_wordCreate, SIGNAL(sendMessageSignal(WMessage)), this, SLOT(slot_handleMessage(WMessage)));
    connect(ui->widget_wordCreate, SIGNAL(wordTimeIncreaseSignal(QString)), p_forgetThread, SLOT(slot_wordTimeIncrease(QString)));

    connect(ui->widget_wordShow, SIGNAL(sendMessageSignal(WMessage)), this, SLOT(slot_handleMessage(WMessage)));
    connect(ui->widget_wordShow, SIGNAL(wordTimeIncreaseSignal(QString)), p_forgetThread, SLOT(slot_wordTimeIncrease(QString)));
    connect(p_forgetThread, SIGNAL(wordTimeDeclineSignal(QString)), ui->widget_wordShow, SLOT(slot_wordTimeDecline(QString)));

//    connect(p_memThread, SIGNAL(wordCanMemorizeSignal(QString)), ui->widget_wordMemorize, SLOT(slot_wordCanMemorize(QString)));
//    connect(ui->widget_wordMemorize, SIGNAL(wordTimeIncreaseSignal(QString)), p_memThread, SLOT(slot_wordTimeIncrease(QString)));
    connect(ui->widget_wordMemorize, SIGNAL(wordTimeIncreaseSignal(QString)), p_forgetThread, SLOT(slot_wordTimeIncrease(QString)));
    connect(ui->widget_wordMemorize, SIGNAL(wordTimeIncreaseSignal(QString)), ui->widget_wordLibrary, SLOT(slot_wordTimeIncrease(QString)));
    connect(ui->widget_wordMemorize, SIGNAL(wordTimeIncreaseSignal(QString)), ui->widget_wordShow, SLOT(slot_wordTimeIncrease(QString)));
    connect(this, SIGNAL(stopWordMemorizeSignal(bool*)), ui->widget_wordMemorize, SLOT(slot_stopWordMemorize(bool*)));

    connect(ui->widget_wordSetting, SIGNAL(sendMessageSignal(WMessage)), this, SLOT(slot_handleMessage(WMessage)));
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

void WordTerminator::slot_wtbuttonPressed()
{
    QString name = sender()->objectName();
    int index = ui->stackedWidget->currentIndex();
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
        if (ui->widget_wordMemorize->getMode() == WordMemorizeWidget::MEMORY)
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
        ui->widget_wordLibrary->reloadGlobalValue();
        ui->widget_wordLibrary->clearSearch();
        ui->widget_wordLibrary->updateWordList();
        ui->widget_wordLibrary->updateWordStatistics();
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

    if (name == "btn_lib")
    {
        ui->btn_lib->setChecked(true);
        ui->btn_mem->setChecked(false);
        ui->btn_fun->setChecked(false);
        ui->btn_set->setChecked(false);
    }
    else if (name == "btn_mem")
    {
        ui->btn_lib->setChecked(false);
        ui->btn_mem->setChecked(true);
        ui->btn_fun->setChecked(false);
        ui->btn_set->setChecked(false);
    }
    else if (name == "btn_fun")
    {
        ui->btn_lib->setChecked(false);
        ui->btn_mem->setChecked(false);
        ui->btn_fun->setChecked(true);
        ui->btn_set->setChecked(false);
    }
    else if (name == "btn_set")
    {
        ui->btn_lib->setChecked(false);
        ui->btn_mem->setChecked(false);
        ui->btn_fun->setChecked(false);
        ui->btn_set->setChecked(true);
    }
}

void WordTerminator::slot_handleMessage(WMessage message)
{
    int msgNum = message.getMessageNum();
    if (msgNum <= 0)
        return;
    auto obj = this->sender();
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
                    QMessageBox::about(this, "提示", "获取单词信息失败");
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
                        QMessageBox::about(this, "提示", "获取单词信息失败");
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
                    QMessageBox::about(this, "提示", "获取单词信息失败");
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

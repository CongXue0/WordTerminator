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

extern WordAdmin *p_wordAdmin;
extern ForgetThread *p_forgetThread;
extern MemoryThread *p_memThread;

WordTerminator::WordTerminator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WordTerminator)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint |
        Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::Dialog);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    Global::setXmlPath(WTool::getConfigPath());
    Global::load();

    WTool::memoryConfigInit();
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

    btn_refresh = new QPushButton(label_menubg);
    btn_refresh->setObjectName("btn_boss");
    btn_refresh->setText("Refresh");
    if (WTool::getScreenSize() == "14")
    {
        btn_refresh->setStyleSheet("QPushButton{font-size:20px;color:#ffffff;"
            "background:#4595d9;qproperty-geometry:rect(30 8 80 40);}");
    }
    else
    {
        btn_refresh->setStyleSheet("QPushButton{font-size:24px;color:#ffffff;"
            "background:#4595d9;qproperty-geometry:rect(40 10 100 50);}");
    }
    connect(btn_refresh, SIGNAL(pressed()), this, SLOT(slot_btnRefresh_pressed()));

    btn_export = new QPushButton(label_menubg);
    btn_export->setObjectName("btn_export");
    btn_export->setText("Export");
    if (WTool::getScreenSize() == "14")
    {
        btn_export->setStyleSheet("QPushButton{font-size:20px;color:#000000;"
            "background:#8effff;qproperty-geometry:rect(30 585 80 40);}");
    }
    else
    {
        btn_export->setStyleSheet("QPushButton{font-size:30px;color:#000000;"
            "background:#8effff;qproperty-geometry:rect(40 730 100 50);}");
    }
    connect(btn_export, SIGNAL(pressed()), this, SLOT(slot_btnExport_pressed()));

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

    p_forgetThread->start();

}

WordTerminator::~WordTerminator()
{
    delete ui;
}

void WordTerminator::showEvent(QShowEvent *)
{
    if (m_first == true)
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
    if (QMessageBox::question(this, "question", "is close WT?", QMessageBox::Yes, QMessageBox::No) ==
        QMessageBox::Yes)
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

void WordTerminator::exportWord(QString msg)
{
    QStringList list = msg.split(',');
    int left = QString(list.at(0)).trimmed().toInt(),
        right = QString(list.at(1)).trimmed().toInt();
    int remember = QString(list.at(2)).trimmed().toInt();
    QString rem;
    if (remember == 0)
        rem = "u";
    else if (remember == 1)
        rem = "r";
    else if (remember == 2)
        rem = "a";
    else
    {
        QMessageBox::about(this, "提示", "格式错误");
        return;
    }
    QString fileName = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).at(0) +
        QString("/word(%1-%2)%3_%4.txt").arg(left).arg(right).arg(rem)
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text) == false)
    {
        QMessageBox::warning(this, "file write", "can't open", QMessageBox::Yes);
    }
    QTextStream in(&file);
    QTextCodec *code = QTextCodec::codecForName("utf8");
    in.setCodec(code);

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
            if (dialog.wasCanceled() == true)
                break;
            if (p_wordAdmin->getWordInfo(list.at(i), &wordInfo) == true)
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

void WordTerminator::slot_btnRefresh_pressed()
{
    int num = p_wordAdmin->resetAllWordRemerber(this);
    QMessageBox::about(this, "提示", "ok " + QString::number(num));
}

void WordTerminator::slot_btnExport_pressed()
{
    bool ok = false;
    QString text = QInputDialog::getText(this, "Export words", "Please input range, such as 0,9,0",
        QLineEdit::Normal, "0,9,0", &ok);
    if (ok == true)
    {
        if (text != "all")
        {
            exportWord(text);
        }
        else
        {
            exportWord("0,9,0");
            exportWord("0,50,1");
            exportWord("100,200,1");
        }
    }
}

void WordTerminator::slot_wtbuttonPressed()
{
    QString name = sender()->objectName();
    int index = stackedWidget->currentIndex();
    if ((name == "btn_lib" && index == Widget_WordLibrary) ||
        (name == "btn_mem" && index == WIdget_WordMemorize) ||
        (name == "btn_set" && index == Widget_Setting))
        return;

    switch (index)//先处理
    {
    case Widget_WordLibrary:
        break;
    case Widget_WordCreate:
        break;
    case Widget_WordShow:
        break;
    case WIdget_WordMemorize:
        if (wordMemorize->getMode() == WordMemorizeWidget::MEMORY)
        {
            bool ret = false;
            emit stopWordMemorizeSignal(&ret);
            if (ret == false)
                return;
        }
        break;
    case Widget_Setting:
        break;
    }

    clearWidgetIndex();
    if (name == "btn_lib")
        index = Widget_WordLibrary;
    else if (name == "btn_mem")
        index = WIdget_WordMemorize;
    else if (name == "btn_set")
        index = Widget_Setting;
    switch (index)//后处理
    {
    case Widget_WordLibrary:
        wordLibrary->clearSearch();
        wordLibrary->updateWordList();
        wordLibrary->updateWordStatistics();
        break;
    case WIdget_WordMemorize:
        wordMemorize->recoveryInterface();
        wordMemorize->updateWordStatistics();
        break;
    case Widget_Setting:
        break;
    }
    pushWidgetIndex(index);
    stackedWidget->setCurrentIndex(index);

    WTButton *btn[3] = {btn_lib, btn_mem, btn_set};
    for (int i = 0; i < 3; i++)
    {
        if (btn[i]->objectName() == name)
            btn[i]->setActive(true);
        else
            btn[i]->setActive(false);
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
                if (wordShow->loadWordInfo(value) == true)
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
                    if (wordShow->loadWordInfo(value) == true)
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
                if (wordCreate->loadWordInfo(value) == true)
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
}

#include "wordautomatedmemorizescenegraphview.h"
#include "wtool.h"
#include "global.h"
#include "dispatcher.h"
#include "wordterminator.h"
#include <QTimer>
#include <QMouseEvent>
#include <QApplication>
#include <QClipboard>

extern WordAdmin *p_wordAdmin;

WordAutomatedMemorizeSceneGraphView::WordAutomatedMemorizeSceneGraphView(QWidget *parent) : QWidget(parent)
{
    m_state = State_Init;

    m_node_bg = new SceneImage(":/main/res/skin/picture/bg_auto_mem_start.png", this);

    m_node_wordBar = new SceneWordScrollbar(m_node_bg);

    auto f = this->font();

    m_node_info = new SceneText(m_node_bg);
    m_node_info->setText("已记忆：999    未记忆：10000");
    f.setPixelSize(36);
    m_node_info->setFont(f);
    m_node_info->setColor(QColor(0x989898));

    m_node_word = new SceneText(m_node_bg);
    m_node_word->setWordWrap(true);
    f.setPixelSize(22);
    m_node_word->setFont(f);
    m_node_word->setColor(Qt::black);

    m_node_timer = new SceneTimer(m_node_bg);

    m_btn_pause = new DtcpImageButton(this);
    m_btn_pause->setObjectName("btn_pause");
    m_btn_pause->setCheckable(true);
    m_btn_pause->setAutoCheck(false);

    m_btn_visible = new DtcpImageButton(this);
    m_btn_visible->setObjectName("btn_visible");
    m_btn_visible->setCheckable(true);
    m_btn_visible->setAutoCheck(false);
    m_btn_visible->setUncheckedTip("Set to unvisible");
    m_btn_visible->setCheckedTip("Set to visible");

    m_btn_minus = new DtcpImageButton(this);
    m_btn_minus->setObjectName("btn_minus");
    m_btn_minus->setCheckable(true);
    m_btn_minus->setAutoCheck(false);
    m_btn_minus->setToolTip("Speed down");

    m_btn_plus = new DtcpImageButton(this);
    m_btn_plus->setObjectName("btn_plus");
    m_btn_plus->setCheckable(true);
    m_btn_plus->setAutoCheck(false);
    m_btn_plus->setToolTip("Speed down");

    m_btn_forever = new DtcpImageButton(this);
    m_btn_forever->setObjectName("btn_forever");
    m_btn_forever->setCheckable(true);
    m_btn_forever->setAutoCheck(false);
    m_btn_forever->setUncheckedTip("Set to forever");
    m_btn_forever->setCheckedTip("Set to not forever");

    m_btn_ok = new DtcpImageButton(this);
    m_btn_ok->setObjectName("btn_ok");
    m_btn_ok->setCheckable(true);
    m_btn_ok->setAutoCheck(false);

    m_combox_group = new QComboBox(this);
    m_combox_group->setObjectName("combox_group");

    setStyleSheet(QString::fromUtf8(u8R"(
/**********QPushButton**********/
QPushButton#btn_pause {
    border: none;
    border-image: url(:/main/res/skin/picture/auto_pause.png);
}
QPushButton#btn_pause:hover {
    border-image: url(:/main/res/skin/picture/auto_pause_hover.png);
}
QPushButton#btn_pause:checked {
    border: none;
    border-image: url(:/main/res/skin/picture/auto_play.png);
}
QPushButton#btn_pause:checked:hover {
    border-image: url(:/main/res/skin/picture/auto_play_hover.png);
}
QPushButton#btn_visible {
    border: none;
    border-image: url(:/main/res/skin/picture/auto_visible.png);
}
QPushButton#btn_visible:hover {
    border-image: url(:/main/res/skin/picture/auto_visible_hover.png);
}
QPushButton#btn_visible:checked {
    border: none;
    border-image: url(:/main/res/skin/picture/auto_unvisible.png);
}
QPushButton#btn_visible:checked:hover {
    border-image: url(:/main/res/skin/picture/auto_unvisible_hover.png);
}
QPushButton#btn_minus {
    border: none;
    border-image: url(:/main/res/skin/picture/auto_minus.png);
}
QPushButton#btn_minus:hover {
    border-image: url(:/main/res/skin/picture/auto_minus_hover.png);
}
QPushButton#btn_plus {
    border: none;
    border-image: url(:/main/res/skin/picture/auto_plus.png);
}
QPushButton#btn_plus:hover {
    border-image: url(:/main/res/skin/picture/auto_plus_hover.png);
}
QPushButton#btn_forever {
    border: none;
    border-image: url(:/main/res/skin/picture/notforever.png);
}
QPushButton#btn_forever:hover {
    border-image: url(:/main/res/skin/picture/notforever_hover.png);
}
QPushButton#btn_forever:checked {
    border: none;
    border-image: url(:/main/res/skin/picture/forever.png);
}
QPushButton#btn_forever:checked:hover {
    border-image: url(:/main/res/skin/picture/forever_hover.png);
}
QPushButton#btn_forever:disabled {
    border: none;
    border-image: url(:/main/res/skin/picture/forever_disabled.png);
}
QPushButton#btn_ok {
    border: none;
    border-image: url(:/main/res/skin/picture/auto_gou.png);
}
QPushButton#btn_ok:hover {
    border-image: url(:/main/res/skin/picture/auto_gou_hover.png);
}

/***********QComboBox***********/
QComboBox#combox_group {
    border: 2px solid #b0c4de;
    background-color: transparent;
    color: #656669;
    font-size: 18px;
}
QComboBox::down-arrow {
    background: url(:/main/res/skin/picture/combox_arrow.png) transparent no-repeat left center;
    padding: 0px;
    width: 40px;
    height: 25px;
}
QComboBox::drop-down {
    border: none;
    background-color: transparent;
}
QComboBox QAbstractItemView {
    background-color: #f5f5f5;
}
)"));

    updatePosition();

    connect(m_node_wordBar, SIGNAL(updateSignal()), this, SLOT(update()));
    connect(m_node_wordBar, SIGNAL(finished()), this, SLOT(OnWordBarAnimationFinished()));
    connect(m_node_info, SIGNAL(updateSignal()), this, SLOT(update()));
    connect(m_node_word, SIGNAL(updateSignal()), this, SLOT(update()));
    connect(m_node_word, SIGNAL(finished()), this, SLOT(OnWordShowFinished()));
    connect(m_node_timer, SIGNAL(updateSignal()), this, SLOT(update()));
    connect(m_node_timer, SIGNAL(finished()), this, SLOT(OnWordTimeOut()));

    connect(m_btn_pause, SIGNAL(clicked()), this, SLOT(OnBtnPauseClicked()));
    connect(m_btn_visible, SIGNAL(clicked()), this, SLOT(OnBtnVisibleClicked()));
    connect(m_btn_minus, SIGNAL(clicked()), this, SLOT(OnBtnMinusClicked()));
    connect(m_btn_plus, SIGNAL(clicked()), this, SLOT(OnBtnPlusClicked()));
    connect(m_btn_forever, SIGNAL(clicked()), this, SLOT(OnBtnForeverClicked()));
    connect(m_btn_ok, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked()));
    connect(m_combox_group, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboxGroup_currentIndexChanged(int)));
}

void WordAutomatedMemorizeSceneGraphView::initiate(QList<WordTest> testList)
{
    if (testList.isEmpty()) return;
    if (testList.size() > 3)
    {
        int count = int(testList.size() * 2.5);
        for (int i = 0; i < count; ++i)
        {
            int r1 = WTool::rand(0, testList.size() - 1);
            int r2 = WTool::rand(0, testList.size() - 1);
            while (r1 == r2) {
                r2 = WTool::rand(0, testList.size() - 1);
            }
            std::swap(testList[r1], testList[r2]);
        }
    }
    m_node_wordBar->setTestList(testList);
    m_node_info->setText(QString("已记忆：%1    未记忆：%2").arg(m_node_wordBar->getPassNum()).arg(m_node_wordBar->getTestNum()));
    m_node_word->setText("");
    m_node_word->setVisible(false);
    m_node_timer->setDuration(10 * 1000);

    QSignalBlocker sb(m_combox_group);
    m_combox_group->clear();
    QStringList groupList = WTool::getGroupList();
    for (int i = 0; i < groupList.count(); ++i)
    {
        m_combox_group->addItem(groupList.at(i));
    }

    m_state = State_Init;
}

void WordAutomatedMemorizeSceneGraphView::start()
{
    setState(State_TextAni);
}

void WordAutomatedMemorizeSceneGraphView::stop()
{
    setState(State_Init);
}

void WordAutomatedMemorizeSceneGraphView::setState(int state)
{
    if (m_state == state) return;
    if (m_state == State_Init)
    {
        if (state == State_TextAni)
        {
            WordInfo wordInfo;
            if (p_wordAdmin->getWordInfo(m_node_wordBar->getCurrentWord()->m_info.m_name, &wordInfo))
            {
                m_btn_forever->setChecked(m_node_wordBar->getCurrentWord()->m_info.m_remember > 0);
                QSignalBlocker sb(m_combox_group);
                for (int i = 0; i < m_combox_group->count(); ++i)
                {
                    if (Global::m_groupName[wordInfo.m_groupid].getValueStr() == m_combox_group->itemText(i))
                    {
                        m_combox_group->setCurrentIndex(i);
                        break;
                    }
                }

                m_node_word->setVisible(false);
                if (m_node_wordBar->getCurrentWord()->isVisible())
                {
                    m_btn_visible->setChecked(false);
                    m_node_word->setText(wordInfo.toText(2));
                    m_node_word->eraseShow();
                }
                else
                {
                    m_btn_visible->setChecked(true);
                    QTimer::singleShot(0, this, [&] { OnWordShowFinished(); });
                }
            }
        }
    }
    else if (m_state == State_TextAni)
    {
        if (state == State_TimerAni)
        {
            m_btn_pause->setChecked(false);
            double p = 1.0;
            if (!m_node_wordBar->getCurrentWord()->isVisible())
                p = 0.8;
            m_node_timer->setDuration(int(m_node_wordBar->getCurrentWord()->toMsecs() * p));
//                qDebug()<<wordInfo.m_name<<", "<<m_node_timer->getDuration();
            m_node_timer->start();
        }
        else if (state == State_Init)
        {
            if (m_node_word->getAnimation())
            {
                m_node_word->getAnimation()->stop();
            }
        }
    }
    else if (m_state == State_TimerAni)
    {
        if (state == State_BarAni)
        {
            BriefWordInfo wordInfo;
            m_node_wordBar->advance(&wordInfo);
            if (!wordInfo.m_name.isEmpty())
            {
                wordInfo.m_times++;
                m_node_info->setText(QString("已记忆：%1    未记忆：%2").arg(m_node_wordBar->getPassNum()).arg(m_node_wordBar->getTestNum()));

                wordInfo.m_modifyTime = QDateTime::currentDateTime();
                if (wordInfo.m_remember == -1 || wordInfo.m_remember == 2)
                {
                    wordInfo.m_remember = (wordInfo.m_remember > 0) ? 1 : 0;
                }
                p_wordAdmin->updateWord(wordInfo.m_name, "Times", QString::number(wordInfo.m_times), "ModifyTime",
                    wordInfo.m_modifyTime.toString(TIMEFORMAT), "RememberState", QString::number(wordInfo.m_remember));
                emit Dispatch(this).signal_wordTimeIncrease(wordInfo.m_name);
            }
            if (m_node_wordBar->isEnd())
            {
                MESSAGE("本次记忆单词数 " + QString::number(m_node_wordBar->getPassNum()));
                emit Dispatch(this).signal_memorizeFinished();
            }
        }
        else if (state == State_Init)
        {
            if (m_node_timer->getAnimation())
            {
                m_node_timer->getAnimation()->stop();
            }
        }
    }
    else if (m_state == State_BarAni)
    {
        if (state == State_TextAni)
        {
            WordInfo wordInfo;
            if (p_wordAdmin->getWordInfo(m_node_wordBar->getCurrentWord()->m_info.m_name, &wordInfo))
            {
                m_btn_forever->setChecked(m_node_wordBar->getCurrentWord()->m_info.m_remember > 0);
                QSignalBlocker sb(m_combox_group);
                for (int i = 0; i < m_combox_group->count(); ++i)
                {
                    if (Global::m_groupName[wordInfo.m_groupid].getValueStr() == m_combox_group->itemText(i))
                    {
                        m_combox_group->setCurrentIndex(i);
                        break;
                    }
                }

                m_node_word->setVisible(false);
                if (m_node_wordBar->getCurrentWord()->isVisible())
                {
                    m_btn_visible->setChecked(false);
                    m_node_word->setText(wordInfo.toText(2));
                    m_node_word->eraseShow();
                }
                else
                {
                    m_btn_visible->setChecked(true);
                    QTimer::singleShot(0, this, [&] { OnWordShowFinished(); });
                }
            }
        }
    }
    m_state = state;
}

QAbstractAnimation *WordAutomatedMemorizeSceneGraphView::currentAnimation()
{
    if (m_state == State_BarAni)
    {
        return m_node_wordBar->getAnimation();
    }
    else if (m_state == State_TextAni)
    {
        return m_node_word->getAnimation();
    }
    else if (m_state == State_TimerAni)
    {
        return m_node_timer->getAnimation();
    }
    return nullptr;
}

void WordAutomatedMemorizeSceneGraphView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setFont(this->font());

    m_node_bg->render(&painter);
}

void WordAutomatedMemorizeSceneGraphView::resizeEvent(QResizeEvent *)
{
    updatePosition();
}

void WordAutomatedMemorizeSceneGraphView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_node_word->isVisible() && !m_node_word->text().isEmpty() && m_node_word->geometry().contains(event->pos()))
    {
        QClipboard *clip= QApplication::clipboard();
        clip->setText(m_node_word->text().split('\n')[0]);
        WTool::opacityMessageBox("Copy", "", 0.1, 100);
    }
}

void WordAutomatedMemorizeSceneGraphView::updatePosition()
{
    m_node_bg->setImgSize(this->size());

    m_node_wordBar->setPos(QPoint(0, 60));

    m_node_info->setGeometry(QRect(10, 0, this->width() - 20, 60));

    m_node_word->setGeometry(QRect(230 + 10, 60 + (this->height() - 60) / 2 - 200, this->width() - 230 - 10, 400));

    int h = (this->height() - 60 - 400) / 2;
    m_node_timer->setPos(QPoint(this->width() - h, 60));
    m_node_timer->setSize(QSize(h, h));

    m_btn_pause->setGeometry(this->width() - h - 80 - ((h - 80) / 2) * 2 / 3, 60 + (h - 80) / 2, 80, 80);
    int x = 230 + 20, y = 60 + h + 400 + 40, spacing = 20;
    m_btn_visible->setGeometry(x, y, 50, 50);
    x += 50 + spacing;
    m_btn_minus->setGeometry(x, y, 50, 50);
    x += 50 + spacing;
    m_btn_plus->setGeometry(x, y, 50, 50);
    x += 50 + spacing;
    m_btn_forever->setGeometry(x, y, 50, 50);
    x += 50 + spacing;
    m_btn_ok->setGeometry(x, y, 50, 50);
    x += 50 + spacing;
    m_combox_group->setGeometry(x, y + 5, 200, 40);
}

void WordAutomatedMemorizeSceneGraphView::OnWordBarAnimationFinished()
{
    setState(State_TextAni);
}

void WordAutomatedMemorizeSceneGraphView::OnWordShowFinished()
{
    setState(State_TimerAni);
}

void WordAutomatedMemorizeSceneGraphView::OnWordTimeOut()
{
    setState(State_BarAni);
}

void WordAutomatedMemorizeSceneGraphView::OnBtnPauseClicked()
{
    if (m_state == State_TimerAni)
    {
        if (m_node_timer->getAnimation()->state() == QAbstractAnimation::Running)
        {
            m_node_timer->getAnimation()->pause();
            m_btn_pause->setChecked(true);
        }
        else if (m_node_timer->getAnimation()->state() == QAbstractAnimation::Paused)
        {
            m_node_timer->getAnimation()->resume();
            m_btn_pause->setChecked(false);
        }
    }
}

void WordAutomatedMemorizeSceneGraphView::OnBtnVisibleClicked()
{
    if (m_state == State_TimerAni)
    {
        if (m_node_wordBar->getCurrentWord()->isVisible())
        {
            m_node_wordBar->getCurrentWord()->m_visible = false;
            m_node_word->setVisible(false);
            m_btn_visible->setChecked(true);
        }
        else
        {
            m_node_wordBar->getCurrentWord()->m_visible = true;
            m_node_word->setVisible(true);
            m_btn_visible->setChecked(true);
        }
    }
}

void WordAutomatedMemorizeSceneGraphView::OnBtnMinusClicked()
{
    if (m_state == State_TimerAni)
    {
        m_node_wordBar->getCurrentWord()->setSpeed(m_node_wordBar->getCurrentWord()->getSpeed() - 1);
        m_node_timer->resetDuration(m_node_wordBar->getCurrentWord()->toMsecs());
    }
}

void WordAutomatedMemorizeSceneGraphView::OnBtnPlusClicked()
{
    if (m_state == State_TimerAni)
    {
        m_node_wordBar->getCurrentWord()->setSpeed(m_node_wordBar->getCurrentWord()->getSpeed() + 1);
        m_node_timer->resetDuration(m_node_wordBar->getCurrentWord()->toMsecs());
    }
}

void WordAutomatedMemorizeSceneGraphView::OnBtnForeverClicked()
{
    if (m_state == State_TimerAni)
    {
        if (m_node_wordBar->getCurrentWord()->m_info.m_remember > 0)
        {
            m_node_wordBar->getCurrentWord()->m_info.m_remember = 0;
            m_btn_forever->setChecked(false);
        }
        else
        {
            m_node_wordBar->getCurrentWord()->m_info.m_remember = 1;
            m_btn_forever->setChecked(true);
        }
    }
}

void WordAutomatedMemorizeSceneGraphView::OnBtnOkClicked()
{
    if (m_state == State_TimerAni)
    {
        m_node_wordBar->passCurrentWord();
        m_node_timer->resetDuration(m_node_wordBar->getCurrentWord()->toMsecs() / 2);
    }
}

void WordAutomatedMemorizeSceneGraphView::slot_comboxGroup_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    if (WordTerminator::instance()->getCurrentWidgetIndex() == WordTerminator::Widget_AutomatedMemorize)
    {
        m_node_wordBar->getCurrentWord()->m_info.m_groupId = WTool::getGroupNo(m_combox_group->currentText());
        p_wordAdmin->updateWord(m_node_wordBar->getCurrentWord()->m_info.m_name, "Groupid",
            QString::number(m_node_wordBar->getCurrentWord()->m_info.m_groupId));
    }
}

#include "scenenode.h"
#include "wtool.h"
#include "global.h"
#include <QTimer>

#define SHOW_LINE false
#define DURATION_SCALE 1.0

//const double M_PI = 3.14159265358979323846;
const auto ANG = [](double x) { return int(x * 16); };
//const auto RAD = [](double x) { return x * M_PI / 180.0; };
//const auto DIST2 = [](auto x1, auto y1, auto x2, auto y2) { return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2); };

SceneNode::SceneNode(QObject *parent) : QObject(parent)
{
    m_win_size = QSize(100, 100);
    m_pos = QPoint(0, 0);
    m_visible = true;
}

SceneNode::~SceneNode()
{
}

void SceneNode::setPos(QPoint pos)
{
    m_pos = pos;
    emit updateSignal();
}

void SceneNode::setVisible(bool vis)
{
    m_visible = vis;
    emit updateSignal();
}

bool SceneNode::isVisible() const
{
    return m_visible;
}

SceneNode *SceneNode::getRoot()
{
    auto par = dynamic_cast<SceneNode *>(this->parent());
    if (!par) return this;
    return par->getRoot();
}

QRect SceneNode::boundingRect() const
{
    return QRect(QPoint(0, 0), m_win_size);
}

QVector<SceneNode *> SceneNode::select(QPoint pos)
{
    QVector<SceneNode *> list;
    if (m_visible)
    {
        auto childs = this->children();
        for (int i = 0; i < childs.size(); ++i)
        {
            if (auto chi = dynamic_cast<SceneNode *>(childs[i]))
            {
                if (chi->boundingRect().contains(pos))
                    list.append(chi);
                list.append(chi->select(pos));
            }
        }
    }
    return list;
}

void SceneNode::render(QPainter *painter)
{
    if (painter == nullptr) return;
    if (!m_visible) return;

    if (auto *w = dynamic_cast<QWidget *>(getRoot()->parent()))
        m_win_size = w->size();

    painter->save();
    painter->translate(m_pos);
    this->selfRender(painter);
    painter->restore();

    auto childs = this->children();
    for (int i = 0; i < childs.size(); ++i)
    {
        if (auto chi = dynamic_cast<SceneNode *>(childs[i]))
        {
            chi->render(painter);
        }
    }
}

void SceneNode::selfRender(QPainter *)
{
}

SceneImage::SceneImage(QString img, QObject *parent) : SceneNode(parent)
{
    setImg(img);
}

void SceneImage::setImg(QString img)
{
    m_img = QPixmap(img);
    m_size = m_img.size();
    emit updateSignal();
}

void SceneImage::setImgSize(QSize size)
{
    m_size = size;
    emit updateSignal();
}

void SceneImage::selfRender(QPainter *painter)
{
    if (!m_img.isNull())
        painter->drawPixmap(QRect(m_pos, m_size), m_img);
}

SceneWordScrollbar::SceneWordScrollbar(QObject *parent) : SceneNode(parent)
{
    m_volume = 1;
    m_silderPos = 0;
    m_silderIndex = 0;
    m_markPass = -1;
    m_parallelGroup = nullptr;

    this->setProperty("rect1", m_r_main);
    this->setProperty("font1", int(22));
    this->setProperty("rect2", m_r_main);
    this->setProperty("font2", int(22));

    m_spacing = 14;
    m_r_main = QRect(5, 350, 400, 30);
    for (int i = 0; i < 10; ++i)
    {
        m_r_next[i] = QRect(5, 350 - (i + 1) * (24 + m_spacing), 400, 24);
        m_r_prev[i] = QRect(5, 350 + 30 + m_spacing + i * (24 + m_spacing), 400, 24);
    }
}

void SceneWordScrollbar::setTestList(const QList<WordTest> &testList)
{
    if (testList.size() < 2) return;
    m_testList = testList;
    m_testNum = m_testList.size();
    m_passNum = 0;
    m_volume = 1;
    m_silderPos = 0;
    m_silderIndex = 0;
    m_markPass = 0;
    emit updateSignal();
}

WordTest *SceneWordScrollbar::getCurrentWord()
{
    int index = m_silderPos + m_silderIndex;
    if (index >= 0 && index <= m_testList.size() - 1)
    {
        return &m_testList[index];
    }
    return nullptr;
}

void SceneWordScrollbar::passCurrentWord()
{
    int index = m_silderPos + m_silderIndex;
    if (index >= 0 && index <= m_testList.size() - 1)
    {
        m_markPass = index + 1;
    }
}

void SceneWordScrollbar::advance(BriefWordInfo *wordInfo)
{
    if (wordInfo)
        (*wordInfo).m_name.clear();
    if (m_silderPos >= m_testList.size()) return;

    int index = m_silderPos + m_silderIndex;
    if (m_markPass > 0)//有标记 pass word
    {
        if (wordInfo)
            *wordInfo = m_testList[m_markPass - 1].m_info;
        m_testList.removeAt(m_markPass - 1);
        m_markPass = -m_markPass;
        m_testNum--;
        m_passNum++;
        m_volume--;
        if (isEnd()) return;

        if (m_silderIndex == m_volume)//to volume end
        {
            if (m_silderIndex == 0)//删除为第一个元素
            {
                m_volume++;
                m_operation.enqueue(std::make_tuple(Down, 300));
            }
            else
            {
                if (index != m_testList.size())//not to list end
                    m_volume++;
                int count = index - m_silderPos;
                int msecs = 200;
                if (msecs * count > 1200)
                    msecs = 1200 / count;
                for (int i = 0; i < count; ++i)
                    m_operation.enqueue(std::make_tuple(Up, msecs));
            }
        }
        else
        {
            m_operation.enqueue(std::make_tuple(Down, 300));
        }
    }
    else
    {
        m_testList[index].m_loopNum++;
        if (m_testList[index].m_loopNum >= Global::m_autoSilderVolume.getValueInt())//word循环次数完成
        {
            if (wordInfo)
                *wordInfo = m_testList[index].m_info;
            m_silderPos++;
            m_silderIndex = -1;
            m_volume--;
            m_testNum--;
            m_passNum++;
            if (isEnd()) return;

            m_operation.enqueue(std::make_tuple(Down, 300));
        }
        else
        {
            if (m_silderIndex + 1 == m_volume)//to volume end
            {
                if (index < m_testList.size() - 1 && m_volume < Global::m_autoSilderVolume.getValueInt())
                    m_volume++;
                int count = index - m_silderPos;
                int msecs = 200;
                if (msecs * count > 1200)
                    msecs = 1200 / count;
                if (count == 0)
                {
                    m_operation.enqueue(std::make_tuple(Stay, msecs));
                }
                else
                {
                    for (int i = 0; i < count; ++i)
                        m_operation.enqueue(std::make_tuple(Up, msecs));
                }
            }
            else if (m_silderIndex + 1 < m_volume)//not to volume end
            {
                m_operation.enqueue(std::make_tuple(Down, 300));
            }
        }
    }
    operate();
}

bool SceneWordScrollbar::isEnd() const
{
    return m_silderPos >= m_testList.size();
}

void SceneWordScrollbar::selfRender(QPainter *painter)
{
    QColor textColor(0xbd632f);
    int w = 230, h = m_win_size.height() - m_pos.y();
    painter->setClipRect(0, 0, 500, h);
    if (SHOW_LINE)
    {
        painter->setPen(QColor(Qt::blue));
        painter->drawRect(0, 0, w, h);
    }
    if (m_testList.isEmpty()) return;
    int count = m_testList.size();
    int index = m_silderPos + m_silderIndex;
    if (index < 0 || (index > count - 1 && m_markPass == 0)) return;

    painter->setPen(QPen(textColor));
    QFont f = painter->font();

    if (m_parallelGroup)
    {
        if (m_markPass < 0)
        {
            QRect rect1 = property("rect1").toRect();
            int ront1Size = property("font1").toInt();
            if (m_dir == Down)
            {
                f.setPixelSize(ront1Size);
                painter->setFont(f);
                painter->drawText(rect1, Qt::AlignLeft | Qt::AlignVCenter, m_testList[index].m_info.m_name);
                if (SHOW_LINE)
                    painter->drawRect(rect1);

                f.setPixelSize(18);
                painter->setFont(f);
                for (int i = 0; i < 10; ++i)
                {
                    int i1 = index + (i + 1), i2 = index - (i + 1);
                    if (i1 >= 0 && i1 <= count - 1)
                    {
                        QRect nextRect(5, rect1.y() - (i + 1) * (24 + m_spacing), 400, 24);
                        painter->drawText(nextRect, Qt::AlignLeft | Qt::AlignVCenter, m_testList[i1].m_info.m_name);
                        if (SHOW_LINE)
                            painter->drawRect(nextRect);
                    }

                    if (i2 >= 0 && i2 <= count - 1)
                    {
                        painter->drawText(m_r_prev[i], Qt::AlignLeft | Qt::AlignVCenter, m_testList[i2].m_info.m_name);
                        if (SHOW_LINE)
                            painter->drawRect(m_r_prev[i]);
                    }
                }
            }
            else if (m_dir == Up)
            {
                if (index - 1 < 0) return;

                f.setPixelSize(ront1Size);
                painter->setFont(f);
                painter->drawText(rect1, Qt::AlignLeft | Qt::AlignVCenter, m_testList[index - 1].m_info.m_name);
                if (SHOW_LINE)
                    painter->drawRect(rect1);

                f.setPixelSize(18);
                painter->setFont(f);
                for (int i = 0; i < 10; ++i)
                {
                    int i1 = index - 1 + (i + 1), i2 = index - 1 - (i + 1);
                    if (i1 >= 0 && i1 <= count - 1)
                    {
                        painter->drawText(m_r_next[i], Qt::AlignLeft | Qt::AlignVCenter, m_testList[i1].m_info.m_name);
                        if (SHOW_LINE)
                            painter->drawRect(m_r_next[i]);
                    }

                    if (i2 >= 0 && i2 <= count - 1)
                    {
                        QRect prevRect(5, rect1.y() + rect1.height() + m_spacing + i * (24 + m_spacing), 400, 24);
                        painter->drawText(prevRect, Qt::AlignLeft | Qt::AlignVCenter, m_testList[i2].m_info.m_name);
                        if (SHOW_LINE)
                            painter->drawRect(m_r_prev[i]);
                    }
                }
            }
        }
        else
        {
            QRect rect1 = property("rect1").toRect(), rect2 = property("rect2").toRect();
            int font1Size = property("font1").toInt(), font2Size = property("font2").toInt();
            if (m_dir == Down)
            {
                f.setPixelSize(font1Size);
                painter->setFont(f);
                painter->drawText(rect1, Qt::AlignLeft | Qt::AlignVCenter, m_testList[index + 1].m_info.m_name);
                if (SHOW_LINE)
                    painter->drawRect(rect1);

                f.setPixelSize(font2Size);
                painter->setFont(f);
                painter->drawText(rect2, Qt::AlignLeft | Qt::AlignVCenter, m_testList[index].m_info.m_name);
                if (SHOW_LINE)
                    painter->drawRect(rect2);

                f.setPixelSize(18);
                painter->setFont(f);
                for (int i = 0; i < 10; ++i)
                {
                    int i1 = index + 1 + (i + 1), i2 = index - (i + 1);
                    if (i1 >= 0 && i1 <= count - 1)
                    {
                        QRect nextRect(5, rect1.y() - (i + 1) * (24 + m_spacing), 400, 24);
                        painter->drawText(nextRect, Qt::AlignLeft | Qt::AlignVCenter, m_testList[i1].m_info.m_name);
                        if (SHOW_LINE)
                            painter->drawRect(nextRect);
                    }

                    if (i2 >= 0 && i2 <= count - 1)
                    {
                        QRect prevRect(5, rect2.y() + rect2.height() + m_spacing + i * (24 + m_spacing), 400, 24);
                        painter->drawText(prevRect, Qt::AlignLeft | Qt::AlignVCenter, m_testList[i2].m_info.m_name);
                        if (SHOW_LINE)
                            painter->drawRect(prevRect);
                    }
                }
            }
            else if (m_dir == Up)
            {
                f.setPixelSize(font1Size);
                painter->setFont(f);
                painter->drawText(rect1, Qt::AlignLeft | Qt::AlignVCenter, m_testList[index].m_info.m_name);
                if (SHOW_LINE)
                    painter->drawRect(rect1);

                f.setPixelSize(font2Size);
                painter->setFont(f);
                painter->drawText(rect2, Qt::AlignLeft | Qt::AlignVCenter, m_testList[index - 1].m_info.m_name);
                if (SHOW_LINE)
                    painter->drawRect(rect2);

                f.setPixelSize(18);
                painter->setFont(f);
                for (int i = 0; i < 10; ++i)
                {
                    int i1 = index + (i + 1), i2 = index - 1 - (i + 1);
                    if (i1 >= 0 && i1 <= count - 1)
                    {
                        QRect nextRect(5, rect1.y() - (i + 1) * (24 + m_spacing), 400, 24);
                        painter->drawText(nextRect, Qt::AlignLeft | Qt::AlignVCenter, m_testList[i1].m_info.m_name);
                        if (SHOW_LINE)
                            painter->drawRect(nextRect);
                    }

                    if (i2 >= 0 && i2 <= count - 1)
                    {
                        QRect prevRect(5, rect2.y() + rect2.height() + m_spacing + i * (24 + m_spacing), 400, 24);
                        painter->drawText(prevRect, Qt::AlignLeft | Qt::AlignVCenter, m_testList[i2].m_info.m_name);
                        if (SHOW_LINE)
                            painter->drawRect(prevRect);
                    }
                }
            }
        }
    }
    else
    {
        if (!(m_markPass < 0 && index == -(m_markPass + 1)))
        {
            f.setPixelSize(22);
            painter->setFont(f);
            painter->drawText(m_r_main, Qt::AlignLeft | Qt::AlignVCenter, m_testList[index].m_info.m_name);
            if (SHOW_LINE)
                painter->drawRect(m_r_main);
        }

        f.setPixelSize(18);
        painter->setFont(f);
        for (int i = 0; i < 10; ++i)
        {
            int i1 = index + (i + 1), i2 = index - (i + 1);
            if (i1 >= 0 && i1 <= count - 1)
            {
                painter->drawText(m_r_next[i], Qt::AlignLeft | Qt::AlignVCenter, m_testList[i1].m_info.m_name);
                if (SHOW_LINE)
                    painter->drawRect(m_r_next[i]);
            }
            if (i2 >= 0 && i2 <= count - 1)
            {
                painter->drawText(m_r_prev[i], Qt::AlignLeft | Qt::AlignVCenter, m_testList[i2].m_info.m_name);
                if (SHOW_LINE)
                    painter->drawRect(m_r_prev[i]);
            }
        }
    }
}

void SceneWordScrollbar::operate()
{
    if (m_operation.isEmpty()) return;
    if (m_parallelGroup && m_parallelGroup->state() == QAbstractAnimation::Stopped)
    {
        delete m_parallelGroup;
        m_parallelGroup = nullptr;
    }

    auto opt = m_operation.dequeue();
    m_dir = std::get<0>(opt);
    m_msec = int(std::get<1>(opt) * DURATION_SCALE);
    if (m_markPass < 0)
    {
        QPropertyAnimation *rect1Animation = new QPropertyAnimation(this, "rect1");
        rect1Animation->setDuration(m_msec);
        if (m_dir == Down)
        {
            rect1Animation->setStartValue(m_r_next[0]);
            rect1Animation->setEndValue(m_r_main);
        }
        else if (m_dir == Up)
        {
            rect1Animation->setStartValue(m_r_prev[0]);
            rect1Animation->setEndValue(m_r_main);
        }
        QPropertyAnimation *font1Animation = new QPropertyAnimation(this, "font1");
        font1Animation->setDuration(m_msec);
        font1Animation->setStartValue(18);
        font1Animation->setEndValue(22);

        m_parallelGroup = new QParallelAnimationGroup(this);
        m_parallelGroup->addAnimation(rect1Animation);
        m_parallelGroup->addAnimation(font1Animation);
    }
    else
    {
        if (m_dir == Down)
        {
            QPropertyAnimation *rect1Animation = new QPropertyAnimation(this, "rect1");
            rect1Animation->setDuration(m_msec);
            rect1Animation->setStartValue(m_r_next[0]);
            rect1Animation->setEndValue(m_r_main);
            QPropertyAnimation *font1Animation = new QPropertyAnimation(this, "font1");
            font1Animation->setDuration(m_msec);
            font1Animation->setStartValue(18);
            font1Animation->setEndValue(22);

            QPropertyAnimation *rect2Animation = new QPropertyAnimation(this, "rect2");
            rect2Animation->setDuration(m_msec);
            rect2Animation->setStartValue(m_r_main);
            rect2Animation->setEndValue(m_r_prev[0]);
            QPropertyAnimation *font2Animation = new QPropertyAnimation(this, "font2");
            font2Animation->setDuration(m_msec);
            font2Animation->setStartValue(22);
            font2Animation->setEndValue(18);

            m_parallelGroup = new QParallelAnimationGroup(this);
            m_parallelGroup->addAnimation(rect1Animation);
            m_parallelGroup->addAnimation(font1Animation);
            m_parallelGroup->addAnimation(rect2Animation);
            m_parallelGroup->addAnimation(font2Animation);
        }
        else if (m_dir == Up)
        {
            QPropertyAnimation *rect1Animation = new QPropertyAnimation(this, "rect1");
            rect1Animation->setDuration(m_msec);
            rect1Animation->setStartValue(m_r_main);
            rect1Animation->setEndValue(m_r_next[0]);
            QPropertyAnimation *font1Animation = new QPropertyAnimation(this, "font1");
            font1Animation->setDuration(m_msec);
            font1Animation->setStartValue(22);
            font1Animation->setEndValue(18);

            QPropertyAnimation *rect2Animation = new QPropertyAnimation(this, "rect2");
            rect2Animation->setDuration(m_msec);
            rect2Animation->setStartValue(m_r_prev[0]);
            rect2Animation->setEndValue(m_r_main);
            QPropertyAnimation *font2Animation = new QPropertyAnimation(this, "font2");
            font2Animation->setDuration(m_msec);
            font2Animation->setStartValue(18);
            font2Animation->setEndValue(22);

            m_parallelGroup = new QParallelAnimationGroup(this);
            m_parallelGroup->addAnimation(rect1Animation);
            m_parallelGroup->addAnimation(font1Animation);
            m_parallelGroup->addAnimation(rect2Animation);
            m_parallelGroup->addAnimation(font2Animation);
        }
        else if (m_dir == Stay)
        {
            QTimer::singleShot(200, this, [&] { OnAnimationFinished(); });
        }
    }
    if (m_parallelGroup)
    {
        m_parallelGroup->start();
        for (int i = 0; i < m_parallelGroup->animationCount(); ++i)
        {
            if (auto ani = dynamic_cast<QPropertyAnimation *>(m_parallelGroup->animationAt(i)))
            {
                connect(ani, SIGNAL(valueChanged(const QVariant &)), this, SLOT(OnAnimationValueChanged(const QVariant &)));
            }
        }
        connect(m_parallelGroup, SIGNAL(finished()), this, SLOT(OnAnimationFinished()));
    }
}

void SceneWordScrollbar::OnAnimationValueChanged(const QVariant &)
{
    emit updateSignal();
}

void SceneWordScrollbar::OnAnimationFinished()
{
    if (m_parallelGroup)
    {
        m_parallelGroup->deleteLater();
        m_parallelGroup = nullptr;
    }
    if (m_markPass < 0)
    {
        m_markPass = 0;
        if (m_dir == Up)
        {
            m_silderIndex--;
        }
    }
    else
    {
        if (m_dir == Down)
        {
            m_silderIndex++;
        }
        else if (m_dir == Up)
        {
            m_silderIndex--;
        }
    }
    if (!m_operation.isEmpty())
    {
        operate();
    }
    else
        emit finished();
}

SceneText::SceneText(QObject *parent) : SceneNode(parent)
{
    m_wrap = false;
    m_color = Qt::white;
    m_rect = QRect(0, 0, 100, 30);
    m_pAnimation = nullptr;
}

void SceneText::setText(QString text)
{
    m_txt = text;
    emit updateSignal();
}

QString SceneText::text() const
{
    return m_txt;
}

void SceneText::setWordWrap(bool on)
{
    m_wrap = on;
    emit updateSignal();
}

void SceneText::setFont(QFont f)
{
    m_font = f;
    emit updateSignal();
}

void SceneText::setColor(QColor color)
{
    m_color = color;
    emit updateSignal();
}

void SceneText::setGeometry(QRect r)
{
    m_rect = r;
    emit updateSignal();
}

QRect SceneText::geometry() const
{
    return m_rect;
}

void SceneText::eraseShow()
{
    if (m_visible) return;
    if (m_pAnimation && m_pAnimation->state() == QAbstractAnimation::Stopped)
    {
        delete m_pAnimation;
        m_pAnimation = nullptr;
    }
    m_visible = true;
    this->setProperty("width", int(0));
    m_pAnimation = new QPropertyAnimation(this, "width");
    m_pAnimation->setDuration(int(500 * DURATION_SCALE));
    m_pAnimation->setStartValue(0);
    m_pAnimation->setEndValue(m_rect.width());
    m_pAnimation->start();
    connect(m_pAnimation, QOverload<const QVariant &>::of(&QPropertyAnimation::valueChanged), this, [&](const QVariant &) {
        emit updateSignal();
    });
    connect(m_pAnimation, &QPropertyAnimation::finished, this, [&] {
        if (m_pAnimation)
        {
            m_pAnimation->deleteLater();
            m_pAnimation = nullptr;
        }
        emit finished();
    });
}

void SceneText::selfRender(QPainter *painter)
{
    if (m_txt.isEmpty()) return;
    if (SHOW_LINE)
    {
        painter->setPen(QColor(0xeb0a7f));
        painter->drawRect(m_rect);
    }
    if (m_pAnimation)
    {
        int width = this->property("width").toInt();
        painter->setClipRect(m_rect.x(), m_rect.y(), width, m_rect.height());
    }
    painter->setFont(m_font);
    painter->setPen(m_color);
    if (m_wrap)
    {
        QTextOption opt;
        opt.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        painter->drawText(m_rect, m_txt, opt);
    }
    else
        painter->drawText(m_rect, Qt::AlignRight | Qt::AlignVCenter, m_txt);
}

SceneTimer::SceneTimer(QObject *parent) : SceneNode(parent)
{
    m_size = QSize(100, 100);
    m_duration = 10;
    m_pAnimation = nullptr;

    setProperty("time", double(m_duration));
}

void SceneTimer::setSize(QSize size)
{
    m_size = size;

    int w = m_size.width() * 2 / 3;
    m_r_circle = QRect((m_size.width() - w) / 2, 10, w, w);

    m_r_text = QRect(0, 10 + w, m_size.width(), m_size.height() - 10 - w);
}

void SceneTimer::setDuration(int msecs)
{
    m_duration = msecs;
    if (m_pAnimation) stop();
    setProperty("time", double(m_duration / 1000.0));
    emit updateSignal();
}

int SceneTimer::getDuration() const
{
    return m_duration;
}

void SceneTimer::start()
{
    if (m_pAnimation && m_pAnimation->state() == QAbstractAnimation::Stopped)
    {
        delete m_pAnimation;
        m_pAnimation = nullptr;
    }

    setProperty("time", double(m_duration));
    m_pAnimation = new QPropertyAnimation(this, "time");
    m_pAnimation->setDuration(int(m_duration * DURATION_SCALE));
    m_pAnimation->setStartValue(double(m_duration / 1000.0));
    m_pAnimation->setEndValue(double(0.0));
    m_pAnimation->start();
    connect(m_pAnimation, QOverload<const QVariant &>::of(&QPropertyAnimation::valueChanged), this, [&](const QVariant &) {
        emit updateSignal();
    });
    connect(m_pAnimation, &QPropertyAnimation::finished, this, [&] {
        if (m_pAnimation)
        {
            m_pAnimation->deleteLater();
            m_pAnimation = nullptr;
        }
        emit finished();
    });
}

void SceneTimer::stop()
{
    if (m_pAnimation)
    {
        m_pAnimation->stop();
        emit updateSignal();
    }
}

void SceneTimer::pause()
{
    if (m_pAnimation && m_pAnimation->state() == QAbstractAnimation::Running)
    {
        m_pAnimation->pause();
        emit updateSignal();
    }
}

void SceneTimer::resume()
{
    if (m_pAnimation && m_pAnimation->state() == QAbstractAnimation::Paused)
    {
        m_pAnimation->resume();
        emit updateSignal();
    }
}

void SceneTimer::resetDuration(int msecs)
{
    if (!m_pAnimation) return;
    if (m_pAnimation->state() == QAbstractAnimation::Running || m_pAnimation->state() == QAbstractAnimation::Paused)
    {
        m_pAnimation->setDuration(int(msecs * DURATION_SCALE));
    }
    emit updateSignal();
}

void SceneTimer::selfRender(QPainter *painter)
{
    if (SHOW_LINE)
    {
        painter->setPen(QColor(0xff9000));
        painter->drawRect(QRect(QPoint(0, 0), m_size));
        painter->drawRect(m_r_circle);
        painter->drawRect(m_r_text);
    }
    double time = this->property("time").toDouble();
    double percent = time * 1000.0 / m_duration;
    auto f = painter->font();
    double ang = 360 * percent;
    painter->setPen(QColor(Qt::transparent));
    painter->setBrush(QColor(Qt::green));
    painter->drawPie(m_r_circle, ANG(90.0), ANG(ang));

    f.setPixelSize(18);
    painter->setFont(f);
    painter->setPen(QColor(Qt::black));
    painter->drawText(m_r_text, Qt::AlignCenter, QString().sprintf("%.3f", time));
}

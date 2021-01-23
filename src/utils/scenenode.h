#ifndef SCENENODE_H
#define SCENENODE_H

#include <QWidget>
#include <QSize>
#include <QPainter>
#include <QPropertyAnimation>
#include <QQueue>
#include <QParallelAnimationGroup>
#include "wordadmin.h"

class SceneNode : public QObject
{
    Q_OBJECT
public:
    explicit SceneNode(QObject *parent);
    virtual ~SceneNode();

    void setPos(QPoint pos);
    void setVisible(bool vis);
    bool isVisible() const;

    SceneNode* getRoot();
    virtual QRect boundingRect() const;
    virtual QVector<SceneNode *> select(QPoint pos);
    void render(QPainter *painter);

protected:
    virtual void selfRender(QPainter *painter);

signals:
    void updateSignal();

protected:
    QSize m_win_size;
    QPoint m_pos;
    bool m_visible;
};

class SceneImage : public SceneNode
{
    Q_OBJECT
public:
    explicit SceneImage(QString img, QObject *parent);
    void setImg(QString img);
    void setImgSize(QSize size);

protected:
    virtual void selfRender(QPainter *painter) override;

protected:
    QPixmap m_img;
    QSize m_size;
};

class SceneWordScrollbar : public SceneNode
{
    Q_OBJECT
public:
    explicit SceneWordScrollbar(QObject *parent);
    void setTestList(const QList<WordTest> &testList);
    QList<WordTest>& getTestList() { return m_testList; }
    WordTest *getCurrentWord();
    int getTestNum() { return m_testNum; }
    int getPassNum() { return m_passNum; }
    void passCurrentWord();
    void advance(BriefWordInfo *wordInfo);
    bool isEnd() const;
    QParallelAnimationGroup *getAnimation() { return m_parallelGroup; }

protected:
    enum { Down, Up, Stay };
    virtual void selfRender(QPainter *painter) override;
    void operate();

protected slots:
    void OnAnimationValueChanged(const QVariant &);
    void OnAnimationFinished();

signals:
    void finished();

protected:
    QList<WordTest> m_testList;
    int m_testNum;
    int m_passNum;

    int m_volume;
    int m_silderPos;
    int m_silderIndex;
    int m_markPass;

    int m_spacing;
    QRect m_r_main;
    QRect m_r_next[10];
    QRect m_r_prev[10];
    QQueue<std::tuple<int/*Down, Up, Stay*/, int/* msecs */>> m_operation;
    QParallelAnimationGroup *m_parallelGroup;
    int m_dir;
    int m_msec;
};

class SceneText : public SceneNode
{
    Q_OBJECT
public:
    explicit SceneText(QObject *parent);
    void setText(QString text);
    QString text() const;
    void setWordWrap(bool on);
    void setFont(QFont f);
    void setColor(QColor color);
    void setGeometry(QRect r);
    QRect geometry() const;
    QPropertyAnimation *getAnimation() { return m_pAnimation; }
    void eraseShow();

protected:
    virtual void selfRender(QPainter *painter) override;

signals:
    void finished();

protected:
    QString m_txt;
    bool m_wrap;
    QFont m_font;
    QColor m_color;
    QRect m_rect;
    QPropertyAnimation *m_pAnimation;
};

class SceneTimer : public SceneNode
{
    Q_OBJECT
public:
    explicit SceneTimer(QObject *parent);
    void setSize(QSize size);
    void setDuration(int msecs);
    int getDuration() const;
    QPropertyAnimation *getAnimation() { return m_pAnimation; }
    void start();
    void stop();
    void pause();
    void resume();
    void resetDuration(int msecs);

protected:
    virtual void selfRender(QPainter *painter) override;

signals:
    void finished();

protected:
    QSize m_size;
    int m_duration;
    QPropertyAnimation *m_pAnimation;

    QRect m_r_circle;
    QRect m_r_text;
};

#endif // SCENENODE_H

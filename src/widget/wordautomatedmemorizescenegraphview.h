#ifndef WORDAUTOMATEDMEMORIZESCENEGRAPHVIEW_H
#define WORDAUTOMATEDMEMORIZESCENEGRAPHVIEW_H

#include <QWidget>
#include "scenenode.h"
#include "wordadmin.h"
#include "dtcp_tool_button.h"

class WordAutomatedMemorizeSceneGraphView : public QWidget
{
    Q_OBJECT
public:
    enum STATE { State_Init, State_BarAni, State_TextAni, State_TimerAni };
    explicit WordAutomatedMemorizeSceneGraphView(QWidget *parent = nullptr);
    void initiate(QList<WordTest> testList);
    void start();
    void stop();
    void setState(int state);
    QAbstractAnimation *currentAnimation();

private:
    virtual void paintEvent(QPaintEvent *) override;
    virtual void resizeEvent(QResizeEvent *) override;
    void updatePosition();

private slots:
    void OnWordBarAnimationFinished();
    void OnWordShowFinished();
    void OnWordTimeOut();
    void OnBtnPauseClicked();
    void OnBtnVisibleClicked();
    void OnBtnMinusClicked();
    void OnBtnPlusClicked();
    void OnBtnForeverClicked();
    void OnBtnOkClicked();

private:
    int m_state;

    SceneImage *m_node_bg;
    SceneWordScrollbar *m_node_wordBar;
    SceneText *m_node_info;
    SceneText *m_node_word;
    SceneTimer *m_node_timer;

    DtcpImageButton *m_btn_pause;
    DtcpImageButton *m_btn_visible;
    DtcpImageButton *m_btn_minus;
    DtcpImageButton *m_btn_plus;
    DtcpImageButton *m_btn_forever;
    DtcpImageButton *m_btn_ok;
};

#endif // WORDAUTOMATEDMEMORIZESCENEGRAPHVIEW_H

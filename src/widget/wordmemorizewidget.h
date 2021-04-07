#ifndef WORDMEMORIZEWIDGET_H
#define WORDMEMORIZEWIDGET_H

#include <QWidget>
#include "wordadmin.h"
#include "copylabel.h"
#include "linklabel.h"
#include "wtool.h"

namespace Ui
{
    class WordMemorizeWidget;
}

class WordMemorizeWidget : public QWidget
{
    Q_OBJECT
public:
    enum MODE {
        WELCOME, MEMORY
    };
    enum TEST {
        NONE = -1, EXPLORATE, RECALL, DICTATE
    };
    enum TEST_STRATEGY {
        STRATEGY0, STRATEGY1
    };

    explicit WordMemorizeWidget(QWidget *parent = nullptr);
    ~WordMemorizeWidget();
    void recoveryInterface();
    void reloadGlobalValue();
    void setReloadFlag(bool flag);
    void updateWordStatistics();
    int getMode();

private:
    void keyPressEvent(QKeyEvent *event);
    void setMode(int mode);
    void testListInit();
    void loadTestInfo();
    void setWordInfo(bool isShield);
    void reloadLayout();
    void bottomLayoutSet(QVector<QVector<QWidget *>> w_set);
    void wordInfoShow(int mode);
    void chooseTestWord();
    void chooseStrategy_level0();
    void chooseStrategy_level1();

private slots:
    void slot_radioButtonClicked();
    void slot_comboxGroup_currentIndexChanged(int index);
    void slot_btnStart_Clicked();
    void slot_btnKnow_Clicked();
    void slot_btnNotKnow_Clicked();
    void slot_btnForever_Clicked();
    void slot_btnNotForever_Clicked();
    void slot_btnNext_Clicked();
    void slot_btnSubmit_Clicked();
//    void slot_wordCanMemorize(QString name);
//    void slot_wordTimeDecline(QString name);
    void slot_stopWordMemorize(bool *ret);

private:
    Ui::WordMemorizeWidget *ui;
    int m_mode;
    int m_test;
    int m_lineNum;
    int m_container_height_;

    int m_testNum;
    int m_passNum;
    int m_curIndex;
    QString m_lastWord;

    bool m_reloadFlag;
    int m_curGroupId;

    QList<WordTest> m_testList;
    QStringList m_nameList;
    WordInfo m_word;

    CopyLabel *copyLabel_property[PROPERTY_NUM];
    CopyLabel *copyLabel_explain[PROPERTY_NUM];
    QWidget *widget_explain[PROPERTY_NUM];
    CopyLabel *copyLabel_example[EXAMPLE_NUM];

    LinkLabel *linkLabel_synonym[RELATED_NUM];
    LinkLabel *linkLabel_antonym[RELATED_NUM];
    LinkLabel *linkLabel_derivative[RELATED_NUM];
};

#endif // WORDMEMORIZEWIDGET_H
